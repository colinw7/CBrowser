#include <CBrowserDiv.h>
#include <CBrowserWindow.h>
#include <CStrUtil.h>

CBrowserDiv::
CBrowserDiv(CBrowserWindow *window, const CBrowserDivData &data) :
 CBrowserObject(window, CHtmlTagId::DIV, data)
{
}

void
CBrowserDiv::
setNameValue(const std::string &name, const std::string &value)
{
  std::string lname  = CStrUtil::toLower(name);
  //std::string lvalue = CStrUtil::toLower(value);

  if      (lname == "id") {
    data_.id = value;
  }
  else if (lname == "class") {
    data_.cclass = value;
  }
  else if (lname == "style") {
    data_.style = value;
  }
  else if (lname == "itemscope") {
  }
  else if (lname == "itemtype") {
  }
  else {
    CBrowserObject::setNameValue(name, value);
  }
}

void
CBrowserDiv::
init()
{
}
