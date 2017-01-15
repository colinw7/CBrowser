#ifndef CBrowserTable_H
#define CBrowserTable_H

#include <CBrowserObject.h>
#include <CBrowserUnitValue.h>
#include <CRGBA.h>

struct CBrowserTableData {
  bool              border       = false;
  int               hspace       = 2;
  int               vspace       = 2;
  CHAlignType       halign       = CHALIGN_TYPE_NONE;
  CVAlignType       valign       = CVALIGN_TYPE_NONE;
  CBrowserUnitValue width;
  CBrowserUnitValue height;
  int               cell_padding = 0;
  int               cell_spacing = 0;
  CRGBA             border_color;
  CRGBA             border_color_dark;
  CRGBA             border_color_light;
};

struct CBrowserTableRowData {
  CHAlignType halign              = CHALIGN_TYPE_NONE;
  CVAlignType valign              = CVALIGN_TYPE_NONE;
  CRGBA       border_color;
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
  CBrowserUnitValue width;
  CBrowserUnitValue height;
  bool              wrap       = true;
  int               colspan    = 0;
  int               rowspan    = 0;
  CRGBA             border_color;
  CRGBA             border_color_dark;
  CRGBA             border_color_light;
};

struct CBrowserTableCaptionData {
  CHAlignType halign = CHALIGN_TYPE_NONE;
  CVAlignType valign = CVALIGN_TYPE_NONE;
};

//------

class CBrowserTable;

//------

class CBrowserTableRow : public CBrowserObject {
 public:
  CBrowserTableRow(CBrowserWindow *window, const CBrowserTableRowData &data);
 ~CBrowserTableRow() { }

  CHAlignType getHAlign() const { return data_.halign; }
  CVAlignType getVAlign() const { return data_.valign; }

  void init();

  void setNameValue(const std::string &name, const std::string &value) override;

  void initProcess() override;
  void termProcess() override;

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

  bool getPad() const { return pad_; }

  int getWidth() const { return data_.width.value(); }

  CScreenUnits::Units getWidthUnit() const { return data_.width.units(); }

  CHAlignType getHAlign() const { return data_.halign; }
  CVAlignType getVAlign() const { return data_.valign; }

  int getColSpan() const { return data_.colspan; }
  int getRowSpan() const { return data_.rowspan; }

  void initProcess() override;
  void termProcess() override;

  CBrowserRegion calcRegion() const override;

 protected:
  CBrowserTableCellData data_;
  bool                  pad_ { false };
};

//------

class CBrowserTablePadCell : public CBrowserTableCell {
 public:
  CBrowserTablePadCell(CBrowserWindow *window, const CBrowserTableCellData &data);
};

//------

class CBrowserTableCaption : public CBrowserObject {
 public:
  CBrowserTableCaption(CBrowserWindow *window, const CBrowserTableCaptionData &data);

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

  void resizeRowSize(int size) { row_cells_.resize(size); }

  void addRow(CBrowserTableRow *row);

  void addRowCell(int row, CBrowserTableCell *cell);

  void layout() override;

  CBrowserRegion calcRegion() const override;

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
  CBrowserTableCaption* caption_ { nullptr };
};

#endif
