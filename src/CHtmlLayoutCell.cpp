#include <CHtmlLayoutCell.h>
#include <CHtmlLayoutArea.h>
#include <CHtmlLayoutMgr.h>
#include <climits>

CHtmlLayoutCell::
CHtmlLayoutCell()
{
  init();
}

CHtmlLayoutCell::
~CHtmlLayoutCell()
{
  term();
}

void
CHtmlLayoutCell::
init()
{
  left_         = 0;
  top_          = 0;
  x_            = 0;
  y_            = 0;
  width_        = 0;
  ascent_       = 0;
  descent_      = 0;
  indent_left_  = 0;
  indent_right_ = 0;
  resizable_    = false;
  halign_       = CHALIGN_TYPE_NONE;
  valign_       = CVALIGN_TYPE_NONE;
  border_       = false;
  clear_        = CHTML_LAYOUT_CLEAR_NONE;
  sub_cell_     = 0;
}

void
CHtmlLayoutCell::
term()
{
  for (auto &d : redraw_datas_)
    delete d;

  freeSubCells();
}

int
CHtmlLayoutCell::
getNumRedrawDatas()
{
  return redraw_datas_.size();
}

CHtmlLayoutRedrawData *
CHtmlLayoutCell::
getRedrawData(int i)
{
  return redraw_datas_[i];
}

void
CHtmlLayoutCell::
addRedrawData(CHtmlLayoutRedrawData *redraw_data)
{
  redraw_datas_.push_back(redraw_data);
}

int
CHtmlLayoutCell::
getNumSubCells()
{
  return sub_cells_.size();
}

CHtmlLayoutSubCell *
CHtmlLayoutCell::
getSubCell(int i)
{
  return sub_cells_[i];
}

void
CHtmlLayoutCell::
addSubCell(CHtmlLayoutSubCell *sub_cell)
{
  sub_cells_.push_back(sub_cell);
}

CHtmlLayoutCell *
CHtmlLayoutCell::
newCellBelow(CHtmlLayoutMgr *layout)
{
  CHtmlLayoutCell *cell1 = layout->getCurrentCell();

  CHtmlLayoutCell *cell;

  if (cell1 == 0 || cell1->getNumRedrawDatas() >= 0) {
    cell = new CHtmlLayoutCell;

    cell->left_ = 0;
    cell->top_  = cell1;

    layout->getCurrentArea()->addRedrawCell(cell);
  }
  else
    cell = cell1;

  cell->indent_left_  = layout->getCurrentArea()->getIndentLeft();
  cell->indent_right_ = layout->getCurrentArea()->getIndentRight();
  cell->halign_       = layout->getHAlign();
  cell->valign_       = layout->getVAlign();

  return cell;
}

CHtmlLayoutCell *
CHtmlLayoutCell::
newCellRight(CHtmlLayoutMgr *layout)
{
  CHtmlLayoutCell *cell1 = layout->getCurrentCell();

  CHtmlLayoutCell *cell = new CHtmlLayoutCell;

  cell->left_ = cell1;
  cell->top_  = 0;

  layout->getCurrentArea()->addRedrawCell(cell);

  cell->indent_left_  = layout->getCurrentArea()->getIndentLeft();
  cell->indent_right_ = layout->getCurrentArea()->getIndentRight();
  cell->halign_       = layout->getHAlign();
  cell->valign_       = layout->getVAlign();

  return cell;
}

void
CHtmlLayoutCell::
resetSubCells()
{
  width_    = 0;
  ascent_   = 0;
  descent_  = 0;
  sub_cell_ = 0;

  freeSubCells();
}

void
CHtmlLayoutCell::
freeSubCells()
{
  for (auto &c : sub_cells_)
    delete c;

  sub_cells_.clear();
}

void
CHtmlLayoutCell::
updateSubCellWidth(int width)
{
  if (sub_cell_)
    sub_cell_->setWidth(std::max(width, sub_cell_->getWidth()));
}

void
CHtmlLayoutCell::
updateSubCellHeight(int ascent, int descent)
{
  if (sub_cell_ && sub_cell_->getAscent() < ascent) {
    ascent_ += ascent - sub_cell_->getAscent();

    sub_cell_->setAscent(ascent);
  }

  if (sub_cell_ && sub_cell_->getDescent() < descent) {
    descent_ += descent - sub_cell_->getDescent();

    sub_cell_->setDescent(descent);
  }
}

void
CHtmlLayoutCell::
getSubCellsBoundingBox(int *x1, int *y1, int *x2, int *y2)
{
  int num_sub_cells = sub_cells_.size();

  getSubCellsBoundingBox(0, num_sub_cells - 1, x1, y1, x2, y2);
}

void
CHtmlLayoutCell::
getSubCellsBoundingBox(int i1, int i2, int *x1, int *y1, int *x2, int *y2)
{
  *x1 = 0;
  *y1 = 0;
  *x2 = 0;
  *y2 = 0;

  for (int i = i1; i <= i2; ++i) {
    CHtmlLayoutSubCell *sub_cell = sub_cells_[i];

    *x1 = std::min(*x1, sub_cell->getX());
    *y1 = std::min(*y1, sub_cell->getY());

    *x2 = std::max(*x2, sub_cell->getX() + sub_cell->getWidth ());
    *y2 = std::max(*y2, sub_cell->getY() + sub_cell->getHeight());
  }
}

void
CHtmlLayoutCell::
alignSubCellsRight(int x)
{
  int num_sub_cells = sub_cells_.size();

  alignSubCellsRight(0, num_sub_cells - 1, x);
}

void
CHtmlLayoutCell::
alignSubCellsRight(int i1, int i2, int x)
{
  for (int i = i1; i <= i2; ++i) {
    CHtmlLayoutSubCell *sub_cell = sub_cells_[i];

    sub_cell->setX(x - sub_cell->getWidth());
  }
}

void
CHtmlLayoutCell::
alignSubCellsCenter(int x)
{
  int num_sub_cells = sub_cells_.size();

  alignSubCellsCenter(0, num_sub_cells - 1, x);
}

void
CHtmlLayoutCell::
alignSubCellsCenter(int i1, int i2, int x)
{
  for (int i = i1; i <= i2; ++i) {
    CHtmlLayoutSubCell *sub_cell = sub_cells_[i];

    sub_cell->setX(x - sub_cell->getWidth()/2);
  }
}

void
CHtmlLayoutCell::
format(CHtmlLayoutMgr *layout, int width)
{
  resetSubCells();

  //-----

  int num_redraw_datas = getNumRedrawDatas();

  for (int i = 0; i < num_redraw_datas; ++i) {
    CHtmlLayoutRedrawData *redraw_data = getRedrawData(i);

    redraw_data->callFormatProc(layout);
  }

  //-----

  width -= getX();

  //-----

  int num_sub_cells = getNumSubCells();

  if (num_sub_cells == 0)
    return;

  //-----

  int x = 0;
  int y = 0;

  int left_margin   = 0;
  int right_margin  = width;
  int align_left_y  = -1;
  int align_right_y = -1;

  int i1 = 0;

  for (int i = 0; i < num_sub_cells; ++i) {
    CHtmlLayoutSubCell *sub_cell = getSubCell(i);

    int x1 = x + sub_cell->getWidth();

    CHtmlLayoutSubCell *top_cell = sub_cell->getTopCell();

    // Move Sub Cell to New Line

    if (top_cell != 0 ||
        (sub_cell->getBreakup() && x1 >= right_margin &&
         left_margin + sub_cell->getWidth() < right_margin)) {
      // Align previous cells baseline

      alignAscentDescent(i1, i - 1);

      //----

      x  = left_margin;
      y += getSubCell(i1)->getHeight();

      //----

      // Clear all cell's on left, right or both

      if      (sub_cell->getClear() == CHTML_LAYOUT_CLEAR_LEFT) {
        if (align_left_y  != -1)
          y = align_left_y;
      }
      else if (sub_cell->getClear() == CHTML_LAYOUT_CLEAR_RIGHT) {
        if (align_right_y != -1)
          y = align_right_y;
      }
      else if (sub_cell->getClear() == CHTML_LAYOUT_CLEAR_ALL) {
        if      (align_left_y != -1 && align_right_y != -1)
          y = std::max(align_left_y, align_right_y);
        else if (align_left_y != -1)
          y = align_left_y;
        else if (align_right_y != -1)
          y = align_right_y;
      }

      //----

      // Update align left and right

      if (align_left_y  != -1 && y >= align_left_y ) {
        align_left_y  = -1;
        left_margin   = 0;
      }

      if (align_right_y != -1 && y >= align_right_y) {
        align_right_y = -1;
        right_margin  = width;
      }

      //----

      i1 = i;
    }

    //----

    sub_cell->setY(y);

    if (sub_cell->getAlign() != CHALIGN_TYPE_RIGHT) {
      sub_cell->setX(x);

      x += sub_cell->getWidth();
    }
    else
      sub_cell->setX(right_margin - sub_cell->getWidth());

    //----

    // Update align left and right

    if      (sub_cell->getAlign() == CHALIGN_TYPE_LEFT) {
      align_left_y = std::max(align_left_y, y + sub_cell->getHeight());

      left_margin = x;

      i1 = i + 1;
    }
    else if (sub_cell->getAlign() == CHALIGN_TYPE_RIGHT) {
      align_right_y = std::max(align_right_y, y + sub_cell->getHeight());

      right_margin -= sub_cell->getWidth();

      i1 = i + 1;
    }
  }

  //----

  // Align previous cells baseline

  alignAscentDescent(i1, num_sub_cells - 1);

  //----

  int x1, y1, x2, y2;

  getSubCellsBoundingBox(&x1, &y1, &x2, &y2);

  setWidth  (x2 - x1);
  setAscent (y2 - y1);
  setDescent(0);
}

void
CHtmlLayoutCell::
alignAscentDescent(int i1, int i2)
{
  int ascent  = 0;
  int descent = 0;

  for (int i = i1; i <= i2; ++i) {
    CHtmlLayoutSubCell *sub_cell = sub_cells_[i];

    ascent  = std::max(ascent , sub_cell->getAscent ());
    descent = std::max(descent, sub_cell->getDescent());
  }

  for (int i = i1; i <= i2; ++i) {
    CHtmlLayoutSubCell *sub_cell = sub_cells_[i];

    sub_cell->setAscent (ascent );
    sub_cell->setDescent(descent);
  }
}

void
CHtmlLayoutCell::
redraw(CHtmlLayoutMgr *layout, int x, int y)
{
  int num_sub_cells = getNumSubCells();

  if (num_sub_cells <= 0)
    return;

  int x1 = x + getX();
  int y1 = y + getY();

  if  (getHAlign() == CHALIGN_TYPE_RIGHT ||
       getHAlign() == CHALIGN_TYPE_CENTER) {
    int offset = 0;

    int j = 0;

    while (j < num_sub_cells) {
      int last_x = -INT_MAX;

      int i = j;

      for ( ; i < num_sub_cells; i++) {
        if (sub_cells_[i]->getX() < last_x)
          break;

        last_x = sub_cells_[i]->getX();
      }

      --i;

      if (getHAlign() == CHALIGN_TYPE_RIGHT) {
        int x = sub_cells_[i]->getX() + sub_cells_[i]->getWidth();

        offset = width_ - x;
      }
      else {
        int x1 = sub_cells_[j]->getX() + sub_cells_[i]->getWidth()/2;
        int x2 = sub_cells_[i]->getX() + sub_cells_[i]->getWidth()/2;

        int xc = (x1 + x2)/2;

        offset = width_/2 - xc;
      }

      for (int k = j; k <= i; k++)
        sub_cells_[k]->setX(sub_cells_[k]->getX() + offset);

      j = i + 1;
    }
  }

  x1 = x + getX();
  y1 = y + getY();

  for (int i = 0; i < num_sub_cells; i++) {
    CHtmlLayoutSubCell *sub_cell = getSubCell(i);

    setCurrentSubCell(sub_cell);

    sub_cell->redraw(layout, x1, y1);
  }
}

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

  left_    = 0;
  top_     = 0;
  x_       = parent_->getX();
  y_       = parent_->getY();
  width_   = 0;
  ascent_  = 0;
  descent_ = 0;
  breakup_ = false;
  align_   = CHALIGN_TYPE_NONE;
  clear_   = CHTML_LAYOUT_CLEAR_NONE;
}

void
CHtmlLayoutSubCell::
term()
{
  if (! redraw_datas_.empty()) {
    int num_redraw_datas = redraw_datas_.size();

    for (int i = 0; i < num_redraw_datas; i++)
      delete redraw_datas_[i];

    redraw_datas_.clear();
  }
}

int
CHtmlLayoutSubCell::
getNumRedrawDatas()
{
  return redraw_datas_.size();
}

CHtmlLayoutRedrawData *
CHtmlLayoutSubCell::
getRedrawData(int i)
{
  return redraw_datas_[i];
}

void
CHtmlLayoutSubCell::
addRedrawData(CHtmlLayoutRedrawData *redraw_data)
{
  redraw_datas_.push_back(redraw_data);
}

CHtmlLayoutSubCell *
CHtmlLayoutSubCell::
newCellBelow(CHtmlLayoutMgr *layout, bool breakup)
{
  CHtmlLayoutCell *cell = layout->getCurrentCell();

  CHtmlLayoutSubCell *sub_cell1 = cell->getCurrentSubCell();

  CHtmlLayoutSubCell *sub_cell;

  if (sub_cell1 == 0 || sub_cell1->getNumRedrawDatas() > 0) {
    sub_cell = new CHtmlLayoutSubCell(cell);

    sub_cell->left_ = 0;
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
  sub_cell->top_  = 0;

  cell->addSubCell(sub_cell);

  sub_cell->breakup_ = breakup;

  sub_cell->parent_->setCurrentSubCell(sub_cell);

  return sub_cell;
}

void
CHtmlLayoutSubCell::
redraw(CHtmlLayoutMgr *layout, int x, int y)
{
  int x1 = x + getX();
  int y1 = y + getY();

  int num_redraw_datas = getNumRedrawDatas();

  for (int i = 0; i < num_redraw_datas; i++) {
    CHtmlLayoutRedrawData *redraw_data = getRedrawData(i);

    int x2 = x1;
    int y2 = y1;

    redraw_data->callRedrawProc(layout, &x2, &y2);
  }
}
