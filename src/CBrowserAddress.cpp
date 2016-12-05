#include <CBrowserAddress.h>
#include <CBrowserWindow.h>

CBrowserAddress::
CBrowserAddress(CBrowserWindow *window) :
 CBrowserObject(CHtmlTagId::ADDRESS), window_(window)
{
}

CBrowserAddress::
~CBrowserAddress()
{
}
