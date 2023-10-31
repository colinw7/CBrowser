#include <CBrowserVideo.h>
#include <CBrowserWindow.h>

CBrowserVideo::
CBrowserVideo(CBrowserWindowIFace *window) :
 CBrowserObject(window, CHtmlTagId::VIDEO)
{
}

void
CBrowserVideo::
setNameValue(const std::string &name, const std::string &value)
{
  std::string lname  = CStrUtil::toLower(name);

  if      (lname == "height") {
    CBrowserObject::setHeight(CBrowserUnitValue(value));
  }
  else if (lname == "width") {
    CBrowserObject::setWidth(CBrowserUnitValue(value));
  }
  else {
    CBrowserObject::setNameValue(name, value);
  }
}
