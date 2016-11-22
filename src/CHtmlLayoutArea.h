#ifndef CHtmlLayoutArea_H
#define CHtmlLayoutArea_H

#include <CHtmlLayoutTypes.h>
#include <CHtmlLayoutVisitor.h>
#include <vector>
#include <iostream>

class CHtmlLayoutCell;
class CHtmlLayoutMgr;

class CHtmlLayoutArea {
 public:
  typedef std::vector<CHtmlLayoutCell *> Cells;

 public:
  CHtmlLayoutArea();
 ~CHtmlLayoutArea();

  int getX() const { return region_.x; }
  void setX(int x) { region_.x = x; }

  int getY() const { return region_.y; }
  void setY(int y) { region_.y = y; }

  int getWidth() const { return region_.width; }
  void setWidth (int width) { region_.width = width ; }

  int getHeight() const { return region_.getHeight(); }
  void setHeight(int height) { region_.setHeight(height); }

  int getIndentLeft() const { return indent_left_; }
  void setIndentLeft (int indent_left ) { indent_left_  = indent_left; }

  int getIndentRight() const { return indent_right_; }
  void setIndentRight(int indent_right) { indent_right_ = indent_right; }

  const Cells &cells() const { return cells_; }

  CHtmlLayoutCell *getCurrentCell() const { return cell_; }

  void init();

  void addRedrawCell(CHtmlLayoutCell *cell);

  void format(CHtmlLayoutMgr *layout);

  void redraw(CHtmlLayoutMgr *layout);

  void accept(CHtmlLayoutVisitor &visitor);

  void printSize(std::ostream &os) {
    os << "(" << getX() << "," << getY() << ") " <<
          "(" << getX() + getWidth() << "x" << getY() + getHeight() << ")";
  }

 private:
  void term();

  void getCellsBoundingBox(int *x1, int *y1, int *x2, int *y2);

 private:
  CHtmlLayoutRegion region_;
  int               indent_left_ { 0 };
  int               indent_right_ { 0 };
  CHtmlLayoutCell  *cell_ { nullptr };
  Cells             cells_;
};

#endif
