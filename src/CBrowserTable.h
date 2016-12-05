#ifndef CBrowserTable_H
#define CBrowserTable_H

#include <CBrowserObject.h>
#include <CRGBA.h>

struct CBrowserTableData {
  bool              border       = false;
  int               hspace       = 2;
  int               vspace       = 2;
  CHAlignType       halign       = CHALIGN_TYPE_NONE;
  CVAlignType       valign       = CVALIGN_TYPE_NONE;
  int               width        = -1;
  CBrowserUnitsType width_unit   = CBrowserUnitsType::NONE;
  int               cell_padding = 0;
  int               cell_spacing = 0;
  CRGBA             border_color;
  CRGBA             background_color;
  CRGBA             border_color_dark;
  CRGBA             border_color_light;
};

struct CBrowserTableRowData {
  CHAlignType halign              = CHALIGN_TYPE_NONE;
  CVAlignType valign              = CVALIGN_TYPE_NONE;
  CRGBA       border_color;
  CRGBA       background_color;
  CRGBA       border_color_dark;
  CRGBA       border_color_light;
};

struct CBrowserTableCellData {
  enum class Type {
    HEADER,
    DATA,
  };

  Type              type       = Type::DATA;
  std::string       id;
  CHAlignType       halign     = CHALIGN_TYPE_NONE;
  CVAlignType       valign     = CVALIGN_TYPE_NONE;
  int               width      = -1;
  CBrowserUnitsType width_unit = CBrowserUnitsType::PIXEL;
  bool              wrap       = true;
  int               colspan    = 0;
  int               rowspan    = 0;
  CRGBA             border_color;
  CRGBA             background_color;
  CRGBA             border_color_dark;
  CRGBA             border_color_light;
};

struct CBrowserTableCaptionData {
  CHAlignType halign = CHALIGN_TYPE_NONE;
  CVAlignType valign = CVALIGN_TYPE_NONE;
};

//------

class CBrowserTableMgr {
 public:
  CBrowserTableMgr(CBrowserWindow *window);

  CBrowserWindow *window() const { return window_; }
  CBrowserTable *currentTable() const { return current_table_; }
  CBrowserTableRow *currentRow() const { return current_row_; }
  CBrowserTableCell *currentCell() const { return current_cell_; }
  CBrowserTableCaption *currentCaption() const { return current_caption_; }

  CBrowserTable *startTable(const CBrowserTableData &data);
  void           endTable();

  void startTableRow(const CBrowserTableRowData &data);
  void endTableRow();

  void startTableCell(const CBrowserTableCellData &cellData);
  void endTableCell();

  void startTableCaption(const CBrowserTableCaptionData &data);
  void endTableCaption();

 private:
  CBrowserWindow*       window_          { nullptr };
  CBrowserTable*        current_table_   { nullptr };
  CBrowserTableRow*     current_row_     { nullptr };
  CBrowserTableCell*    current_cell_    { nullptr };
  CBrowserTableCaption* current_caption_ { nullptr };
};

//------

class CBrowserTable;

//------

class CBrowserTableRow {
 public:
  CBrowserTableRow(CBrowserTable *table, const CBrowserTableRowData &data);
 ~CBrowserTableRow() { }

  CHAlignType getHAlign() const { return halign_; }
  CVAlignType getVAlign() const { return valign_; }

  CRGBA getBackgroundColor() const { return background_color_; }

 private:
  CHAlignType halign_ { CHALIGN_TYPE_NONE };
  CVAlignType valign_ { CVALIGN_TYPE_NONE };
  CRGBA       border_color_;
  CRGBA       background_color_;
  CRGBA       border_color_dark_;
  CRGBA       border_color_light_;
};

//------

class CBrowserTableCell {
 public:
  CBrowserTableCell(CBrowserTableRow *row, const CBrowserTableCellData &data);

  virtual ~CBrowserTableCell() {
    delete area_data_;
  }

  bool getPad() const { return pad_; }

  int getWidth() const { return width_; }

  CBrowserUnitsType getWidthUnit() const { return width_unit_; }

  int getHeight() const { return height_; }

  CHAlignType getHAlign() const { return halign_; }
  CVAlignType getVAlign() const { return valign_; }

  int getColSpan() const { return colspan_; }
  int getRowSpan() const { return rowspan_; }

  CRGBA getBackgroundColor() const { return background_color_; }

  CHtmlLayoutArea *getAreaData() const { return area_data_; }

 protected:
  bool              pad_ { false };
  int               x_ { 0 };
  int               y_ { 0 };
  int               width_ { 0 };
  CBrowserUnitsType width_unit_ { CBrowserUnitsType::PIXEL };
  int               height_ { 0 };
  CHAlignType       halign_ { CHALIGN_TYPE_NONE };
  CVAlignType       valign_ { CVALIGN_TYPE_NONE };
  bool              wrap_ { false };
  int               colspan_ { 0 };
  int               rowspan_ { 0 };
  CRGBA             border_color_;
  CRGBA             background_color_;
  CRGBA             border_color_dark_;
  CRGBA             border_color_light_;
  CHtmlLayoutArea*  area_data_ { nullptr };
};

//------

class CBrowserTablePadCell : public CBrowserTableCell {
 public:
  CBrowserTablePadCell(CBrowserTableRow *row, const CBrowserTableCellData &data);
};

//------

class CBrowserTableCaption {
 public:
  CBrowserTableCaption(CBrowserTable *table, const CBrowserTableCaptionData &data);

 ~CBrowserTableCaption() {
    delete area_data_;
  }

  CHAlignType getHAlign() const { return halign_; }
  CVAlignType getVAlign() const { return valign_; }

  CHtmlLayoutArea *getAreaData() const { return area_data_; }

 private:
  CHAlignType      halign_ { CHALIGN_TYPE_NONE };
  CVAlignType      valign_ { CVALIGN_TYPE_NONE };
  CHtmlLayoutArea *area_data_ { nullptr };
};

//------

class CBrowserTable : public CBrowserObject {
 public:
  CBrowserTable(CBrowserWindow *window, const CBrowserTableData &data);
 ~CBrowserTable();

  void endTable();

  CHAlignType getHAlign() const { return halign_; }
  CVAlignType getVAlign() const { return valign_; }

  int getWidth() const { return width_; }

  CBrowserUnitsType getWidthUnit() const { return width_unit_; }

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

  void addRow(CBrowserTableRow *row);

  void addRowCell(int row, CBrowserTableCell *cell);

  void format(CHtmlLayoutMgr *) override;
  void draw(CHtmlLayoutMgr *, const CHtmlLayoutRegion &) override;

 private:
  typedef std::vector<CBrowserTableCell *> Cells;
  typedef std::vector<CBrowserTableRow  *> Rows;
  typedef std::vector<Cells>               RowCells;

  CBrowserWindow*       window_ { nullptr };
  bool                  border_ { false };
  int                   hspace_ { 0 };
  int                   vspace_ { 0 };
  CHAlignType           halign_ { CHALIGN_TYPE_NONE };
  CVAlignType           valign_ { CVALIGN_TYPE_NONE };
  int                   width_ { 0 };
  CBrowserUnitsType     width_unit_ { CBrowserUnitsType::PIXEL };
  int                   cell_padding_ { 0 };
  int                   cell_spacing_ { 0 };
  CRGBA                 border_color_;
  CRGBA                 background_color_;
  CRGBA                 border_color_dark_;
  CRGBA                 border_color_light_;
  int                   row_num_ { 0 };
  int                   col_num_ { 0 };
  Rows                  rows_;
  int                   num_rows_ { 0 };
  int                   num_cols_ { 0 };
  RowCells              row_cells_;
  CBrowserTableCaption* caption_ { nullptr };
};

#endif
