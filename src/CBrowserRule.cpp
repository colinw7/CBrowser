#include <CBrowserRule.h>
#include <CBrowserWindow.h>

CBrowserRule::
CBrowserRule(CBrowserWindow *window, const CBrowserRuleData &data) :
 CBrowserObject(Type::RULE), window_(window), align_(data.align), shade_(data.shade),
 size_(data.size), width_(data.width), unit_(data.unit)
{
  size_ = std::max(size_, 2);
}

CBrowserRule::
~CBrowserRule()
{
}

void
CBrowserRule::
format(CHtmlLayoutMgr *)
{
  CHtmlLayoutSubCell::newCellBelow(window_->getLayoutMgr(), false);

  int ascent;

  if (size_ > 14)
    ascent = size_ + 2;
  else
    ascent = 16;

  window_->updateSubCellHeight(ascent, 0);

  if (width_ > 0 && unit_ == UNIT_PIXEL)
    window_->updateSubCellWidth(width_);
  else
    window_->updateSubCellWidth(window_->getCurrentArea()->getWidth());

  /*-------------*/

  window_->addSubCellRedrawData(this);
}

void
CBrowserRule::
draw(CHtmlLayoutMgr *, const CHtmlLayoutRegion &region)
{
  CHtmlLayoutSubCell *sub_cell = window_->getCurrentSubCell();

  int width;

  if      (width_ == -1)
    width = sub_cell->getWidth();
  else if (unit_ == UNIT_PERCENT)
    width = (int) (width_* sub_cell->getWidth()/100.0);
  else
    width = width_;

  int x1 = region.x;

  if      (align_ == CHALIGN_TYPE_CENTER)
    x1 += (window_->getCurrentArea()->getWidth() - width)/2;
  else if (align_ == CHALIGN_TYPE_RIGHT)
    x1 += window_->getCurrentArea()->getWidth() - width;

  int x2 = x1 + width;

  int y1 = region.y + sub_cell->getAscent()/2;

  /*---------*/

  if (shade_)
    window_->drawHRule(x1, x2, y1, size_);
  else {
    window_->setForeground(window_->getFg());

    window_->fillRectangle(x1, y1, x2 - x1 + 1, size_);
  }
}
