#include <CBrowserBlockQuote.h>
#include <CBrowserWindow.h>

CBrowserBlockQuote::
CBrowserBlockQuote(CBrowserWindow *window) :
 CBrowserObject(window, CHtmlTagId::BLOCKQUOTE)
{
}

void
CBrowserBlockQuote::
initProcess()
{
  window_->startItalic();
}

void
CBrowserBlockQuote::
termProcess()
{
  window_->endItalic();
}

void
CBrowserBlockQuote::
initLayout()
{
  window_->indentLeft (2);
  window_->indentRight(2);

  window_->skipLine();
}

void
CBrowserBlockQuote::
termLayout()
{
  window_->indentLeft (-2);
  window_->indentRight(-2);

  window_->skipLine();
}
