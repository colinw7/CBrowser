#include <CBrowserHead.h>

CBrowserHead::
CBrowserHead(CBrowserWindow *window) :
 CBrowserObject(window, CHtmlTagId::HEAD)
{
  setDisplay(Display::NONE);
}

void
CBrowserHead::
init()
{
  CBrowserObject::init();
}

void
CBrowserHead::
setNameValue(const std::string &name, const std::string &value)
{
  CBrowserObject::setNameValue(name, value);
}
