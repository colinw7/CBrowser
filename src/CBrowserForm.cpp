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
#include <CQUtil.h>
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

namespace {

void initWidget(const CBrowserObject *obj, QWidget *w) {
  if (obj->background().color().isValid())
    CQUtil::setBackground(w, CQUtil::toQColor(obj->background().color().color()));

  w->setFont(CQUtil::toQFont(obj->getWindow()->getFont()));

  //if (obj->width().isValid())
  //  w->setFixedWidth(obj->width().value());
}

}

//---

CBrowserFormFileUpload::
CBrowserFormFileUpload(CBrowserWindow *window, const CBrowserFormInputData &data) :
 CBrowserFormInput(window, CHtmlTagId::INPUT, CBrowserFormInputType::FILE, data)
{
  init();
}

void
CBrowserFormFileUpload::
init()
{
  if (data_.name == "")
    data_.name = "file";

  setObjectName(data_.name.c_str());
}

void
CBrowserFormFileUpload::
createWidget() const
{
  if (! widget_) {
    QWidget *rowcol = new QWidget(window_->widget());

    rowcol->setObjectName(data_.name.c_str());

    QHBoxLayout *hlayout = new QHBoxLayout(rowcol);

    QLineEdit *edit = new QLineEdit(rowcol);

    edit->setObjectName("edit");

    edit->setText(data_.value.c_str());

    widget_ = edit;

    initWidget(this, widget_);

    hlayout->addWidget(edit);

    if (maxlength_ > 0) edit->setMaxLength(maxlength_);

    QPushButton *button = new QPushButton("button");

    button->setText("Browse ...");

    initWidget(this, button);

    QObject::connect(button, SIGNAL(clicked()), this, SLOT(buttonProc()));

    hlayout->addWidget(button);
  }
}

CBrowserRegion
CBrowserFormFileUpload::
calcRegion() const
{
  createWidget();

  if (! widget_)
    return CBrowserRegion();

  //---

  QSize size = widget_->sizeHint();

  width_  = size.width () + 4;
  height_ = size.height() + 4;

  return CBrowserRegion(width_, height_/2, height_/2);
}

void
CBrowserFormFileUpload::
drawWidget(CBrowserWindow *, const CTextBox &region)
{
  QWidget *container = widget_->parentWidget();

  container->move(region.x() + 2, region.y() + 2);
  container->resize(width_ - 4, height_ - 4);
}

void
CBrowserFormFileUpload::
buttonProc()
{
  QString filename = QFileDialog::getOpenFileName(widget_, "Select File");

  QLineEdit *edit = qobject_cast<QLineEdit *>(widget_);

  edit->setText(filename);
}

//---

CBrowserFormRadio::
CBrowserFormRadio(CBrowserWindow *window, const CBrowserFormInputData &data) :
 CBrowserFormInput(window, CHtmlTagId::INPUT, CBrowserFormInputType::RADIO_BUTTON, data)
{
  if (data_.name == "")
    data_.name = "radio";

  setObjectName(data_.name.c_str());
}

void
CBrowserFormRadio::
createWidget() const
{
  if (! widget_) {
    QRadioButton *radio = new QRadioButton(window_->widget());

    radio->setObjectName(data_.name.c_str());

    radio->setText("");

    widget_ = radio;

    if (checked_)
      radio->setChecked(true);

    QObject::connect(radio, SIGNAL(clicked()), this, SLOT(buttonProc()));
  }
}

CBrowserRegion
CBrowserFormRadio::
calcRegion() const
{
  createWidget();

  if (! widget_)
    return CBrowserRegion();

  //---

  QSize size = widget_->sizeHint();

  width_  = size.width () + 4;
  height_ = size.height() + 4;

  return CBrowserRegion(width_, height_/2, height_/2);
}

void
CBrowserFormRadio::
drawWidget(CBrowserWindow *, const CTextBox &region)
{
  widget_->move(region.x() + 2, region.y() + 2);
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

    QWidget *widget1 = const_cast<QWidget *>(input1->getWidget());

    QRadioButton *radio1 = qobject_cast<QRadioButton *>(widget1);

    radio1->setChecked(false);
  }
}

//---

CBrowserFormRange::
CBrowserFormRange(CBrowserWindow *window, const CBrowserFormInputData &data) :
 CBrowserFormInput(window, CHtmlTagId::INPUT, CBrowserFormInputType::RANGE, data)
{
  if (data_.name == "")
    data_.name = "range";

  setObjectName(data_.name.c_str());

  CStrUtil::toReal(data_.min  , &min_);
  CStrUtil::toReal(data_.max  , &max_);
  CStrUtil::toReal(data_.step , &step_);
  CStrUtil::toReal(data_.value, &value_);
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
createWidget() const
{
  if (! widget_) {
    CBrowserFormRange *th = const_cast<CBrowserFormRange *>(this);

    CQJSlider *slider = new CQJSlider(window_->widget(), th);

    slider->blockSignals(true);

    slider->setRange(min_, max_);
    slider->setSingleStep(step_);

    slider->setValue(value_);

    slider->blockSignals(false);

    slider->setObjectName(data_.name.c_str());

    widget_ = slider;
  }
}

CBrowserRegion
CBrowserFormRange::
calcRegion() const
{
  createWidget();

  if (! widget_)
    return CBrowserRegion();

  //---

  QSize size = widget_->sizeHint();

  width_  = size.width () + 4;
  height_ = size.height() + 4;

  return CBrowserRegion(width_, height_/2, height_/2);
}

void
CBrowserFormRange::
drawWidget(CBrowserWindow *, const CTextBox &region)
{
  widget_->move(region.x() + 2, region.y() + 2);
  widget_->resize(width_ - 4, height_ - 4);
}

//---

CBrowserForm::
CBrowserForm(CBrowserWindow *window, const CBrowserFormData &data) :
 CBrowserObject(window, CHtmlTagId::FORM, data), data_(data)
{
  CBrowserDocument *document = window_->getDocument();

  document->addForm(this);
}

CBrowserForm::
~CBrowserForm()
{
  //CBrowserDocument *document = window_->getDocument();

  //document->removeForm(this);
}

void
CBrowserForm::
init()
{
}

void
CBrowserForm::
setNameValue(const std::string &name, const std::string &value)
{
  std::string lname  = CStrUtil::toLower(name);
  std::string lvalue = CStrUtil::toLower(value);

  if      (lname == "action") {
    data_.action = value;
  }
  else if (lname == "enctype") {
  }
  else if (lname == "method") {
    if      (lvalue == "get" )
      data_.method = CBrowserFormMethodType::GET;
    else if (lvalue == "post")
      data_.method = CBrowserFormMethodType::POST;
    else
      window_->displayError("Illegal 'form' method '%s'\n", value.c_str());
  }
  else if (lname == "name") {
    data_.name = value;
  }
  else {
    CBrowserObject::setNameValue(name, value);
  }
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

  window_->setDocument(url);
}

//---

CBrowserFormOption::
CBrowserFormOption(CBrowserWindow *window, const CBrowserFormOptionData &data) :
 CBrowserObject(window, CHtmlTagId::OPTION), data_(data)
{
  setDisplay(Display::INLINE);

  text_ = "";
}

void
CBrowserFormOption::
initProcess()
{
  CBrowserFormSelect *select = parentType<CBrowserFormSelect>();

  if (select)
    select->addOption(this);
}

void
CBrowserFormOption::
termProcess()
{
}

//---

CBrowserFormInput::
CBrowserFormInput(CBrowserWindow *window, CHtmlTagId id, CBrowserFormInputType type,
                  const CBrowserFormInputData &data) :
 CBrowserObject(window, id, data), type_(type), data_(data)
{
  widget_ = nullptr;
  width_  = 0;
  height_ = 0;
}

CBrowserFormInput::
~CBrowserFormInput()
{
}

void
CBrowserFormInput::
initProcess()
{
  form_ = parentType<CBrowserForm>();

  if (form_)
    form_->addInput(this);
}

void
CBrowserFormInput::
termProcess()
{
}

void
CBrowserFormInput::
getInlineWords(Words &words) const
{
  CBrowserFormInput *th = const_cast<CBrowserFormInput *>(this);

  words.push_back(CBrowserWord(th, /*break*/false, isHierSelected()));
}

void
CBrowserFormInput::
draw(const CTextBox &region)
{
  if (getType() == CBrowserFormInputType::HIDDEN)
    return;

  //---

  fillBackground(region);

  drawWidget(window_, region);

  //---

  window_->drawOutline(region.x(), region.y(), region.width(), region.height(),
                       CPen(CRGBA(1,0,0)));

  //region.setX(region.x() + getWidth());
}

void
CBrowserFormInput::
onClickProc()
{
  CBrowserCeilInst->runScript(getForm()->getWindow(), getOnClick());
}

void
CBrowserFormInput::
onChangeProc()
{
  CBrowserCeilInst->runScript(getForm()->getWindow(), getOnChange());
}

//---

CBrowserFormButton::
CBrowserFormButton(CBrowserWindow *window, const CBrowserFormButtonData &data) :
 CBrowserFormInput(window, CHtmlTagId::BUTTON, CBrowserFormInputType::BUTTON, data)
{
  if (data_.name == "")
    data_.name = "button";

  setObjectName(data_.name.c_str());
}

CBrowserFormButton::
CBrowserFormButton(CBrowserWindow *window, const CBrowserFormInputData &data) :
 CBrowserFormInput(window, CHtmlTagId::INPUT, CBrowserFormInputType::BUTTON, data)
{
  if (data_.name == "")
    data_.name = "button";

  setObjectName(data_.name.c_str());
}

void
CBrowserFormButton::
createWidget() const
{
  if (! widget_) {
    QPushButton *button = new QPushButton(window_->widget());

    button->setObjectName(data_.name.c_str());

    if (data_.onclick != "")
      QObject::connect(button, SIGNAL(clicked()), this, SLOT(onClickProc()));

    if (data_.value != "")
      button->setText(data_.value.c_str());
    else
      button->setText("Button");

    widget_ = button;

    initWidget(this, widget_);
  }
}

CBrowserRegion
CBrowserFormButton::
calcRegion() const
{
  createWidget();

  if (! widget_)
    return CBrowserRegion();

  //---

  QSize size = widget_->sizeHint();

  width_  = size.width () + 4;
  height_ = size.height() + 4;

  return CBrowserRegion(width_, height_/2, height_/2);
}

void
CBrowserFormButton::
drawWidget(CBrowserWindow *, const CTextBox &region)
{
  widget_->move(region.x() + 2, region.y() + 2);
  widget_->resize(width_ - 4, height_ - 4);
}

//---

CBrowserFormCheckBox::
CBrowserFormCheckBox(CBrowserWindow *window, const CBrowserFormInputData &data) :
 CBrowserFormInput(window, CHtmlTagId::INPUT, CBrowserFormInputType::CHECKBOX, data)
{
  if (data_.name == "")
    data_.name = "checkbox";

  setObjectName(data_.name.c_str());
}

void
CBrowserFormCheckBox::
createWidget() const
{
  if (! widget_) {
    QCheckBox *button = new QCheckBox(window_->widget());

    button->setObjectName(data_.name.c_str());

    widget_ = button;

    button->setText("");

    if (checked_)
      button->setChecked(true);

    initWidget(this, widget_);
  }
}

CBrowserRegion
CBrowserFormCheckBox::
calcRegion() const
{
  createWidget();

  if (! widget_)
    return CBrowserRegion();

  //---

  QSize size = widget_->sizeHint();

  width_  = size.width () + 4;
  height_ = size.height() + 4;

  return CBrowserRegion(width_, height_/2, height_/2);
}

void
CBrowserFormCheckBox::
drawWidget(CBrowserWindow *, const CTextBox &region)
{
  widget_->move(region.x() + 2, region.y() + 2);
  widget_->resize(width_ - 4, height_ - 4);
}

//---

CBrowserFormImage::
CBrowserFormImage(CBrowserWindow *window, const CBrowserFormInputData &data) :
 CBrowserFormInput(window, CHtmlTagId::INPUT, CBrowserFormInputType::IMAGE, data),
 align_(CBrowserImageAlign::BOTTOM)
{
  if (data_.name == "")
    data_.name = "image";

  align_ = CBrowserImageAlign::BOTTOM;

  if      (CStrUtil::casecmp(data_.align, "top"       ) == 0)
    align_ = CBrowserImageAlign::TOP;
  else if (CStrUtil::casecmp(data_.align, "middle"    ) == 0)
    align_ = CBrowserImageAlign::MIDDLE;
  else if (CStrUtil::casecmp(data_.align, "bottom"    ) == 0)
    align_ = CBrowserImageAlign::BOTTOM;
  else if (CStrUtil::casecmp(data_.align, "left"      ) == 0)
    align_ = CBrowserImageAlign::LEFT;
  else if (CStrUtil::casecmp(data_.align, "right"     ) == 0)
    align_ = CBrowserImageAlign::RIGHT;
  else if (CStrUtil::casecmp(data_.align, "texttop"   ) == 0)
    align_ = CBrowserImageAlign::TEXTTOP;
  else if (CStrUtil::casecmp(data_.align, "absmiddle" ) == 0)
    align_ = CBrowserImageAlign::ABSMIDDLE;
  else if (CStrUtil::casecmp(data_.align, "absbottom" ) == 0)
    align_ = CBrowserImageAlign::ABSBOTTOM;

  CImageFileSrc file(data_.src);

  image_ = CImageMgrInst->createImage(file);

  setObjectName(data_.name.c_str());
}

void
CBrowserFormImage::
createWidget() const
{
}

CBrowserRegion
CBrowserFormImage::
calcRegion() const
{
  int hspace = 2;
  int vspace = 2;
  int width  = image_->getWidth ();
  int height = image_->getHeight();

  width_  = width  + 2*hspace;
  height_ = height + 2*vspace;

#if 0
  if      (align_ == CBrowserImageAlign::TOP    ||
           align_ == CBrowserImageAlign::TEXTTOP)
    return CBrowserRegion(width_, vspace, height + vspace);
  else if (align_ == CBrowserImageAlign::MIDDLE ||
           align_ == CBrowserImageAlign::ABSMIDDLE)
    return CBrowserRegion(width_, (height + 1)/2 + vspace, height/2 + vspace);
  else
    return CBrowserRegion(width_, height + vspace, vspace);
#endif
   return CBrowserRegion(width_, height_ - vspace, vspace);
}

void
CBrowserFormImage::
drawWidget(CBrowserWindow *window, const CTextBox &region)
{
  int hspace = 2;
  int vspace = 2;
  int border = 2;

  //---

  int dy = height_ - region.height();

  int x1 = region.x() + hspace;
  int y1 = region.y() + vspace;

  if      (align_ == CBrowserImageAlign::TOP)
    y1 += 0;
  else if (align_ == CBrowserImageAlign::MIDDLE)
    y1 += dy; // TODO: ascent
  else if (align_ == CBrowserImageAlign::BOTTOM)
    y1 += dy; // TODO: ascent
  else if (align_ == CBrowserImageAlign::TEXTTOP)
    y1 += 0;
  else if (align_ == CBrowserImageAlign::ABSMIDDLE)
    y1 += dy/2;
  else if (align_ == CBrowserImageAlign::ABSBOTTOM)
    y1 += dy;
  else
    y1 += 0;

  window->drawImage(x1, y1, image_);

  CRGBA color = window->getDocument()->getLinkColor();

  for (int i = 0; i < border; ++i)
    window->drawRectangle(x1 - i - 1, y1 - i - 1,
                          image_->getWidth () + 2*i + 1,
                          image_->getHeight() + 2*i + 1,
                          CPen(color));

/*
  int x2 = x1 + image_->getWidth ();
  int y2 = y1 + image_->getHeight();

  link->addRect(x1, y1, x2, y2);
*/
}

//---

CBrowserFormTel::
CBrowserFormTel(CBrowserWindow *window, const CBrowserFormInputData &data) :
 CBrowserFormInput(window, CHtmlTagId::INPUT, CBrowserFormInputType::TEL, data)
{
  if (data_.name == "")
    data_.name = "tel";

  setObjectName(data_.name.c_str());
}

void
CBrowserFormTel::
createWidget() const
{
  if (! widget_) {
    QLineEdit *edit = new QLineEdit(window_->widget());

    edit->setObjectName(data_.name.c_str());

    edit->setText("");

    widget_ = edit;

    if (maxlength_ > 0) edit->setMaxLength(maxlength_);

    initWidget(this, widget_);
  }
}

CBrowserRegion
CBrowserFormTel::
calcRegion() const
{
  createWidget();

  if (! widget_)
    return CBrowserRegion();

  //---

  QSize size = widget_->sizeHint();

  width_  = size.width () + 4;
  height_ = size.height() + 4;

  return CBrowserRegion(width_, height_/2, height_/2);
}

void
CBrowserFormTel::
drawWidget(CBrowserWindow *, const CTextBox &region)
{
  widget_->move(region.x() + 2, region.y() + 2);
  widget_->resize(width_ - 4, height_ - 4);
}

void
CBrowserFormTel::
reset()
{
  QLineEdit *edit = qobject_cast<QLineEdit *>(widget_);

  edit->setText("");
}

void
CBrowserFormTel::
submit(std::string &url)
{
  QLineEdit *edit = qobject_cast<QLineEdit *>(widget_);

  QString value = edit->text();

  url += data_.name;
  url += "=";
  url += value.toStdString();
}

//---

CBrowserFormMonth::
CBrowserFormMonth(CBrowserWindow *window, const CBrowserFormInputData &data) :
 CBrowserFormInput(window, CHtmlTagId::INPUT, CBrowserFormInputType::MONTH, data)
{
  if (data_.name == "")
    data_.name = "month";

  setObjectName(data_.name.c_str());
}

void
CBrowserFormMonth::
createWidget() const
{
  if (! widget_) {
    QLineEdit *edit = new QLineEdit(window_->widget());

    edit->setObjectName(data_.name.c_str());

    edit->setText("");

    widget_ = edit;

    if (maxlength_ > 0) edit->setMaxLength(maxlength_);

    initWidget(this, widget_);
  }
}

CBrowserRegion
CBrowserFormMonth::
calcRegion() const
{
  createWidget();

  if (! widget_)
    return CBrowserRegion();

  //---

  QSize size = widget_->sizeHint();

  width_  = size.width () + 4;
  height_ = size.height() + 4;

  return CBrowserRegion(width_, height_/2, height_/2);
}

void
CBrowserFormMonth::
drawWidget(CBrowserWindow *, const CTextBox &region)
{
  widget_->move(region.x() + 2, region.y() + 2);
  widget_->resize(width_ - 4, height_ - 4);
}

void
CBrowserFormMonth::
reset()
{
  QLineEdit *edit = qobject_cast<QLineEdit *>(widget_);

  edit->setText("");
}

void
CBrowserFormMonth::
submit(std::string &url)
{
  QLineEdit *edit = qobject_cast<QLineEdit *>(widget_);

  QString value = edit->text();

  url += data_.name;
  url += "=";
  url += value.toStdString();
}

//---

CBrowserFormDate::
CBrowserFormDate(CBrowserWindow *window, const CBrowserFormInputData &data) :
 CBrowserFormInput(window, CHtmlTagId::INPUT, CBrowserFormInputType::MONTH, data)
{
  if (data_.name == "")
    data_.name = "date";

  setObjectName(data_.name.c_str());
}

void
CBrowserFormDate::
createWidget() const
{
  if (! widget_) {
    QLineEdit *edit = new QLineEdit(window_->widget());

    edit->setObjectName(data_.name.c_str());

    edit->setText("");

    widget_ = edit;

    if (maxlength_ > 0) edit->setMaxLength(maxlength_);

    initWidget(this, widget_);
  }
}

CBrowserRegion
CBrowserFormDate::
calcRegion() const
{
  createWidget();

  if (! widget_)
    return CBrowserRegion();

  //---

  QSize size = widget_->sizeHint();

  width_  = size.width () + 4;
  height_ = size.height() + 4;

  return CBrowserRegion(width_, height_/2, height_/2);
}

void
CBrowserFormDate::
drawWidget(CBrowserWindow *, const CTextBox &region)
{
  widget_->move(region.x() + 2, region.y() + 2);
  widget_->resize(width_ - 4, height_ - 4);
}

void
CBrowserFormDate::
reset()
{
  QLineEdit *edit = qobject_cast<QLineEdit *>(widget_);

  edit->setText("");
}

void
CBrowserFormDate::
submit(std::string &url)
{
  QLineEdit *edit = qobject_cast<QLineEdit *>(widget_);

  QString value = edit->text();

  url += data_.name;
  url += "=";
  url += value.toStdString();
}

//---

CBrowserFormSearch::
CBrowserFormSearch(CBrowserWindow *window, const CBrowserFormInputData &data) :
 CBrowserFormInput(window, CHtmlTagId::INPUT, CBrowserFormInputType::MONTH, data)
{
  if (data_.name == "")
    data_.name = "search";

  setObjectName(data_.name.c_str());
}

void
CBrowserFormSearch::
createWidget() const
{
  if (! widget_) {
    QLineEdit *edit = new QLineEdit(window_->widget());

    edit->setObjectName(data_.name.c_str());

    edit->setText("");

    widget_ = edit;

    if (maxlength_ > 0) edit->setMaxLength(maxlength_);

    initWidget(this, widget_);
  }
}

CBrowserRegion
CBrowserFormSearch::
calcRegion() const
{
  createWidget();

  if (! widget_)
    return CBrowserRegion();

  //---

  QSize size = widget_->sizeHint();

  width_  = size.width () + 4;
  height_ = size.height() + 4;

  return CBrowserRegion(width_, height_/2, height_/2);
}

void
CBrowserFormSearch::
drawWidget(CBrowserWindow *, const CTextBox &region)
{
  widget_->move(region.x() + 2, region.y() + 2);
  widget_->resize(width_ - 4, height_ - 4);
}

void
CBrowserFormSearch::
reset()
{
  QLineEdit *edit = qobject_cast<QLineEdit *>(widget_);

  edit->setText("");
}

void
CBrowserFormSearch::
submit(std::string &url)
{
  QLineEdit *edit = qobject_cast<QLineEdit *>(widget_);

  QString value = edit->text();

  url += data_.name;
  url += "=";
  url += value.toStdString();
}

//---

CBrowserFormNumber::
CBrowserFormNumber(CBrowserWindow *window, const CBrowserFormInputData &data) :
 CBrowserFormInput(window, CHtmlTagId::INPUT, CBrowserFormInputType::NUMBER, data)
{
  if (data_.name == "")
    data_.name = "number";

  setObjectName(data_.name.c_str());
}

void
CBrowserFormNumber::
createWidget() const
{
  if (! widget_) {
    CBrowserFormNumber *th = const_cast<CBrowserFormNumber *>(this);

    CQJLineEdit *edit = new CQJLineEdit(window_->widget(), th);

    edit->setText(data_.value.c_str());

    edit->setPlaceholderText(data_.placeholder.c_str());

    if (maxlength_ > 0)
      edit->setMaxLength(maxlength_);

    //if (data_.onchange != "")
    //  QObject::connect(edit, SLOT(returnPressed()), this, SLOT(onChangeProc()));

    widget_ = edit;

    initWidget(this, widget_);
  }
}

CBrowserRegion
CBrowserFormNumber::
calcRegion() const
{
  createWidget();

  if (! widget_)
    return CBrowserRegion();

  //---

  QSize size = widget_->sizeHint();

  width_  = size.width () + 4;
  height_ = size.height() + 4;

  return CBrowserRegion(width_, height_/2, height_/2);
}

void
CBrowserFormNumber::
drawWidget(CBrowserWindow *, const CTextBox &region)
{
  widget_->move(region.x() + 2, region.y() + 2);
  widget_->resize(width_ - 4, height_ - 4);
}

void
CBrowserFormNumber::
reset()
{
  QLineEdit *edit = qobject_cast<QLineEdit *>(widget_);

  edit->setText("");
}

void
CBrowserFormNumber::
submit(std::string &url)
{
  QLineEdit *edit = qobject_cast<QLineEdit *>(widget_);

  QString value = edit->text();

  url += data_.name;
  url += "=";
  url += value.toStdString();
}

//---

CBrowserFormEmail::
CBrowserFormEmail(CBrowserWindow *window, const CBrowserFormInputData &data) :
 CBrowserFormInput(window, CHtmlTagId::INPUT, CBrowserFormInputType::EMAIL, data)
{
  if (data_.name == "")
    data_.name = "email";

  setObjectName(data_.name.c_str());
}

std::string
CBrowserFormEmail::
text() const
{
  QLineEdit *edit = dynamic_cast<QLineEdit *>(widget_);

  return (edit ? edit->text().toStdString() : "");
}

void
CBrowserFormEmail::
createWidget() const
{
  if (! widget_) {
    QLineEdit *edit = new QLineEdit(window_->widget());

    edit->setObjectName(data_.name.c_str());

    edit->setText("");

    widget_ = edit;

    if (maxlength_ > 0) edit->setMaxLength(maxlength_);

    initWidget(this, widget_);
  }
}

CBrowserRegion
CBrowserFormEmail::
calcRegion() const
{
  createWidget();

  if (! widget_)
    return CBrowserRegion();

  //---

  QSize size = widget_->sizeHint();

  width_  = size.width () + 4;
  height_ = size.height() + 4;

  return CBrowserRegion(width_, height_/2, height_/2);
}

void
CBrowserFormEmail::
drawWidget(CBrowserWindow *, const CTextBox &region)
{
  widget_->move(region.x() + 2, region.y() + 2);
  widget_->resize(width_ - 4, height_ - 4);
}

void
CBrowserFormEmail::
reset()
{
  QLineEdit *edit = qobject_cast<QLineEdit *>(widget_);

  edit->setText("");
}

void
CBrowserFormEmail::
submit(std::string &url)
{
  QLineEdit *edit = qobject_cast<QLineEdit *>(widget_);

  QString value = edit->text();

  url += data_.name;
  url += "=";
  url += value.toStdString();
}

//---

CBrowserFormPassword::
CBrowserFormPassword(CBrowserWindow *window, const CBrowserFormInputData &data) :
 CBrowserFormInput(window, CHtmlTagId::INPUT, CBrowserFormInputType::PASSWORD_TEXT, data)
{
  if (data_.name == "")
    data_.name = "password";

  setObjectName(data_.name.c_str());
}

void
CBrowserFormPassword::
createWidget() const
{
  if (! widget_) {
    QLineEdit *edit = new QLineEdit(window_->widget());

    edit->setObjectName(data_.name.c_str());

    edit->setText("");

    widget_ = edit;

    if (maxlength_ > 0) edit->setMaxLength(maxlength_);

    // TODO: make text unreadable

    initWidget(this, widget_);
  }
}

CBrowserRegion
CBrowserFormPassword::
calcRegion() const
{
  createWidget();

  if (! widget_)
    return CBrowserRegion();

  //---

  QSize size = widget_->sizeHint();

  width_  = size.width () + 4;
  height_ = size.height() + 4;

  return CBrowserRegion(width_, height_/2, height_/2);
}

void
CBrowserFormPassword::
drawWidget(CBrowserWindow *, const CTextBox &region)
{
  widget_->move(region.x() + 2, region.y() + 2);
  widget_->resize(width_ - 4, height_ - 4);
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

  url += data_.name;
  url += "=";
  url += value.toStdString();
}

//---

CBrowserFormHidden::
CBrowserFormHidden(CBrowserWindow *window, const CBrowserFormInputData &data) :
 CBrowserFormInput(window, CHtmlTagId::INPUT, CBrowserFormInputType::HIDDEN, data)
{
  if (data_.name == "")
    data_.name = "hidden";

  setObjectName(data_.name.c_str());
}

void
CBrowserFormHidden::
createWidget() const
{
}

void
CBrowserFormHidden::
drawWidget(CBrowserWindow *, const CTextBox &)
{
  //widget_->move(region.x() + 2, region.y() + 2);
  //widget_->resize(width_ - 4, height_ - 4);
}

void
CBrowserFormHidden::
submit(std::string &url)
{
  url += data_.name;
  url += "=";
  url += data_.value;
}

//---

CBrowserFormText::
CBrowserFormText(CBrowserWindow *window, const CBrowserFormInputData &data) :
 CBrowserFormInput(window, CHtmlTagId::INPUT, CBrowserFormInputType::TEXT, data)
{
  if (data_.name == "")
    data_.name = "text";

  setObjectName(data_.name.c_str());
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
createWidget() const
{
  if (! widget_) {
    CBrowserFormText *th = const_cast<CBrowserFormText *>(this);

    if (classStr_ == "color") {
      CQJColorEdit *colorEdit = new CQJColorEdit(window_->widget(), th);

      colorEdit->setText(data_.value.c_str());

      if (maxlength_ > 0)
        colorEdit->setMaxLength(maxlength_);

      if (data_.onchange != "")
        QObject::connect(colorEdit, SLOT(returnPressed()), this, SLOT(onChangeProc()));

      widget_ = colorEdit;
    }
    else {
      CQJLineEdit *lineEdit = new CQJLineEdit(window_->widget(), th);

      lineEdit->setText(data_.value.c_str());

      lineEdit->setPlaceholderText(data_.placeholder.c_str());

      if (maxlength_ > 0)
        lineEdit->setMaxLength(maxlength_);

      if (data_.onchange != "")
        QObject::connect(lineEdit, SLOT(returnPressed()), this, SLOT(onChangeProc()));

      widget_ = lineEdit;
    }

    initWidget(this, widget_);
  }
}

CBrowserRegion
CBrowserFormText::
calcRegion() const
{
  createWidget();

  if (! widget_)
    return CBrowserRegion();

  //---

  QSize size = widget_->sizeHint();

  width_  = size.width () + 4;
  height_ = size.height() + 4;

  return CBrowserRegion(width_, height_/2, height_/2);
}

void
CBrowserFormText::
drawWidget(CBrowserWindow *, const CTextBox &region)
{
  widget_->move  (region.x() + 2, region.y() + 2);
  widget_->resize(width_ - 4, height_ - 4);
}

void
CBrowserFormText::
submit(std::string &url)
{
  QLineEdit *edit = qobject_cast<QLineEdit *>(widget_);

  QString value = edit->text();

  url += data_.name;
  url += "=";
  url += value.toStdString();
}

void
CBrowserFormText::
reset()
{
  QLineEdit *edit = qobject_cast<QLineEdit *>(widget_);

  edit->setText(data_.value.c_str());
}

//---

CBrowserFormTextarea::
CBrowserFormTextarea(CBrowserWindow *window, const CBrowserFormTextareaData &data) :
 CBrowserFormInput(window, CHtmlTagId::TEXTAREA, CBrowserFormInputType::TEXTAREA, data),
 data_(data), wrap_(CBrowserFormTextAreaWrapType::OFF)
{
  if      (CStrUtil::casecmp(data_.wrap, "off"     ) == 0)
    wrap_ = CBrowserFormTextAreaWrapType::OFF;
  else if (CStrUtil::casecmp(data_.wrap, "virtual" ) == 0)
    wrap_ = CBrowserFormTextAreaWrapType::VIRTUAL;
  else if (CStrUtil::casecmp(data_.wrap, "physical") == 0)
    wrap_ = CBrowserFormTextAreaWrapType::PHYSICAL;

  if (data_.name == "")
    data_.name = "textarea";

  setObjectName(data_.name.c_str());
}

void
CBrowserFormTextarea::
init()
{
}

void
CBrowserFormTextarea::
setNameValue(const std::string &name, const std::string &value)
{
  std::string lname  = CStrUtil::toLower(name);
  std::string lvalue = CStrUtil::toLower(value);

  if      (lname == "cols") {
    if (CStrUtil::isInteger(value))
      data_.cols = CStrUtil::toInteger(value);
    else {
      window_->displayError("Illegal 'textarea' Value for cols '%s'\n", value.c_str());
      data_.cols = 40;
    }
  }
  else if (lname == "id") {
    data_.id = value;
  }
  else if (lname == "name") {
    data_.name = value;
  }
  else if (lname == "rows") {
    if (CStrUtil::isInteger(value))
      data_.rows = CStrUtil::toInteger(value);
    else {
      window_->displayError("Illegal 'textarea' Value for rows '%s'\n", value.c_str());
      data_.rows = 5;
    }
  }
  else if (lname == "wrap") {
    data_.wrap = value;
  }
  else {
    CBrowserObject::setNameValue(name, value);
  }
}

void
CBrowserFormTextarea::
initLayout()
{
  CBrowserForm *form = parentType<CBrowserForm>();

  if (form)
    form->addInput(this);
}

void
CBrowserFormTextarea::
termLayout()
{
}

void
CBrowserFormTextarea::
createWidget() const
{
  if (! widget_) {
    QTextEdit *edit = new QTextEdit(window_->widget());

    edit->setObjectName(data_.name.c_str());

    std::string text = this->text();

    if (text == "")
      text = data_.value;

    edit->setText(text.c_str());

    widget_ = edit;

    initWidget(this, widget_);
  }
}

CBrowserRegion
CBrowserFormTextarea::
calcRegion() const
{
  createWidget();

  if (! widget_)
    return CBrowserRegion();

  //---

  QSize size = widget_->sizeHint();

  width_  = size.width () + 4;
  height_ = size.height() + 4;

  return CBrowserRegion(width_, height_/2, height_/2);
}

void
CBrowserFormTextarea::
drawWidget(CBrowserWindow *, const CTextBox &region)
{
  widget_->move(region.x() + 2, region.y() + 2);
  widget_->resize(width_ - 4, height_ - 4);
}

void
CBrowserFormTextarea::
submit(std::string &url)
{
  QTextEdit *edit = qobject_cast<QTextEdit *>(widget_);

  QString value = edit->toPlainText();

  url += data_.name;
  url += "=";
  url += value.toStdString();
}

void
CBrowserFormTextarea::
reset()
{
  QTextEdit *edit = qobject_cast<QTextEdit *>(widget_);

  std::string text = this->text();

  if (text == "")
    text = data_.value;

  edit->setText(text.c_str());
}

//---

CBrowserFormReset::
CBrowserFormReset(CBrowserWindow *window, const CBrowserFormInputData &data) :
 CBrowserFormInput(window, CHtmlTagId::INPUT, CBrowserFormInputType::RESET_BUTTON, data)
{
  if (data_.name == "")
    data_.name = "input";

  setObjectName(data_.name.c_str());
}

void
CBrowserFormReset::
createWidget() const
{
  if (! widget_) {
    QPushButton *button = new QPushButton(window_->widget());

    button->setObjectName(data_.name.c_str());

    widget_ = button;

    QObject::connect(button, SIGNAL(clicked()), this, SLOT(resetProc()));

    if (data_.value != "")
      button->setText(data_.value.c_str());
    else
      button->setText("Reset");

    initWidget(this, widget_);
  }
}

CBrowserRegion
CBrowserFormReset::
calcRegion() const
{
  createWidget();

  if (! widget_)
    return CBrowserRegion();

  //---

  QSize size = widget_->sizeHint();

  width_  = size.width () + 4;
  height_ = size.height() + 4;

  return CBrowserRegion(width_, height_/2, height_/2);
}

void
CBrowserFormReset::
drawWidget(CBrowserWindow *, const CTextBox &region)
{
  widget_->move(region.x() + 2, region.y() + 2);
  widget_->resize(width_ - 4, height_ - 4);
}

void
CBrowserFormReset::
resetProc()
{
  std::cerr << "Reset" << std::endl;
}

//---

CBrowserFormSelect::
CBrowserFormSelect(CBrowserWindow *window, const CBrowserFormSelectData &data) :
 CBrowserFormInput(window, CHtmlTagId::SELECT, CBrowserFormInputType::SELECT, data),
 data_(data)
{
  if (data_.name == "")
    data_.name = "select";

  setObjectName(data_.name.c_str());
}

void
CBrowserFormSelect::
init()
{
  CBrowserFormInput::init();
}

void
CBrowserFormSelect::
setNameValue(const std::string &name, const std::string &value)
{
  std::string lname = CStrUtil::toLower(name);

  if      (lname == "multiple") {
    data_.multiple = true;
  }
  else if (lname == "id") {
    data_.id = value;
  }
  else if (lname == "name") {
    data_.name = value;
  }
  else if (lname == "size") {
    if (CStrUtil::isInteger(value))
      data_.size = CStrUtil::toInteger(value);
    else {
      window_->displayError("Illegal 'select' Value '%s' for '%s'\n",
                            value.c_str(), lname.c_str());
      data_.size = 1;
    }
  }
  else if (lname == "onchange") {
    data_.onchange = value;
  }
  else {
    CBrowserFormInput::setNameValue(name, value);
  }
}

std::string
CBrowserFormSelect::
value() const
{
  if (data_.multiple || data_.size != 1) {
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
addOption(CBrowserFormOption *option)
{
  options_.push_back(option);
}

void
CBrowserFormSelect::
createWidget() const
{
  if (! widget_) {
    CBrowserFormSelect *th = const_cast<CBrowserFormSelect *>(this);

    if (data_.multiple || data_.size != 1) {
      QListWidget *list = new QListWidget(window_->widget());

      list->setObjectName(data_.name.c_str());

      widget_ = list;

      list->setSelectionMode(data_.multiple ?
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
      CQJComboBox *combo = new CQJComboBox(window_->widget(), th);

      combo->setObjectName(data_.name.c_str());

      widget_ = combo;

      uint num = getNumOptions();

      for (uint i = 0; i < num; ++i) {
        CBrowserFormOption *option = getOption(i);

        combo->addItem(option->getText().c_str());
      }
    }

    initWidget(this, widget_);
  }
}

CBrowserRegion
CBrowserFormSelect::
calcRegion() const
{
  createWidget();

  if (! widget_)
    return CBrowserRegion();

  //---

  QSize size = widget_->sizeHint();

  width_  = size.width () + 4;
  height_ = size.height() + 4;

  return CBrowserRegion(width_, height_/2, height_/2);
}

void
CBrowserFormSelect::
drawWidget(CBrowserWindow *, const CTextBox &region)
{
  widget_->move(region.x() + 2, region.y() + 2);
  widget_->resize(width_ - 4, height_ - 4);
}

//---

CBrowserFormSubmit::
CBrowserFormSubmit(CBrowserWindow *window, const CBrowserFormInputData &data) :
 CBrowserFormInput(window, CHtmlTagId::INPUT, CBrowserFormInputType::SUBMIT_BUTTON, data)
{
  if (data_.name == "")
    data_.name = "submit";

  setObjectName(data_.name.c_str());
}

void
CBrowserFormSubmit::
createWidget() const
{
  if (! widget_) {
    QPushButton *button = new QPushButton(window_->widget());

    button->setObjectName(data_.name.c_str());

    widget_ = button;

    QObject::connect(button, SIGNAL(clicked()), form_, SLOT(submitProc()));

    if (data_.onclick != "")
      QObject::connect(button, SIGNAL(clicked()), this, SLOT(onClickProc()));

    if (data_.value != "")
      button->setText(data_.value.c_str());
    else
      button->setText("Submit Query");

    initWidget(this, widget_);
  }
}

CBrowserRegion
CBrowserFormSubmit::
calcRegion() const
{
  createWidget();

  if (! widget_)
    return CBrowserRegion();

  //---

  QSize size = widget_->sizeHint();

  width_  = size.width () + 4;
  height_ = size.height() + 4;

  return CBrowserRegion(width_, height_/2, height_/2);
}

void
CBrowserFormSubmit::
drawWidget(CBrowserWindow *, const CTextBox &region)
{
  widget_->move(region.x() + 2, region.y() + 2);
  widget_->resize(width_ - 4, height_ - 4);
}
