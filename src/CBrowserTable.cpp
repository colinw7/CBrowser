#include <CBrowserTable.h>
#include <CBrowserWindow.h>
#include <CRGBName.h>

CBrowserTable::
CBrowserTable(CBrowserWindow *window, const CBrowserTableData &data) :
 CBrowserObject(window, CHtmlTagId::TABLE), data_(data)
{
  setDisplay(Display::TABLE);

//borderRef().setCollapse("separate");
//borderRef().setSpacing("2px");
  borderRef().setLineColor(CBrowserColor("gray"));
}

CBrowserTable::
~CBrowserTable()
{
}

void
CBrowserTable::
init()
{
  CBrowserObject::init();
}

void
CBrowserTable::
setNameValue(const std::string &name, const std::string &value)
{
  std::string lname  = CStrUtil::toLower(name);
  std::string lvalue = CStrUtil::toLower(value);

  if      (lname == "align") {
    if      (lvalue == "left") {
      data_.halign = CHALIGN_TYPE_LEFT;
    }
    else if (lvalue == "center") {
      data_.halign = CHALIGN_TYPE_CENTER;
    }
    else if (lvalue == "right") {
      data_.halign = CHALIGN_TYPE_RIGHT;
    }
    else
      window_->displayError("Illegal 'table' Alignment '%s'\n", value.c_str());
  }
  else if (lname == "border") {
    if (value != "") {
      if (CStrUtil::isInteger(value))
        data_.border = (CStrUtil::toInteger(value) != 0);
      else {
        window_->displayError("Illegal 'table' Value for border '%s'\n", value.c_str());

        data_.border = false;
      }
    }
    else
      data_.border = true;
  }
  else if (lname == "bordercolor") {
    if (value != "")
      data_.border_color = CRGBName::toRGBA(value);
  }
  else if (lname == "bordercolorlight") {
    if (value != "")
      data_.border_color_light = CRGBName::toRGBA(value);
  }
  else if (lname == "bordercolordark") {
    if (value != "")
      data_.border_color_dark = CRGBName::toRGBA(value);
  }
  else if (lname == "cellpadding") {
    if (CStrUtil::isInteger(value))
      data_.cell_padding = CStrUtil::toInteger(value);
    else {
      window_->displayError("Illegal 'table' Value for cellpadding '%s'\n", value.c_str());

      data_.cell_padding = 1;
    }
  }
  else if (lname == "cellspacing") {
    if (CStrUtil::isInteger(value))
      data_.cell_spacing = CStrUtil::toInteger(value);
    else {
      window_->displayError("Illegal 'table' Value for cellspacing '%s'\n", value.c_str());

      data_.cell_spacing = 2;
    }
  }
  else if (lname == "hspace") {
    if (CStrUtil::isInteger(value))
      data_.hspace = CStrUtil::toInteger(value);
    else {
      window_->displayError("Illegal 'table' Value for hspace '%s'\n", value.c_str());

      data_.hspace = 0;
    }
  }
  else if (lname == "valign") {
    if      (lvalue == "top") {
      data_.valign = CVALIGN_TYPE_TOP;
    }
    else if (lvalue == "bottom") {
      data_.valign = CVALIGN_TYPE_BOTTOM;
    }
    else
      window_->displayError("Illegal 'table' vertical alignment '%s'\n", value.c_str());
  }
  else if (lname == "vspace") {
    if (CStrUtil::isInteger(value))
      data_.vspace = CStrUtil::toInteger(value);
    else {
      window_->displayError("Illegal 'table' Value for vspace '%s'\n", value.c_str());

      data_.vspace = 0;
    }
  }
  else if (lname == "width") {
    data_.width = CBrowserUnitValue(value);

    if (! data_.width.isValid())
      window_->displayError("Illegal 'table' value for width '%s'\n", value.c_str());
  }
  else {
    CBrowserObject::setNameValue(name, value);
  }
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
addPadCells()
{
  for (int i = 0; i < getNumRows(); ++i) {
    int num_cols = row_cells_[i].size();

    for (int j = num_cols; j < getNumCols(); ++j) {
      CBrowserTableCellData cellData;

      CBrowserTableCell *table_cell = new CBrowserTablePadCell(window_, cellData);

      addRowCell(i, table_cell);
    }
  }
}

void
CBrowserTable::
layout()
{
  CIBBox2D box = this->content();

  addPadCells();

  for (int i = 0; i < getNumRows(); ++i) {
    for (int j = 0; j < getNumCols(); ++j) {
      CBrowserTableCell *rowCell = row_cells_[i][j];

      if (rowCell->getPad())
        continue;

      if (rowCell->getWidthUnit() == CScreenUnits::Units::PX)
        rowCell->setContentWidth(rowCell->getWidth());

      if (getWidthUnit() == CScreenUnits::Units::PX && rowCell->contentWidth() > getWidth())
        rowCell->setContentWidth(getWidth());

      if (rowCell->contentWidth() > box.getWidth())
        rowCell->setContentWidth(box.getWidth());
    }
  }

  //---

  // set row heights
  std::vector<int> row_heights;

  row_heights.resize(getNumRows() + 1);

  for (int i = 0; i < getNumRows(); ++i) {
    row_heights[i] = 0;

    for (int j = 0; j < getNumCols(); ++j) {
      CBrowserTableCell *rowCell = row_cells_[i][j];

      int k = i;

      // find index of first row if pad cell
      if (rowCell->getPad()) {
        k--;

        while (k >= 0 && row_cells_[k][j]->getPad())
          k--;

        if (k < 0)
          continue;
      }

      CBrowserTableCell *rowCell1 = row_cells_[k][j];

      CBrowserRegion cellRegion = rowCell1->calcRegion();

      int rowSpan = std::max(rowCell1->getRowSpan(), 1);

      int height1 = cellRegion.height()/rowSpan;

      row_heights[i] = std::max(row_heights[i], height1);
    }
  }

  for (int i = 0; i < getNumRows(); ++i) {
    for (int j = 0; j < getNumCols(); ++j) {
      CBrowserTableCell *rowCell = row_cells_[i][j];

      rowCell->setContentHeight(row_heights[i]);
    }
  }

  //---

  // set columns widths
  std::vector<int> col_widths;

  col_widths.resize(getNumCols() + 1);

  for (int j = 0; j < getNumCols(); ++j) {
    col_widths[j] = 0;

    for (int i = 0; i < getNumRows(); ++i) {
      CBrowserTableCell *rowCell = row_cells_[i][j];

      int k = j;

      // find index of first column if pad cell
      if (rowCell->getPad()) {
        k--;

        while (k >= 0 && row_cells_[i][k]->getPad())
          k--;

        if (k < 0)
          continue;
      }

      CBrowserTableCell *rowCell1 = row_cells_[i][k];

      CBrowserRegion cellRegion = rowCell1->calcRegion();

      int colSpan = std::max(rowCell1->getColSpan(), 1);

      int width1 = cellRegion.width()/colSpan;

      col_widths[j] = std::max(col_widths[j], width1);
    }
  }

  for (int j = 0; j < getNumCols(); ++j) {
    for (int i = 0; i < getNumRows(); ++i) {
      CBrowserTableCell *rowCell = row_cells_[i][j];

      rowCell->setContentWidth(col_widths[j]);
    }
  }

  //---

  if (caption_) {
    CBrowserRegion captionRegion;

    captionRegion.setX(0);
    captionRegion.setY(0);

    captionRegion.setWidth (box.getWidth());
    captionRegion.setHeight(0);

    setContentHeight(CBrowserBox::height() + captionRegion.height());
  }
}

CBrowserRegion
CBrowserTable::
calcRegion() const
{
  int width = 0;

  for (int i = 0; i < getNumCols(); ++i) {
    CBrowserTableCell *rowCell = row_cells_[0][i];

    CBrowserRegion cellRegion = rowCell->calcRegion();

    width += cellRegion.width();
  }

  width += 2*getNumCols()*data_.cell_padding;

  width += (getNumCols() + 1)*data_.cell_spacing;

  if (data_.border)
    width += 2*getNumCols() + 2*data_.border;

  //---

  int height = 0;

  for (int i = 0; i < getNumRows(); ++i) {
    CBrowserTableCell *rowCell = row_cells_[i][0];

    CBrowserRegion cellRegion = rowCell->calcRegion();

    height += cellRegion.height();
  }

  height += 2*getNumRows()*data_.cell_padding;

  height += (getNumRows() + 1)*data_.cell_spacing;

  if (data_.border)
    height += 2*getNumRows() + 2*data_.border;

  return CBrowserRegion(width, height, 0);
}

void
CBrowserTable::
draw(const CTextBox &box)
{
  addPadCells();

  fillBackground(box);

  //---

  int x1 = box.x();
  int y1 = box.y();

  //---

  // place caption at top if align top
  if (caption_ && caption_->getVAlign() == CVALIGN_TYPE_TOP) {
    caption_->setX(x1);
    caption_->setY(y1);

    caption_->draw(caption_->contentBox());

    y1 += caption_->contentHeight();
  }

  //---

  int y2 = y1 + data_.cell_spacing;

  if (data_.border)
    y2 += data_.border + 1;

  for (int i = 0; i < getNumRows(); ++i) {
    CBrowserTableRow *row = rows_[i];
    assert(row);

    //---

    int x2 = x1 + data_.cell_spacing;

    if (data_.border)
      x2 += data_.border + 1;

    //---

    CBrowserTableCell *rowCell = row_cells_[i][0];

    if (row->background().color().isValid()) {
      int height = rowCell->contentHeight();

      CBrush brush(row->background().color().color());

      window_->fillRectangle(x1, y2, box.width(), height, brush);
    }

    //---

    for (int j = 0; j < getNumCols(); ++j) {
      CBrowserTableCell *rowCell = row_cells_[i][j];
      assert(rowCell);

      if (! rowCell->getPad()) {
        int width  = (rowCell->contentWidth () + 2*data_.cell_padding)*rowCell->getColSpan();
        int height = (rowCell->contentHeight() + 2*data_.cell_padding)*rowCell->getRowSpan();

        if (rowCell->getColSpan() > 1) {
          width += data_.cell_spacing*(rowCell->getColSpan() - 1);

          if (data_.border)
            width += 2*(rowCell->getColSpan() - 1);
        }

        if (rowCell->getRowSpan() > 1) {
          height += data_.cell_spacing*(rowCell->getRowSpan() - 1);

          if (data_.border)
            height += 2*(rowCell->getRowSpan() - 1);
        }

        //---

        if (rowCell->background().color().isValid()) {
          CBrush brush(rowCell->background().color().color());

          window_->fillRectangle(x2, y2, width, height, brush);
        }

        //---

        if (data_.border)
          window_->drawBorder(x2 - 1, y2 - 1, width + 2, height + 2, CBrowserBorderType::IN);

        //---

        rowCell->setX(x2 + data_.cell_padding);
        rowCell->setY(y2 + data_.cell_padding);

        width  = rowCell->contentWidth();
        height = rowCell->contentHeight();

        CTextBox rowBox = rowCell->contentBox();

        rowBox.setWidth (width *rowCell->getColSpan());
        rowBox.setHeight(height*rowCell->getRowSpan());

        if (rowCell->getColSpan() > 1) {
          rowBox.setWidth(rowBox.width() +
                          (2*data_.cell_padding + data_.cell_spacing)*
                          (rowCell->getColSpan() - 1));

          if (data_.border)
            rowBox.setWidth(rowBox.width() + 2*(rowCell->getColSpan() - 1));
        }

        if (rowCell->getRowSpan() > 1) {
          rowBox.setHeight(rowBox.height() +
                           (2*data_.cell_padding + data_.cell_spacing)*
                           (rowCell->getRowSpan() - 1));

          if (data_.border)
            rowBox.setHeight(rowBox.height() + 2*(rowCell->getRowSpan() - 1));
        }

        rowCell->draw(rowBox);
      }

      x2 += rowCell->contentWidth() + 2*data_.cell_padding + data_.cell_spacing;

      if (data_.border)
        x2 += 2;
    }

    //---

    y2 += rowCell->contentHeight() + 2*data_.cell_padding + data_.cell_spacing;

    if (data_.border)
      y2 += 2;
  }

  //---

  int width = 0;

  for (int i = 0; i < getNumCols(); ++i) {
    CBrowserTableCell *rowCell = row_cells_[0][i];

    width += rowCell->contentWidth();
  }

  width += 2*getNumCols()*data_.cell_padding;

  width += (getNumCols() + 1)*data_.cell_spacing;

  if (data_.border)
    width += 2*getNumCols() + 2*data_.border;

  //---

  int height = 0;

  for (int i = 0; i < getNumRows(); ++i) {
    CBrowserTableCell *rowCell = row_cells_[i][0];

    height += rowCell->contentHeight();
  }

  height += 2*getNumRows()*data_.cell_padding;

  height += (getNumRows() + 1)*data_.cell_spacing;

  if (data_.border)
    height += 2*getNumRows() + 2*data_.border;

  //---

  for (int i = 0; i < data_.border; ++i)
    window_->drawBorder(x1 + i, y1 + i, width - 2*i, height - 2*i, CBrowserBorderType::OUT);

  //---

  // place caption at bottom if align bottom
  if (caption_ && caption_->getVAlign() == CVALIGN_TYPE_BOTTOM) {
    y1 += height + 2;

    caption_->setX(x1);
    caption_->setY(y1);

    caption_->draw(caption_->contentBox());

    y1 += caption_->contentHeight();
  }

  //---

  //box.setY(y2);
}

//------

CBrowserTableRow::
CBrowserTableRow(CBrowserWindow *window, const CBrowserTableRowData &data) :
 CBrowserObject(window, CHtmlTagId::TR), data_(data)
{
  setDisplay(Display::TABLE_ROW);

//setNameValue("vertical-align", "inherit");
  borderRef().setLineColor(CBrowserColor("inherit"));
}

void
CBrowserTableRow::
init()
{
  CBrowserObject::init();
}

void
CBrowserTableRow::
setNameValue(const std::string &name, const std::string &value)
{
  std::string lname = CStrUtil::toLower(name);

  if      (lname == "align") {
    window_->parseHAlignOption(value, data_.halign);
  }
  else if (lname == "bordercolor") {
    if (value != "")
      data_.border_color = CRGBName::toRGBA(value);
  }
  else if (lname == "bordercolorlight") {
    if (value != "")
      data_.border_color_light = CRGBName::toRGBA(value);
  }
  else if (lname == "bordercolordark") {
    if (value != "")
      data_.border_color_dark = CRGBName::toRGBA(value);
  }
  else if (lname == "valign") {
    window_->parseVAlignOption(value, data_.valign);
  }
  else {
    CBrowserObject::setNameValue(name, value);
  }
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

//------

CBrowserTableCell::
CBrowserTableCell(CBrowserWindow *window, CHtmlTagId id, const CBrowserTableCellData &data) :
 CBrowserObject(window, id), data_(data)
{
  setDisplay(Display::TABLE_CELL);

//setNameValue("vertical-align", "inherit");
}

CBrowserTableCell::
~CBrowserTableCell()
{
}

void
CBrowserTableCell::
init()
{
}

void
CBrowserTableCell::
setNameValue(const std::string &name, const std::string &value)
{
  std::string lname  = CStrUtil::toLower(name);
  std::string lvalue = CStrUtil::toLower(value);

  if      (lname == "align") {
    window_->parseHAlignOption(lvalue, data_.halign);
  }
  else if (lname == "bordercolor") {
    if (lvalue != "")
      data_.border_color = CRGBName::toRGBA(lvalue);
  }
  else if (lname == "bordercolorlight") {
    if (lvalue != "")
      data_.border_color_light = CRGBName::toRGBA(lvalue);
  }
  else if (lname == "bordercolordark") {
    if (lvalue != "")
      data_.border_color_dark = CRGBName::toRGBA(lvalue);
  }
  else if (lname == "colspan") {
    if (CStrUtil::isInteger(lvalue))
      data_.colspan = CStrUtil::toInteger(lvalue);
    else {
      window_->displayError("Illegal 'td' Value for colspan '%s'\n", value.c_str());
      data_.colspan = 1;
    }
  }
  else if (lname == "id") {
    data_.id = value;
  }
  else if (lname == "nowrap") {
    data_.wrap = false;

    if (value != "")
      window_->displayError("No Value needed for nowrap\n");
  }
  else if (lname == "rowspan") {
    if (CStrUtil::isInteger(lvalue))
      data_.rowspan = CStrUtil::toInteger(lvalue);
    else {
      window_->displayError("Illegal 'td' value for rowspan '%s'\n", value.c_str());
      data_.rowspan = 1;
    }
  }
  else if (lname == "valign") {
    window_->parseVAlignOption(lvalue, data_.valign);
  }
  else if (lname == "width") {
    data_.width = CBrowserUnitValue(lvalue);

    if (! data_.width.isValid())
      window_->displayError("Illegal 'td' value for width '%s'\n", value.c_str());
  }
  else if (lname == "height") {
    data_.height = CBrowserUnitValue(lvalue);

    if (! data_.height.isValid())
      window_->displayError("Illegal 'td' value for height '%s'\n", value.c_str());
  }
  else {
    CBrowserObject::setNameValue(name, value);
  }
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

  //---

  if (table) {
    table->setNumRows(table->getRowNum() + data_.rowspan - 1);

    table->resizeRowSize(table->getNumRows());

    table->addRowCell(table->getRowNum() - 1, this);

    for (int i = 0; i < data_.rowspan; ++i) {
      for (int j = 0; j < data_.colspan; ++j) {
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

CBrowserRegion
CBrowserTableCell::
calcRegion() const
{
  CBrowserRegion region;

  for (auto &c : children_) {
    if (c->type() == CHtmlTagId::TEXT)
      region = c->calcRegion();
  }

  return region;
}

//------

CBrowserTablePadCell::
CBrowserTablePadCell(CBrowserWindow *window, const CBrowserTableCellData &data) :
 CBrowserTableCell(window, CHtmlTagId::NONE, data)
{
}

//------

CBrowserTableCaption::
CBrowserTableCaption(CBrowserWindow *window, const CBrowserTableCaptionData &data) :
 CBrowserObject(window, CHtmlTagId::CAPTION), data_(data)
{
  setDisplay(Display::TABLE_CAPTION);

//setNameValue("text-align", "center");
}

CBrowserTableCaption::
~CBrowserTableCaption()
{
}

void
CBrowserTableCaption::
init()
{
}

void
CBrowserTableCaption::
setNameValue(const std::string &name, const std::string &value)
{
  std::string lname  = CStrUtil::toLower(name);
  std::string lvalue = CStrUtil::toLower(value);

  if      (lname == "align") {
    if      (lvalue == "left") {
      data_.halign = CHALIGN_TYPE_LEFT;
    }
    else if (lvalue == "center") {
      data_.halign = CHALIGN_TYPE_CENTER;
    }
    else if (lvalue == "right") {
      data_.halign = CHALIGN_TYPE_RIGHT;
    }
    else if (lvalue == "top") {
      data_.valign = CVALIGN_TYPE_TOP;
    }
    else if (lvalue == "bottom") {
      data_.valign = CVALIGN_TYPE_BOTTOM;
    }
    else
      window_->displayError("Illegal '%s' Alignment '%s'\n", "Table Caption",
                            value.c_str());
  }
  else if (lname == "valign") {
    if      (lvalue == "top") {
      data_.valign = CVALIGN_TYPE_TOP;
    }
    else if (lvalue == "bottom") {
      data_.valign = CVALIGN_TYPE_BOTTOM;
    }
    else
      window_->displayError("Illegal '%s' Vertical Alignment '%s'\n", "Table Caption",
                            value.c_str());
  }
  else {
    CBrowserObject::setNameValue(name, value);
  }
}

void
CBrowserTableCaption::
initProcess()
{
  CBrowserTable *table = parentType<CBrowserTable>();

  //---

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
