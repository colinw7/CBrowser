#include <CBrowserKbd.h>
#include <CBrowserWindow.h>

CBrowserKbd::
CBrowserKbd(CBrowserWindow *window) :
 CBrowserObject(window, CHtmlTagId::KBD)
{
  font_.setFamily(CBrowserFontFamily("monospace"));
}
