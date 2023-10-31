#include <CBrowserDiv.h>
#include <CBrowserWindow.h>
#include <CStrUtil.h>

CBrowserDiv::
CBrowserDiv(CBrowserWindowIFace *window) :
 CBrowserObject(window, CHtmlTagId::DIV)
{
  setDisplay(Display::BLOCK);
}

void
CBrowserDiv::
setNameValue(const std::string &name, const std::string &value)
{
  std::string lname  = CStrUtil::toLower(name);
  //std::string lvalue = CStrUtil::toLower(value);

  if      (lname == "itemscope") {
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
  CBrowserObject::init();
}
