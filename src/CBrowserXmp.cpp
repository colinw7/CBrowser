#include <CBrowserXmp.h>
#include <CBrowserWindow.h>

CBrowserXmp::
CBrowserXmp(CBrowserWindow *window) :
 CBrowserObject(window, CHtmlTagId::XMP)
{
  setDisplay(Display::BLOCK);

  font_.setFamily(CBrowserFontFamily("monospace"));

  marginRef().setTop   (CBrowserUnitValue("1em"));
  marginRef().setBottom(CBrowserUnitValue("1em"));

  setWhiteSpace(WhiteSpace::PRE);
}
