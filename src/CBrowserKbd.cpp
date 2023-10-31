#include <CBrowserKbd.h>
#include <CBrowserWindow.h>

CBrowserKbd::
CBrowserKbd(CBrowserWindowIFace *window) :
 CBrowserObject(window, CHtmlTagId::KBD)
{
  font_.setFamily(CBrowserFontFamily("monospace"));
}
