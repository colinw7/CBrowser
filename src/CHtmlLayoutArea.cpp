#include <CHtmlLayoutArea.h>
#include <CHtmlLayoutCell.h>
#include <CHtmlLayoutMgr.h>

CHtmlLayoutArea::
CHtmlLayoutArea()
{
  init();
}

CHtmlLayoutArea::
~CHtmlLayoutArea()
{
  term();
}

void
CHtmlLayoutArea::
init()
{
  if (! cells_.empty())
    term();

  region_.reset();

  indent_left_  = 0;
  indent_right_ = 0;
  cell_         = nullptr;
}

void
CHtmlLayoutArea::
term()
{
  for (auto &c : cells_)
    delete c;

  cells_.clear();
}

void
CHtmlLayoutArea::
addRedrawCell(CHtmlLayoutCell *cell)
{
  cells_.push_back(cell);

  cell_ = cell;
}

void
CHtmlLayoutArea::
format(CHtmlLayoutMgr *layout)
{
  layout->startArea(this);

  for (auto &c : cells_) {
    cell_ = c;

    if (cell_->getNumBoxes() == 0)
      continue;

    if      (cell_->getLeftCell() != 0) {
      CHtmlLayoutCell *cell1 = cell_->getLeftCell();

      cell_->setX(cell1->getX() + cell1->getWidth());
      cell_->setY(cell1->getY());
    }
    else if (cell_->getTopCell() != 0) {
      CHtmlLayoutCell *cell1 = cell_->getTopCell();

      cell_->setX(cell_->getIndentLeft());
      cell_->setY(cell1->getY() + cell1->getHeight());
    }
    else {
      cell_->setX(cell_->getIndentLeft());
      cell_->setY(region_.y);
    }

    cell_->format(layout, region_.width);
  }

  int x1, y1, x2, y2;

  getCellsBoundingBox(&x1, &y1, &x2, &y2);

  region_.width = std::max(region_.width , x2 - x1);

  region_.setHeight(std::max(region_.getHeight(), y2 - y1));

  layout->endArea();
}

void
CHtmlLayoutArea::
getCellsBoundingBox(int *x1, int *y1, int *x2, int *y2)
{
  int num_cells = cells_.size();

  *x1 = 0;
  *y1 = 0;
  *x2 = 0;
  *y2 = 0;

  for (int i = 0; i < num_cells; ++i) {
    CHtmlLayoutCell *cell = cells_[i];

    *x1 = std::min(*x1, cell->getX());
    *y1 = std::min(*y1, cell->getY());

    *x2 = std::max(*x2, cell->getX() + cell->getWidth ());
    *y2 = std::max(*y2, cell->getY() + cell->getHeight());
  }
}

void
CHtmlLayoutArea::
redraw(CHtmlLayoutMgr *layout)
{
  layout->startArea(this);

  for (const auto &cell : cells_) {
    cell_ = cell;

    if (cell_->getNumSubCells() == 0)
      continue;

    CHtmlLayoutRegion region = region_;

    if      (cell_->getHAlign() == CHALIGN_TYPE_LEFT)
      ;
    else if (cell_->getHAlign() == CHALIGN_TYPE_CENTER)
      region.x += (region.width - cell_->getWidth())/2;
    else if (cell_->getHAlign() == CHALIGN_TYPE_RIGHT)
      region.x +=  region.width - cell_->getWidth();

    if      (cell_->getVAlign() == CVALIGN_TYPE_TOP)
      ;
    else if (cell_->getVAlign() == CVALIGN_TYPE_CENTER)
      region.y += (region.getHeight() - cell_->getHeight())/2;
    else if (cell_->getVAlign() == CVALIGN_TYPE_BOTTOM)
      region.y +=  region.getHeight() - cell_->getHeight();
    else if (cell_->getVAlign() == CVALIGN_TYPE_BASELINE)
      region.y += cell_->getAscent();

    cell_->redraw(layout, region);
  }

  layout->endArea();
}

void
CHtmlLayoutArea::
accept(CHtmlLayoutVisitor &visitor)
{
  visitor.enter(this);

  for (const auto &cell : cells()) {
    cell->accept(visitor);
  }

  visitor.leave(this);
}
