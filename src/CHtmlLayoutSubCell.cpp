#include <CHtmlLayoutSubCell.h>
#include <CHtmlLayoutCell.h>
#include <CHtmlLayoutBox.h>
#include <CHtmlLayoutMgr.h>

CHtmlLayoutSubCell::
CHtmlLayoutSubCell(CHtmlLayoutCell *parent) :
 parent_(parent)
{
  init();
}

CHtmlLayoutSubCell::
~CHtmlLayoutSubCell()
{
  term();
}

void
CHtmlLayoutSubCell::
init()
{
  term();

  left_ = nullptr;
  top_  = nullptr;

  region_.reset();

  region_.x = parent_->getX();
  region_.y = parent_->getY();

  breakup_ = false;
  align_   = CHALIGN_TYPE_NONE;
  clear_   = CHtmlLayoutClearType::NONE;
}

void
CHtmlLayoutSubCell::
term()
{
  //for (auto &b : boxes_)
  //  delete b;

  boxes_.clear();
}

int
CHtmlLayoutSubCell::
getNumBoxes() const
{
  return boxes_.size();
}

CHtmlLayoutBox *
CHtmlLayoutSubCell::
getBox(int i)
{
  return boxes_[i];
}

void
CHtmlLayoutSubCell::
addBox(CHtmlLayoutBox *box)
{
  boxes_.push_back(box);
}

CHtmlLayoutSubCell *
CHtmlLayoutSubCell::
newCellBelow(CHtmlLayoutMgr *layout, bool breakup)
{
  CHtmlLayoutCell *cell = layout->getCurrentCell();

  CHtmlLayoutSubCell *sub_cell1 = cell->getCurrentSubCell();

  CHtmlLayoutSubCell *sub_cell;

  if (! sub_cell1 || sub_cell1->getNumBoxes() > 0) {
    sub_cell = new CHtmlLayoutSubCell(cell);

    sub_cell->left_ = nullptr;
    sub_cell->top_  = sub_cell1;

    cell->addSubCell(sub_cell);
  }
  else {
    sub_cell = sub_cell1;

    sub_cell->init();
  }

  sub_cell->breakup_ = breakup;

  sub_cell->parent_->setCurrentSubCell(sub_cell);

  return sub_cell;
}

CHtmlLayoutSubCell *
CHtmlLayoutSubCell::
newCellRight(CHtmlLayoutMgr *layout, bool breakup)
{
  CHtmlLayoutCell *cell = layout->getCurrentCell();

  CHtmlLayoutSubCell *sub_cell = new CHtmlLayoutSubCell(cell);

  sub_cell->left_ = cell->getCurrentSubCell();
  sub_cell->top_  = nullptr;

  cell->addSubCell(sub_cell);

  sub_cell->breakup_ = breakup;

  sub_cell->parent_->setCurrentSubCell(sub_cell);

  return sub_cell;
}

void
CHtmlLayoutSubCell::
redraw(CHtmlLayoutMgr *layout, const CHtmlLayoutRegion &region)
{
  CHtmlLayoutRegion region1 = region;

  region1.x = region.x + getX();
  region1.y = region.y + getY();

  for (const auto &box : boxes()) {
    box->draw(layout, region1);
  }
}

void
CHtmlLayoutSubCell::
accept(CHtmlLayoutVisitor &visitor)
{
  visitor.enter(this);

  for (const auto &box : boxes()) {
    box->accept(visitor);
  }

  visitor.leave(this);
}

void
CHtmlLayoutSubCell::
print(std::ostream &os) const
{
  printSize(os);

  for (const auto &box : boxes()) {
    os << "  ";
    box->print(os);
    os << std::endl;
  }
}
