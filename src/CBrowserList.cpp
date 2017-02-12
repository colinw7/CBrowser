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
      styleType_ = CBrowserListStyleType(words[0]);

#if 0
    if (words.size() > 1)
      position = words[1];

    if (words.size() > 2)
      image = words[2];
#endif
  }
  else if (lname == "list-style-type") {
    styleType_ = CBrowserListStyleType(lvalue);
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
