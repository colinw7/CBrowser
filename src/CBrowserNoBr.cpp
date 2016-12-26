#include <CBrowserNoBr.h>
#include <CBrowserWindow.h>

CBrowserNoBr::
CBrowserNoBr(CBrowserWindow *window) :
 CBrowserObject(window, CHtmlTagId::NOBR)
{
}

void
CBrowserNoBr::
initProcess()
{
  window_->setTextBreakup(false);
}

void
CBrowserNoBr::
termProcess()
{
  window_->setTextBreakup(true);
}
