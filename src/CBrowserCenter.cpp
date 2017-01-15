#include <CBrowserCenter.h>
#include <CBrowserWindow.h>

CBrowserCenter::
CBrowserCenter(CBrowserWindow *window) :
 CBrowserObject(window, CHtmlTagId::CENTER)
{
  setHAlign(CHALIGN_TYPE_CENTER);
}
