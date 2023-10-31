#include <CBrowserHtml.h>
#include <CBrowserWindow.h>
#include <CBrowserDocument.h>

CBrowserHtml::
CBrowserHtml(CBrowserWindowIFace *window) :
 CBrowserObject(window, CHtmlTagId::HTML)
{
  setDisplay(Display::BLOCK);

  window_->setRootObject(this);
}

void
CBrowserHtml::
init()
{
  CBrowserObject::init();
}

void
CBrowserHtml::
setNameValue(const std::string &name, const std::string &value)
{
  std::string lname = CStrUtil::toLower(name);

  if      (lname == "lang") {
  }
  else if (lname == "manifest") {
  }
  else
    CBrowserObject::setNameValue(name, value);
}
