#include <CBrowserLabel.h>
#include <CBrowserText.h>
#include <CBrowserWindow.h>
#include <CBrowserLink.h>
#include <CFont.h>

CBrowserLabel::
CBrowserLabel(CBrowserWindow *window, const std::string &text, int width,
              CHAlignType align, const CRGBA &color) :
 CBrowserObject(window, CHtmlTagId::LABEL), text_(text), width_(width),
 align_(align), color_(color)
{
  font_ = window->getFont();
}

CBrowserLabel::
~CBrowserLabel()
{
}

void
CBrowserLabel::
initLayout()
{
  window_->addCellRedrawData(this);
}

void
CBrowserLabel::
termLayout()
{
}

void
CBrowserLabel::
format(CHtmlLayoutMgr *)
{
  window_->newSubCellRight(false);

  int ascent  = 0;
  int descent = 0;

  window_->getTextHeight(font_, &ascent, &descent);

  window_->updateSubCellHeight(ascent, descent);

  int char_width = 0;

  window_->getTextWidth(font_, "X", &char_width);

  window_->updateSubCellWidth(width_*char_width);

  //---

  window_->addSubCellRedrawData(this);
}

void
CBrowserLabel::
draw(CHtmlLayoutMgr *, const CHtmlLayoutRegion &region)
{
  int char_width;

  window_->getTextWidth(font_, "X", &char_width);

  int text_width;

  window_->getTextWidth(font_, text_, &text_width);

  //---

  int x1 = region.x;

  if      (align_ == CHALIGN_TYPE_LEFT)
    ;
  else if (align_ == CHALIGN_TYPE_CENTER)
    x1 += (width_*char_width - text_width)/2;
  else if (align_ == CHALIGN_TYPE_RIGHT)
    x1 +=  width_*char_width - text_width;

  CHtmlLayoutSubCell *sub_cell = window_->getCurrentSubCell();

  int y1 = region.y + sub_cell->getAscent();

  //---

  window_->setFont(font_);

  window_->setForeground(color_);

  window_->drawString(x1, y1, text_);

  //---

  if (isSelected())
    window_->drawSelected(region.getX(), region.getY(), region.getWidth(), region.getHeight());

  //region.x += width_*char_width;
}
