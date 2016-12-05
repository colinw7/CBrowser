#include <CBrowserText.h>
#include <CBrowserWindow.h>
#include <CBrowserLink.h>
#include <CFont.h>

CBrowserText::
CBrowserText(CBrowserWindow *window, const std::string &text, const CBrowserTextData &data) :
 CBrowserObject(CHtmlTagId::TEXT), window_(window), text_(text), data_(data)
{
  font_ = window->getFont();
  link_ = window->linkMgr()->getCurrentLink();
}

CBrowserText::
CBrowserText(CBrowserWindow *window, const CBrowserText &draw_text, const std::string &text) :
 CBrowserObject(CHtmlTagId::TEXT), window_(window), text_(text), font_(draw_text.font_),
 data_(draw_text.data_), link_(draw_text.link_)
{
}

CBrowserText::
~CBrowserText()
{
}

void
CBrowserText::
format(CHtmlLayoutMgr *)
{
  if (texts_.empty()) {
    // split text into words
    if      (data_.format && data_.breakup) {
      int i = 0;

      CHtmlLayoutSubCell *sub_cell = window_->getCurrentSubCell();

      if (text_[i] != '\0' && isspace(text_[i])) {
        if (sub_cell && sub_cell->getLeftCell())
          addText(" ", CBrowserTextPos::RIGHT, false);

        while (text_[i] != '\0' && isspace(text_[i]))
          i++;
      }

      while (text_[i] != '\0') {
        int j = i;

        while (text_[i] != '\0' && ! isspace(text_[i]))
          i++;

        if (i - j == 0)
          break;

        addText(text_.substr(j, i - j), CBrowserTextPos::RIGHT, true);

        if (text_[i] != '\0' && isspace(text_[i])) {
          addText(" ", CBrowserTextPos::RIGHT, false);

          while (text_[i] != '\0' && isspace(text_[i]))
            i++;
        }
      }
    }
    // no split
    else if (data_.format) {
      addText(text_, CBrowserTextPos::RIGHT, true);
    }
    // split text into lines
    else {
      int i = 0;

      while (text_[i] != '\0') {
        while (text_[i] == '\n') {
          addText("", CBrowserTextPos::BELOW, false);

          i++;
        }

        int j = i;

        while (text_[i] != '\0' && text_[i] != '\n')
          i++;

        if (i > j)
          addText(text_.substr(j, i - j), CBrowserTextPos::RIGHT, false);

        if (text_[i] == '\n') {
          addText("", CBrowserTextPos::BELOW, false);

          i++;
        }
      }
    }
  }

  // format
  for (const auto &t : texts_) {
    if (pos_ == CBrowserTextPos::RIGHT)
      window_->newSubCellRight(t->data_.breakup);
    else
      window_->newSubCellBelow(t->data_.breakup);

    int ascent, descent;

    window_->getTextHeight(t->font(), &ascent, &descent);

    window_->updateSubCellHeight(ascent, descent);

    int width;

    window_->getTextWidth(t->font(), t->text(), &width);

    window_->updateSubCellWidth(width);

    window_->addSubCellRedrawData(t);
  }
}

void
CBrowserText::
addText(const std::string &str, CBrowserTextPos pos, bool breakup)
{
  CBrowserText *text = new CBrowserText(window_, *this, str);

  text->data_.breakup = breakup;
  text->pos_          = pos;

  texts_.push_back(text);

  addChild(text);
}

void
CBrowserText::
draw(CHtmlLayoutMgr *, const CHtmlLayoutRegion &region)
{
  CHtmlLayoutSubCell *sub_cell = window_->getCurrentSubCell();

  window_->setFont(font_);

  window_->setForeground(data_.color);

  int width, ascent, descent;

  getTextBounds(font_, text_, &width, &ascent, &descent);

  int y_offset = 0;

  if      (data_.place == CBrowserTextPlaceType::SUBSCRIPT)
    y_offset =  ascent/2;
  else if (data_.place == CBrowserTextPlaceType::SUPERSCRIPT)
    y_offset = -ascent/2;

  int x1 = region.x;
  int y1 = region.y + y_offset + sub_cell->getAscent();

  window_->drawString(x1, y1, text_);

  if (data_.strike) {
    window_->drawLine(x1, y1 - ascent/2    , x1 + width, y1 - ascent/2    );
    window_->drawLine(x1, y1 - ascent/2 + 1, x1 + width, y1 - ascent/2 + 1);
  }

  if (data_.underline)
    window_->drawLine(x1, y1 + 1, x1 + width, y1 + 1);

  if (link_)
    link_->addRect(x1, y1 - sub_cell->getAscent(), x1 + width, y1 + sub_cell->getDescent());

  //---

  window_->drawOutline(region.getX(), region.getY(),
                       region.getWidth(), region.getHeight(), CRGBA(1,0,0));

  //region.x += width;
}

void
CBrowserText::
getTextBounds(CFontPtr font, const std::string &text, int *width, int *ascent, int *descent)
{
  *width   = font->getStringWidth(text);
  *ascent  = font->getCharAscent();
  *descent = font->getCharDescent();
}
