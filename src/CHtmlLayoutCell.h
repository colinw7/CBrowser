#ifndef CHtmlLayoutCell_H
#define CHtmlLayoutCell_H

#include <CHtmlLayoutTypes.h>
#include <CHtmlLayoutVisitor.h>
#include <CAlignType.h>
#include <vector>
#include <iostream>

class CHtmlLayoutBox;
class CHtmlLayoutSubCell;

class CHtmlLayoutCell {
 public:
  typedef std::vector<CHtmlLayoutBox *>     Boxes;
  typedef std::vector<CHtmlLayoutSubCell *> SubCells;

 public:
  CHtmlLayoutCell();
 ~CHtmlLayoutCell();

  CHtmlLayoutCell *getLeftCell() const { return left_; }
  CHtmlLayoutCell *getTopCell () const { return top_ ; }

  int getX() const { return region_.x; }
  void setX(int x) { region_.x = x; }

  int getY() const { return region_.y; }
  void setY(int y) { region_.y = y; }

  int getWidth() const { return region_.width; }
  void setWidth(int width) { region_.width = width; }

  int getAscent() const { return region_.ascent ; }
  void setAscent(int ascent) { region_.ascent = ascent; }

  int getDescent() const { return region_.descent; }
  void setDescent(int descent) { region_.descent = descent; }

  int getHeight () const { return region_.ascent + region_.descent; }

  int getIndentLeft() const { return indent_left_; }
  void setIndentLeft (int indent_left ) { indent_left_  = indent_left; }

  int getIndentRight() const { return indent_right_; }
  void setIndentRight(int indent_right) { indent_right_ = indent_right; }

  CHAlignType getHAlign() const { return halign_; }
  void setHAlign(CHAlignType halign) { halign_ = halign; }

  CVAlignType getVAlign() const { return valign_; }
  void setVAlign(CVAlignType valign) { valign_ = valign; }

  CHtmlLayoutClearType getClear() const { return clear_; }
  void setClear(CHtmlLayoutClearType clear) { clear_ = clear; }

  const Boxes &boxes() const { return boxes_; }

  const SubCells &subCells() const { return sub_cells_; }

  int getNumBoxes() const;

  CHtmlLayoutBox *getBox(int i);

  void addBox(CHtmlLayoutBox *box);

  CHtmlLayoutSubCell *getCurrentSubCell() const { return sub_cell_; }

  void setCurrentSubCell(CHtmlLayoutSubCell *sub_cell) { sub_cell_ = sub_cell; }

  int getNumSubCells();

  CHtmlLayoutSubCell *getSubCell(int i);

  void addSubCell(CHtmlLayoutSubCell *sub_cell);

  static CHtmlLayoutCell *newCellBelow(CHtmlLayoutMgr *layout);
  static CHtmlLayoutCell *newCellRight(CHtmlLayoutMgr *layout);

  void resetSubCells();
  void freeSubCells();

  void updateSubCellWidth(int width);
  void updateSubCellHeight(int ascent, int descent);

  void getSubCellsBoundingBox(int *x1, int *y1, int *x2, int *y2);
  void getSubCellsBoundingBox(int i1, int i2, int *x1, int *y1,
                              int *x2, int *y2);

  void alignSubCellsRight(int x);
  void alignSubCellsRight(int i1, int i2, int x);

  void alignSubCellsCenter(int x);
  void alignSubCellsCenter(int i1, int i2, int x);

  void format(CHtmlLayoutMgr *layout, int width);

  void alignAscentDescent(int i1, int i2);

  void redraw(CHtmlLayoutMgr *layout, const CHtmlLayoutRegion &region);

  void accept(CHtmlLayoutVisitor &visitor);

  void printSize(std::ostream &os) const {
    os << "(" << region_.x << "," << region_.y << "," <<
          region_.x + region_.width << "," << region_.y + region_.ascent + region_.descent << ")";
  }

  void print(std::ostream &os) const;

 private:
  void init();
  void term();

 private:
  CHtmlLayoutCell      *left_ { nullptr };
  CHtmlLayoutCell      *top_ { nullptr };
  CHtmlLayoutRegion     region_;
  int                   indent_left_ { 0 };
  int                   indent_right_ { 0 };
  bool                  resizable_ { false };
  CHAlignType           halign_ { CHALIGN_TYPE_NONE };
  CVAlignType           valign_ { CVALIGN_TYPE_NONE };
  bool                  border_ { false };
  CHtmlLayoutClearType  clear_ { CHtmlLayoutClearType::LEFT };
  Boxes                 boxes_;
  CHtmlLayoutSubCell   *sub_cell_ { nullptr };
  SubCells              sub_cells_;
};

#endif
