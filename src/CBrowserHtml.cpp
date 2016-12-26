#include <CBrowserHtml.h>
#include <CBrowserWindow.h>
#include <CBrowserDocument.h>

CBrowserHtml::
CBrowserHtml(CBrowserWindow *window) :
 CBrowserObject(window, CHtmlTagId::HTML)
{
  window_->setRootObject(this);
}

void
CBrowserHtml::
initLayout()
{
}

void
CBrowserHtml::
termLayout()
{
}
