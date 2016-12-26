#include <CBrowserTT.h>
#include <CBrowserWindow.h>

CBrowserTT::
CBrowserTT(CBrowserWindow *window) :
 CBrowserObject(window, CHtmlTagId::TT)
{
}

void
CBrowserTT::
initProcess()
{
  window_->startFontFace("courier");
}

void
CBrowserTT::
termProcess()
{
  window_->endFontFace();
}
