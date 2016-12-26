#include <CBrowserStyle.h>
#include <CBrowserWindow.h>
#include <CRGBName.h>

CBrowserBStyle::
CBrowserBStyle(CBrowserWindow *window) :
 CBrowserObject(window, CHtmlTagId::B)
{
}

CBrowserBStyle::
~CBrowserBStyle()
{
}

void
CBrowserBStyle::
initProcess()
{
  window_->startBold();
}

void
CBrowserBStyle::
termProcess()
{
  window_->endBold();
}

//---

CBrowserBigStyle::
CBrowserBigStyle(CBrowserWindow *window) :
 CBrowserObject(window, CHtmlTagId::BIG)
{
}

CBrowserBigStyle::
~CBrowserBigStyle()
{
}

void
CBrowserBigStyle::
initProcess()
{
  window_->increaseFontSize();
}

void
CBrowserBigStyle::
termProcess()
{
  window_->decreaseFontSize();
}

//---

CBrowserBlinkStyle::
CBrowserBlinkStyle(CBrowserWindow *window) :
 CBrowserObject(window, CHtmlTagId::BLINK)
{
}

CBrowserBlinkStyle::
~CBrowserBlinkStyle()
{
}

void
CBrowserBlinkStyle::
initProcess()
{
  window_->startBold();
}

void
CBrowserBlinkStyle::
termProcess()
{
  window_->endBold();
}

//---

CBrowserCiteStyle::
CBrowserCiteStyle(CBrowserWindow *window) :
 CBrowserObject(window, CHtmlTagId::CITE)
{
}

CBrowserCiteStyle::
~CBrowserCiteStyle()
{
}

void
CBrowserCiteStyle::
initProcess()
{
  window_->startItalic();
}

void
CBrowserCiteStyle::
termProcess()
{
  window_->endItalic();
}

//---

CBrowserDfnStyle::
CBrowserDfnStyle(CBrowserWindow *window) :
 CBrowserObject(window, CHtmlTagId::DFN)
{
}

CBrowserDfnStyle::
~CBrowserDfnStyle()
{
}

void
CBrowserDfnStyle::
initProcess()
{
  window_->startItalic();
}

void
CBrowserDfnStyle::
termProcess()
{
  window_->endItalic();
}

//---

CBrowserEmStyle::
CBrowserEmStyle(CBrowserWindow *window) :
 CBrowserObject(window, CHtmlTagId::EM)
{
}

CBrowserEmStyle::
~CBrowserEmStyle()
{
}

void
CBrowserEmStyle::
initProcess()
{
  window_->startItalic();
}

void
CBrowserEmStyle::
termProcess()
{
  window_->endItalic();
}

//---

CBrowserFontStyle::
CBrowserFontStyle(CBrowserWindow *window, const CBrowserFontData &data) :
 CBrowserObject(window, CHtmlTagId::FONT), data_(data)
{
}

CBrowserFontStyle::
~CBrowserFontStyle()
{
}

void
CBrowserFontStyle::
initProcess()
{
  // push current font state
  std::string face      = window_->getCurrentFontFace();
  std::string colorName = window_->getFontColor();
  CRGBA       color     = window_->getTextColor();
  int         size      = window_->getFontSize();

  saveData_ = CBrowserSaveFontData(face, colorName, color, size);

  //---

  // start new font
  if (data_.face != "") {
    window_->setCurrentFontFace(data_.face);

    saveData_.setFlag(CBrowserSaveFontData::Flag::FACE);
  }

  if (data_.color != "") {
    window_->setFontColor(data_.color);
    window_->setTextColor(CRGBName::toRGBA(data_.color));

    saveData_.setFlag(CBrowserSaveFontData::Flag::COLOR);
  }

  int base_size = window_->getBaseFontSize();

  if (data_.delta != 0)
    size = base_size + data_.delta*data_.size;
  else
    size = data_.size;

  if (size > 0) {
    window_->setFontSize(size);

    saveData_.setFlag(CBrowserSaveFontData::Flag::SIZE);
  }
}

void
CBrowserFontStyle::
termProcess()
{
  if (saveData_.getFlag(CBrowserSaveFontData::Flag::FACE))
    window_->setCurrentFontFace(saveData_.getFace());

  if (saveData_.getFlag(CBrowserSaveFontData::Flag::COLOR)) {
    window_->setFontColor(saveData_.getColorName());
    window_->setTextColor(saveData_.getColor());
  }

  if (saveData_.getFlag(CBrowserSaveFontData::Flag::SIZE))
    window_->setFontSize(saveData_.getSize());
}

//---

CBrowserIStyle::
CBrowserIStyle(CBrowserWindow *window) :
 CBrowserObject(window, CHtmlTagId::I)
{
}

CBrowserIStyle::
~CBrowserIStyle()
{
}

void
CBrowserIStyle::
initProcess()
{
  window_->startItalic();
}

void
CBrowserIStyle::
termProcess()
{
  window_->endItalic();
}

//---

CBrowserSmallStyle::
CBrowserSmallStyle(CBrowserWindow *window) :
 CBrowserObject(window, CHtmlTagId::SMALL)
{
}

CBrowserSmallStyle::
~CBrowserSmallStyle()
{
}

void
CBrowserSmallStyle::
initProcess()
{
  window_->decreaseFontSize();
}

void
CBrowserSmallStyle::
termProcess()
{
  window_->increaseFontSize();
}

//---

CBrowserStrikeStyle::
CBrowserStrikeStyle(CBrowserWindow *window) :
 CBrowserObject(window, CHtmlTagId::STRIKE)
{
}

CBrowserStrikeStyle::
~CBrowserStrikeStyle()
{
}

void
CBrowserStrikeStyle::
initProcess()
{
  window_->setTextStrike(true);
}

void
CBrowserStrikeStyle::
termProcess()
{
  window_->setTextStrike(false);
}

//---

CBrowserStrongStyle::
CBrowserStrongStyle(CBrowserWindow *window) :
 CBrowserObject(window, CHtmlTagId::STRONG)
{
}

CBrowserStrongStyle::
~CBrowserStrongStyle()
{
}

void
CBrowserStrongStyle::
initProcess()
{
  window_->startBold();
}

void
CBrowserStrongStyle::
termProcess()
{
  window_->endBold();
}

//---

CBrowserSubStyle::
CBrowserSubStyle(CBrowserWindow *window) :
 CBrowserObject(window, CHtmlTagId::SUB)
{
}

CBrowserSubStyle::
~CBrowserSubStyle()
{
}

void
CBrowserSubStyle::
initProcess()
{
  window_->setTextPlace(CBrowserTextPlaceType::SUBSCRIPT);
}

void
CBrowserSubStyle::
termProcess()
{
  window_->setTextPlace(CBrowserTextPlaceType::NORMAL);
}

//---

CBrowserSupStyle::
CBrowserSupStyle(CBrowserWindow *window) :
 CBrowserObject(window, CHtmlTagId::SUP)
{
}

CBrowserSupStyle::
~CBrowserSupStyle()
{
}

void
CBrowserSupStyle::
initProcess()
{
  window_->setTextPlace(CBrowserTextPlaceType::SUPERSCRIPT);
}

void
CBrowserSupStyle::
termProcess()
{
  window_->setTextPlace(CBrowserTextPlaceType::NORMAL);
}

//---

CBrowserUStyle::
CBrowserUStyle(CBrowserWindow *window) :
 CBrowserObject(window, CHtmlTagId::U)
{
}

CBrowserUStyle::
~CBrowserUStyle()
{
}

void
CBrowserUStyle::
initProcess()
{
  window_->setTextUnderline(true);
}

void
CBrowserUStyle::
termProcess()
{
  window_->setTextUnderline(false);
}

//---

CBrowserVarStyle::
CBrowserVarStyle(CBrowserWindow *window) :
 CBrowserObject(window, CHtmlTagId::VAR)
{
}

CBrowserVarStyle::
~CBrowserVarStyle()
{
}

void
CBrowserVarStyle::
initProcess()
{
  window_->startItalic();
}

void
CBrowserVarStyle::
termProcess()
{
  window_->endItalic();
}
