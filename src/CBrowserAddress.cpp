#include <CBrowserAddress.h>
#include <CBrowserWindow.h>

CBrowserAddress::
CBrowserAddress(CBrowserWindow *window) :
 CBrowserObject(window, CHtmlTagId::ADDRESS)
{
}

CBrowserAddress::
~CBrowserAddress()
{
}

void
CBrowserAddress::
initProcess()
{
  window_->startItalic();
}

void
CBrowserAddress::
termProcess()
{
  window_->endItalic();
}
