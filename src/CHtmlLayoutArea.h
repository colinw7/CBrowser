#ifndef CHTML_LAYOUT_AREA_H
#define CHTML_LAYOUT_AREA_H

#include <vector>

class CHtmlLayoutCell;
class CHtmlLayoutMgr;

class CHtmlLayoutArea {
 public:
  CHtmlLayoutArea();
 ~CHtmlLayoutArea();

  int getX() const { return x_; }
  int getY() const { return y_; }

  void setX(int x) { x_ = x; }
  void setY(int y) { y_ = y; }

  int getWidth () const { return width_ ; }
  int getHeight() const { return height_; }

  void setWidth (int width ) { width_  = width ; }
  void setHeight(int height) { height_ = height; }

  int getIndentLeft () const { return indent_left_ ; }
  int getIndentRight() const { return indent_right_; }

  void setIndentLeft (int indent_left ) { indent_left_  = indent_left ; }
  void setIndentRight(int indent_right) { indent_right_ = indent_right; }

  CHtmlLayoutCell *getCurrentCell() const { return cell_; }

  void init();

  void addRedrawCell(CHtmlLayoutCell *cell);

  void format(CHtmlLayoutMgr *layout);
  void redraw(CHtmlLayoutMgr *layout);

 private:
  void term();

  void getCellsBoundingBox(int *x1, int *y1, int *x2, int *y2);

 private:
  typedef std::vector<CHtmlLayoutCell *> CellList;

  int              x_;
  int              y_;
  int              width_;
  int              height_;
  int              indent_left_;
  int              indent_right_;
  CHtmlLayoutCell *cell_;
  CellList         cells_;
};

#endif
