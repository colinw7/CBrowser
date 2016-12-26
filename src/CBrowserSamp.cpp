#include <CBrowserSamp.h>
#include <CBrowserWindow.h>

CBrowserSamp::
CBrowserSamp(CBrowserWindow *window) :
 CBrowserObject(window, CHtmlTagId::SAMP)
{
}

void
CBrowserSamp::
initProcess()
{
  window_->startFontFace("courier");
}

void
CBrowserSamp::
termProcess()
{
  window_->endFontFace();
}
