#include <CBrowserStyle.h>
#include <CBrowserWindow.h>
#include <CRGBName.h>

CBrowserBStyle::
CBrowserBStyle(CBrowserWindowIFace *window) :
 CBrowserObject(window, CHtmlTagId::B)
{
  setDisplay(CBrowserObject::Display::INLINE);

  font_.setBold();
}

CBrowserBStyle::
~CBrowserBStyle()
{
}

//---

CBrowserBigStyle::
CBrowserBigStyle(CBrowserWindowIFace *window) :
 CBrowserObject(window, CHtmlTagId::BIG)
{
  setDisplay(CBrowserObject::Display::INLINE);

  font_.setBig();
}

CBrowserBigStyle::
~CBrowserBigStyle()
{
}

//---

CBrowserBlinkStyle::
CBrowserBlinkStyle(CBrowserWindowIFace *window) :
 CBrowserObject(window, CHtmlTagId::BLINK)
{
  setDisplay(CBrowserObject::Display::INLINE);

  font_.setBold();
}

CBrowserBlinkStyle::
~CBrowserBlinkStyle()
{
}

//---

CBrowserCiteStyle::
CBrowserCiteStyle(CBrowserWindowIFace *window) :
 CBrowserObject(window, CHtmlTagId::CITE)
{
  setDisplay(CBrowserObject::Display::INLINE);

  font_.setItalic();
}

CBrowserCiteStyle::
~CBrowserCiteStyle()
{
}

//---

CBrowserDfnStyle::
CBrowserDfnStyle(CBrowserWindowIFace *window) :
 CBrowserObject(window, CHtmlTagId::DFN)
{
  setDisplay(CBrowserObject::Display::INLINE);

  font_.setItalic();
}

CBrowserDfnStyle::
~CBrowserDfnStyle()
{
}

//---

CBrowserEmStyle::
CBrowserEmStyle(CBrowserWindowIFace *window) :
 CBrowserObject(window, CHtmlTagId::EM)
{
  setDisplay(CBrowserObject::Display::INLINE);

  font_.setItalic();
}

CBrowserEmStyle::
~CBrowserEmStyle()
{
}

//---

CBrowserFontStyleObj::
CBrowserFontStyleObj(CBrowserWindowIFace *window) :
 CBrowserObject(window, CHtmlTagId::FONT)
{
  setDisplay(CBrowserObject::Display::INLINE);
}

CBrowserFontStyleObj::
~CBrowserFontStyleObj()
{
}

void
CBrowserFontStyleObj::
init()
{
  CBrowserObject::init();
}

void
CBrowserFontStyleObj::
setNameValue(const std::string &name, const std::string &value)
{
  std::string lname = CStrUtil::toLower(name);

  if      (lname == "color") {
    data_.color = value;

    setForeground(CBrowserColor(data_.color));
  }
  else if (lname == "face") {
    data_.face = value;

    font_.setFamily(CBrowserFontFamily(data_.face));
  }
  else if (lname == "size") {
    std::string value1 = value;

    if      (value1[0] == '+' || value1[0] == '-') {
      data_.delta = (value1[0] == '+' ? 1 : -1);

      value1 = value1.substr(1);
    }
    else
      data_.delta = 0;

    if (CStrUtil::isInteger(value1)) {
      data_.size = CStrUtil::toInteger(value1);
    }
    else {
      window_->displayError("Illegal 'font' Value for Size '%s'\n", value.c_str());

      data_.size  = -1;
      data_.delta = 1;
    }

    //---

    if (data_.delta != 0) {
      if (data_.delta > 0)
        font_.setSize(CBrowserFontSize(CBrowserFontSize::Type::LARGER, data_.delta));
      else
        font_.setSize(CBrowserFontSize(CBrowserFontSize::Type::SMALLER, data_.delta));
    }
    else {
      if (data_.size >= 0) {
        font_.setSize(CBrowserFontSize(window_->sizeToFontSize(data_.size)));
      }
    }

    data_.sizeSet = true;
  }
  else {
    CBrowserObject::setNameValue(name, value);
  }
}

//---

CBrowserIStyle::
CBrowserIStyle(CBrowserWindowIFace *window) :
 CBrowserObject(window, CHtmlTagId::I)
{
  setDisplay(CBrowserObject::Display::INLINE);

  font_.setItalic();
}

CBrowserIStyle::
~CBrowserIStyle()
{
}

//---

CBrowserSmallStyle::
CBrowserSmallStyle(CBrowserWindowIFace *window) :
 CBrowserObject(window, CHtmlTagId::SMALL)
{
  setDisplay(CBrowserObject::Display::INLINE);

  font_.setSmall();
}

CBrowserSmallStyle::
~CBrowserSmallStyle()
{
}

//---

CBrowserStrikeStyle::
CBrowserStrikeStyle(CBrowserWindowIFace *window) :
 CBrowserObject(window, CHtmlTagId::STRIKE)
{
  setDisplay(CBrowserObject::Display::INLINE);

  textProp_.setDecoration(CBrowserTextDecoration("line-through"));
}

CBrowserStrikeStyle::
~CBrowserStrikeStyle()
{
}

//---

CBrowserStrongStyle::
CBrowserStrongStyle(CBrowserWindowIFace *window) :
 CBrowserObject(window, CHtmlTagId::STRONG)
{
  setDisplay(CBrowserObject::Display::INLINE);

  font_.setBold();
}

CBrowserStrongStyle::
~CBrowserStrongStyle()
{
}

//---

CBrowserSubStyle::
CBrowserSubStyle(CBrowserWindowIFace *window) :
 CBrowserObject(window, CHtmlTagId::SUB)
{
  setDisplay(CBrowserObject::Display::INLINE);

  textProp_.setVerticalAlign(CBrowserTextVAlign("sub"));

  font_.setSmall();
}

CBrowserSubStyle::
~CBrowserSubStyle()
{
}

//---

CBrowserSupStyle::
CBrowserSupStyle(CBrowserWindowIFace *window) :
 CBrowserObject(window, CHtmlTagId::SUP)
{
  setDisplay(CBrowserObject::Display::INLINE);

  textProp_.setVerticalAlign(CBrowserTextVAlign("super"));

  font_.setSmall();
}

CBrowserSupStyle::
~CBrowserSupStyle()
{
}

//---

CBrowserUStyle::
CBrowserUStyle(CBrowserWindowIFace *window) :
 CBrowserObject(window, CHtmlTagId::U)
{
  setDisplay(CBrowserObject::Display::INLINE);

  textProp_.setDecoration(CBrowserTextDecoration("underline"));
}

CBrowserUStyle::
~CBrowserUStyle()
{
}

//---

CBrowserVarStyle::
CBrowserVarStyle(CBrowserWindowIFace *window) :
 CBrowserObject(window, CHtmlTagId::VAR)
{
  setDisplay(CBrowserObject::Display::INLINE);

  font_.setItalic();
}

CBrowserVarStyle::
~CBrowserVarStyle()
{
}
