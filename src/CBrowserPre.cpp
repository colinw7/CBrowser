#include <CBrowserPre.h>
#include <CBrowserData.h>
#include <CBrowserWindow.h>

CBrowserPre::
CBrowserPre(CBrowserWindowIFace *window) :
 CBrowserObject(window, CHtmlTagId::PRE)
{
  setDisplay(Display::BLOCK);

  font_.setFamily(CBrowserFontFamily("monospace"));

  marginRef().setTop   (CBrowserUnitValue("1em"));
  marginRef().setBottom(CBrowserUnitValue("1em"));

  setWhiteSpace(WhiteSpace::PRE);
}

void
CBrowserPre::
init()
{
  CBrowserObject::init();
}

void
CBrowserPre::
setNameValue(const std::string &name, const std::string &value)
{
  font_.setFamily(CBrowserFontFamily("monospace"));

  CBrowserObject::setNameValue(name, value);
}
