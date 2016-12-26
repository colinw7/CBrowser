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

CBrowserFormFileUpload::
CBrowserFormFileUpload(CBrowserWindow *window, const CBrowserFormInputData &data) :
 CBrowserFormInput(window, CHtmlTagId::INPUT, CBrowserFormInputType::FILE, data)
{
  if (data_.name == "")
    data_.name = "file";

  setObjectName(data_.name.c_str());
}

void
CBrowserFormFileUpload::
createWidget(CBrowserWindow *window)
{
  if (! widget_) {
    QWidget *rowcol = new QWidget(window->widget());

    rowcol->setObjectName(data_.name.c_str());

    QHBoxLayout *hlayout = new QHBoxLayout(rowcol);

    QLineEdit *edit = new QLineEdit(rowcol);

    edit->setObjectName("edit");

    edit->setText(data_.value.c_str());

    widget_ = edit;

    hlayout->addWidget(edit);

    if (maxlength_ > 0) edit->setMaxLength(maxlength_);

    QPushButton *button = new QPushButton("button");

    button->setText("Browse ...");

    QObject::connect(button, SIGNAL(clicked()), this, SLOT(buttonProc()));

    hlayout->addWidget(button);

    QSize size = rowcol->sizeHint();

    width_  = size.width () + 4;
    height_ = size.height() + 4;
  }

  window->updateSubCellHeight(height_/2, height_/2);
  window->updateSubCellWidth (width_);
}

void
CBrowserFormFileUpload::
drawWidget(CBrowserWindow *, const CHtmlLayoutRegion &region)
{
  QWidget *container = widget_->parentWidget();

  container->move(region.x + 2, region.y + 2);
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
createWidget(CBrowserWindow *window)
{
  if (! widget_) {
    QRadioButton *radio = new QRadioButton(window->widget());

    radio->setObjectName(data_.name.c_str());

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

    slider->setObjectName(data_.name.c_str());

    widget_ = slider;

    QSize size = widget_->sizeHint();

    width_  = size.width () + 4;
    height_ = size.height() + 4;
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

//---

CBrowserForm::
CBrowserForm(CBrowserWindow *window, const CBrowserFormData &data) :
 CBrowserObject(window, CHtmlTagId::FORM), data_(data)
{
  target_   = "";
  encoding_ = "";
}

void
CBrowserForm::
initProcess()
{
}

void
CBrowserForm::
termProcess()
{
}

void
CBrowserForm::
initLayout()
{
}

void
CBrowserForm::
termLayout()
{
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

void
CBrowserFormOption::
initLayout()
{
}

void
CBrowserFormOption::
termLayout()
{
}

//---

CBrowserFormInput::
CBrowserFormInput(CBrowserWindow *window, CHtmlTagId id, CBrowserFormInputType type,
                  const CBrowserFormInputData &data) :
 CBrowserObject(window, id), type_(type), data_(data)
{
  widget_ = nullptr;
  width_  = 0;
  height_ = 0;

  if (data_.id != "")
    setId(data_.id);
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
initLayout()
{
  window_->addCellRedrawData(this);
}

void
CBrowserFormInput::
termLayout()
{
}

void
CBrowserFormInput::
format(CHtmlLayoutMgr *)
{
  if (getType() == CBrowserFormInputType::HIDDEN)
    return;

  //---

  window_->newSubCellRight(true);

  //---

  createWidget(window_);

  //---

  window_->addSubCellRedrawData(this);
}

void
CBrowserFormInput::
draw(CHtmlLayoutMgr *, const CHtmlLayoutRegion &region)
{
  if (getType() == CBrowserFormInputType::HIDDEN)
    return;

  //---

  drawWidget(window_, region);

  //---

  window_->drawOutline(region.getX(), region.getY(),
                       region.getWidth(), region.getHeight(), CRGBA(1,0,0));

  //region.x += getWidth();
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
CBrowserFormButton(CBrowserWindow *window, const CBrowserFormInputData &data) :
 CBrowserFormInput(window, CHtmlTagId::INPUT, CBrowserFormInputType::BUTTON, data)
{
  if (data_.name == "")
    data_.name = "button";

  setObjectName(data_.name.c_str());
}

void
CBrowserFormButton::
createWidget(CBrowserWindow *window)
{
  if (! widget_) {
    QPushButton *button = new QPushButton(window->widget());

    button->setObjectName(data_.name.c_str());

    if (data_.onclick != "")
      QObject::connect(button, SIGNAL(clicked()), this, SLOT(onClickProc()));

    if (data_.value != "")
      button->setText(data_.value.c_str());
    else
      button->setText("Button");

    widget_ = button;

    widget_->setFont(CQUtil::toQFont(window_->getFont()));

    QSize size = widget_->sizeHint();

    width_  = size.width () + 4;
    height_ = size.height() + 4;
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
createWidget(CBrowserWindow *window)
{
  if (! widget_) {
    QCheckBox *button = new QCheckBox(window->widget());

    button->setObjectName(data_.name.c_str());

    widget_ = button;

    button->setText("");

    if (checked_)
      button->setChecked(true);

    widget_->setFont(CQUtil::toQFont(window_->getFont()));

    QSize size = widget_->sizeHint();

    width_  = size.width () + 4;
    height_ = size.height() + 4;
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
createWidget(CBrowserWindow *window)
{
  int hspace = 2;
  int vspace = 2;
  int width  = image_->getWidth ();
  int height = image_->getHeight();

  //---

  CHtmlLayoutCell *redraw_cell = window->getCurrentCell();

  CHtmlLayoutSubCell *sub_cell = redraw_cell->getCurrentSubCell();

  if      (align_ == CBrowserImageAlign::LEFT)
    sub_cell->setAlign(CHALIGN_TYPE_LEFT);
  else if (align_ == CBrowserImageAlign::RIGHT)
    sub_cell->setAlign(CHALIGN_TYPE_RIGHT);
  else
    sub_cell->setAlign(CHALIGN_TYPE_NONE);

  //---

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

  //---

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

  //---

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
createWidget(CBrowserWindow *window)
{
  if (! widget_) {
    QLineEdit *edit = new QLineEdit(window->widget());

    edit->setObjectName(data_.name.c_str());

    edit->setText("");

    widget_ = edit;

    if (maxlength_ > 0) edit->setMaxLength(maxlength_);

    // TODO: make text unreadable

    widget_->setFont(CQUtil::toQFont(window_->getFont()));

    QSize size = widget_->sizeHint();

    width_  = size.width () + 4;
    height_ = size.height() + 4;
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
createWidget(CBrowserWindow *window)
{
  if (! widget_) {
    if (classStr_ == "color") {
      CQJColorEdit *colorEdit = new CQJColorEdit(window->widget(), this);

      colorEdit->setText(data_.value.c_str());

      if (maxlength_ > 0)
        colorEdit->setMaxLength(maxlength_);

      if (data_.onchange != "")
        QObject::connect(colorEdit, SLOT(returnPressed()), this, SLOT(onChangeProc()));

      widget_ = colorEdit;
    }
    else {
      CQJLineEdit *lineEdit = new CQJLineEdit(window->widget(), this);

      lineEdit->setText(data_.value.c_str());

      lineEdit->setPlaceholderText(placeholder_.c_str());

      if (maxlength_ > 0)
        lineEdit->setMaxLength(maxlength_);

      if (data_.onchange != "")
        QObject::connect(lineEdit, SLOT(returnPressed()), this, SLOT(onChangeProc()));

      widget_ = lineEdit;
    }

    widget_->setFont(CQUtil::toQFont(window_->getFont()));

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

  if (data_.id != "")
    setId(data_.id);
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
createWidget(CBrowserWindow *window)
{
  if (! widget_) {
    QTextEdit *edit = new QTextEdit(window->widget());

    edit->setObjectName(data_.name.c_str());

    edit->setText(data_.value.c_str());

    widget_ = edit;

    widget_->setFont(CQUtil::toQFont(window_->getFont()));

    QSize size = widget_->sizeHint();

    width_  = size.width () + 4;
    height_ = size.height() + 4;
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

  url += data_.name;
  url += "=";
  url += value.toStdString();
}

void
CBrowserFormTextarea::
reset()
{
  QTextEdit *edit = qobject_cast<QTextEdit *>(widget_);

  edit->setText(data_.value.c_str());
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
createWidget(CBrowserWindow *window)
{
  if (! widget_) {
    QPushButton *button = new QPushButton(window->widget());

    button->setObjectName(data_.name.c_str());

    widget_ = button;

    QObject::connect(button, SIGNAL(clicked()), this, SLOT(resetProc()));

    if (data_.value != "")
      button->setText(data_.value.c_str());
    else
      button->setText("Reset");

    widget_->setFont(CQUtil::toQFont(window_->getFont()));

    QSize size = widget_->sizeHint();

    width_  = size.width () + 4;
    height_ = size.height() + 4;
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

  if (data_.id != "")
    setId(data_.id);
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
initLayout()
{
}

void
CBrowserFormSelect::
termLayout()
{
}

void
CBrowserFormSelect::
createWidget(CBrowserWindow *window)
{
  if (! widget_) {
    if (data_.multiple || data_.size != 1) {
      QListWidget *list = new QListWidget(window->widget());

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
      CQJComboBox *combo = new CQJComboBox(window->widget(), this);

      combo->setObjectName(data_.name.c_str());

      widget_ = combo;

      uint num = getNumOptions();

      for (uint i = 0; i < num; ++i) {
        CBrowserFormOption *option = getOption(i);

        combo->addItem(option->getText().c_str());
      }
    }

    widget_->setFont(CQUtil::toQFont(window_->getFont()));

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
createWidget(CBrowserWindow *window)
{
  if (! widget_) {
    QPushButton *button = new QPushButton(window->widget());

    button->setObjectName(data_.name.c_str());

    widget_ = button;

    QObject::connect(button, SIGNAL(clicked()), form_, SLOT(submitProc()));

    if (data_.onclick != "")
      QObject::connect(button, SIGNAL(clicked()), this, SLOT(onClickProc()));

    if (data_.value != "")
      button->setText(data_.value.c_str());
    else
      button->setText("Submit Query");

    widget_->setFont(CQUtil::toQFont(window_->getFont()));

    QSize size = widget_->sizeHint();

    width_  = size.width () + 4;
    height_ = size.height() + 4;
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
