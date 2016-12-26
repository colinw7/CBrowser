#include <CBrowserCenter.h>
#include <CBrowserWindow.h>

CBrowserCenter::
CBrowserCenter(CBrowserWindow *window) :
 CBrowserObject(window, CHtmlTagId::CENTER)
{
}

void
CBrowserCenter::
initLayout()
{
  window_->skipLine();

  window_->setAlign(CHALIGN_TYPE_CENTER, CVALIGN_TYPE_TOP);
}

void
CBrowserCenter::
termLayout()
{
  window_->skipLine();

  window_->setAlign(CHALIGN_TYPE_LEFT, CVALIGN_TYPE_TOP);
}
