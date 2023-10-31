#include <CBrowserCode.h>
#include <CBrowserWindow.h>

CBrowserCode::
CBrowserCode(CBrowserWindowIFace *window) :
 CBrowserObject(window, CHtmlTagId::CODE)
{
  font_.setFamily(CBrowserFontFamily("monospace"));
}
