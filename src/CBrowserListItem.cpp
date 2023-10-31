#include <CBrowserListItem.h>
#include <CBrowserWindow.h>
#include <CBrowserList.h>
#include <CBrowserMisc.h>
#include <CBrowserGraphics.h>

#include <CRomanNumber.h>

CBrowserListItem::
CBrowserListItem(CBrowserWindowIFace *window) :
 CBrowserObject(window, CHtmlTagId::LI)
{
}

void
CBrowserListItem::
init()
{
  auto *currentList = const_cast<CBrowserList *>(this->currentList());

  if (currentList) {
    if (data_.symbol != "")
      currentList->setSymbol(data_.symbol);

    if (data_.item_num > 0)
      currentList->setItemNum(data_.item_num);
  }

  //---

  symbol_  = (currentList ? currentList->getSymbol () : data_.symbol  );
  itemNum_ = (currentList ? currentList->getItemNum() : data_.item_num);

  //---

  if (currentList)
    currentList->setItemNum(itemNum_ + 1);

  //---

  CBrowserObject::init();
}

std::string
CBrowserListItem::
orderedListText(const std::string &symbol, int item_num) const
{
  std::string text;

  if      (symbol == "")
    text = CStrUtil::toString(item_num);
  else if (symbol == "A") {
    text = CBrowserMisc::integerToAlphabetic(item_num);

    CStrUtil::toUpper(text);
  }
  else if (symbol == "a") {
    text = CBrowserMisc::integerToAlphabetic(item_num);

    CStrUtil::toLower(text);
  }
  else if (symbol == "I") {
    CRomanNumber roman(item_num);

    text = roman.getString();

    CStrUtil::toUpper(roman.getString());
  }
  else if (symbol == "i") {
    CRomanNumber roman(item_num);

    text = roman.getString();

    CStrUtil::toLower(text);
  }
  else if (symbol == "1")
    text = CStrUtil::toString(item_num);
  else
    text = CStrUtil::toString(item_num);

  return text;
}

CBrowserListStyleType
CBrowserListItem::
unorderedListType(const std::string &symbol, int depth) const
{
  CBrowserListStyleType type;

  if      (symbol == "") {
    int num = std::max(depth - 1, 0);

    num %= 4;

    type = CBrowserListStyleType(CBrowserListStyleType::Type::DISC);

    if      (num == 0) type = CBrowserListStyleType(CBrowserListStyleType::Type::DISC);
    else if (num == 1) type = CBrowserListStyleType(CBrowserListStyleType::Type::CIRCLE);
    else if (num == 2) type = CBrowserListStyleType(CBrowserListStyleType::Type::BLOCK);
    else               type = CBrowserListStyleType(CBrowserListStyleType::Type::SQUARE);
  }
  else {
    type = CBrowserListStyleType(symbol);
  }

  return type;
}

void
CBrowserListItem::
setNameValue(const std::string &name, const std::string &value)
{
  std::string lname  = CStrUtil::toLower(name);
  std::string lvalue = CStrUtil::toLower(value);

  if      (lname == "type") {
    data_.symbol = value;
  }
  else if (lname == "value") {
    if (CStrUtil::isInteger(lvalue))
      data_.item_num = CStrUtil::toInteger(lvalue);
    else
      window_->displayError("Illegal 'li' Value for Value '%s'\n", value.c_str());

  }
  else if (lname == "width") {
  }
  else {
    CBrowserObject::setNameValue(name, value);
  }
}

void
CBrowserListItem::
draw(const CTextBox &region)
{
  CBrowserList *currentList = parentType<CBrowserList>();

  //---

  CFontPtr font = hierFont();
  CRGBA    tc   = hierFgColor();

  CPen   pen(tc);
  CBrush brush(tc);

  CHtmlTagId listId = (currentList ? currentList->type() : CHtmlTagId::NONE);

  auto *graphics = window_->graphics();

  if      (listId == CHtmlTagId::OL) {
    std::string text = orderedListText(symbol_, itemNum_) + ".";

    int width  = font->getStringWidth(text + " ");
    int ascent = font->getCharAscent();

    graphics->drawText(region.x() - width, region.y() + ascent, text, pen, font);
  }
  else if (listId == CHtmlTagId::UL || listId == CHtmlTagId::DIR || listId == CHtmlTagId::MENU) {
    int width  = font->getStringWidth("X "); // marker + space
    int ascent = font->getCharAscent();

    int cx = region.x() - width;
    int cy = region.y() + ascent/2;

    //---

    double size = ascent*0.6;

    int depth = currentList->listDepth();

    CBrowserListStyleType type = calcStyleStype();

    if (! type.isValid())
      type = unorderedListType(symbol_, depth);

    switch (type.type()) {
      case CBrowserListStyleType::Type::DISC:
        graphics->fillCircle(cx, cy, size/2, brush); // x, y, r
        break;
      case CBrowserListStyleType::Type::CIRCLE:
        graphics->drawCircle(cx, cy, size/2, pen); // x, y, r
        break;
      case CBrowserListStyleType::Type::BLOCK:
        graphics->fillRectangle(cx - size/2, cy - size/2, size, size, brush); // x, y, w, h
        break;
      case CBrowserListStyleType::Type::SQUARE:
        graphics->drawRectangle(cx - size/2, cy - size/2, size, size, pen); // x, y, w, h
        break;
      case CBrowserListStyleType::Type::NONE:
        break;
      default:
        window_->displayError("Illegal type for list item symbol\n");
        break;
    }
  }
}

const CBrowserList *
CBrowserListItem::
currentList() const
{
  const auto *obj = window_->currentObj();

  while (obj && ! dynamic_cast<const CBrowserList *>(obj))
    obj = obj->parent();

  const auto *currentList = dynamic_cast<const CBrowserList *>(obj);

  return currentList;
}

CBrowserListStyleType
CBrowserListItem::
calcStyleStype() const
{
  if (styleType().isValid())
    return styleType();

  CBrowserList *currentList = parentType<CBrowserList>();

  if (currentList->styleType().isValid())
    return currentList->styleType();

  return CBrowserListStyleType();
}

//---

CBrowserDataListData::
CBrowserDataListData(CBrowserWindowIFace *window) :
 CBrowserObject(window, CHtmlTagId::DD)
{
  setDisplay(Display::BLOCK);

  marginRef().setLeft(CBrowserUnitValue("40px"));
}

//---

CBrowserDataListTerm::
CBrowserDataListTerm(CBrowserWindowIFace *window) :
 CBrowserObject(window, CHtmlTagId::DT)
{
  setDisplay(Display::BLOCK);
}
