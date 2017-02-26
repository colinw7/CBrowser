#include <CBrowserBlockQuote.h>
#include <CBrowserWindow.h>

CBrowserBlockQuote::
CBrowserBlockQuote(CBrowserWindow *window) :
 CBrowserObject(window, CHtmlTagId::BLOCKQUOTE)
{
  setDisplay(Display::BLOCK);

  marginRef().setTop   (CBrowserUnitValue("1.00em"));
  marginRef().setBottom(CBrowserUnitValue("1.00em"));
  marginRef().setLeft  (CBrowserUnitValue("40px"));
  marginRef().setRight (CBrowserUnitValue("40px"));
}
