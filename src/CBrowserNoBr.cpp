#include <CBrowserNoBr.h>
#include <CBrowserWindow.h>

CBrowserNoBr::
CBrowserNoBr(CBrowserWindow *window) :
 CBrowserObject(window, CHtmlTagId::NOBR)
{
  setDisplay(Display::INLINE);

  setWhiteSpace(WhiteSpace::NOWRAP);
}
