#include <CBrowserTable.h>
#include <CBrowserWindow.h>
#include <CRGBName.h>

CBrowserTableMgr::
CBrowserTableMgr(CBrowserWindow *window) :
 window_(window)
{
}

CBrowserTable *
CBrowserTableMgr::
startTable(const CBrowserTableData &tableData)
{
  if (current_table_)
    current_table_->endTable();

  current_table_ = new CBrowserTable(window_, tableData);

  return current_table_;
}

void
CBrowserTableMgr::
endTable()
{
  if (current_table_)
    current_table_->endTable();

  current_table_ = nullptr;
}

void
CBrowserTableMgr::
startTableRow(const CBrowserTableRowData &data)
{
  if (! current_table_)
    return;

  /*----------*/

  if (current_cell_)
    endTableCell();

  /*----------*/

  CBrowserTableRow *row = new CBrowserTableRow(current_table_, data);

  current_table_->addRow(row);

  /*----------*/

  current_row_ = row;
}

void
CBrowserTableMgr::
endTableRow()
{
  if (! current_table_ || ! current_row_)
    return;

  /*----------*/

  if (current_cell_)
    endTableCell();

  /*----------*/

  current_row_ = nullptr;
}

void
CBrowserTableMgr::
startTableCell(const CBrowserTableCellData &data)
{
  if (! current_table_)
    return;

  /*----------*/

  if (current_cell_)
    endTableCell();

  /*----------*/

  current_table_->setNumRows(current_table_->getRowNum() + data.rowspan - 1);

  current_table_->resizeRowSize(current_table_->getNumRows());

  CBrowserTableCell *table_cell = new CBrowserTableCell(current_row_, data);

  current_table_->addRowCell(current_table_->getRowNum() - 1, table_cell);

  window_->startArea(table_cell->getAreaData());

  for (int i = 0; i < data.rowspan; i++) {
    for (int j = 0; j < data.colspan; j++) {
      if (i == 0 && j == 0)
        continue;

      CBrowserTableCell *table_cell = new CBrowserTablePadCell(current_row_, data);

      current_table_->addRowCell(current_table_->getRowNum() + i - 1, table_cell);
    }
  }

  window_->setAlign(table_cell->getHAlign(), table_cell->getVAlign());

  window_->newColumn();

  current_table_->setColNum(current_table_->getColNum() + data.colspan);

  if (current_table_->getColNum() > current_table_->getNumCols())
    current_table_->setNumCols(current_table_->getColNum());

  /*----------*/

  current_cell_ = table_cell;
}

void
CBrowserTableMgr::
endTableCell()
{
  if (! current_table_ || ! current_cell_)
    return;

  /*----------*/

  current_cell_ = nullptr;

  /*----------*/

  window_->endArea();
}

void
CBrowserTableMgr::
startTableCaption(const CBrowserTableCaptionData &data)
{
  if (! current_table_)
    return;

  /*-----------*/

  CBrowserTableCaption *caption = new CBrowserTableCaption(current_table_, data);

  window_->startArea(caption->getAreaData());

  window_->setAlign(caption->getHAlign(), caption->getVAlign());

  window_->newLine();

  current_table_->setCaption(caption);

  /*-----------*/

  current_caption_ = caption;
}

void
CBrowserTableMgr::
endTableCaption()
{
  if (! current_table_ || ! current_caption_)
    return;

  /*----------*/

  window_->endArea();

  /*----------*/

  current_caption_ = nullptr;
}

//------

CBrowserTable::
CBrowserTable(CBrowserWindow *window, const CBrowserTableData &data) :
 CBrowserObject(CHtmlTagId::TABLE), window_(window)
{
  border_             = data.border;
  hspace_             = data.hspace;
  vspace_             = data.vspace;
  halign_             = data.halign;
  valign_             = data.valign;
  width_              = data.width;
  width_unit_         = data.width_unit;
  cell_padding_       = data.cell_padding;
  cell_spacing_       = data.cell_spacing;
  border_color_       = data.border_color;
  background_color_   = data.background_color;
  border_color_dark_  = data.border_color_dark;
  border_color_light_ = data.border_color_light;
}

CBrowserTable::
~CBrowserTable()
{
}

void
CBrowserTable::
endTable()
{
  if (window_->tableMgr()->currentRow())
    window_->tableMgr()->endTableRow();

  if (window_->tableMgr()->currentCaption())
    window_->tableMgr()->endTableCaption();

  /*----------*/

  window_->addCellRedrawData(this);

  /*----------*/

  window_->newLine();
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

    /*-------*/

    CBrowserTableCellData cellData;

    for ( ; j < getNumCols(); j++) {
      CBrowserTableCell *table_cell = new CBrowserTablePadCell(nullptr, cellData);

      addRowCell(i, table_cell);
    }
  }

  /*-------*/

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

      int height1 = area_data->getHeight()/row_cells_[k][j]->getRowSpan();

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

  /*-------*/

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

      int width1 = area_data->getWidth()/row_cells_[i][k]->getColSpan();

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

  /*-------------*/

  window_->newSubCellBelow(false);

  /*-------------*/

  int width = 0;

  for (int i = 0; i < getNumCols(); i++)
    width += row_cells_[0][i]->getAreaData()->getWidth();

  width += 2*getNumCols()*cell_padding_;

  width += (getNumCols() + 1)*cell_spacing_;

  if (border_)
    width += 2*getNumCols() + 2*border_;

  /*-------------*/

  int height = 0;

  for (int i = 0; i < getNumRows(); i++)
    height += row_cells_[i][0]->getAreaData()->getHeight();

  height += 2*getNumRows()*cell_padding_;

  height += (getNumRows() + 1)*cell_spacing_;

  if (border_)
    height += 2*getNumRows() + 2*border_;

  /*-------------*/

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

  /*-------------*/

  window_->addSubCellRedrawData(this);
}

void
CBrowserTable::
draw(CHtmlLayoutMgr *, const CHtmlLayoutRegion &region)
{
  int x1 = region.x;
  int y1 = region.y;

  /*-------------*/

  if (caption_ && caption_->getVAlign() == CVALIGN_TYPE_TOP) {
    CHtmlLayoutArea *area_data = caption_->getAreaData();

    area_data->setX(x1);
    area_data->setY(y1);

    area_data->redraw(window_->getLayoutMgr());

    y1 += area_data->getHeight() + 2;
  }

  /*-------------*/

  int y2 = y1 + cell_spacing_;

  if (border_)
    y2 += border_ + 1;

  for (int i = 0; i < getNumRows(); i++) {
    int x2 = x1 + cell_spacing_;

    if (border_)
      x2 += border_ + 1;

    for (int j = 0; j < getNumCols(); j++) {
      CHtmlLayoutArea *area_data = row_cells_[i][j]->getAreaData();

      if (! row_cells_[i][j]->getPad()) {
        int width  = (area_data->getWidth()  + 2*cell_padding_)*
                     row_cells_[i][j]->getColSpan();
        int height = (area_data->getHeight() + 2*cell_padding_)*
                     row_cells_[i][j]->getRowSpan();

        if (row_cells_[i][j]->getColSpan() > 1) {
          width += cell_spacing_*(row_cells_[i][j]->getColSpan() - 1);

          if (border_)
            width += 2*(row_cells_[i][j]->getColSpan() - 1);
        }

        if (row_cells_[i][j]->getRowSpan() > 1) {
          height += cell_spacing_*(row_cells_[i][j]->getRowSpan() - 1);

          if (border_)
            height += 2*(row_cells_[i][j]->getRowSpan() - 1);
        }

        if (row_cells_[i][j]->getBackgroundColor() !=
            window_->getBg()) {
          window_->setForeground(row_cells_[i][j]->getBackgroundColor());

          window_->fillRectangle(x2, y2, width, height);
        }

        if (border_)
          window_->drawBorder(x2 - 1, y2 - 1, width + 2, height + 2, CBrowserBorderType::IN);

        area_data->setX(x2 + cell_padding_);
        area_data->setY(y2 + cell_padding_);

        width  = area_data->getWidth();
        height = area_data->getHeight();

        area_data->setWidth (width *row_cells_[i][j]->getColSpan());
        area_data->setHeight(height*row_cells_[i][j]->getRowSpan());

        if (row_cells_[i][j]->getColSpan() > 1) {
          area_data->setWidth(area_data->getWidth() +
                              (2*cell_padding_ + cell_spacing_)*
                              (row_cells_[i][j]->getColSpan() - 1));

          if (border_)
            area_data->setWidth(area_data->getWidth() +
              2*(row_cells_[i][j]->getColSpan() - 1));
        }

        if (row_cells_[i][j]->getRowSpan() > 1) {
          area_data->setHeight(area_data->getHeight() +
                               (2*cell_padding_ + cell_spacing_)*
                               (row_cells_[i][j]->getRowSpan() - 1));

          if (border_)
            area_data->setHeight(area_data->getHeight() +
              2*(row_cells_[i][j]->getRowSpan() - 1));
        }

        area_data->redraw(window_->getLayoutMgr());

        area_data->setWidth (width );
        area_data->setHeight(height);
      }

      x2 += area_data->getWidth() + 2*cell_padding_ + cell_spacing_;

      if (border_)
        x2 += 2;
    }

    CHtmlLayoutArea *area_data = row_cells_[i][0]->getAreaData();

    y2 += area_data->getHeight() + 2*cell_padding_ + cell_spacing_;

    if (border_)
      y2 += 2;
  }

  /*-------------*/

  int width = 0;

  for (int i = 0; i < getNumCols(); i++)
    width += row_cells_[0][i]->getAreaData()->getWidth();

  width += 2*getNumCols()*cell_padding_;

  width += (getNumCols() + 1)*cell_spacing_;

  if (border_)
    width += 2*getNumCols() + 2*border_;

  /*-------------*/

  int height = 0;

  for (int i = 0; i < getNumRows(); i++)
    height += row_cells_[i][0]->getAreaData()->getHeight();

  height += 2*getNumRows()*cell_padding_;

  height += (getNumRows() + 1)*cell_spacing_;

  if (border_)
    height += 2*getNumRows() + 2*border_;

  /*-------------*/

  for (int i = 0; i < border_; i++)
    window_->drawBorder(x1 + i, y1 + i, width - 2*i, height - 2*i, CBrowserBorderType::OUT);

  /*-------------*/

  if (caption_ && caption_->getVAlign() == CVALIGN_TYPE_BOTTOM) {
    CHtmlLayoutArea *area_data = caption_->getAreaData();

    area_data->setX(x1);
    area_data->setY(y1 + height + 2);

    area_data->redraw(window_->getLayoutMgr());

    y2 += area_data->getHeight() + 2;
  }

  /*-------------*/

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

  /*-------------*/

  int num_row_cells = row_cells_.size();

  for (int i = 0; i < num_row_cells; i++) {
    int num_row_cells1 = row_cells_[i].size();

    for (int j = 0; j < num_row_cells1; j++)
      delete row_cells_[i][j];
  }

  /*-------------*/

  delete caption_;
  delete this;
}
#endif

//------

CBrowserTableRow::
CBrowserTableRow(CBrowserTable *table, const CBrowserTableRowData &data)
{
  halign_             = data.halign;
  valign_             = data.valign;
  border_color_       = data.border_color;
  background_color_   = data.background_color;
  border_color_dark_  = data.border_color_dark;
  border_color_light_ = data.border_color_light;

  if (halign_ == CHALIGN_TYPE_NONE)
    halign_ = table->getHAlign();

  if (valign_ == CVALIGN_TYPE_NONE)
    valign_ = table->getVAlign();
}

//------

CBrowserTableCell::
CBrowserTableCell(CBrowserTableRow *row, const CBrowserTableCellData &data)
{
  width_              = data.width;
  width_unit_         = data.width_unit;
  halign_             = data.halign;
  valign_             = data.valign;
  wrap_               = data.wrap;
  colspan_            = data.colspan;
  rowspan_            = data.rowspan;
  border_color_       = data.border_color;
  background_color_   = data.background_color;
  border_color_dark_  = data.border_color_dark;
  border_color_light_ = data.border_color_light;

  if (halign_ == CHALIGN_TYPE_NONE)
    halign_ = row->getHAlign();

  if (halign_ == CHALIGN_TYPE_NONE) {
    if (data.type == CBrowserTableCellData::Type::HEADER)
      halign_ = CHALIGN_TYPE_CENTER;
    else
      halign_ = CHALIGN_TYPE_LEFT;
  }

  if (valign_ == CVALIGN_TYPE_NONE)
    valign_ = row->getVAlign();

  if (valign_ == CVALIGN_TYPE_NONE) {
    if (data.type == CBrowserTableCellData::Type::HEADER)
      valign_ = CVALIGN_TYPE_CENTER;
    else
      valign_ = CVALIGN_TYPE_CENTER;
  }

  area_data_ = new CHtmlLayoutArea();
}

//------

CBrowserTablePadCell::
CBrowserTablePadCell(CBrowserTableRow *row, const CBrowserTableCellData &data) :
 CBrowserTableCell(row, data)
{
  area_data_ = new CHtmlLayoutArea();
}

//------

CBrowserTableCaption::
CBrowserTableCaption(CBrowserTable *table, const CBrowserTableCaptionData &data)
{
  halign_    = data.halign;
  valign_    = data.valign;
  area_data_ = new CHtmlLayoutArea();

  if (halign_ == CHALIGN_TYPE_NONE)
    halign_ = table->getHAlign();

  if (halign_ == CHALIGN_TYPE_NONE)
    halign_ = CHALIGN_TYPE_CENTER;

  if (valign_ == CVALIGN_TYPE_NONE)
    valign_ = table->getVAlign();

  if (valign_ == CVALIGN_TYPE_NONE)
    valign_ = CVALIGN_TYPE_TOP;
}
