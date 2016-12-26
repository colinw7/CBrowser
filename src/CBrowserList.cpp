#include <CBrowserList.h>
#include <CBrowserWindow.h>
#include <CBrowserOutputList.h>
#include <CBrowserLabel.h>
#include <CBrowserMisc.h>
#include <CRomanNumber.h>

CBrowserListItem::
CBrowserListItem(CBrowserWindow *window, const CBrowserOutputListItemData &data) :
 CBrowserObject(window, CHtmlTagId::LI), data_(data)
{
  init();
}

void
CBrowserListItem::
init()
{
  CBrowserOutputList *currentList = const_cast<CBrowserOutputList *>(this->currentList());

  if (currentList) {
    if (data_.symbol != "")
      currentList->setSymbol(data_.symbol);

    if (data_.item_num > 0)
      currentList->setItemNum(data_.item_num);
  }

  //---

  std::string symbol   = (currentList ? currentList->getSymbol () : data_.symbol  );
  int         item_num = (currentList ? currentList->getItemNum() : data_.item_num);

  //---

  CRGBA tc = window_->getTextColor();

  CHtmlTagId listId = (currentList ? currentList->type() : CHtmlTagId::NONE);

  if      (listId == CHtmlTagId::OL) {
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

    labelSymbolDatas_.push_back(LabelSymbolData(text, 4, CHALIGN_TYPE_RIGHT, tc));
    labelSymbolDatas_.push_back(LabelSymbolData(". ", 2, CHALIGN_TYPE_LEFT , tc));
  }

  else if (listId == CHtmlTagId::UL || listId == CHtmlTagId::DIR || listId == CHtmlTagId::MENU) {
    CBrowserSymbol::Type type = CBrowserSymbol::Type::NONE;

    if      (symbol == "") {
      int num = currentList->listDepth();

      num %= 4;

      type = CBrowserSymbol::Type::CIRCLE;

      if      (num == 0) type = CBrowserSymbol::Type::DISC;
      else if (num == 1) type = CBrowserSymbol::Type::CIRCLE;
      else if (num == 2) type = CBrowserSymbol::Type::BLOCK;
      else               type = CBrowserSymbol::Type::SQUARE;
    }
    else if (CStrUtil::casecmp(symbol, "disc"  ) == 0)
      type = CBrowserSymbol::Type::DISC;
    else if (CStrUtil::casecmp(symbol, "circle") == 0)
      type = CBrowserSymbol::Type::CIRCLE;
    else if (CStrUtil::casecmp(symbol, "square") == 0)
      type = CBrowserSymbol::Type::SQUARE;
    else
      type = CBrowserSymbol::Type::DISC;

    labelSymbolDatas_.push_back(LabelSymbolData(type));
    labelSymbolDatas_.push_back(LabelSymbolData(" ", 1, CHALIGN_TYPE_LEFT, tc));
  }

  if (currentList)
    currentList->setItemNum(item_num + 1);
}

void
CBrowserListItem::
addLabelSymbols()
{
  for (const auto &data : labelSymbolDatas_) {
    CBrowserObject *obj { nullptr };

    if      (data.type == LabelSymbolData::Type::LABEL)
      obj = new CBrowserLabel(window_, data.text, data.width, data.align, data.color);
    else if (data.type == LabelSymbolData::Type::SYMBOL)
      obj = new CBrowserSymbol(window_, data.symbol);

    window_->startObject(obj, /*add*/true);
    window_->endObject  ();
  }
}

void
CBrowserListItem::
initLayout()
{
  CBrowserOutputList *currentList = const_cast<CBrowserOutputList *>(this->currentList());

  if (! currentList || ! currentList->getCompact())
    window_->skipLine();
  else
    window_->newLine();

}

void
CBrowserListItem::
termLayout()
{
}

const CBrowserOutputList *
CBrowserListItem::
currentList() const
{
  const CBrowserObject *obj = window_->currentObj();

  while (obj && ! dynamic_cast<const CBrowserOutputList *>(obj))
    obj = obj->parent();

  const CBrowserOutputList *currentList = dynamic_cast<const CBrowserOutputList *>(obj);

  return currentList;
}

//---

CBrowserDataListData::
CBrowserDataListData(CBrowserWindow *window) :
 CBrowserObject(window, CHtmlTagId::DD)
{
}

void
CBrowserDataListData::
initLayout()
{
  window_->indentLeft(4);

  window_->newLine();
}

void
CBrowserDataListData::
termLayout()
{
  window_->indentLeft(-4);

  window_->newLine();
}

//---

CBrowserDataListTerm::
CBrowserDataListTerm(CBrowserWindow *window) :
 CBrowserObject(window, CHtmlTagId::DT)
{
}

void
CBrowserDataListTerm::
initProcess()
{
  window_->startBold();
}

void
CBrowserDataListTerm::
termProcess()
{
  window_->endBold();
}
