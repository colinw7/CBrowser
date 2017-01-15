#include <CBrowserTT.h>
#include <CBrowserWindow.h>

CBrowserTT::
CBrowserTT(CBrowserWindow *window) :
 CBrowserObject(window, CHtmlTagId::TT)
{
  setDisplay(Display::INLINE);

  font_.setFamily(CBrowserFontFamily("monospace"));
}
