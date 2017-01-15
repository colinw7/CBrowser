#include <CBrowserCSSStyle.h>
#include <CBrowserWindow.h>

CBrowserCSSStyle::
CBrowserCSSStyle(CBrowserWindow *window) :
 CBrowserObject(window, CHtmlTagId::STYLE)
{
}

void
CBrowserCSSStyle::
initProcess()
{
}

void
CBrowserCSSStyle::
termProcess()
{
  if (text_ != "") {
    window_->loadCSSText(text_);
  }
}
