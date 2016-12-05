#include <CHtmlLayoutMgr.h>
#include <CHtmlLayoutArea.h>
#include <CHtmlLayoutCell.h>
#include <CHtmlLayoutSubCell.h>

CHtmlLayoutMgr::
CHtmlLayoutMgr()
{
  init();
}

CHtmlLayoutMgr::
~CHtmlLayoutMgr()
{
}

void
CHtmlLayoutMgr::
init()
{
  current_area_ = nullptr;

  halign_ = CHALIGN_TYPE_NONE;
  valign_ = CVALIGN_TYPE_NONE;
}

void
CHtmlLayoutMgr::
startArea(CHtmlLayoutArea *area)
{
  if (current_area_)
    current_area_stack_.push_back(current_area_);

  current_area_ = area;
}

void
CHtmlLayoutMgr::
endArea()
{
  if (! current_area_stack_.empty()) {
    current_area_ = current_area_stack_[current_area_stack_.size() - 1];

    current_area_stack_.pop_back();
  }
  else
    current_area_ = 0;
}

CHtmlLayoutCell *
CHtmlLayoutMgr::
newCellBelow()
{
  return CHtmlLayoutCell::newCellBelow(this);
}

CHtmlLayoutCell *
CHtmlLayoutMgr::
newCellRight()
{
  return CHtmlLayoutCell::newCellRight(this);
}

CHtmlLayoutSubCell *
CHtmlLayoutMgr::
newSubCellBelow(bool breakup)
{
  return CHtmlLayoutSubCell::newCellBelow(this, breakup);
}

CHtmlLayoutSubCell *
CHtmlLayoutMgr::
newSubCellRight(bool breakup)
{
  return CHtmlLayoutSubCell::newCellRight(this, breakup);
}


void
CHtmlLayoutMgr::
setAlign(CHAlignType halign, CVAlignType valign)
{
  CHtmlLayoutCell *cell = getCurrentCell();

  if (cell) {
    cell->setHAlign(halign);
    cell->setVAlign(valign);
  }

  halign_ = halign;
  valign_ = valign;
}

CHtmlLayoutCell *
CHtmlLayoutMgr::
getCurrentCell() const
{
  return current_area_->getCurrentCell();
}
