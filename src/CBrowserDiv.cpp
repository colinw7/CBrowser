#include <CBrowserDiv.h>

CBrowserDiv::
CBrowserDiv(CBrowserWindow *window, const CBrowserDivData &data) :
 CBrowserObject(window, CHtmlTagId::DIV)
{
  setId(data.id);
}

void
CBrowserDiv::
initLayout()
{
}

void
CBrowserDiv::
termLayout()
{
}
