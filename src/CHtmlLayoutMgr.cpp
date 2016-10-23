#include <CHtmlLayoutMgr.h>
#include <CHtmlLayoutCell.h>
#include <CHtmlLayoutArea.h>

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
  current_area_ = 0;

  halign_ = CHALIGN_TYPE_NONE;
  valign_ = CVALIGN_TYPE_NONE;
}

void
CHtmlLayoutMgr::
startArea(CHtmlLayoutArea *area)
{
  if (current_area_ != 0)
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

void
CHtmlLayoutMgr::
setAlign(CHAlignType halign, CVAlignType valign)
{
  CHtmlLayoutCell *cell = getCurrentCell();

  if (cell != 0) {
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
