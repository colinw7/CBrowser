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

  x_            = 0;
  y_            = 0;
  width_        = 0;
  height_       = 0;
  indent_left_  = 0;
  indent_right_ = 0;
  cell_         = 0;
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

    if (cell_->getNumRedrawDatas() == 0)
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
      cell_->setY(y_);
    }

    cell_->format(layout, width_);
  }

  int x1, y1, x2, y2;

  getCellsBoundingBox(&x1, &y1, &x2, &y2);

  width_  = std::max(width_ , x2 - x1);
  height_ = std::max(height_, y2 - y1);

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

  int num = cells_.size();

  for (int i = 0; i < num; ++i) {
    cell_ = cells_[i];

    if (cell_->getNumSubCells() == 0)
      continue;

    int x1 = x_;

    if      (cell_->getHAlign() == CHALIGN_TYPE_LEFT)
      ;
    else if (cell_->getHAlign() == CHALIGN_TYPE_CENTER)
      x1 += (width_ - cell_->getWidth())/2;
    else if (cell_->getHAlign() == CHALIGN_TYPE_RIGHT)
      x1 +=  width_ - cell_->getWidth();

    int y1 = y_;

    if      (cell_->getVAlign() == CVALIGN_TYPE_TOP)
      ;
    else if (cell_->getVAlign() == CVALIGN_TYPE_CENTER)
      y1 += (height_ - cell_->getHeight())/2;
    else if (cell_->getVAlign() == CVALIGN_TYPE_BOTTOM)
      y1 +=  height_ - cell_->getHeight();
    else if (cell_->getVAlign() == CVALIGN_TYPE_BASELINE)
      y1 += cell_->getAscent();

    cell_->redraw(layout, x1, y1);
  }

  layout->endArea();
}
