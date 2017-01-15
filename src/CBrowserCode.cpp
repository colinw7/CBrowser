#include <CBrowserCode.h>
#include <CBrowserWindow.h>

CBrowserCode::
CBrowserCode(CBrowserWindow *window) :
 CBrowserObject(window, CHtmlTagId::CODE)
{
  font_.setFamily(CBrowserFontFamily("monospace"));
}
