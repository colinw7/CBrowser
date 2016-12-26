#include <CBrowserXmp.h>
#include <CBrowserWindow.h>

CBrowserXmp::
CBrowserXmp(CBrowserWindow *window) :
 CBrowserObject(window, CHtmlTagId::XMP)
{
}

void
CBrowserXmp::
initProcess()
{
  window_->startFontFace("courier");

  window_->setTextBreakup(false);
  window_->setTextFormat (false);
}

void
CBrowserXmp::
termProcess()
{
  window_->setTextBreakup(true);
  window_->setTextFormat (true);

  window_->endFontFace();
}

void
CBrowserXmp::
initLayout()
{
  window_->skipLine();
}

void
CBrowserXmp::
termLayout()
{
  window_->skipLine();
}
