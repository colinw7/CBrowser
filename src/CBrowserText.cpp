#include <CBrowserText.h>
#include <CBrowserWindow.h>
#include <CBrowserLink.h>
#include <CFont.h>

CBrowserText::
CBrowserText(CBrowserWindow *window, const std::string &text, const CRGBA &color,
             bool underline, bool strike, CBrowserTextPlaceType place, bool breakup,
             bool format) :
 CBrowserObject(Type::TEXT), window_(window), text_(text), color_(color),
 underline_(underline), strike_(strike), place_(place), breakup_(breakup), format_(format)
{
  font_ = window->getFont();
  link_ = window->linkMgr()->getCurrentLink();
}

CBrowserText::
CBrowserText(CBrowserWindow *window, const CBrowserText &draw_text, const std::string &text) :
 CBrowserObject(Type::TEXT), window_(window), text_(text), font_(draw_text.font_),
 color_(draw_text.color_), underline_(draw_text.underline_), strike_(draw_text.strike_),
 place_(draw_text.place_), breakup_(draw_text.breakup_), format_(draw_text.format_),
 link_(draw_text.link_)
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
  if      (format_ && breakup_) {
    int i = 0;

    CHtmlLayoutSubCell *sub_cell = window_->getCurrentSubCell();

    if (text_[i] != '\0' && isspace(text_[i])) {
      if (sub_cell != NULL && sub_cell->getLeftCell() != NULL) {
        CHtmlLayoutSubCell::newCellRight(window_->getLayoutMgr(), false);

        window_->addText(this, " ");
      }

      while (text_[i] != '\0' && isspace(text_[i]))
        i++;
    }

    while (text_[i] != '\0') {
      int j = i;

      while (text_[i] != '\0' && ! isspace(text_[i]))
        i++;

      if (i - j == 0)
        break;

      CHtmlLayoutSubCell::newCellRight(window_->getLayoutMgr(), true);

      window_->addText(this, text_.substr(j, i - j));

      if (text_[i] != '\0' && isspace(text_[i])) {
        CHtmlLayoutSubCell::newCellRight(window_->getLayoutMgr(), false);

        window_->addText(this, " ");

        while (text_[i] != '\0' && isspace(text_[i]))
          i++;
      }
    }
  }
  else if (format_) {
    CHtmlLayoutSubCell::newCellRight(window_->getLayoutMgr(), true);

    window_->addText(this, text_);
  }
  else {
    int i = 0;

    while (text_[i] != '\0') {
      while (text_[i] == '\n') {
        CHtmlLayoutSubCell::newCellBelow(window_->getLayoutMgr(), false);

        window_->addText(this, "");

        i++;
      }

      int j = i;

      while (text_[i] != '\0' && text_[i] != '\n')
        i++;

      if (i > j) {
        CHtmlLayoutSubCell::newCellRight(window_->getLayoutMgr(), false);

        window_->addText(this, text_.substr(j, i - j));
      }

      if (text_[i] == '\n') {
        CHtmlLayoutSubCell::newCellBelow(window_->getLayoutMgr(), false);

        window_->addText(this, "");

        i++;
      }
    }
  }
}

void
CBrowserText::
draw(CHtmlLayoutMgr *, const CHtmlLayoutRegion &region)
{
  CHtmlLayoutSubCell *sub_cell = window_->getCurrentSubCell();

  window_->setFont(font_);

  window_->setForeground(color_);

  int width, ascent, descent;

  getTextBounds(font_, text_, &width, &ascent, &descent);

  int y_offset = 0;

  if      (place_ == CBrowserTextPlaceType::SUBSCRIPT)
    y_offset =  ascent/2;
  else if (place_ == CBrowserTextPlaceType::SUPERSCRIPT)
    y_offset = -ascent/2;

  int x1 = region.x;
  int y1 = region.y + y_offset + sub_cell->getAscent();

  window_->drawString(x1, y1, text_);

  if (strike_) {
    window_->drawLine(x1        , y1 - ascent/2    , x1 + width, y1 - ascent/2    );
    window_->drawLine(x1        , y1 - ascent/2 + 1, x1 + width, y1 - ascent/2 + 1);
  }

  if (underline_)
    window_->drawLine(x1, y1 + 1, x1 + width, y1 + 1);

  if (link_ != NULL)
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
