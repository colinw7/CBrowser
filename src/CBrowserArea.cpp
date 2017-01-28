#include <CBrowserArea.h>
#include <CBrowserWindow.h>

CBrowserArea::
CBrowserArea(CBrowserWindow *window) :
 CBrowserObject(window, CHtmlTagId::AREA)
{
  setDisplay(Display::NONE);
}

void
CBrowserArea::
init()
{
  CBrowserObject::init();
}

void
CBrowserArea::
setNameValue(const std::string &name, const std::string &value)
{
  std::string lname = CStrUtil::toLower(name);

  if      (lname == "alt") {
  }
  else if (lname == "coords") {
  }
  else if (lname == "href") {
  }
  else if (lname == "shape") {
  }
  else {
    CBrowserObject::setNameValue(name, value);
  }
}
