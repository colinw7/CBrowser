#include <CBrowserIFrame.h>
#include <CBrowserWindow.h>

CBrowserIFrame::
CBrowserIFrame(CBrowserWindowIFace *window) :
 CBrowserObject(window, CHtmlTagId::IFRAME)
{
  // iframe:focus { outline: none; }
  // iframe[seamless] { display: block; }
}

void
CBrowserIFrame::
setNameValue(const std::string &name, const std::string &value)
{
  std::string lname  = CStrUtil::toLower(name);
  std::string lvalue = CStrUtil::toLower(value);

  if      (lname == "align") {
  }
  else if (lname == "frameborder") {
  }
  else if (lname == "height") {
    CBrowserObject::setHeight(CBrowserUnitValue(value));
  }
  else if (lname == "longdesc") {
  }
  else if (lname == "marginheight") {
  }
  else if (lname == "marginwidth") {
  }
  else if (lname == "name") {
  }
  else if (lname == "sandbox") {
  }
  else if (lname == "scrolling") {
  }
  else if (lname == "src") {
  }
  else if (lname == "srcdoc") {
  }
  else if (lname == "width") {
    CBrowserObject::setWidth(CBrowserUnitValue(value));
  }
  else {
    CBrowserObject::setNameValue(name, value);
  }
}
