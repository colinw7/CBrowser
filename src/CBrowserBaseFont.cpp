#include <CBrowserBaseFont.h>
#include <CBrowserWindow.h>

CBrowserBaseFont::
CBrowserBaseFont(CBrowserWindow *window, const CBrowserBaseFontData &data) :
 CBrowserObject(window, CHtmlTagId::BASEFONT), data_(data)
{
}

void
CBrowserBaseFont::
initProcess()
{
  if (data_.sizeSet) {
    if      (data_.sizeSign == 1)
      window_->increaseBaseFontSize(data_.size);
    else if (data_.sizeSign == -1)
      window_->decreaseBaseFontSize(data_.size);
    else
      window_->setBaseFontSize(data_.size);
  }
  else
    window_->resetBaseFontSize();
}

void
CBrowserBaseFont::
termProcess()
{
  window_->resetFontSize();
}
