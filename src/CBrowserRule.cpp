#include <CBrowserRule.h>
#include <CBrowserWindow.h>

CBrowserRule::
CBrowserRule(CBrowserWindow *window, const CBrowserRuleData &data) :
 CBrowserObject(window, CHtmlTagId::HR), data_(data)
{
  data_.size = std::max(data_.size, 2);
}

CBrowserRule::
~CBrowserRule()
{
}

void
CBrowserRule::
initLayout()
{
  window_->newLine();

  window_->addCellRedrawData(this);

  window_->newLine();
}

void
CBrowserRule::
termLayout()
{
}

void
CBrowserRule::
format(CHtmlLayoutMgr *)
{
  window_->newSubCellBelow(false);

  int ascent;

  if (data_.size > 14)
    ascent = data_.size + 2;
  else
    ascent = 16;

  window_->updateSubCellHeight(ascent, 0);

  if (data_.width > 0 && data_.unit == CBrowserUnitsType::PIXEL)
    window_->updateSubCellWidth(data_.width);
  else
    window_->updateSubCellWidth(window_->getCurrentArea()->getWidth());

  //---

  window_->addSubCellRedrawData(this);
}

void
CBrowserRule::
draw(CHtmlLayoutMgr *, const CHtmlLayoutRegion &region)
{
  CHtmlLayoutSubCell *sub_cell = window_->getCurrentSubCell();

  int width;

  if      (data_.width == -1)
    width = sub_cell->getWidth();
  else if (data_.unit == CBrowserUnitsType::PERCENT)
    width = (int) (data_.width*sub_cell->getWidth()/100.0);
  else
    width = data_.width;

  int x1 = region.x;

  if      (data_.align == CHALIGN_TYPE_CENTER)
    x1 += (window_->getCurrentArea()->getWidth() - width)/2;
  else if (data_.align == CHALIGN_TYPE_RIGHT)
    x1 += window_->getCurrentArea()->getWidth() - width;

  int x2 = x1 + width;

  int y1 = region.y + sub_cell->getAscent()/2;

  //---

  if (data_.shade)
    window_->drawHRule(x1, x2, y1, data_.size);
  else {
    window_->setForeground(window_->getFg());

    window_->fillRectangle(x1, y1, x2 - x1 + 1, data_.size);
  }
}
