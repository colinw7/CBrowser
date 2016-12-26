#include <CBrowserTable.h>
#include <CBrowserWindow.h>
#include <CRGBName.h>

CBrowserTable::
CBrowserTable(CBrowserWindow *window, const CBrowserTableData &data) :
 CBrowserObject(window, CHtmlTagId::TABLE), data_(data)
{
}

CBrowserTable::
~CBrowserTable()
{
}

void
CBrowserTable::
addRow(CBrowserTableRow *row)
{
  rows_.push_back(row);

  ++row_num_;

  if (row_num_ > num_rows_) {
    col_num_ = 0;

    ++num_rows_;

    resizeRowSize(num_rows_);
  }
  else
    col_num_ = row_cells_[row_num_ - 1].size();
}

void
CBrowserTable::
addRowCell(int row, CBrowserTableCell *cell)
{
  row_cells_[row].push_back(cell);
}

void
CBrowserTable::
initProcess()
{
}

void
CBrowserTable::
termProcess()
{
}

void
CBrowserTable::
initLayout()
{
}

void
CBrowserTable::
termLayout()
{
  window_->addCellRedrawData(this);

  //---

  window_->newLine();
}

void
CBrowserTable::
format(CHtmlLayoutMgr *)
{
  for (int i = 0; i < getNumRows(); i++) {
    int num_cols = row_cells_[i].size();

    int j = 0;

    for ( ; j < num_cols; j++) {
      if (row_cells_[i][j]->getPad())
        continue;

      CHtmlLayoutArea *area_data = row_cells_[i][j]->getAreaData();

      area_data->setX(0);
      area_data->setY(0);

      area_data->setHeight(0);

      if (row_cells_[i][j]->getWidthUnit() == CBrowserUnitsType::PIXEL)
        area_data->setWidth(row_cells_[i][j]->getWidth());
      else
        area_data->setWidth(0);

      area_data->format(window_->getLayoutMgr());

      if (getWidthUnit() == CBrowserUnitsType::PIXEL &&
          area_data->getWidth() > getWidth()) {
        area_data->setWidth(getWidth());

        area_data->format(window_->getLayoutMgr());
      }

      if (area_data->getWidth() > window_->getCurrentArea()->getWidth()) {
        area_data->setWidth(window_->getCurrentArea()->getWidth());

        area_data->format(window_->getLayoutMgr());
      }
    }

    //---

    CBrowserTableCellData cellData;

    for ( ; j < getNumCols(); j++) {
      CBrowserTableCell *table_cell = new CBrowserTablePadCell(window_, cellData);

      addRowCell(i, table_cell);
    }
  }

  //---

  int *row_heights = new int [getNumRows() + 1];

  for (int i = 0; i < getNumRows(); i++) {
    row_heights[i] = 0;

    for (int j = 0; j < getNumCols(); j++) {
      int k = i;

      if (row_cells_[i][j]->getPad()) {
        k--;

        while (k >= 0 && row_cells_[k][j]->getPad())
          k--;

        if (k < 0)
          continue;
      }

      CHtmlLayoutArea *area_data = row_cells_[k][j]->getAreaData();

      int rowSpan = std::max(row_cells_[k][j]->getRowSpan(), 1);

      int height1 = area_data->getHeight()/rowSpan;

      if (height1 > row_heights[i])
        row_heights[i] = height1;
    }
  }

  for (int i = 0; i < getNumRows(); i++) {
    for (int j = 0; j < getNumCols(); j++) {
      CHtmlLayoutArea *area_data = row_cells_[i][j]->getAreaData();

      area_data->setHeight(row_heights[i]);
    }
  }

  delete row_heights;

  //---

  int *col_widths = new int [getNumCols() + 1];

  for (int j = 0; j < getNumCols(); j++) {
    col_widths[j] = 0;

    for (int i = 0; i < getNumRows(); i++) {
      int k = j;

      if (row_cells_[i][j]->getPad()) {
        k--;

        while (k >= 0 && row_cells_[i][k]->getPad())
          k--;

        if (k < 0)
          continue;
      }

      CHtmlLayoutArea *area_data = row_cells_[i][k]->getAreaData();

      int colSpan = std::max(row_cells_[i][k]->getColSpan(), 1);

      int width1 = area_data->getWidth()/colSpan;

      if (width1 > col_widths[j])
        col_widths[j] = width1;
    }
  }

  for (int j = 0; j < getNumCols(); j++) {
    for (int i = 0; i < getNumRows(); i++) {
      CHtmlLayoutArea *area_data = row_cells_[i][j]->getAreaData();

      area_data->setWidth(col_widths[j]);
    }
  }

  delete col_widths;

  //---

  window_->newSubCellBelow(false);

  //---

  int width = 0;

  for (int i = 0; i < getNumCols(); i++)
    width += row_cells_[0][i]->getAreaData()->getWidth();

  width += 2*getNumCols()*data_.cell_padding;

  width += (getNumCols() + 1)*data_.cell_spacing;

  if (data_.border)
    width += 2*getNumCols() + 2*data_.border;

  //---

  int height = 0;

  for (int i = 0; i < getNumRows(); i++)
    height += row_cells_[i][0]->getAreaData()->getHeight();

  height += 2*getNumRows()*data_.cell_padding;

  height += (getNumRows() + 1)*data_.cell_spacing;

  if (data_.border)
    height += 2*getNumRows() + 2*data_.border;

  //---

  if (caption_) {
    CHtmlLayoutArea *area_data = caption_->getAreaData();

    area_data->setX(0);
    area_data->setY(0);

    area_data->setWidth (width);
    area_data->setHeight(0);

    area_data->format(window_->getLayoutMgr());

    if (area_data->getWidth() > width)
      width = area_data->getWidth();

    height += area_data->getHeight();
  }

  window_->updateSubCellWidth (width);
  window_->updateSubCellHeight(height, 0);

  //---

  window_->addSubCellRedrawData(this);
}

void
CBrowserTable::
draw(CHtmlLayoutMgr *, const CHtmlLayoutRegion &region)
{
  int x1 = region.x;
  int y1 = region.y;

  //---

  if (caption_ && caption_->getVAlign() == CVALIGN_TYPE_TOP) {
    CHtmlLayoutArea *area_data = caption_->getAreaData();

    area_data->setX(x1);
    area_data->setY(y1);

    area_data->redraw(window_->getLayoutMgr());

    y1 += area_data->getHeight() + 2;
  }

  //---

  int y2 = y1 + data_.cell_spacing;

  if (data_.border)
    y2 += data_.border + 1;

  for (int i = 0; i < getNumRows(); i++) {
    int x2 = x1 + data_.cell_spacing;

    if (data_.border)
      x2 += data_.border + 1;

    for (int j = 0; j < getNumCols(); j++) {
      CHtmlLayoutArea *area_data = row_cells_[i][j]->getAreaData();

      if (! row_cells_[i][j]->getPad()) {
        int width  = (area_data->getWidth()  + 2*data_.cell_padding)*
                     row_cells_[i][j]->getColSpan();
        int height = (area_data->getHeight() + 2*data_.cell_padding)*
                     row_cells_[i][j]->getRowSpan();

        if (row_cells_[i][j]->getColSpan() > 1) {
          width += data_.cell_spacing*(row_cells_[i][j]->getColSpan() - 1);

          if (data_.border)
            width += 2*(row_cells_[i][j]->getColSpan() - 1);
        }

        if (row_cells_[i][j]->getRowSpan() > 1) {
          height += data_.cell_spacing*(row_cells_[i][j]->getRowSpan() - 1);

          if (data_.border)
            height += 2*(row_cells_[i][j]->getRowSpan() - 1);
        }

        if (row_cells_[i][j]->getBackgroundColor() !=
            window_->getBg()) {
          window_->setForeground(row_cells_[i][j]->getBackgroundColor());

          window_->fillRectangle(x2, y2, width, height);
        }

        if (data_.border)
          window_->drawBorder(x2 - 1, y2 - 1, width + 2, height + 2, CBrowserBorderType::IN);

        area_data->setX(x2 + data_.cell_padding);
        area_data->setY(y2 + data_.cell_padding);

        width  = area_data->getWidth();
        height = area_data->getHeight();

        area_data->setWidth (width *row_cells_[i][j]->getColSpan());
        area_data->setHeight(height*row_cells_[i][j]->getRowSpan());

        if (row_cells_[i][j]->getColSpan() > 1) {
          area_data->setWidth(area_data->getWidth() +
                              (2*data_.cell_padding + data_.cell_spacing)*
                              (row_cells_[i][j]->getColSpan() - 1));

          if (data_.border)
            area_data->setWidth(area_data->getWidth() +
              2*(row_cells_[i][j]->getColSpan() - 1));
        }

        if (row_cells_[i][j]->getRowSpan() > 1) {
          area_data->setHeight(area_data->getHeight() +
                               (2*data_.cell_padding + data_.cell_spacing)*
                               (row_cells_[i][j]->getRowSpan() - 1));

          if (data_.border)
            area_data->setHeight(area_data->getHeight() +
              2*(row_cells_[i][j]->getRowSpan() - 1));
        }

        area_data->redraw(window_->getLayoutMgr());

        area_data->setWidth (width );
        area_data->setHeight(height);
      }

      x2 += area_data->getWidth() + 2*data_.cell_padding + data_.cell_spacing;

      if (data_.border)
        x2 += 2;
    }

    CHtmlLayoutArea *area_data = row_cells_[i][0]->getAreaData();

    y2 += area_data->getHeight() + 2*data_.cell_padding + data_.cell_spacing;

    if (data_.border)
      y2 += 2;
  }

  //---

  int width = 0;

  for (int i = 0; i < getNumCols(); i++)
    width += row_cells_[0][i]->getAreaData()->getWidth();

  width += 2*getNumCols()*data_.cell_padding;

  width += (getNumCols() + 1)*data_.cell_spacing;

  if (data_.border)
    width += 2*getNumCols() + 2*data_.border;

  //---

  int height = 0;

  for (int i = 0; i < getNumRows(); i++)
    height += row_cells_[i][0]->getAreaData()->getHeight();

  height += 2*getNumRows()*data_.cell_padding;

  height += (getNumRows() + 1)*data_.cell_spacing;

  if (data_.border)
    height += 2*getNumRows() + 2*data_.border;

  //---

  for (int i = 0; i < data_.border; i++)
    window_->drawBorder(x1 + i, y1 + i, width - 2*i, height - 2*i, CBrowserBorderType::OUT);

  //---

  if (caption_ && caption_->getVAlign() == CVALIGN_TYPE_BOTTOM) {
    CHtmlLayoutArea *area_data = caption_->getAreaData();

    area_data->setX(x1);
    area_data->setY(y1 + height + 2);

    area_data->redraw(window_->getLayoutMgr());

    y2 += area_data->getHeight() + 2;
  }

  //---

  //region.y = y2;
}

#if 0
void
CBrowserTable::
freeProc()
{
  int num_rows = rows_.size();

  for (int i = 0; i < num_rows; i++)
    delete rows_[i];

  //---

  int num_row_cells = row_cells_.size();

  for (int i = 0; i < num_row_cells; i++) {
    int num_row_cells1 = row_cells_[i].size();

    for (int j = 0; j < num_row_cells1; j++)
      delete row_cells_[i][j];
  }

  //---

  delete caption_;
  delete this;
}
#endif

//------

CBrowserTableRow::
CBrowserTableRow(CBrowserWindow *window, const CBrowserTableRowData &data) :
 CBrowserObject(window, CHtmlTagId::TR), data_(data)
{
}

void
CBrowserTableRow::
initProcess()
{
  CBrowserTable *table = parentType<CBrowserTable>();

  //---

  if (data_.halign == CHALIGN_TYPE_NONE && table)
    data_.halign = table->getHAlign();

  if (data_.valign == CVALIGN_TYPE_NONE && table)
    data_.valign = table->getVAlign();

  //---

  if (table)
    table->addRow(this);
}

void
CBrowserTableRow::
termProcess()
{
}

void
CBrowserTableRow::
initLayout()
{
}

void
CBrowserTableRow::
termLayout()
{
}

//------

CBrowserTableCell::
CBrowserTableCell(CBrowserWindow *window, CHtmlTagId id, const CBrowserTableCellData &data) :
 CBrowserObject(window, id), data_(data)
{
}

void
CBrowserTableCell::
initProcess()
{
  CBrowserTable    *table = parentType<CBrowserTable>();
  CBrowserTableRow *row   = parentType<CBrowserTableRow>();

  //---

  if (data_.halign == CHALIGN_TYPE_NONE) {
    if (row)
      data_.halign = row->getHAlign();
    else
      data_.halign = CHALIGN_TYPE_LEFT;
  }

  if (data_.halign == CHALIGN_TYPE_NONE) {
    if (data_.type == CBrowserTableCellData::Type::HEADER)
      data_.halign = CHALIGN_TYPE_CENTER;
    else
      data_.halign = CHALIGN_TYPE_LEFT;
  }

  if (data_.valign == CVALIGN_TYPE_NONE) {
    if (row)
      data_.valign = row->getVAlign();
    else
      data_.valign = CVALIGN_TYPE_CENTER;
  }

  if (data_.valign == CVALIGN_TYPE_NONE) {
    if (data_.type == CBrowserTableCellData::Type::HEADER)
      data_.valign = CVALIGN_TYPE_CENTER;
    else
      data_.valign = CVALIGN_TYPE_CENTER;
  }

  area_data_ = new CHtmlLayoutArea();

  //---

  if (table) {
    table->setNumRows(table->getRowNum() + data_.rowspan - 1);

    table->resizeRowSize(table->getNumRows());

    table->addRowCell(table->getRowNum() - 1, this);

    for (int i = 0; i < data_.rowspan; i++) {
      for (int j = 0; j < data_.colspan; j++) {
        if (i == 0 && j == 0)
          continue;

        if (! table)
          continue;

        CBrowserTableCell *padCell = new CBrowserTablePadCell(window_, data_);

        table->addRowCell(table->getRowNum() + i - 1, padCell);
      }
    }

    //---

    table->setColNum(table->getColNum() + data_.colspan);

    if (table->getColNum() > table->getNumCols())
      table->setNumCols(table->getColNum());
  }
}

void
CBrowserTableCell::
termProcess()
{
}

void
CBrowserTableCell::
initLayout()
{
  window_->startArea(getAreaData());

  window_->setAlign(getHAlign(), getVAlign());

  window_->newColumn();
}

void
CBrowserTableCell::
termLayout()
{
  window_->endArea();
}

//------

CBrowserTablePadCell::
CBrowserTablePadCell(CBrowserWindow *window, const CBrowserTableCellData &data) :
 CBrowserTableCell(window, CHtmlTagId::NONE, data)
{
  area_data_ = new CHtmlLayoutArea();
}

//------

CBrowserTableCaption::
CBrowserTableCaption(CBrowserWindow *window, const CBrowserTableCaptionData &data) :
 CBrowserObject(window, CHtmlTagId::CAPTION), data_(data)
{
}

void
CBrowserTableCaption::
initProcess()
{
  CBrowserTable *table = parentType<CBrowserTable>();

  //---

  area_data_ = new CHtmlLayoutArea();

  if (data_.halign == CHALIGN_TYPE_NONE && table)
    data_.halign = table->getHAlign();

  if (data_.halign == CHALIGN_TYPE_NONE)
    data_.halign = CHALIGN_TYPE_CENTER;

  if (data_.valign == CVALIGN_TYPE_NONE && table)
    data_.valign = table->getVAlign();

  if (data_.valign == CVALIGN_TYPE_NONE)
    data_.valign = CVALIGN_TYPE_TOP;

  //---

  if (table) {
    table->setCaption(this);
  }
}

void
CBrowserTableCaption::
termProcess()
{
}

void
CBrowserTableCaption::
initLayout()
{
  window_->startArea(getAreaData());

  window_->setAlign(getHAlign(), getVAlign());

  window_->newLine();
}

void
CBrowserTableCaption::
termLayout()
{
  window_->endArea();
}
