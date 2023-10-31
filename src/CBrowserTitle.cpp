#include <CBrowserTitle.h>
#include <CBrowserWindow.h>
#include <CBrowserDocument.h>

CBrowserTitle::
CBrowserTitle(CBrowserWindowIFace *window) :
 CBrowserObject(window, CHtmlTagId::TITLE)
{
  setDisplay(Display::NONE);
}
