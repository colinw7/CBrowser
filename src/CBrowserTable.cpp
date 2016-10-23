#include <CBrowserHtmlI.h>
#include <CRGBName.h>

class CBrowserTableRow {
 private:
  CHAlignType halign_;
  CVAlignType valign_;
  CRGBA       border_color_;
  CRGBA       background_color_;
  CRGBA       border_color_dark_;
  CRGBA       border_color_light_;

 public:
  CBrowserTableRow();
 ~CBrowserTableRow() { }

  CHAlignType getHAlign() const { return halign_; }
  CVAlignType getVAlign() const { return valign_; }

  CRGBA getBackgroundColor() const { return background_color_; }
};

class CBrowserTableCell {
 protected:
  bool             pad_;
  int              x_;
  int              y_;
  int              width_;
  int              width_unit_;
  int              height_;
  CHAlignType      halign_;
  CVAlignType      valign_;
  bool             wrap_;
  int              colspan_;
  int              rowspan_;
  CRGBA            border_color_;
  CRGBA            background_color_;
  CRGBA            border_color_dark_;
  CRGBA            border_color_light_;
  CHtmlLayoutArea *area_data_;

 public:
  CBrowserTableCell();

  virtual ~CBrowserTableCell() {
    delete area_data_;
  }

  bool getPad() const { return pad_; }

  int getWidth() const { return width_; }

  int getWidthUnit() const { return width_unit_; }

  int getHeight() const { return height_; }

  CHAlignType getHAlign() const { return halign_; }
  CVAlignType getVAlign() const { return valign_; }

  int getColSpan() const { return colspan_; }
  int getRowSpan() const { return rowspan_; }

  CRGBA getBackgroundColor() const { return background_color_; }

  CHtmlLayoutArea *getAreaData() const { return area_data_; }
};

class CBrowserTablePadCell : public CBrowserTableCell {
 public:
  CBrowserTablePadCell();
};

class CBrowserTableCaption {
 private:
  CHAlignType      halign_;
  CVAlignType      valign_;
  CHtmlLayoutArea *area_data_;

 public:
  CBrowserTableCaption();

 ~CBrowserTableCaption() {
    delete area_data_;
  }

  CHAlignType getHAlign() const { return halign_; }
  CVAlignType getVAlign() const { return valign_; }

  CHtmlLayoutArea *getAreaData() const { return area_data_; }
};

class CBrowserTable {
 public:
  CBrowserTable(CBrowserWindow *window);
 ~CBrowserTable();

  void endTable();

  CHAlignType getHAlign() const { return halign_; }
  CVAlignType getVAlign() const { return valign_; }

  int getWidth() const { return width_; }

  int getWidthUnit() const { return width_unit_; }

  CRGBA getBackgroundColor() const { return background_color_; }

  int getRowNum() const { return row_num_; }
  int getColNum() const { return col_num_; }

  void setRowNum(int row_num) { row_num_ = row_num; }
  void setColNum(int col_num) { col_num_ = col_num; }

  int getNumRows() const { return num_rows_; }
  int getNumCols() const { return num_cols_; }

  void setNumRows(int num_rows) { num_rows_ = num_rows; }
  void setNumCols(int num_cols) { num_cols_ = num_cols; }

  void setCaption(CBrowserTableCaption *caption) { caption_ = caption; }

  void resizeRowSize(int size) { row_cells_.resize(size); }

  void addRow(CBrowserTableRow *row) {
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

  void addRowCell(int row, CBrowserTableCell *cell) {
    row_cells_[row].push_back(cell);
  }

  void formatProc();
  void redrawProc(int *x, int *y);
  void freeProc();

 private:
  typedef std::vector<CBrowserTableCell *> Cells;
  typedef std::vector<CBrowserTableRow  *> Rows;
  typedef std::vector<Cells>               RowCells;

  CBrowserWindow                    *window_;
  bool                               border_;
  int                                hspace_;
  int                                vspace_;
  CHAlignType                        halign_;
  CVAlignType                        valign_;
  int                                width_;
  int                                width_unit_;
  int                                cell_padding_;
  int                                cell_spacing_;
  CRGBA                              border_color_;
  CRGBA                              background_color_;
  CRGBA                              border_color_dark_;
  CRGBA                              border_color_light_;
  int                                row_num_;
  int                                col_num_;
  Rows                               rows_;
  int                                num_rows_;
  int                                num_cols_;
  RowCells                           row_cells_;
  CBrowserTableCaption              *caption_;
  CBrowserFormatProc<CBrowserTable> *format_proc_;
  CBrowserRedrawProc<CBrowserTable> *redraw_proc_;
  CBrowserFreeProc<CBrowserTable>   *free_proc_;
};

static CBrowserTable        *current_table                 = nullptr;
static CBrowserTableRow     *current_row                   = nullptr;
static CBrowserTableCell    *current_cell                  = nullptr;
static CBrowserTableCaption *current_caption               = nullptr;
static bool                  table_border                  = false;
static int                   table_hspace                  = 2;
static int                   table_vspace                  = 2;
static CHAlignType           table_halign                  = CHALIGN_TYPE_NONE;
static CVAlignType           table_valign                  = CVALIGN_TYPE_NONE;
static int                   table_width                   = -1;
static int                   table_width_unit              = UNIT_NONE;
static int                   table_cell_padding            = 0;
static int                   table_cell_spacing            = 0;
static CRGBA                 table_border_color;
static CRGBA                 table_background_color;
static CRGBA                 table_border_color_dark;
static CRGBA                 table_border_color_light;
static CHAlignType           table_row_halign              = CHALIGN_TYPE_NONE;
static CVAlignType           table_row_valign              = CVALIGN_TYPE_NONE;
static CRGBA                 table_row_border_color;
static CRGBA                 table_row_background_color;
static CRGBA                 table_row_border_color_dark;
static CRGBA                 table_row_border_color_light;
static int                   table_cell_type               = DATA_CELL;
static CHAlignType           table_cell_halign             = CHALIGN_TYPE_NONE;
static CVAlignType           table_cell_valign             = CVALIGN_TYPE_NONE;
static int                   table_cell_width              = -1;
static int                   table_cell_width_unit         = UNIT_PIXEL;
static bool                  table_cell_wrap               = true;
static int                   table_cell_colspan            = 0;
static int                   table_cell_rowspan            = 0;
static CRGBA                 table_cell_border_color;
static CRGBA                 table_cell_background_color;
static CRGBA                 table_cell_border_color_dark;
static CRGBA                 table_cell_border_color_light;
static CHAlignType           table_caption_halign          = CHALIGN_TYPE_NONE;
static CVAlignType           table_caption_valign          = CVALIGN_TYPE_NONE;

void
HtmlSetTableBorder(bool border)
{
  table_border = border;
}

void
HtmlSetTableSpace(int hspace, int vspace)
{
  table_hspace = hspace;
  table_vspace = vspace;
}

void
HtmlSetTableAlign(CHAlignType halign, CVAlignType valign)
{
  table_halign = halign;
  table_valign = valign;
}

void
HtmlSetTableWidth(int width, int unit)
{
  table_width      = width;
  table_width_unit = unit;
}

void
HtmlSetTableCellPadding(int cell_padding)
{
  table_cell_padding = cell_padding;
}

void
HtmlSetTableCellSpacing(int cell_spacing)
{
  table_cell_spacing = cell_spacing;
}

void
HtmlSetTableBackgroundColor(CBrowserWindow *window, const std::string &background_color)
{
  table_background_color = window->getBg();

  CRGBA color = CRGBName::toRGBA(background_color);

  table_background_color = color;
}

void
HtmlSetTableBorderColors(CBrowserWindow *, const std::string &border_color,
                         const std::string &border_color_light,
                         const std::string &border_color_dark)
{
  CRGBA color = CRGBName::toRGBA(border_color);

  table_border_color = color;

  color = CRGBName::toRGBA(border_color_light);

  table_border_color_light = color;

  color = CRGBName::toRGBA(border_color_dark);

  table_border_color_dark = color;
}

void
HtmlStartTable(CBrowserWindow *window)
{
  if (current_table != NULL)
    current_table->endTable();

  current_table = new CBrowserTable(window);
}

void
HtmlEndTable(CBrowserWindow *)
{
  if (current_table != NULL)
    current_table->endTable();

  current_table = nullptr;
}

CBrowserTable::
CBrowserTable(CBrowserWindow *window) :
 window_(window)
{
  border_             = table_border;
  hspace_             = table_hspace;
  vspace_             = table_vspace;
  halign_             = table_halign;
  valign_             = table_valign;
  width_              = table_width;
  width_unit_         = table_width_unit;
  cell_padding_       = table_cell_padding;
  cell_spacing_       = table_cell_spacing;
  border_color_       = table_border_color;
  background_color_   = table_background_color;
  border_color_dark_  = table_border_color_dark;
  border_color_light_ = table_border_color_light;
  row_num_            = 0;
  col_num_            = 0;
  num_rows_           = 0;
  num_cols_           = 0;
  caption_            = nullptr;

  format_proc_ = new CBrowserFormatProc<CBrowserTable>(this);
  redraw_proc_ = nullptr;
  free_proc_   = new CBrowserFreeProc<CBrowserTable>(this);
}

CBrowserTable::
~CBrowserTable()
{
  delete format_proc_;
  delete redraw_proc_;
  delete free_proc_;
}

void
CBrowserTable::
endTable()
{
  if (current_row != NULL)
    HtmlEndTableRow(window_);

  if (current_caption != NULL)
    HtmlEndTableCaption(window_);

  /*----------*/

  window_->addCellRedrawData(format_proc_, free_proc_);

  /*----------*/

  CHtmlLayoutCell::newCellBelow(window_->getLayoutMgr());
}

void
HtmlSetTableRowAlign(CHAlignType halign, CVAlignType valign)
{
  if (halign == CHALIGN_TYPE_NONE)
    halign = table_halign;

  table_row_halign = halign;

  if (valign == CVALIGN_TYPE_NONE)
    valign = table_valign;

  table_row_valign = valign;
}

void
HtmlSetTableRowBackgroundColor(CBrowserWindow *, const std::string &background_color)
{
  table_row_background_color = table_background_color;

  CRGBA color = CRGBName::toRGBA(background_color);

  table_row_background_color = color;
}

void
HtmlSetTableRowBorderColors(CBrowserWindow *, const std::string &border_color,
                           const std::string &border_color_light,
                           const std::string &border_color_dark)
{
  CRGBA color = CRGBName::toRGBA(border_color);

  table_row_border_color = color;

  color = CRGBName::toRGBA(border_color_light);

  table_row_border_color_light = color;

  color = CRGBName::toRGBA(border_color_dark);

  table_row_border_color_dark = color;
}

void
HtmlStartTableRow(CBrowserWindow *window)
{
  if (current_table == NULL)
    return;

  /*----------*/

  if (current_cell != NULL)
    HtmlEndTableCell(window);

  /*----------*/

  CBrowserTableRow *row = new CBrowserTableRow;

  current_table->addRow(row);

  /*----------*/

  current_row = row;
}

CBrowserTableRow::
CBrowserTableRow()
{
  halign_             = table_row_halign;
  valign_             = table_row_valign;
  border_color_       = table_row_border_color;
  background_color_   = table_row_background_color;
  border_color_dark_  = table_row_border_color_dark;
  border_color_light_ = table_row_border_color_light;
}

void
HtmlEndTableRow(CBrowserWindow *window)
{
  if (current_table == NULL || current_row == NULL)
    return;

  /*----------*/

  if (current_cell != NULL)
    HtmlEndTableCell(window);

  /*----------*/

  current_row = nullptr;
}

void
HtmlSetTableCellType(int type)
{
  table_cell_type = type;
}

void
HtmlSetTableCellAlign(CHAlignType halign, CVAlignType valign)
{
  if (halign == CHALIGN_TYPE_NONE)
    halign = table_row_halign;

  if (halign == CHALIGN_TYPE_NONE) {
    if (table_cell_type == HEADER_CELL)
      halign = CHALIGN_TYPE_CENTER;
    else
      halign = CHALIGN_TYPE_LEFT;
  }

  table_cell_halign = halign;

  if (valign == CVALIGN_TYPE_NONE)
    valign = table_row_valign;

  if (valign == CVALIGN_TYPE_NONE) {
    if (table_cell_type == HEADER_CELL)
      valign = CVALIGN_TYPE_CENTER;
    else
      valign = CVALIGN_TYPE_CENTER;
  }

  table_cell_valign = valign;
}

void
HtmlSetTableCellWidth(int width, int unit)
{
  table_cell_width      = width;
  table_cell_width_unit = unit;
}

void
HtmlSetTableCellSpan(int colspan, int rowspan)
{
  table_cell_colspan = colspan;
  table_cell_rowspan = rowspan;
}

void
HtmlSetTableCellWrap(bool wrap)
{
  table_cell_wrap = wrap;
}

void
HtmlSetTableCellBackgroundColor(CBrowserWindow *, const std::string &background_color)
{
  table_cell_background_color = table_row_background_color;

  CRGBA color = CRGBName::toRGBA(background_color);

  table_cell_background_color = color;
}

void
HtmlSetTableCellBorderColors(CBrowserWindow *, const std::string &border_color,
                            const std::string &border_color_light,
                            const std::string &border_color_dark)
{
  CRGBA color = CRGBName::toRGBA(border_color);

  table_cell_border_color = color;

  color = CRGBName::toRGBA(border_color_light);

  table_cell_border_color_light = color;

  color = CRGBName::toRGBA(border_color_dark);

  table_cell_border_color_dark = color;
}

void
HtmlStartTableCell(CBrowserWindow *window)
{
  if (current_table == NULL)
    return;

  /*----------*/

  if (current_cell != NULL)
    HtmlEndTableCell(window);

  /*----------*/

  current_table->setNumRows(
    current_table->getRowNum() + table_cell_rowspan - 1);

  current_table->resizeRowSize(current_table->getNumRows());

  CBrowserTableCell *table_cell = new CBrowserTableCell;

  current_table->addRowCell(current_table->getRowNum() - 1, table_cell);

  window->getLayoutMgr()->startArea(table_cell->getAreaData());

  for (int i = 0; i < table_cell_rowspan; i++) {
    for (int j = 0; j < table_cell_colspan; j++) {
      if (i == 0 && j == 0)
        continue;

      CBrowserTableCell *table_cell = new CBrowserTablePadCell;

      current_table->addRowCell(current_table->getRowNum() + i - 1,
                                table_cell);
    }
  }

  window->setAlign(table_cell->getHAlign(), table_cell->getVAlign());

  CHtmlLayoutCell::newCellRight(window->getLayoutMgr());

  current_table->setColNum(current_table->getColNum() + table_cell_colspan);

  if (current_table->getColNum() > current_table->getNumCols())
    current_table->setNumCols(current_table->getColNum());

  /*----------*/

  current_cell = table_cell;
}

CBrowserTableCell::
CBrowserTableCell()
{
  pad_                = false;
  x_                  = 0;
  y_                  = 0;
  width_              = table_cell_width;
  width_unit_         = table_cell_width_unit;
  height_             = 0;
  halign_             = table_cell_halign;
  valign_             = table_cell_valign;
  wrap_               = table_cell_wrap;
  colspan_            = table_cell_colspan;
  rowspan_            = table_cell_rowspan;
  border_color_       = table_cell_border_color;
  background_color_   = table_cell_background_color;
  border_color_dark_  = table_cell_border_color_dark;
  border_color_light_ = table_cell_border_color_light;
  area_data_          = new CHtmlLayoutArea();
}

CBrowserTablePadCell::
CBrowserTablePadCell()
{
  pad_        = true;
  x_          = 0;
  y_          = 0;
  width_      = 0;
  width_unit_ = UNIT_PIXEL;
  height_     = 0;
  halign_     = CHALIGN_TYPE_NONE;
  valign_     = CVALIGN_TYPE_NONE;
  wrap_       = false;
  colspan_    = 0;
  rowspan_    = 0;
  area_data_  = new CHtmlLayoutArea();
}

void
HtmlEndTableCell(CBrowserWindow *window)
{
  if (current_table == NULL || current_cell == NULL)
    return;

  /*----------*/

  current_cell = nullptr;

  /*----------*/

  window->getLayoutMgr()->endArea();
}

void
HtmlSetTableCaptionAlign(CHAlignType halign, CVAlignType valign)
{
  if (halign == CHALIGN_TYPE_NONE)
    halign = table_halign;

  if (halign == CHALIGN_TYPE_NONE)
    halign = CHALIGN_TYPE_CENTER;

  table_caption_halign = halign;

  if (valign == CVALIGN_TYPE_NONE)
    valign = table_valign;

  if (valign == CVALIGN_TYPE_NONE)
    valign = CVALIGN_TYPE_TOP;

  table_caption_valign = valign;
}

void
HtmlStartTableCaption(CBrowserWindow *window)
{
  if (current_table == NULL)
    return;

  /*-----------*/

  CBrowserTableCaption *caption = new CBrowserTableCaption;

  window->getLayoutMgr()->startArea(caption->getAreaData());

  window->setAlign(caption->getHAlign(), caption->getVAlign());

  CHtmlLayoutCell::newCellBelow(window->getLayoutMgr());

  current_table->setCaption(caption);

  /*-----------*/

  current_caption = caption;
}

CBrowserTableCaption::
CBrowserTableCaption()
{
  halign_    = table_caption_halign;
  valign_    = table_caption_valign;
  area_data_ = new CHtmlLayoutArea();
}

void
HtmlEndTableCaption(CBrowserWindow *window)
{
  if (current_table == NULL || current_caption == NULL)
    return;

  /*----------*/

  window->getLayoutMgr()->endArea();

  /*----------*/

  current_caption = nullptr;
}

void
CBrowserTable::
formatProc()
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

      if (row_cells_[i][j]->getWidthUnit() == UNIT_PIXEL)
        area_data->setWidth(row_cells_[i][j]->getWidth());
      else
        area_data->setWidth(0);

      area_data->format(window_->getLayoutMgr());

      if (getWidthUnit() == UNIT_PIXEL &&
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

    for ( ; j < getNumCols(); j++) {
      CBrowserTableCell *table_cell = new CBrowserTablePadCell;

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

  CHtmlLayoutSubCell::newCellBelow(window_->getLayoutMgr(), false);

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

  if (caption_ != NULL) {
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

  delete redraw_proc_;

  redraw_proc_ = new CBrowserRedrawProc<CBrowserTable>(this);

  window_->addSubCellRedrawData(redraw_proc_);
}

void
CBrowserTable::
redrawProc(int *x, int *y)
{
  int x1 = *x;
  int y1 = *y;

  /*-------------*/

  if (caption_ != NULL && caption_->getVAlign() == CVALIGN_TYPE_TOP) {
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
          window_->drawBorder(x2 - 1, y2 - 1, width + 2, height + 2,
                             CBROWSER_BORDER_TYPE_IN);

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
    window_->drawBorder(x1 + i, y1 + i, width - 2*i, height - 2*i,
                       CBROWSER_BORDER_TYPE_OUT);

  /*-------------*/

  if (caption_ != NULL && caption_->getVAlign() == CVALIGN_TYPE_BOTTOM) {
    CHtmlLayoutArea *area_data = caption_->getAreaData();

    area_data->setX(x1);
    area_data->setY(y1 + height + 2);

    area_data->redraw(window_->getLayoutMgr());

    y2 += area_data->getHeight() + 2;
  }

  /*-------------*/

  *y = y2;
}

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
