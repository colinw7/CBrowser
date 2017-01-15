#include <CBrowserTitle.h>
#include <CBrowserWindow.h>
#include <CBrowserDocument.h>

CBrowserTitle::
CBrowserTitle(CBrowserWindow *window) :
 CBrowserObject(window, CHtmlTagId::TITLE)
{
  setDisplay(Display::NONE);
}
