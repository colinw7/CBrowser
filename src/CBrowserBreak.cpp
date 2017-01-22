#include <CBrowserBreak.h>
#include <CBrowserWindow.h>

CBrowserBreak::
CBrowserBreak(CBrowserWindow *window, const CBrowserBreakData &data) :
 CBrowserObject(window, CHtmlTagId::BR), data_(data)
{
  setDisplay(Display::INLINE);
}

CBrowserBreak::
~CBrowserBreak()
{
}

void
CBrowserBreak::
init()
{
}

void
CBrowserBreak::
setNameValue(const std::string &name, const std::string &value)
{
  std::string lname  = CStrUtil::toLower(name);
  std::string lvalue = CStrUtil::toLower(value);

  if (lname == "clear") {
    if      (lvalue == "left" ) data_.clear = CBrowserClear::Type::LEFT;
    else if (lvalue == "right") data_.clear = CBrowserClear::Type::RIGHT;
    else if (lvalue == "all"  ) data_.clear = CBrowserClear::Type::BOTH;
    else window_->displayError("Illegal 'br' Clear '%s'\n", value.c_str());
  }
  else {
    CBrowserObject::setNameValue(name, value);
  }
}

void
CBrowserBreak::
getInlineWords(Words &words) const
{
  CBrowserBreak *th = const_cast<CBrowserBreak *>(this);

  words.push_back(CBrowserWord(th, isHierSelected()));
}

CBrowserRegion
CBrowserBreak::
calcRegion() const
{
  int width, ascent, descent;

  window_->getTextWidth (hierFont(), text_, &width);
  window_->getTextHeight(hierFont(), &ascent, &descent);

  return CBrowserRegion(width, ascent, descent);
}

//------

// word break opportunity
CBrowserWbr::
CBrowserWbr(CBrowserWindow *window) :
 CBrowserObject(window, CHtmlTagId::WBR)
{
  setDisplay(Display::INLINE);
}

CBrowserWbr::
~CBrowserWbr()
{
}
