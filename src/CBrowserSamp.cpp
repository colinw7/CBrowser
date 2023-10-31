#include <CBrowserSamp.h>
#include <CBrowserWindow.h>

CBrowserSamp::
CBrowserSamp(CBrowserWindowIFace *window) :
 CBrowserObject(window, CHtmlTagId::SAMP)
{
  font_.setFamily(CBrowserFontFamily("monospace"));
}
