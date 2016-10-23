#include <CBrowserHtmlI.h>

#include <QPushButton>
#include <QCheckBox>
#include <QRadioButton>
#include <QListWidget>
#include <QListWidgetItem>
#include <QLineEdit>
#include <QTextEdit>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QMenu>

enum CBrowserFormInputType {
  CBROWSER_FORM_INPUT_NONE,
  CBROWSER_FORM_INPUT_BUTTON,
  CBROWSER_FORM_INPUT_CHECKBOX,
  CBROWSER_FORM_INPUT_FILE,
  CBROWSER_FORM_INPUT_HIDDEN,
  CBROWSER_FORM_INPUT_IMAGE,
  CBROWSER_FORM_INPUT_PASSWORD_TEXT,
  CBROWSER_FORM_INPUT_RADIO_BUTTON,
  CBROWSER_FORM_INPUT_RESET_BUTTON,
  CBROWSER_FORM_INPUT_SELECT,
  CBROWSER_FORM_INPUT_SUBMIT_BUTTON,
  CBROWSER_FORM_INPUT_TEXT,
  CBROWSER_FORM_INPUT_TEXTAREA
};

enum CBrowserFormTextAreaWrapType {
  CBROWSER_FORM_TEXT_AREA_WRAP_OFF,
  CBROWSER_FORM_TEXT_AREA_WRAP_VIRTUAL,
  CBROWSER_FORM_TEXT_AREA_WRAP_PHYSICAL
};

//---------------------

class CBrowserFormInput;

class CBrowserForm : public QObject {
 public:
  CBrowserForm(const std::string &name, CBrowserDocument *document,
               int method, const std::string &action) :
   name_(name), document_(document), method_(method), action_(action) {
    target_   = "";
    encoding_ = "";
  }

  std::string getName() const { return name_; }

  CBrowserDocument *getDocument() const { return document_; }

  int         getMethod() const { return method_; }
  std::string getAction() const { return action_; }
  std::string getTarget() const { return target_; }

  std::string getEncoding() const { return encoding_; }

  void addInput(CBrowserFormInput *input) { input_list_.push_back(input); }

  int getNumInputs() const { return input_list_.size(); }

  CBrowserFormInput *getInput(int i) const { return input_list_[i]; }

 public slots:
  void resetProc();
  void submitProc();

 private:
  typedef std::vector<CBrowserFormInput *> FormInputs;

  std::string       name_;
  CBrowserDocument *document_;
  int               method_;
  std::string       action_;
  std::string       target_;
  std::string       encoding_;
  FormInputs        input_list_;
};

class CBrowserFormOption {
 public:
  CBrowserFormOption(const std::string &value, int selected) :
   value_(value), selected_(selected) {
    text_ = "";
  }

  int getSelected() const { return selected_; }

  std::string getText() const { return text_; }

  void setText(const std::string &text) { text_ = text; }

 private:
  std::string value_;
  int         selected_;
  std::string text_;
};

class CBrowserFormInput {
 public:
  CBrowserFormInput(CBrowserDocument *document, CBrowserFormInputType type,
                    const std::string &name, const std::string &value);

  virtual ~CBrowserFormInput();

  CBrowserFormInputType getType() const { return type_; }

  CBrowserForm *getForm() const { return form_; }

  std::string getName() const { return name_; }

  std::string getValue() const { return value_; }

  void setValue(const std::string &value) { value_ = value; }

  std::string getOnChange() const { return onchange_; }
  std::string getOnClick () const { return onclick_; }
  std::string getOnFocus () const { return onfocus_; }

  QWidget *getWidget() const { return widget_; }

  int getWidth () const { return width_ ; }
  int getHeight() const { return height_; }

  virtual void createWidget(CBrowserWindow *window) = 0;

  virtual void drawWidget(CBrowserWindow *window, int x, int y) = 0;

  virtual void reset() { };

  virtual void submit(std::string & /*url*/) { };

  void formatProc();
  void redrawProc(int *x, int *y);
  void freeProc();

  void onClickProc ();
  void onChangeProc();

 protected:
  CBrowserDocument                      *document_;
  CBrowserFormInputType                  type_;
  CBrowserForm                          *form_;
  std::string                            name_;
  std::string                            value_;
  std::string                            onblur_;
  std::string                            onchange_;
  std::string                            onclick_;
  std::string                            onfocus_;
  QWidget*                               widget_;
  int                                    width_;
  int                                    height_;
  CBrowserFormatProc<CBrowserFormInput> *format_proc_;
  CBrowserRedrawProc<CBrowserFormInput> *redraw_proc_;
  CBrowserFreeProc<CBrowserFormInput>   *free_proc_;
};

class CBrowserFormButton : public QObject, public CBrowserFormInput {
 public:
  CBrowserFormButton(CBrowserDocument *document, const std::string &name,
                     const std::string &value) :
   CBrowserFormInput(document, CBROWSER_FORM_INPUT_BUTTON, name, value) {
    if (name_ == "")
      name_ = "button";
  }

  void createWidget(CBrowserWindow *window);

  void drawWidget(CBrowserWindow *window, int x, int y);

 protected:
  int dummy_;
};

class CBrowserFormCheckBox : public QObject, public CBrowserFormInput {
 public:
  CBrowserFormCheckBox(CBrowserDocument *document, const std::string &name,
                       const std::string &value, int checked) :
   CBrowserFormInput(document, CBROWSER_FORM_INPUT_CHECKBOX, name, value),
   checked_(checked) {
    if (name_ == "")
      name_ = "checkbox";
  }

  void createWidget(CBrowserWindow *window);

  void drawWidget(CBrowserWindow *window, int x, int y);

 protected:
  int checked_;
};

class CBrowserFormFileUpload : public QObject, public CBrowserFormInput {
 public:
  CBrowserFormFileUpload(CBrowserDocument *document, const std::string &name,
                         const std::string &value, int size, int maxlength) :
   CBrowserFormInput(document, CBROWSER_FORM_INPUT_FILE, name, value),
   size_(size), maxlength_(maxlength) {
    if (name_ == "")
      name_ = "file";
  }

  void createWidget(CBrowserWindow *window);

  void drawWidget(CBrowserWindow *window, int x, int y);

 public slots:
  void buttonProc();

 protected:
  int size_;
  int maxlength_;
};

class CBrowserFormHidden : public QObject, public CBrowserFormInput {
 public:
  CBrowserFormHidden(CBrowserDocument *document, const std::string &name,
                     const std::string &value) :
   CBrowserFormInput(document, CBROWSER_FORM_INPUT_HIDDEN, name, value) {
    if (name_ == "")
      name_ = "hidden";
  }

  void createWidget(CBrowserWindow *window);

  void drawWidget(CBrowserWindow *window, int x, int y);

  void submit(std::string &url);

 protected:
  int dummy_;
};

class CBrowserFormImage : public QObject, public CBrowserFormInput {
 public:
  CBrowserFormImage(CBrowserDocument *document, const std::string &name,
                    const std::string &value, const std::string &src,
                    const CImagePtr &image, CBrowserImageAlignType align) :
   CBrowserFormInput(document, CBROWSER_FORM_INPUT_IMAGE, name, value),
   src_(src), image_(image), align_(align) {
    if (name_ == "")
      name_ = "image";
  }

 ~CBrowserFormImage() {
  }

  void createWidget(CBrowserWindow *window);

  void drawWidget(CBrowserWindow *window, int x, int y);

 protected:
  std::string            src_;
  CImagePtr              image_;
  CBrowserImageAlignType align_;
};

class CBrowserFormPassword : public QObject, public CBrowserFormInput {
 public:
  CBrowserFormPassword(CBrowserDocument *document, const std::string &name,
                       const std::string &value, int size, int maxlength) :
   CBrowserFormInput(document, CBROWSER_FORM_INPUT_PASSWORD_TEXT, name, value),
   size_(size), maxlength_(maxlength) {
    if (name_ == "")
      name_ = "password";
  }

  void createWidget(CBrowserWindow *window);

  void drawWidget(CBrowserWindow *window, int x, int y);

  void reset();

  void submit(std::string &url);

 protected:
  int size_;
  int maxlength_;
};

class CBrowserFormRadio : public QObject, public CBrowserFormInput {
 public:
  CBrowserFormRadio(CBrowserDocument *document, const std::string &name,
                    const std::string &value, int checked) :
   CBrowserFormInput(document, CBROWSER_FORM_INPUT_RADIO_BUTTON, name, value),
   checked_(checked) {
    if (name_ == "")
      name_ = "radio";
  }

  void createWidget(CBrowserWindow *window);

  void drawWidget(CBrowserWindow *window, int x, int y);

 public slots:
  void buttonProc();

 protected:
  int checked_;
};

class CBrowserFormReset : public QObject, public CBrowserFormInput {
 public:
  CBrowserFormReset(CBrowserDocument *document, const std::string &name,
                    const std::string &value) :
   CBrowserFormInput(document, CBROWSER_FORM_INPUT_RESET_BUTTON, name, value) {
    if (name_ == "")
      name_ = "input";
  }

  void createWidget(CBrowserWindow *window);

  void drawWidget(CBrowserWindow *window, int x, int y);

 protected:
  int dummy_;
};

class CBrowserFormSelect : public QObject, public CBrowserFormInput {
 public:
  CBrowserFormSelect(CBrowserDocument *document, const std::string &name,
                     const std::string &value, int size, int multiple) :
   CBrowserFormInput(document, CBROWSER_FORM_INPUT_SELECT, name, value),
   size_(size), multiple_(multiple) {
    if (name_ == "")
      name_ = "select";
  }

  void addOption(CBrowserFormOption *option) {
    option_list_.push_back(option);
  }

  int getNumOptions() const { return option_list_.size(); }

  CBrowserFormOption *getOption(int i) { return option_list_[i]; }

  void createWidget(CBrowserWindow *window);

  void drawWidget(CBrowserWindow *window, int x, int y);

 protected:
  int                               size_;
  int                               multiple_;
  std::vector<CBrowserFormOption *> option_list_;
};

class CBrowserFormSubmit : public QObject, public CBrowserFormInput {
 public:
  CBrowserFormSubmit(CBrowserDocument *document, const std::string &name,
                     const std::string &value) :
   CBrowserFormInput(document, CBROWSER_FORM_INPUT_SUBMIT_BUTTON, name, value) {
    if (name_ == "")
      name_ = "submit";
  }

  void createWidget(CBrowserWindow *window);

  void drawWidget(CBrowserWindow *window, int x, int y);

 protected:
  int dummy_;
};

class CBrowserFormText : public QObject, public CBrowserFormInput {
 public:
  CBrowserFormText(CBrowserDocument *document, const std::string &name,
                   const std::string &value, int size, int maxlength) :
   CBrowserFormInput(document, CBROWSER_FORM_INPUT_TEXT, name, value),
   size_(size), maxlength_(maxlength) {
    if (name_ == "")
      name_ = "text";
  }

  void createWidget(CBrowserWindow *window);

  void drawWidget(CBrowserWindow *window, int x, int y);

  void reset();

  void submit(std::string &url);

 protected:
  int size_;
  int maxlength_;
};

class CBrowserFormTextarea : public QObject, public CBrowserFormInput {
 public:
  CBrowserFormTextarea(CBrowserDocument *document, const std::string &name,
                       const std::string &value, int rows, int cols,
                       CBrowserFormTextAreaWrapType wrap) :
   CBrowserFormInput(document, CBROWSER_FORM_INPUT_TEXTAREA, name, value),
   rows_(rows), cols_(cols), wrap_(wrap) {
    if (name_ == "")
      name_ = "textarea";
  }

  void createWidget(CBrowserWindow *window);

  void drawWidget(CBrowserWindow *window, int x, int y);

  void reset();

  void submit(std::string &url);

 protected:
  int                          rows_;
  int                          cols_;
  CBrowserFormTextAreaWrapType wrap_;
};

//---------------------

static CBrowserForm         *current_form     = NULL;
static CBrowserFormSelect   *current_select   = NULL;
static CBrowserFormOption   *current_option   = NULL;
static CBrowserFormTextarea *current_textarea = NULL;
static std::string           input_onblur     = "";
static std::string           input_onclick    = "";
static std::string           input_onchange   = "";
static std::string           input_onfocus    = "";

void
HtmlStartForm(CBrowserDocument *document, const std::string &name, int method,
              const std::string &action)
{
  if (current_form != NULL)
    HtmlEndForm(document);

  CBrowserForm *form = new CBrowserForm(name, document, method, action);

  document->addForm(form);

  current_form = form;
}

void
HtmlEndForm(CBrowserDocument *document)
{
  if (current_form == NULL || current_form->getDocument() != document)
    return;

  current_form = NULL;
}

void
CBrowserFormSetInputOnBlur(const std::string &onblur)
{
  input_onblur = onblur;
}

void
CBrowserFormSetInputOnChange(const std::string &onchange)
{
  input_onchange = onchange;
}

void
CBrowserFormSetInputOnClick(const std::string &onclick)
{
  input_onclick = onclick;
}

void
CBrowserFormSetInputOnFocus(const std::string &onfocus)
{
  input_onfocus = onfocus;
}

void
CBrowserFormStartSelect(CBrowserDocument *document, const std::string &name, int size, int multiple)
{
  if (current_form == NULL)
    return;

  if (current_select != NULL)
    CBrowserFormEndSelect(document);

  current_select = new CBrowserFormSelect(document, name, "", size, multiple);
}

void
CBrowserFormEndSelect(CBrowserDocument *document)
{
  if (current_form == NULL || current_form->getDocument() != document)
    return;

  current_select = NULL;
}

void
CBrowserFormStartOption(const std::string &value, int selected)
{
  if (current_form == NULL || current_select == NULL)
    return;

  if (current_option != NULL)
    CBrowserFormEndOption("????");

  current_option =
    new CBrowserFormOption(value, selected);
}

void
CBrowserFormEndOption(const std::string &text)
{
  if (current_form == NULL || current_select == NULL ||
      current_option == NULL)
    return;

  if (text != "")
    current_option->setText(text);
  else
    current_option->setText("????");

  current_select->addOption(current_option);

  current_option = NULL;
}

void
CBrowserFormStartTextarea(CBrowserDocument *document,
                          const std::string &name, int rows,
                          int cols, const std::string &wrap)
{
  if (current_form == NULL)
    return;

  if (current_textarea != NULL)
    CBrowserFormEndTextarea(document, "");

  CBrowserFormTextAreaWrapType wrap1 = CBROWSER_FORM_TEXT_AREA_WRAP_OFF;

  if      (CStrUtil::casecmp(wrap, "off"     ) == 0)
    wrap1 = CBROWSER_FORM_TEXT_AREA_WRAP_OFF;
  else if (CStrUtil::casecmp(wrap, "virtual" ) == 0)
    wrap1 = CBROWSER_FORM_TEXT_AREA_WRAP_VIRTUAL;
  else if (CStrUtil::casecmp(wrap, "physical") == 0)
    wrap1 = CBROWSER_FORM_TEXT_AREA_WRAP_PHYSICAL;

  CBrowserFormTextarea *input = new CBrowserFormTextarea(document, name, "", rows, cols, wrap1);

  current_form->addInput(input);

  current_textarea = input;
}

void
CBrowserFormEndTextarea(CBrowserDocument *document, const std::string &text)
{
  if (current_form == NULL || current_form->getDocument() != document ||
      current_textarea == NULL)
    return;

  current_textarea->setValue(text);

  current_textarea = NULL;
}

void
CBrowserFormAddButtonInput(CBrowserDocument *document, const std::string &name,
                           const std::string &value)
{
  if (current_form == NULL)
    return;

  CBrowserFormButton *input = new CBrowserFormButton(document, name, value);

  current_form->addInput(input);
}

void
CBrowserFormAddCheckboxInput(CBrowserDocument *document, const std::string &name,
                             const std::string &value, int checked)
{
  if (current_form == NULL)
    return;

  CBrowserFormCheckBox *input = new CBrowserFormCheckBox(document, name, value, checked);

  current_form->addInput(input);
}

void
CBrowserFormAddFileInput(CBrowserDocument *document, const std::string &name,
                         const std::string &value, int size, int maxlength)
{
  if (current_form == NULL)
    return;

  CBrowserFormFileUpload *input =
    new CBrowserFormFileUpload(document, name, value, size, maxlength);

  current_form->addInput(input);
}

void
CBrowserFormAddHiddenInput(CBrowserDocument *document, const std::string &name,
                           const std::string &value)
{
  if (current_form == NULL)
    return;

  CBrowserFormHidden *input = new CBrowserFormHidden(document, name, value);

  current_form->addInput(input);
}

void
CBrowserFormAddImageInput(CBrowserDocument *document, const std::string &name,
                          const std::string &src, const std::string &align)
{
  if (current_form == NULL)
    return;

  CImageFileSrc file(src);

  CImagePtr image = CImageMgrInst->createImage(file);

  if (! image)
    return;

  CBrowserImageAlignType align1 = CBROWSER_IMAGE_ALIGN_BOTTOM;

  if      (align == "")
    align1 = CBROWSER_IMAGE_ALIGN_BOTTOM;
  else if (CStrUtil::casecmp(align, "top"       ) == 0)
    align1 = CBROWSER_IMAGE_ALIGN_TOP;
  else if (CStrUtil::casecmp(align, "middle"    ) == 0)
    align1 = CBROWSER_IMAGE_ALIGN_MIDDLE;
  else if (CStrUtil::casecmp(align, "bottom"    ) == 0)
    align1 = CBROWSER_IMAGE_ALIGN_BOTTOM;
  else if (CStrUtil::casecmp(align, "left"      ) == 0)
    align1 = CBROWSER_IMAGE_ALIGN_LEFT;
  else if (CStrUtil::casecmp(align, "right"     ) == 0)
    align1 = CBROWSER_IMAGE_ALIGN_RIGHT;
  else if (CStrUtil::casecmp(align, "texttop"   ) == 0)
    align1 = CBROWSER_IMAGE_ALIGN_TEXTTOP;
  else if (CStrUtil::casecmp(align, "absmiddle" ) == 0)
    align1 = CBROWSER_IMAGE_ALIGN_ABSMIDDLE;
  else if (CStrUtil::casecmp(align, "absbottom" ) == 0)
    align1 = CBROWSER_IMAGE_ALIGN_ABSBOTTOM;

  CBrowserFormImage *input = new CBrowserFormImage(document, name, "", src, image, align1);

  current_form->addInput(input);
}

void
CBrowserFormAddPasswordInput(CBrowserDocument *document, const std::string &name,
                             int size, int maxlength)
{
  if (current_form == NULL)
    return;

  CBrowserFormPassword *input = new CBrowserFormPassword(document, name, "", size, maxlength);

  current_form->addInput(input);
}

void
CBrowserFormAddRadioInput(CBrowserDocument *document, const std::string &name,
                          const std::string &value, int checked)
{
  if (current_form == NULL)
    return;

  CBrowserFormRadio *input = new CBrowserFormRadio(document, name, value, checked);

  current_form->addInput(input);
}

void
CBrowserFormAddResetInput(CBrowserDocument *document, const std::string &name,
                          const std::string &value)
{
  if (current_form == NULL)
    return;

  CBrowserFormReset *input = new CBrowserFormReset(document, name, value);

  current_form->addInput(input);
}

void
CBrowserFormAddSubmitInput(CBrowserDocument *document, const std::string &name,
                           const std::string &value)
{
  if (current_form == NULL)
    return;

  CBrowserFormSubmit *input = new CBrowserFormSubmit(document, name, value);

  current_form->addInput(input);
}

void
CBrowserFormAddTextInput(CBrowserDocument *document, const std::string &name,
                         const std::string &value, int size, int maxlength)
{
  if (current_form == NULL)
    return;

  CBrowserFormText *input = new CBrowserFormText(document, name, value, size, maxlength);

  current_form->addInput(input);
}

/*---------------------*/

void
CBrowserFormFileUpload::
createWidget(CBrowserWindow *window)
{
  QWidget *rowcol = new QWidget(window->getCanvas());

  QHBoxLayout *hlayout = new QHBoxLayout(rowcol);

  QLineEdit *edit = new QLineEdit(rowcol);

  edit->setText(value_.c_str());

  widget_ = edit;

  hlayout->addWidget(edit);

  //if (size_ > 0) XcwSetValue(edit, XmNcolumns, size_);

  if (maxlength_ > 0) edit->setMaxLength(maxlength_);

  QPushButton *button = new QPushButton("button");

  button->setText("Browse ...");

  QObject::connect(button, SIGNAL(clicked), this, SLOT(buttonProc()));

  hlayout->addWidget(button);

  width_  = rowcol->width () + 4;
  height_ = rowcol->height() + 4;

  window->updateSubCellHeight(height_/2, height_/2);
  window->updateSubCellWidth (width_);
}

void
CBrowserFormFileUpload::
drawWidget(CBrowserWindow *, int x, int y)
{
  widget_->move(x + 2, y + 2);
}

void
CBrowserFormFileUpload::
buttonProc()
{
  QString filename = QFileDialog::getOpenFileName(widget_, "Select File");

  QLineEdit *edit = qobject_cast<QLineEdit *>(widget_);

  edit->setText(filename);
}

/*---------------------*/

void
CBrowserFormRadio::
createWidget(CBrowserWindow *window)
{
  QRadioButton *radio = new QRadioButton(window->getCanvas());

  radio->setText("");

  widget_ = radio;

  if (checked_)
    radio->setChecked(true);

  QObject::connect(radio, SIGNAL(clicked()), this, SLOT(buttonProc));

  width_  = widget_->width () + 4;
  height_ = widget_->height() + 4;

  window->updateSubCellHeight(height_/2, height_/2);
  window->updateSubCellWidth (width_);
}

void
CBrowserFormRadio::
drawWidget(CBrowserWindow *, int x, int y)
{
  widget_->move(x + 2, y + 2);
}

void
CBrowserFormRadio::
buttonProc()
{
  QRadioButton *radio = qobject_cast<QRadioButton *>(widget_);

  if (! radio->isChecked()) {
    radio->setChecked(true);

    return;
  }

  int num = form_->getNumInputs();

  for (int i = 1; i <= num; i++) {
    CBrowserFormInput *input1 = form_->getInput(i);

    if (input1            == this      ||
        input1->getType() != getType() ||
        input1->getName() != getName())
      continue;

    QWidget *widget1 = input1->getWidget();

    QRadioButton *radio1 = qobject_cast<QRadioButton *>(widget1);

    radio1->setChecked(false);
  }
}

/*---------------------*/

void
CBrowserForm::
resetProc()
{
  int num = getNumInputs();

  for (int i = 1; i <= num; i++) {
    CBrowserFormInput *input = getInput(i);

    input->reset();
  }
}

void
CBrowserForm::
submitProc()
{
  std::string url = getAction();

  if (url == "")
    return;

  url += "?";

  int num = getNumInputs();

  for (int i = 1; i <= num; i++) {
    CBrowserFormInput *input = getInput(i);

    input->submit(url);
  }

  CBrowserWindow *window = getDocument()->getWindow();

  window->setDocument(url);
}

/*---------------------*/

CBrowserFormInput::
CBrowserFormInput(CBrowserDocument *document, CBrowserFormInputType type,
                  const std::string &name, const std::string &value) :
 document_(document), type_(type),  name_(name), value_(value) {
  form_ = current_form;

  onblur_   = input_onblur;
  onclick_  = input_onclick;
  onchange_ = input_onchange;
  onfocus_  = input_onfocus;

  input_onblur  = "";
  input_onclick = "";
  input_onblur  = "";
  input_onblur  = "";

  widget_ = NULL;
  width_  = 0;
  height_ = 0;

  CBrowserWindow *window = document->getWindow();

  format_proc_ = new CBrowserFormatProc<CBrowserFormInput>(this);
  redraw_proc_ = NULL;
  free_proc_   = new CBrowserFreeProc<CBrowserFormInput>(this);

  window->addCellRedrawData(format_proc_, free_proc_);
}

CBrowserFormInput::
~CBrowserFormInput()
{
  delete format_proc_;
  delete redraw_proc_;
  delete free_proc_;
}

void
CBrowserFormInput::
formatProc()
{
  if (getType() == CBROWSER_FORM_INPUT_HIDDEN)
    return;

  /*----*/

  CBrowserWindow *window = document_->getWindow();

  CHtmlLayoutSubCell::newCellRight(window->getLayoutMgr(), true);

  /*----*/

  createWidget(window);

  /*----*/

  delete redraw_proc_;

  redraw_proc_ = new CBrowserRedrawProc<CBrowserFormInput>(this);

  window->addSubCellRedrawData(redraw_proc_);
}

void
CBrowserFormInput::
redrawProc(int *x, int *y)
{
  if (getType() == CBROWSER_FORM_INPUT_HIDDEN)
    return;

  /*----*/

  CBrowserWindow *window = document_->getWindow();

  drawWidget(window, *x, *y);

  /*----*/

  *x += getWidth();
}

void
CBrowserFormInput::
freeProc()
{
  delete this;
}

void
CBrowserFormInput::
onClickProc()
{
  HtmlRunScript(getForm()->getDocument()->getWindow(), getOnClick());
}

void
CBrowserFormInput::
onChangeProc()
{
  HtmlRunScript(getForm()->getDocument()->getWindow(), getOnChange());
}

/*---------------------*/

void
CBrowserFormButton::
createWidget(CBrowserWindow *window)
{
  QPushButton *button = new QPushButton(window->getCanvas());

  if (onclick_ != "")
    QObject::connect(button, SIGNAL(clicked()), this, SLOT(onClickProc()));

  if (value_ != "")
    button->setText(value_.c_str());
  else
    button->setText("Button");

  widget_ = button;

  width_  = widget_->width () + 4;
  height_ = widget_->height() + 4;

  window->updateSubCellHeight(height_/2, height_/2);
  window->updateSubCellWidth (width_);
}

void
CBrowserFormButton::
drawWidget(CBrowserWindow *, int x, int y)
{
  widget_->move(x + 2, y + 2);
}

/*---------------------*/

void
CBrowserFormCheckBox::
createWidget(CBrowserWindow *window)
{
  QCheckBox *button = new QCheckBox(window->getCanvas());

  widget_ = button;

  button->setText("");

  if (checked_)
    button->setChecked(true);

  width_  = widget_->width () + 4;
  height_ = widget_->height() + 4;

  window->updateSubCellHeight(height_/2, height_/2);
  window->updateSubCellWidth (width_);
}

void
CBrowserFormCheckBox::
drawWidget(CBrowserWindow *, int x, int y)
{
  widget_->move(x + 2, y + 2);
}

/*---------------------*/

void
CBrowserFormImage::
createWidget(CBrowserWindow *window)
{
  int hspace = 2;
  int vspace = 2;
  int width  = image_->getWidth ();
  int height = image_->getHeight();

  /*-------------*/

  CHtmlLayoutCell *redraw_cell = window->getCurrentCell();

  CHtmlLayoutSubCell *sub_cell = redraw_cell->getCurrentSubCell();

  if      (align_ == CBROWSER_IMAGE_ALIGN_LEFT)
    sub_cell->setAlign(CHALIGN_TYPE_LEFT);
  else if (align_ == CBROWSER_IMAGE_ALIGN_RIGHT)
    sub_cell->setAlign(CHALIGN_TYPE_RIGHT);
  else
    sub_cell->setAlign(CHALIGN_TYPE_NONE);

  /*-------------*/

  if      (align_ == CBROWSER_IMAGE_ALIGN_TOP    ||
           align_ == CBROWSER_IMAGE_ALIGN_TEXTTOP)
    window->updateSubCellHeight(vspace, height + vspace);
  else if (align_ == CBROWSER_IMAGE_ALIGN_MIDDLE ||
           align_ == CBROWSER_IMAGE_ALIGN_ABSMIDDLE)
    window->updateSubCellHeight((height + 1)/2 + vspace,
                                 height     /2 + vspace);
  else
    window->updateSubCellHeight(height + vspace, vspace);

  window->updateSubCellWidth(width + 2*hspace);

  /*-------------*/

  width_  = width  + 2*hspace;
  height_ = height + 2*vspace;
}

void
CBrowserFormImage::
drawWidget(CBrowserWindow *window, int x, int y)
{
  int hspace = 2;
  int vspace = 2;
  int border = 2;

  /*-------------*/

  int x1 = x + hspace;
  int y1 = y + vspace;

  CHtmlLayoutSubCell *sub_cell = window->getCurrentSubCell();

  if      (align_ == CBROWSER_IMAGE_ALIGN_TOP   )
    y1 += sub_cell->getAscent();
  else if (align_ == CBROWSER_IMAGE_ALIGN_MIDDLE)
    y1 +=  sub_cell->getAscent() - image_->getHeight()/2;
  else if (align_ == CBROWSER_IMAGE_ALIGN_ABSMIDDLE)
    y1 += (sub_cell->getHeight() - image_->getHeight())/2;
  else if (align_ == CBROWSER_IMAGE_ALIGN_ABSBOTTOM)
    y1 += sub_cell->getDescent();
  else
    y1 += 0;

  window->drawImage(x1, y1, image_);

  CRGBA color = window->getDocument()->getLinkColor();

  window->setForeground(color);

  for (int i = 0; i < border; i++)
    window->drawRectangle(x1 - i - 1, y1 - i - 1,
                          image_->getWidth () + 2*i + 1,
                          image_->getHeight() + 2*i + 1);

/*
  int x2 = x1 + image_->getWidth ();
  int y2 = y1 + image_->getHeight();

  link->addRect(x1, y1, x2, y2);
*/
}

/*---------------------*/

void
CBrowserFormPassword::
createWidget(CBrowserWindow *window)
{
  QLineEdit *edit = new QLineEdit(window->getCanvas());

  edit->setText("");

  widget_ = edit;

  //if (size_ > 0) XcwSetValue(edit, XmNcolumns, size_);

  if (maxlength_ > 0) edit->setMaxLength(maxlength_);

  //XcwForceTextUnreadable(edit);

  width_  = widget_->width () + 4;
  height_ = widget_->height() + 4;

  window->updateSubCellHeight(height_/2, height_/2);
  window->updateSubCellWidth (width_);
}

void
CBrowserFormPassword::
drawWidget(CBrowserWindow *, int x, int y)
{
  widget_->move(x + 2, y + 2);
}

void
CBrowserFormPassword::
reset()
{
  QLineEdit *edit = qobject_cast<QLineEdit *>(widget_);

  edit->setText("");
}

void
CBrowserFormPassword::
submit(std::string &url)
{
  QLineEdit *edit = qobject_cast<QLineEdit *>(widget_);

  //char *value = XcwGetUnreadableText(edit);
  QString value = edit->text();

  url += name_;
  url += "=";
  url += value.toStdString();
}

/*---------------------*/

void
CBrowserFormHidden::
createWidget(CBrowserWindow *)
{
}

void
CBrowserFormHidden::
drawWidget(CBrowserWindow *, int, int)
{
}

void
CBrowserFormHidden::
submit(std::string &url)
{
  url += name_;
  url += "=";
  url += value_;
}

/*---------------------*/

void
CBrowserFormText::
createWidget(CBrowserWindow *window)
{
  QLineEdit *edit = new QLineEdit(window->getCanvas());

  edit->setText(value_.c_str());

  widget_ = edit;

  //if (size_ > 0) XcwSetValue(edit, XmNcolumns, size_);

  if (maxlength_ > 0) edit->setMaxLength(maxlength_);

  if (onchange_ != "")
    QObject::connect(edit, SLOT(returnPressed()), this, SLOT(onChangeProc()));

  width_  = widget_->width () + 4;
  height_ = widget_->height() + 4;

  window->updateSubCellHeight(height_/2, height_/2);
  window->updateSubCellWidth (width_);
}

void
CBrowserFormText::
drawWidget(CBrowserWindow *, int x, int y)
{
  widget_->move(x + 2, y + 2);
}

void
CBrowserFormText::
submit(std::string &url)
{
  QLineEdit *edit = qobject_cast<QLineEdit *>(widget_);

  QString value = edit->text();

  url += name_;
  url += "=";
  url += value.toStdString();
}

void
CBrowserFormText::
reset()
{
  QLineEdit *edit = qobject_cast<QLineEdit *>(widget_);

  edit->setText(value_.c_str());
}

/*---------------------*/

void
CBrowserFormTextarea::
createWidget(CBrowserWindow *window)
{
  //XtSetArg(args[0], XmNrows   , rows_);
  //XtSetArg(args[1], XmNcolumns, cols_);

  QTextEdit *edit = new QTextEdit(window->getCanvas());

  edit->setText(value_.c_str());

  widget_ = edit;

  width_  = widget_->width () + 4;
  height_ = widget_->height() + 4;

  window->updateSubCellHeight(height_/2, height_/2);
  window->updateSubCellWidth (width_);
}

void
CBrowserFormTextarea::
drawWidget(CBrowserWindow *, int x, int y)
{
  widget_->parentWidget()->move(x + 2, y + 2);
}

void
CBrowserFormTextarea::
submit(std::string &url)
{
  QTextEdit *edit = qobject_cast<QTextEdit *>(widget_);

  QString value = edit->toPlainText();

  url += name_;
  url += "=";
  url += value.toStdString();
}

void
CBrowserFormTextarea::
reset()
{
  QTextEdit *edit = qobject_cast<QTextEdit *>(widget_);

  edit->setText(value_.c_str());
}

/*---------------------*/

void
CBrowserFormReset::
createWidget(CBrowserWindow *window)
{
  QPushButton *button = new QPushButton(window->getCanvas());

  widget_ = button;

  QObject::connect(button, SIGNAL(clicked()), this, SLOT(resetProc));

  if (value_ != "")
    button->setText(value_.c_str());
  else
    button->setText("Reset");

  width_  = widget_->width () + 4;
  height_ = widget_->height() + 4;

  window->updateSubCellHeight(height_/2, height_/2);
  window->updateSubCellWidth (width_);
}

void
CBrowserFormReset::
drawWidget(CBrowserWindow *, int x, int y)
{
  widget_->move(x + 2, y + 2);
}

/*---------------------*/

void
CBrowserFormSelect::
createWidget(CBrowserWindow *window)
{
  if (multiple_ || size_ != 1) {
    //XtSetArg(args[1], XmNvisibleItemCount, size_);

    QListWidget *list = new QListWidget(window->getCanvas());

    widget_ = list;

    list->setSelectionMode(multiple_ ?
      QAbstractItemView::ExtendedSelection : QAbstractItemView::SingleSelection);

    uint num = option_list_.size();

    for (uint i = 1; i <= num; i++) {
      CBrowserFormOption *option = (CBrowserFormOption *) option_list_[i];

      QListWidgetItem *item = new QListWidgetItem(option->getText().c_str());

      list->addItem(item);

      if (option->getSelected())
        item->setSelected(true);
    }
  }
  else {
    QMenu *menu = new QMenu(window->getCanvas());

    widget_ = menu;

    uint num = getNumOptions();

    for (uint i = 1; i <= num; i++) {
      CBrowserFormOption *option = getOption(i);

      menu->addAction(option->getText().c_str());
    }
  }

  width_  = widget_->width () + 4;
  height_ = widget_->height() + 4;

  window->updateSubCellHeight(height_/2, height_/2);
  window->updateSubCellWidth (width_);
}

void
CBrowserFormSelect::
drawWidget(CBrowserWindow *, int x, int y)
{
  if (multiple_ || size_ != 1)
    widget_->parentWidget()->move(x + 2, y + 2);
  else
    widget_->move(x + 2, y + 2);
}

/*---------------------*/

void
CBrowserFormSubmit::
createWidget(CBrowserWindow *window)
{
  QPushButton *button = new QPushButton(window->getCanvas());

  widget_ = button;

  QObject::connect(button, SLOT(clicked()), form_, SLOT(submitProc()));

  if (onclick_ != "")
    QObject::connect(button, SLOT(clicked()), this, SLOT(onClickProc()));

  if (value_ != "")
    button->setText(value_.c_str());
  else
    button->setText("Submit Query");

  width_  = widget_->width () + 4;
  height_ = widget_->height() + 4;

  window->updateSubCellHeight(height_/2, height_/2);
  window->updateSubCellWidth (width_);
}

void
CBrowserFormSubmit::
drawWidget(CBrowserWindow *, int x, int y)
{
  widget_->move(x + 2, y + 2);
}
