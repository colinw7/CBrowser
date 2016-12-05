#include <CBrowserForm.h>
#include <CBrowserImage.h>
#include <CBrowserDocument.h>
#include <CBrowserWindow.h>
#include <CBrowserIFace.h>
#include <CBrowserWindowWidget.h>
#include <CBrowserCeil.h>
#include <CBrowserJS.h>
#include <CQJLineEdit.h>
#include <CQJColorEdit.h>
#include <CQJComboBox.h>
#include <CQJSlider.h>
#include <CImageLib.h>

#include <QPushButton>
#include <QCheckBox>
#include <QRadioButton>
#include <QListWidget>
#include <QListWidgetItem>
#include <QTextEdit>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QMenu>

CBrowserFormMgr::
CBrowserFormMgr(CBrowserDocument *document) :
 document_(document)
{
}

void
CBrowserFormMgr::
startForm(const std::string &name, CBrowserFormMethodType method, const std::string &action)
{
  if (currentForm())
    endForm();

  CBrowserForm *form = new CBrowserForm(name, document_, method, action);

  document_->addForm(form);

  setCurrentForm(form);
}

void
CBrowserFormMgr::
endForm()
{
  if (! currentForm())
    return;

  setCurrentForm(nullptr);
}

CBrowserFormSelect *
CBrowserFormMgr::
startSelect(const std::string &name, int size, int multiple)
{
  if (! currentForm())
    return nullptr;

  if (currentSelect())
    endSelect();

  CBrowserFormSelect *select = new CBrowserFormSelect(document_, name, "", size, multiple);

  setCurrentSelect(select);

  return select;
}

void
CBrowserFormMgr::
endSelect()
{
  if (! currentForm())
    return;

  setCurrentSelect(nullptr);
}

CBrowserFormOption *
CBrowserFormMgr::
startOption(const std::string &value, bool selected)
{
  if (! currentForm() || ! currentSelect())
    return 0;

  if (currentOption())
    endOption("????");

  CBrowserFormOption *option = new CBrowserFormOption(document_, value, selected);

  setCurrentOption(option);

  return option;
}

void
CBrowserFormMgr::
endOption(const std::string &text)
{
  if (! currentForm() || ! currentSelect() || ! currentOption())
    return;

  if (text != "")
    currentOption()->setText(text);
  else
    currentOption()->setText("????");

  currentSelect()->addOption(currentOption());

  setCurrentOption(nullptr);
}

CBrowserFormTextarea *
CBrowserFormMgr::
startTextarea(const std::string &name, int rows, int cols, const std::string &wrap)
{
  if (! currentForm())
    return nullptr;

  if (currentTextarea())
    endTextarea("");

  CBrowserFormTextAreaWrapType wrap1 = CBrowserFormTextAreaWrapType::OFF;

  if      (CStrUtil::casecmp(wrap, "off"     ) == 0)
    wrap1 = CBrowserFormTextAreaWrapType::OFF;
  else if (CStrUtil::casecmp(wrap, "virtual" ) == 0)
    wrap1 = CBrowserFormTextAreaWrapType::VIRTUAL;
  else if (CStrUtil::casecmp(wrap, "physical") == 0)
    wrap1 = CBrowserFormTextAreaWrapType::PHYSICAL;

  CBrowserFormTextarea *input = new CBrowserFormTextarea(document_, name, "", rows, cols, wrap1);

  currentForm()->addInput(input);

  setCurrentTextarea(input);

  return input;
}

void
CBrowserFormMgr::
endTextarea(const std::string &text)
{
  if (! currentForm() || ! currentTextarea())
    return;

  currentTextarea()->setValue(text);

  setCurrentTextarea(nullptr);
}

CBrowserFormButton *
CBrowserFormMgr::
addButtonInput(const std::string &name, const std::string &value)
{
  if (! currentForm())
    return nullptr;

  CBrowserFormButton *input = new CBrowserFormButton(document_, name, value);

  currentForm()->addInput(input);

  return input;
}

CBrowserFormCheckBox *
CBrowserFormMgr::
addCheckboxInput(const std::string &name, const std::string &value, int checked)
{
  if (! currentForm())
    return nullptr;

  CBrowserFormCheckBox *input = new CBrowserFormCheckBox(document_, name, value, checked);

  currentForm()->addInput(input);

  return input;
}

CBrowserFormFileUpload *
CBrowserFormMgr::
addFileInput(const std::string &name, const std::string &value, int size, int maxlength)
{
  if (! currentForm())
    return nullptr;

  CBrowserFormFileUpload *input =
    new CBrowserFormFileUpload(document_, name, value, size, maxlength);

  currentForm()->addInput(input);

  return input;
}

CBrowserFormHidden *
CBrowserFormMgr::
addHiddenInput(const std::string &name, const std::string &value)
{
  if (! currentForm())
    return nullptr;

  CBrowserFormHidden *input = new CBrowserFormHidden(document_, name, value);

  currentForm()->addInput(input);

  return input;
}

CBrowserFormImage *
CBrowserFormMgr::
addImageInput(const std::string &name, const std::string &src, const std::string &align)
{
  if (! currentForm())
    return nullptr;

  CImageFileSrc file(src);

  CImagePtr image = CImageMgrInst->createImage(file);

  if (! image)
    return nullptr;

  CBrowserImageAlign align1 = CBrowserImageAlign::BOTTOM;

  if      (align == "")
    align1 = CBrowserImageAlign::BOTTOM;
  else if (CStrUtil::casecmp(align, "top"       ) == 0)
    align1 = CBrowserImageAlign::TOP;
  else if (CStrUtil::casecmp(align, "middle"    ) == 0)
    align1 = CBrowserImageAlign::MIDDLE;
  else if (CStrUtil::casecmp(align, "bottom"    ) == 0)
    align1 = CBrowserImageAlign::BOTTOM;
  else if (CStrUtil::casecmp(align, "left"      ) == 0)
    align1 = CBrowserImageAlign::LEFT;
  else if (CStrUtil::casecmp(align, "right"     ) == 0)
    align1 = CBrowserImageAlign::RIGHT;
  else if (CStrUtil::casecmp(align, "texttop"   ) == 0)
    align1 = CBrowserImageAlign::TEXTTOP;
  else if (CStrUtil::casecmp(align, "absmiddle" ) == 0)
    align1 = CBrowserImageAlign::ABSMIDDLE;
  else if (CStrUtil::casecmp(align, "absbottom" ) == 0)
    align1 = CBrowserImageAlign::ABSBOTTOM;

  CBrowserFormImage *input = new CBrowserFormImage(document_, name, "", src, image, align1);

  currentForm()->addInput(input);

  return input;
}

CBrowserFormPassword *
CBrowserFormMgr::
addPasswordInput(const std::string &name, int size, int maxlength)
{
  if (! currentForm())
    return nullptr;

  CBrowserFormPassword *input = new CBrowserFormPassword(document_, name, "", size, maxlength);

  currentForm()->addInput(input);

  return input;
}

CBrowserFormRadio *
CBrowserFormMgr::
addRadioInput(const std::string &name, const std::string &value, int checked)
{
  if (! currentForm())
    return nullptr;

  CBrowserFormRadio *input = new CBrowserFormRadio(document_, name, value, checked);

  currentForm()->addInput(input);

  return input;
}

CBrowserFormRange *
CBrowserFormMgr::
addRangeInput(const std::string &name, const std::string &value, const std::string &min,
              const std::string &max, const std::string &step)
{
  if (! currentForm())
    return nullptr;

  CBrowserFormRange *input = new CBrowserFormRange(document_, name, value, min, max, step);

  currentForm()->addInput(input);

  return input;
}

CBrowserFormReset *
CBrowserFormMgr::
addResetInput(const std::string &name, const std::string &value)
{
  if (! currentForm())
    return nullptr;

  CBrowserFormReset *input = new CBrowserFormReset(document_, name, value);

  currentForm()->addInput(input);

  return input;
}

CBrowserFormSubmit *
CBrowserFormMgr::
addSubmitInput(const std::string &name, const std::string &value)
{
  if (! currentForm())
    return nullptr;

  CBrowserFormSubmit *input = new CBrowserFormSubmit(document_, name, value);

  currentForm()->addInput(input);

  return input;
}

CBrowserFormText *
CBrowserFormMgr::
addTextInput(const std::string &name, const std::string &value, const std::string &classStr,
             int size, int maxlength, const std::string &placeholder)
{
  if (! currentForm())
    return nullptr;

  CBrowserFormText *input =
    new CBrowserFormText(document_, name, value, classStr, size, maxlength, placeholder);

  currentForm()->addInput(input);

  return input;
}

/*---------------------*/

CBrowserFormFileUpload::
CBrowserFormFileUpload(CBrowserDocument *document, const std::string &name,
                       const std::string &value, int size, int maxlength) :
 CBrowserFormInput(document, CHtmlTagId::INPUT, CBrowserFormInputType::FILE, name, value),
 size_(size), maxlength_(maxlength)
{
  if (name_ == "")
    name_ = "file";

  setObjectName(name_.c_str());
}

void
CBrowserFormFileUpload::
createWidget(CBrowserWindow *window)
{
  if (! widget_) {
    QWidget *rowcol = new QWidget(window->widget());

    rowcol->setObjectName(name_.c_str());

    QHBoxLayout *hlayout = new QHBoxLayout(rowcol);

    QLineEdit *edit = new QLineEdit(rowcol);

    edit->setObjectName("edit");

    edit->setText(value_.c_str());

    widget_ = edit;

    hlayout->addWidget(edit);

    if (maxlength_ > 0) edit->setMaxLength(maxlength_);

    QPushButton *button = new QPushButton("button");

    button->setText("Browse ...");

    QObject::connect(button, SIGNAL(clicked()), this, SLOT(buttonProc()));

    hlayout->addWidget(button);

    width_  = rowcol->width () + 4;
    height_ = rowcol->height() + 4;
  }

  window->updateSubCellHeight(height_/2, height_/2);
  window->updateSubCellWidth (width_);
}

void
CBrowserFormFileUpload::
drawWidget(CBrowserWindow *, const CHtmlLayoutRegion &region)
{
  widget_->move(region.x + 2, region.y + 2);
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

CBrowserFormRadio::
CBrowserFormRadio(CBrowserDocument *document, const std::string &name,
                  const std::string &value, int checked) :
 CBrowserFormInput(document, CHtmlTagId::INPUT, CBrowserFormInputType::RADIO_BUTTON,
                   name, value),
 checked_(checked)
{
  if (name_ == "")
    name_ = "radio";

  setObjectName(name_.c_str());
}

void
CBrowserFormRadio::
createWidget(CBrowserWindow *window)
{
  if (! widget_) {
    QRadioButton *radio = new QRadioButton(window->widget());

    radio->setObjectName(name_.c_str());

    radio->setText("");

    widget_ = radio;

    if (checked_)
      radio->setChecked(true);

    QObject::connect(radio, SIGNAL(clicked()), this, SLOT(buttonProc()));

    QSize size = widget_->sizeHint();

    width_  = size.width () + 4;
    height_ = size.height() + 4;
  }

  window->updateSubCellHeight(height_/2, height_/2);
  window->updateSubCellWidth (width_);
}

void
CBrowserFormRadio::
drawWidget(CBrowserWindow *, const CHtmlLayoutRegion &region)
{
  widget_->move(region.x + 2, region.y + 2);
  widget_->resize(width_ - 4, height_ - 4);
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

  for (int i = 0; i < num; i++) {
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

CBrowserFormRange::
CBrowserFormRange(CBrowserDocument *document, const std::string &name,
                  const std::string &value, const std::string &min,
                  const std::string &max, const std::string &step) :
 CBrowserFormInput(document, CHtmlTagId::INPUT, CBrowserFormInputType::RANGE, name, value)
{
  if (name_ == "")
    name_ = "range";

  setObjectName(name_.c_str());

  CStrUtil::toReal(min  , &min_);
  CStrUtil::toReal(max  , &max_);
  CStrUtil::toReal(step , &step_);
  CStrUtil::toReal(value, &value_);
}

double
CBrowserFormRange::
value() const
{
  CQJSlider *slider = dynamic_cast<CQJSlider *>(widget_);

  return (slider ? slider->value() : 0);
}

void
CBrowserFormRange::
createWidget(CBrowserWindow *window)
{
  if (! widget_) {
    CQJSlider *slider = new CQJSlider(window->widget(), this);

    slider->blockSignals(true);

    slider->setRange(min_, max_);
    slider->setSingleStep(step_);

    slider->setValue(value_);

    slider->blockSignals(false);

    slider->setFixedWidth(100);

    slider->setObjectName(name_.c_str());

    widget_ = slider;

    width_  = widget_->width () + 4;
    height_ = widget_->height() + 4;
  }

  window->updateSubCellHeight(height_/2, height_/2);
  window->updateSubCellWidth (width_);
}

void
CBrowserFormRange::
drawWidget(CBrowserWindow *, const CHtmlLayoutRegion &region)
{
  widget_->move(region.x + 2, region.y + 2);
}

/*---------------------*/

CBrowserForm::
CBrowserForm(const std::string &name, CBrowserDocument *document,
             CBrowserFormMethodType method, const std::string &action) :
 name_(name), document_(document), method_(method), action_(action)
{
  target_   = "";
  encoding_ = "";
}

void
CBrowserForm::
resetProc()
{
  int num = getNumInputs();

  for (int i = 0; i < num; i++) {
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

  for (int i = 0; i < num; i++) {
    CBrowserFormInput *input = getInput(i);

    input->submit(url);
  }

  CBrowserWindow *window = getDocument()->getWindow();

  window->setDocument(url);
}

/*---------------------*/

CBrowserFormOption::
CBrowserFormOption(CBrowserDocument *document, const std::string &value, bool selected) :
 CBrowserObject(CHtmlTagId::OPTION), document_(document), value_(value), selected_(selected)
{
  text_ = "";
}

/*---------------------*/

CBrowserFormInput::
CBrowserFormInput(CBrowserDocument *document, CHtmlTagId id, CBrowserFormInputType type,
                  const std::string &name, const std::string &value) :
 CBrowserObject(id), document_(document), type_(type), name_(name), value_(value)
{
  form_ = document->formMgr()->currentForm();

  onblur_   = document->formMgr()->inputOnBlur();
  onclick_  = document->formMgr()->inputOnClick();
  onchange_ = document->formMgr()->inputOnChange();
  onfocus_  = document->formMgr()->inputOnFocus();

  document->formMgr()->setInputOnBlur("");
  document->formMgr()->setInputOnClick("");
  document->formMgr()->setInputOnChange("");
  document->formMgr()->setInputOnFocus("");

  widget_ = nullptr;
  width_  = 0;
  height_ = 0;

  CBrowserWindow *window = document->getWindow();

  window->addCellRedrawData(this);
}

CBrowserFormInput::
~CBrowserFormInput()
{
}

void
CBrowserFormInput::
format(CHtmlLayoutMgr *)
{
  if (getType() == CBrowserFormInputType::HIDDEN)
    return;

  /*----*/

  CBrowserWindow *window = document_->getWindow();

  window->newSubCellRight(true);

  /*----*/

  createWidget(window);

  /*----*/

  window->addSubCellRedrawData(this);
}

void
CBrowserFormInput::
draw(CHtmlLayoutMgr *, const CHtmlLayoutRegion &region)
{
  if (getType() == CBrowserFormInputType::HIDDEN)
    return;

  //---

  CBrowserWindow *window = document_->getWindow();

  drawWidget(window, region);

  //---

  window->drawOutline(region.getX(), region.getY(),
                      region.getWidth(), region.getHeight(), CRGBA(1,0,0));

  //region.x += getWidth();
}

void
CBrowserFormInput::
onClickProc()
{
  CBrowserCeilInst->runScript(getForm()->getDocument()->getWindow(), getOnClick());
}

void
CBrowserFormInput::
onChangeProc()
{
  CBrowserCeilInst->runScript(getForm()->getDocument()->getWindow(), getOnChange());
}

/*---------------------*/

CBrowserFormButton::
CBrowserFormButton(CBrowserDocument *document, const std::string &name,
                   const std::string &value) :
 CBrowserFormInput(document, CHtmlTagId::INPUT, CBrowserFormInputType::BUTTON, name, value)
{
  if (name_ == "")
    name_ = "button";

  setObjectName(name_.c_str());
}

void
CBrowserFormButton::
createWidget(CBrowserWindow *window)
{
  if (! widget_) {
    QPushButton *button = new QPushButton(window->widget());

    button->setObjectName(name_.c_str());

    if (onclick_ != "")
      QObject::connect(button, SIGNAL(clicked()), this, SLOT(onClickProc()));

    if (value_ != "")
      button->setText(value_.c_str());
    else
      button->setText("Button");

    widget_ = button;

    width_  = widget_->width () + 4;
    height_ = widget_->height() + 4;
  }

  window->updateSubCellHeight(height_/2, height_/2);
  window->updateSubCellWidth (width_);
}

void
CBrowserFormButton::
drawWidget(CBrowserWindow *, const CHtmlLayoutRegion &region)
{
  widget_->move(region.x + 2, region.y + 2);
}

/*---------------------*/

CBrowserFormCheckBox::
CBrowserFormCheckBox(CBrowserDocument *document, const std::string &name,
                     const std::string &value, int checked) :
 CBrowserFormInput(document, CHtmlTagId::INPUT, CBrowserFormInputType::CHECKBOX, name, value),
 checked_(checked)
{
  if (name_ == "")
    name_ = "checkbox";

  setObjectName(name_.c_str());
}

void
CBrowserFormCheckBox::
createWidget(CBrowserWindow *window)
{
  if (! widget_) {
    QCheckBox *button = new QCheckBox(window->widget());

    button->setObjectName(name_.c_str());

    widget_ = button;

    button->setText("");

    if (checked_)
      button->setChecked(true);

    width_  = widget_->width () + 4;
    height_ = widget_->height() + 4;
  }

  window->updateSubCellHeight(height_/2, height_/2);
  window->updateSubCellWidth (width_);
}

void
CBrowserFormCheckBox::
drawWidget(CBrowserWindow *, const CHtmlLayoutRegion &region)
{
  widget_->move(region.x + 2, region.y + 2);
}

/*---------------------*/

CBrowserFormImage::
CBrowserFormImage(CBrowserDocument *document, const std::string &name,
                  const std::string &value, const std::string &src,
                  const CImagePtr &image, CBrowserImageAlign align) :
 CBrowserFormInput(document, CHtmlTagId::INPUT, CBrowserFormInputType::IMAGE, name, value),
 src_(src), image_(image), align_(align)
{
  if (name_ == "")
    name_ = "image";

  setObjectName(name_.c_str());
}

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

  if      (align_ == CBrowserImageAlign::LEFT)
    sub_cell->setAlign(CHALIGN_TYPE_LEFT);
  else if (align_ == CBrowserImageAlign::RIGHT)
    sub_cell->setAlign(CHALIGN_TYPE_RIGHT);
  else
    sub_cell->setAlign(CHALIGN_TYPE_NONE);

  /*-------------*/

  if      (align_ == CBrowserImageAlign::TOP    ||
           align_ == CBrowserImageAlign::TEXTTOP)
    window->updateSubCellHeight(vspace, height + vspace);
  else if (align_ == CBrowserImageAlign::MIDDLE ||
           align_ == CBrowserImageAlign::ABSMIDDLE)
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
drawWidget(CBrowserWindow *window, const CHtmlLayoutRegion &region)
{
  int hspace = 2;
  int vspace = 2;
  int border = 2;

  /*-------------*/

  int x1 = region.x + hspace;
  int y1 = region.y + vspace;

  CHtmlLayoutSubCell *sub_cell = window->getCurrentSubCell();

  if      (align_ == CBrowserImageAlign::TOP   )
    y1 += sub_cell->getAscent();
  else if (align_ == CBrowserImageAlign::MIDDLE)
    y1 +=  sub_cell->getAscent() - image_->getHeight()/2;
  else if (align_ == CBrowserImageAlign::ABSMIDDLE)
    y1 += (sub_cell->getHeight() - image_->getHeight())/2;
  else if (align_ == CBrowserImageAlign::ABSBOTTOM)
    y1 += sub_cell->getDescent();
  else
    y1 += 0;

  window->drawImage(x1, y1, image_);

  CRGBA color = window->getDocument()->getLinkColor();

  window->setForeground(color);

  for (int i = 0; i < border; ++i)
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

CBrowserFormPassword::
CBrowserFormPassword(CBrowserDocument *document, const std::string &name,
                     const std::string &value, int size, int maxlength) :
 CBrowserFormInput(document, CHtmlTagId::INPUT, CBrowserFormInputType::PASSWORD_TEXT,
                   name, value),
 size_(size), maxlength_(maxlength)
{
  if (name_ == "")
    name_ = "password";

  setObjectName(name_.c_str());
}

void
CBrowserFormPassword::
createWidget(CBrowserWindow *window)
{
  if (! widget_) {
    QLineEdit *edit = new QLineEdit(window->widget());

    edit->setObjectName(name_.c_str());

    edit->setText("");

    widget_ = edit;

    if (maxlength_ > 0) edit->setMaxLength(maxlength_);

    // TODO: make text unreadable

    width_  = widget_->width () + 4;
    height_ = widget_->height() + 4;
  }

  window->updateSubCellHeight(height_/2, height_/2);
  window->updateSubCellWidth (width_);
}

void
CBrowserFormPassword::
drawWidget(CBrowserWindow *, const CHtmlLayoutRegion &region)
{
  widget_->move(region.x + 2, region.y + 2);
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

  QString value = edit->text();

  url += name_;
  url += "=";
  url += value.toStdString();
}

/*---------------------*/

CBrowserFormHidden::
CBrowserFormHidden(CBrowserDocument *document, const std::string &name,
                   const std::string &value) :
 CBrowserFormInput(document, CHtmlTagId::INPUT, CBrowserFormInputType::HIDDEN, name, value)
{
  if (name_ == "")
    name_ = "hidden";

  setObjectName(name_.c_str());
}

void
CBrowserFormHidden::
createWidget(CBrowserWindow *)
{
}

void
CBrowserFormHidden::
drawWidget(CBrowserWindow *, const CHtmlLayoutRegion &)
{
  //widget_->move(region.x + 2, region.y + 2);
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

CBrowserFormText::
CBrowserFormText(CBrowserDocument *document, const std::string &name, const std::string &value,
                 const std::string &classStr, const int size, int maxlength,
                 const std::string &placeholder) :
 CBrowserFormInput(document, CHtmlTagId::INPUT, CBrowserFormInputType::TEXT, name, value),
 classStr_(classStr), size_(size), maxlength_(maxlength), placeholder_(placeholder)
{
  if (name_ == "")
    name_ = "text";

  setObjectName(name_.c_str());
}

std::string
CBrowserFormText::
text() const
{
  QLineEdit *edit = dynamic_cast<QLineEdit *>(widget_);

  return (edit ? edit->text().toStdString() : "");
}

void
CBrowserFormText::
createWidget(CBrowserWindow *window)
{
  if (! widget_) {
    if (classStr_ == "color") {
      CQJColorEdit *colorEdit = new CQJColorEdit(window->widget(), this);

      colorEdit->setText(value_.c_str());

      if (maxlength_ > 0)
        colorEdit->setMaxLength(maxlength_);

      if (onchange_ != "")
        QObject::connect(colorEdit, SLOT(returnPressed()), this, SLOT(onChangeProc()));

      widget_ = colorEdit;
    }
    else {
      CQJLineEdit *lineEdit = new CQJLineEdit(window->widget(), this);

      lineEdit->setText(value_.c_str());

      lineEdit->setPlaceholderText(placeholder_.c_str());

      if (maxlength_ > 0)
        lineEdit->setMaxLength(maxlength_);

      if (onchange_ != "")
        QObject::connect(lineEdit, SLOT(returnPressed()), this, SLOT(onChangeProc()));

      widget_ = lineEdit;
    }

    QSize size = widget_->sizeHint();

    width_  = size.width () + 4;
    height_ = size.height() + 4;
  }

  window->updateSubCellHeight(height_/2, height_/2);
  window->updateSubCellWidth (width_);
}

void
CBrowserFormText::
drawWidget(CBrowserWindow *, const CHtmlLayoutRegion &region)
{
  widget_->move  (region.x + 2, region.y + 2);
  widget_->resize(width_ - 4, height_ - 4);
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

CBrowserFormTextarea::
CBrowserFormTextarea(CBrowserDocument *document, const std::string &name,
                     const std::string &value, int rows, int cols,
                     CBrowserFormTextAreaWrapType wrap) :
 CBrowserFormInput(document, CHtmlTagId::TEXTAREA, CBrowserFormInputType::TEXTAREA, name, value),
 rows_(rows), cols_(cols), wrap_(wrap)
{
  if (name_ == "")
    name_ = "textarea";

  setObjectName(name_.c_str());
}

void
CBrowserFormTextarea::
createWidget(CBrowserWindow *window)
{
  if (! widget_) {
    QTextEdit *edit = new QTextEdit(window->widget());

    edit->setObjectName(name_.c_str());

    edit->setText(value_.c_str());

    widget_ = edit;

    width_  = widget_->width () + 4;
    height_ = widget_->height() + 4;
  }

  window->updateSubCellHeight(height_/2, height_/2);
  window->updateSubCellWidth (width_);
}

void
CBrowserFormTextarea::
drawWidget(CBrowserWindow *, const CHtmlLayoutRegion &region)
{
  widget_->move(region.x + 2, region.y + 2);
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

CBrowserFormReset::
CBrowserFormReset(CBrowserDocument *document, const std::string &name,
                  const std::string &value) :
 CBrowserFormInput(document, CHtmlTagId::INPUT, CBrowserFormInputType::RESET_BUTTON,
                   name, value)
{
  if (name_ == "")
    name_ = "input";

  setObjectName(name_.c_str());
}

void
CBrowserFormReset::
createWidget(CBrowserWindow *window)
{
  if (! widget_) {
    QPushButton *button = new QPushButton(window->widget());

    button->setObjectName(name_.c_str());

    widget_ = button;

    QObject::connect(button, SIGNAL(clicked()), this, SLOT(resetProc()));

    if (value_ != "")
      button->setText(value_.c_str());
    else
      button->setText("Reset");

    width_  = widget_->width () + 4;
    height_ = widget_->height() + 4;
  }

  window->updateSubCellHeight(height_/2, height_/2);
  window->updateSubCellWidth (width_);
}

void
CBrowserFormReset::
drawWidget(CBrowserWindow *, const CHtmlLayoutRegion &region)
{
  widget_->move(region.x + 2, region.y + 2);
}

/*---------------------*/

CBrowserFormSelect::
CBrowserFormSelect(CBrowserDocument *document, const std::string &name,
                   const std::string &value, int size, int multiple) :
 CBrowserFormInput(document, CHtmlTagId::INPUT, CBrowserFormInputType::SELECT, name, value),
 size_(size), multiple_(multiple)
{
  if (name_ == "")
    name_ = "select";

  setObjectName(name_.c_str());
}

std::string
CBrowserFormSelect::
value() const
{
  if (multiple_ || size_ != 1) {
    return "";
  }
  else {
    CQJComboBox *combo = dynamic_cast<CQJComboBox *>(widget_);
    if (! combo) return "";

    int ind = combo->currentIndex();

    CBrowserFormOption *option = getOption(ind);

    std::string value = option->getValue();
    if (value != "") return value;

    std::string text = option->getText();
    if (text != "") return text;

    return combo->currentText().toStdString();
  }
}

void
CBrowserFormSelect::
createWidget(CBrowserWindow *window)
{
  if (! widget_) {
    if (multiple_ || size_ != 1) {
      QListWidget *list = new QListWidget(window->widget());

      list->setObjectName(name_.c_str());

      widget_ = list;

      list->setSelectionMode(multiple_ ?
        QAbstractItemView::ExtendedSelection : QAbstractItemView::SingleSelection);

      uint num = getNumOptions();

      for (uint i = 0; i < num; ++i) {
        CBrowserFormOption *option = getOption(i);

        QListWidgetItem *item = new QListWidgetItem(option->getText().c_str());

        list->addItem(item);

        if (option->getSelected())
          item->setSelected(true);
      }
    }
    else {
      CQJComboBox *combo = new CQJComboBox(window->widget(), this);

      combo->setObjectName(name_.c_str());

      widget_ = combo;

      uint num = getNumOptions();

      for (uint i = 0; i < num; ++i) {
        CBrowserFormOption *option = getOption(i);

        combo->addItem(option->getText().c_str());
      }
    }

    QSize size = widget_->sizeHint();

    width_  = size.width () + 4;
    height_ = size.height() + 4;
  }

  window->updateSubCellHeight(height_/2, height_/2);
  window->updateSubCellWidth (width_);
}

void
CBrowserFormSelect::
drawWidget(CBrowserWindow *, const CHtmlLayoutRegion &region)
{
  widget_->move(region.x + 2, region.y + 2);
  widget_->resize(width_ - 4, height_ - 4);
}

/*---------------------*/

CBrowserFormSubmit::
CBrowserFormSubmit(CBrowserDocument *document, const std::string &name,
                   const std::string &value) :
 CBrowserFormInput(document, CHtmlTagId::INPUT, CBrowserFormInputType::SUBMIT_BUTTON,
                   name, value)
{
  if (name_ == "")
    name_ = "submit";

  setObjectName(name_.c_str());
}

void
CBrowserFormSubmit::
createWidget(CBrowserWindow *window)
{
  if (! widget_) {
    QPushButton *button = new QPushButton(window->widget());

    button->setObjectName(name_.c_str());

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
  }

  window->updateSubCellHeight(height_/2, height_/2);
  window->updateSubCellWidth (width_);
}

void
CBrowserFormSubmit::
drawWidget(CBrowserWindow *, const CHtmlLayoutRegion &region)
{
  widget_->move(region.x + 2, region.y + 2);
}
