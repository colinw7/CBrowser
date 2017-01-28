#ifndef CBrowserTable_H
#define CBrowserTable_H

#include <CBrowserObject.h>
#include <CBrowserUnitValue.h>
#include <CRGBA.h>

struct CBrowserTableData {
  bool              border { false };
  int               hspace { 2 };
  int               vspace { 2 };
  CHAlignType       halign { CHALIGN_TYPE_NONE };
  CVAlignType       valign { CVALIGN_TYPE_NONE };
  CBrowserUnitValue width;
  CBrowserUnitValue height;
  int               cell_padding = 0;
  int               cell_spacing = 0;
  CRGBA             border_color;
  CRGBA             border_color_dark;
  CRGBA             border_color_light;
};

struct CBrowserTableRowData {
  CHAlignType halign { CHALIGN_TYPE_NONE };
  CVAlignType valign { CVALIGN_TYPE_NONE };
  CRGBA       border_color;
  CRGBA       border_color_dark;
  CRGBA       border_color_light;
};

struct CBrowserTableCellData {
  enum class Type {
    HEADER,
    DATA,
  };

  Type              type { Type::DATA };
  std::string       id;
  CHAlignType       halign { CHALIGN_TYPE_NONE };
  CVAlignType       valign { CVALIGN_TYPE_NONE };
  CBrowserUnitValue width;
  CBrowserUnitValue height;
  bool              wrap { true };
  int               colspan { 0 };
  int               rowspan { 0 };
  CRGBA             border_color;
  CRGBA             border_color_dark;
  CRGBA             border_color_light;
};

struct CBrowserTableCaptionData {
  CHAlignType halign { CHALIGN_TYPE_NONE };
  CVAlignType valign { CVALIGN_TYPE_NONE };
};

//------

class CBrowserTable : public CBrowserObject {
 public:
  CBrowserTable(CBrowserWindow *window, const CBrowserTableData &data);
 ~CBrowserTable();

  void init();

  void setNameValue(const std::string &name, const std::string &value) override;

  void endTable();

  CHAlignType getHAlign() const { return data_.halign; }
  CVAlignType getVAlign() const { return data_.valign; }

  int getWidth() const { return data_.width.value(); }

  CScreenUnits::Units getWidthUnit() const { return data_.width.units(); }

  int getRowNum() const { return row_num_; }
  int getColNum() const { return col_num_; }

  void setRowNum(int row_num) { row_num_ = row_num; }
  void setColNum(int col_num) { col_num_ = col_num; }

  int getNumRows() const { return num_rows_; }
  int getNumCols() const { return num_cols_; }

  void setNumRows(int num_rows) { num_rows_ = num_rows; }
  void setNumCols(int num_cols) { num_cols_ = num_cols; }

  void setCaption(CBrowserTableCaption *caption) { caption_ = caption; }

  void resizeRowSize(int size);

  int numRowCells(int row) const { return row_cells_[row].size(); }

  void addRow(CBrowserTableRow *row);

  void addRowCell(int row, CBrowserTableCell *cell);

  void layout() override;

  void updateRowHeights() const;
  void updateColumnWidths() const;

  void dumpCells() const;

  CBrowserRegion calcRegion() const override;

  bool layoutChildren() const override { return false; }
  bool renderChildren() const override { return false; }

  void draw(const CTextBox &) override;

 private:
  void addPadCells();

 private:
  typedef std::vector<CBrowserTableCell *> Cells;
  typedef std::vector<CBrowserTableRow  *> Rows;
  typedef std::vector<Cells>               RowCells;

  CBrowserTableData     data_;
  int                   row_num_ { 0 };
  int                   col_num_ { 0 };
  Rows                  rows_;
  int                   num_rows_ { 0 };
  int                   num_cols_ { 0 };
  RowCells              row_cells_;
  std::vector<int>      row_heights_;
  std::vector<int>      col_widths_;
  CBrowserTableCaption* caption_ { nullptr };
};

//------

class CBrowserTableCaption : public CBrowserObject {
 public:
  explicit CBrowserTableCaption(CBrowserWindow *window);

 ~CBrowserTableCaption();

  CHAlignType getHAlign() const { return data_.halign; }
  CVAlignType getVAlign() const { return data_.valign; }

  void init() override;

  void setNameValue(const std::string &name, const std::string &value) override;

  void initProcess() override;
  void termProcess() override;

 private:
  CBrowserTableCaptionData data_;
};

//------

class CBrowserTableRow : public CBrowserObject {
 public:
  explicit CBrowserTableRow(CBrowserWindow *window);

 ~CBrowserTableRow() { }

  CHAlignType getHAlign() const { return data_.halign; }
  CVAlignType getVAlign() const { return data_.valign; }

  void init();

  void setNameValue(const std::string &name, const std::string &value) override;

  void initProcess() override;
  void termProcess() override;

  bool layoutChildren() const override { return false; }
  bool renderChildren() const override { return false; }

 private:
  CBrowserTableRowData data_;
};

//------

class CBrowserTableCell : public CBrowserObject {
 public:
  CBrowserTableCell(CBrowserWindow *window, CHtmlTagId id, const CBrowserTableCellData &data);

  virtual ~CBrowserTableCell();

  void init() override;

  void setNameValue(const std::string &name, const std::string &value) override;

  bool isPad() const { return pad_; }

  int getWidth() const { return data_.width.value(); }

  CScreenUnits::Units getWidthUnit() const { return data_.width.units(); }

  CHAlignType getHAlign() const { return data_.halign; }
  CVAlignType getVAlign() const { return data_.valign; }

  int getColSpan() const { return data_.colspan; }
  int getRowSpan() const { return data_.rowspan; }

  int dataWidth() const { return dataWidth_; }
  void setDataWidth(int i) { dataWidth_ = i; }

  int dataHeight() const { return dataHeight_; }
  void setDataHeight(int i) { dataHeight_ = i; }

  int cellWidth() const { return cellWidth_; }
  void setCellWidth(int w) { cellWidth_ = w; }

  int cellHeight() const { return cellHeight_; }
  void setCellHeight(int h) { cellHeight_ = h; }

  int row() const { return row_; }
  int col() const { return col_; }

  void setPos(int row, int col) { row_ = row; col_ = col; }

  void initProcess() override;
  void termProcess() override;

  //bool layoutChildren() const override { return false; }
  //bool renderChildren() const override { return false; }

  void addPadCells(int rowspan, int colspan);

  void calcHeightForWidth(CTextBox &box) override;

  CBrowserRegion calcRegion() const override;

 protected:
  CBrowserTableCellData data_;
  bool                  pad_ { false };
  int                   row_ { -1 };
  int                   col_ { -1 };
  int                   dataWidth_ { 0 };
  int                   dataHeight_ { 0 };
  int                   cellWidth_ { 0 };
  int                   cellHeight_ { 0 };
};

//------

class CBrowserTablePadCell : public CBrowserTableCell {
 public:
  CBrowserTablePadCell(CBrowserWindow *window, const CBrowserTableCellData &data);
};

#endif
