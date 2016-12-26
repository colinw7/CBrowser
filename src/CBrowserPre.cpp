#include <CBrowserPre.h>
#include <CBrowserData.h>
#include <CBrowserWindow.h>

CBrowserPre::
CBrowserPre(CBrowserWindow *window, const CBrowserPreData &data) :
 CBrowserObject(window, CHtmlTagId::PRE), data_(data)
{
}

void
CBrowserPre::
initProcess()
{
  window_->startFontFace("courier");

  window_->setTextBreakup(false);
  window_->setTextFormat (false);
}

void
CBrowserPre::
termProcess()
{
  window_->endFontFace();

  window_->setTextBreakup(true);
  window_->setTextFormat (true);
}

void
CBrowserPre::
initLayout()
{
  window_->skipLine();
}

void
CBrowserPre::
termLayout()
{
  window_->skipLine();
}
