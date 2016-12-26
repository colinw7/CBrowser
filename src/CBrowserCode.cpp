#include <CBrowserCode.h>
#include <CBrowserWindow.h>

CBrowserCode::
CBrowserCode(CBrowserWindow *window) :
 CBrowserObject(window, CHtmlTagId::CODE)
{
}

void
CBrowserCode::
initProcess()
{
  window_->startFontFace("courier");
}

void
CBrowserCode::
termProcess()
{
  window_->endFontFace();
}
