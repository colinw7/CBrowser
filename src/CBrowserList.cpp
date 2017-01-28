#include <CBrowserList.h>
#include <CBrowserWindow.h>

namespace {

std::string listIdName(CHtmlTagId id) {
  if      (id == CHtmlTagId::DIR ) return "dir";
  else if (id == CHtmlTagId::DL  ) return "dl";
  else if (id == CHtmlTagId::MENU) return "menu";
  else if (id == CHtmlTagId::OL  ) return "ol";
  else if (id == CHtmlTagId::UL  ) return "ul";
  else                             return "??";
}

}

//---

CBrowserList::SymbolType
CBrowserList::
stringToSymbol(const std::string &value)
{
  std::string lvalue = CStrUtil::toLower(value);

  if      (lvalue == "none"                ) return SymbolType::NONE;
  else if (lvalue == "circle"              ) return SymbolType::CIRCLE;
  else if (lvalue == "decimal"             ) return SymbolType::DECIMAL;
  else if (lvalue == "decimal-leading-zero") return SymbolType::DECIMAL_ZERO;
  else if (lvalue == "disc"                ) return SymbolType::DISC;
  else if (lvalue == "lower-alpha"         ) return SymbolType::LOWER_ALPHA;
  else if (lvalue == "lower-latin"         ) return SymbolType::LOWER_LATIN;
  else if (lvalue == "lower-roman"         ) return SymbolType::LOWER_ROMAN;
  else if (lvalue == "square"              ) return SymbolType::SQUARE;
  else if (lvalue == "upper-alpha"         ) return SymbolType::UPPER_ALPHA;
  else if (lvalue == "upper-latin"         ) return SymbolType::UPPER_LATIN;
  else if (lvalue == "upper-roman"         ) return SymbolType::UPPER_ROMAN;
  else if (lvalue == "initial"             ) return SymbolType::INITIAL;
  else if (lvalue == "inherit"             ) return SymbolType::INHERIT;

  return SymbolType::NONE;
}

CBrowserList::
CBrowserList(CBrowserWindow *window, CHtmlTagId id) :
 CBrowserObject(window, id)
{
  marginRef().setTop   (CBrowserUnitValue("1.00em"));
  marginRef().setBottom(CBrowserUnitValue("1.00em"));

  marginRef().setLeft(CBrowserUnitValue("40px"));
}

void
CBrowserList::
setNameValue(const std::string &name, const std::string &value)
{
  std::string lname  = CStrUtil::toLower(name);
  std::string lvalue = CStrUtil::toLower(value);

  if      (lname == "compact") {
    data_.compact = true;

    if (lvalue != "")
      window_->displayError("No value needed for 'compact'\n");
  }
  else if (lname == "start") {
    if (CStrUtil::isInteger(lvalue)) {
      int item_num = CStrUtil::toInteger(lvalue);

      data_.item_num = item_num;
    }
    else {
      window_->displayError("Illegal '%s' for Value '%s'\n",
                            listIdName(type()).c_str(), value.c_str());

      data_.item_num = 1;
    }
  }
  else if (lname == "type") {
    data_.symbol = value;
  }
  else if (lname == "list-style") {
    std::vector<std::string> words;

    CStrUtil::toWords(lvalue, words);

    if (words.size() > 0)
      symbolType_ = stringToSymbol(words[0]);

#if 0
    if (words.size() > 1)
      position = words[1];

    if (words.size() > 2)
      image = words[2];
#endif
  }
  else if (lname == "list-style-type") {
    symbolType_ = stringToSymbol(lvalue);
  }
  else if (lname == "list-style-image") {
  }
  else if (lname == "list-style-position") {
  }
  else {
    CBrowserObject::setNameValue(name, lvalue);
  }
}

void
CBrowserList::
init()
{
  CBrowserObject::init();
}

int
CBrowserList::
listDepth() const
{
  int depth = 1;

  const CBrowserObject *parent = window_->currentObj();

  while (parent) {
    if (dynamic_cast<const CBrowserList *>(parent))
      ++depth;

    parent = parent->parent();
  }

  return depth;
}
