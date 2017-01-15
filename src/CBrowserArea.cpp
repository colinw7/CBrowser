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

  if      (name == "alt") {
  }
  else if (name == "coords") {
  }
  else if (name == "href") {
  }
  else if (name == "shape") {
  }
  else {
    CBrowserObject::setNameValue(name, value);
  }
}
