#include <CBrowserKbd.h>
#include <CBrowserWindow.h>

CBrowserKbd::
CBrowserKbd(CBrowserWindow *window) :
 CBrowserObject(window, CHtmlTagId::KBD)
{
}

void
CBrowserKbd::
initProcess()
{
  window_->startFontFace("courier");
}

void
CBrowserKbd::
termProcess()
{
  window_->endFontFace();
}
