#include <CBrowserAddress.h>
#include <CBrowserWindow.h>

CBrowserAddress::
CBrowserAddress(CBrowserWindow *window) :
 CBrowserObject(window, CHtmlTagId::ADDRESS)
{
  setDisplay(Display::BLOCK);

  font_.setStyle(CBrowserFontStyle(CBrowserFontStyle::Type::ITALIC));
}
