#include <CBrowserLine.h>
#include <CBrowserWindow.h>
#include <CBrowserForm.h>
#include <CBrowserText.h>
#include <CBrowserLink.h>
#include <CBrowserGraphics.h>

void
CBrowserLine::
addWord(int x, int y, const CBrowserWord &word)
{
  words_.push_back(PosWord(x, y, word));

  width_ += word.width();

  if (word.getFloat() == CBrowserWord::Float::NONE) {
    ascent_  = std::max(ascent_ , word.ascent ());
    descent_ = std::max(descent_, word.descent());
  }

  if (word.getFloat() != CBrowserWord::Float::RIGHT)
    empty_ = false;
}

void
CBrowserLine::
draw(CBrowserWindowIFace *window, int width, const CHAlignType &halign)
{
  int xo = 0;

  if      (halign == CHALIGN_TYPE_RIGHT)
    xo =  width - width_;
  else if (halign == CHALIGN_TYPE_CENTER)
    xo = (width - width_)/2;

  auto *graphics = window->graphics();

  for (auto &w : words_) {
    auto &word = w.word;

    if      (word.type() == CBrowserWord::Type::TEXT) {
      CBrowserText *text = word.textObj();

      graphics->drawText(xo + w.x, w.y + ascent_, word.text(), word.pen(), word.font());

      if (text->link())
        text->link()->addRect(xo + w.x, w.y, xo + w.x + word.width(), w.y + word.height());
    }
    else if (word.type() == CBrowserWord::Type::IMAGE) {
      graphics->drawImage(xo + w.x, w.y, word.image());
    }
    else if (word.type() == CBrowserWord::Type::INPUT) {
      auto *input = word.inputObj();

      input->drawWidget(window, CTextBox(xo + w.x, w.y, word.width(), word.height()));
    }

    if (word.isSelected())
      window->drawSelected(xo + w.x, w.y, word.width(), ascent_ + descent_);
  }
}

bool
CBrowserLine::
isEmpty() const
{
  return empty_;
  //return words_.empty();
}

void
CBrowserLine::
clear()
{
  words_.clear();

  empty_ = true;

  width_   = 0;
  ascent_  = 0;
  descent_ = 0;
}
