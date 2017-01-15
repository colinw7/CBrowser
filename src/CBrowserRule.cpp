#include <CBrowserRule.h>
#include <CBrowserWindow.h>

CBrowserRule::
CBrowserRule(CBrowserWindow *window, const CBrowserRuleData &data) :
 CBrowserObject(window, CHtmlTagId::HR), data_(data)
{
  setDisplay(CBrowserObject::Display::BLOCK);

  marginRef().setTop   (CBrowserUnitValue("0.50em"));
  marginRef().setBottom(CBrowserUnitValue("0.50em"));
  marginRef().setLeft  (CBrowserUnitValue("auto"));
  marginRef().setRight (CBrowserUnitValue("auto"));

  borderRef().setLineWidth(CBrowserUnitValue("1px"));
  borderRef().setLineStyle(CBrowserBorder::stringToStyle("inset"));

  data_.size = std::max(data_.size, 2);
}

CBrowserRule::
~CBrowserRule()
{
}

void
CBrowserRule::
init()
{
}

void
CBrowserRule::
setNameValue(const std::string &name, const std::string &value)
{
  std::string lname  = CStrUtil::toLower(name);
//std::string lvalue = CStrUtil::toLower(value);

  if      (lname == "align") {
    CHAlignType align;

    window_->parseHAlignOption(value, align);

    setHAlign(align);
  }
  else if (lname == "noshade") {
    data_.shade = false;

    if (value != "")
      window_->displayError("No Value needed for Noshade\n");
  }
  else if (lname == "size") {
    if (CStrUtil::isInteger(value))
      data_.size = CStrUtil::toInteger(value);
    else {
      window_->displayError("Illegal 'hr' Value for Size '%s'\n", value.c_str());
      data_.size = -1;
    }
  }
  else {
    CBrowserObject::setNameValue(name, value);
  }
}

CBrowserRegion
CBrowserRule::
calcRegion() const
{
  int width, ascent, descent = 0;

  if (data_.size > 14)
    ascent = data_.size + 2;
  else
    ascent = 16;

  if (this->width().isValid() && this->width().units() == CScreenUnits::Units::PX)
    width = this->width().value();
  else
    width = 0;

  return CBrowserRegion(width, ascent, descent);
}

void
CBrowserRule::
draw(const CTextBox &region)
{
  fillBackground(region);

  //---

  int width;

  if      (! this->width().isValid())
    width = region.width();
  else if (this->width().units() == CScreenUnits::Units::PERCENT)
    width = (int) (this->width().value()*region.width()/100.0);
  else
    width = this->width().value();

  int x1 = region.x();

  if      (halign() == CHALIGN_TYPE_CENTER)
    x1 += (region.width() - width)/2;
  else if (halign() == CHALIGN_TYPE_RIGHT)
    x1 += region.width() - width;

  int x2 = x1 + width;

  int y1 = region.y() + region.ascent()/2;

  //---

  if (data_.shade)
    window_->drawHRule(x1, x2, y1, data_.size);
  else
    window_->fillRectangle(x1, y1, x2 - x1 + 1, data_.size, CBrush(window_->getFgColor()));

  //---

  if (isSelected())
    window_->drawSelected(region.x(), region.y(), region.width(), region.height());
}
