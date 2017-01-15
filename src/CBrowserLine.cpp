#include <CBrowserLine.h>
#include <CBrowserWindow.h>
#include <CBrowserForm.h>

void
CBrowserLine::
addWord(int x, int y, const CBrowserWord &word)
{
  words_.push_back(PosWord(x, y, word));

  width_ += word.width();

  ascent_  = std::max(ascent_ , word.ascent ());
  descent_ = std::max(descent_, word.descent());
}

void
CBrowserLine::
draw(CBrowserWindow *window, int width, const CHAlignType &halign)
{
  int xo = 0;

  if      (halign == CHALIGN_TYPE_RIGHT)
    xo =  width - width_;
  else if (halign == CHALIGN_TYPE_CENTER)
    xo = (width - width_)/2;

  for (auto &w : words_) {
    if      (w.word.type() == CBrowserWord::Type::TEXT) {
      window->drawText(xo + w.x, w.y + ascent_, w.word.text(), w.word.pen(), w.word.font());
    }
    else if (w.word.type() == CBrowserWord::Type::IMAGE) {
      window->drawImage(xo + w.x, w.y, w.word.image());
    }
    else if (w.word.type() == CBrowserWord::Type::INPUT) {
      w.word.input()->drawWidget(window, CTextBox(xo + w.x, w.y, w.word.width(), w.word.height()));
    }

    if (w.word.isSelected())
      window->drawSelected(xo + w.x, w.y, w.word.width(), ascent_ + descent_);
  }
}

bool
CBrowserLine::
isEmpty() const
{
  return words_.empty();
}

void
CBrowserLine::
clear()
{
  words_.clear();

  width_   = 0;
  ascent_  = 0;
  descent_ = 0;
}
