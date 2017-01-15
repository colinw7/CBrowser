#include <CBrowserSamp.h>
#include <CBrowserWindow.h>

CBrowserSamp::
CBrowserSamp(CBrowserWindow *window) :
 CBrowserObject(window, CHtmlTagId::SAMP)
{
  font_.setFamily(CBrowserFontFamily("monospace"));
}
