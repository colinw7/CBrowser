#ifndef CHTML_LAYOUT_CELL_H
#define CHTML_LAYOUT_CELL_H

#include <CHtmlLayoutTypes.h>
#include <CAlignType.h>
#include <vector>

class CHtmlLayoutRedrawData;
class CHtmlLayoutSubCell;

class CHtmlLayoutCell {
 public:
  CHtmlLayoutCell();
 ~CHtmlLayoutCell();

  CHtmlLayoutCell *getLeftCell() const { return left_; }
  CHtmlLayoutCell *getTopCell () const { return top_ ; }

  int getX() const { return x_; }
  int getY() const { return y_; }

  void setX(int x) { x_ = x; }
  void setY(int y) { y_ = y; }

  int getWidth  () const { return width_  ; }
  int getAscent () const { return ascent_ ; }
  int getDescent() const { return descent_; }
  int getHeight () const { return ascent_ + descent_; }

  void setWidth  (int width  ) { width_   = width  ; }
  void setAscent (int ascent ) { ascent_  = ascent ; }
  void setDescent(int descent) { descent_ = descent; }

  int getIndentLeft () const { return indent_left_ ; }
  int getIndentRight() const { return indent_right_; }

  void setIndentLeft (int indent_left ) { indent_left_  = indent_left ; }
  void setIndentRight(int indent_right) { indent_right_ = indent_right; }

  CHAlignType getHAlign() const { return halign_; }
  CVAlignType getVAlign() const { return valign_; }

  void setHAlign(CHAlignType halign) { halign_ = halign; }
  void setVAlign(CVAlignType valign) { valign_ = valign; }

  CHtmlLayoutClearType getClear() const { return clear_; }

  void setClear(CHtmlLayoutClearType clear) { clear_ = clear; }

  int getNumRedrawDatas();

  CHtmlLayoutRedrawData *getRedrawData(int i);

  void addRedrawData(CHtmlLayoutRedrawData *redraw_data);

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

  void redraw(CHtmlLayoutMgr *layout, int x, int y);

 private:
  void init();
  void term();

 private:
  typedef std::vector<CHtmlLayoutRedrawData *> RedrawDataList;
  typedef std::vector<CHtmlLayoutSubCell *>    SubCellList;

  CHtmlLayoutCell      *left_;
  CHtmlLayoutCell      *top_;
  int                   x_;
  int                   y_;
  int                   width_;
  int                   ascent_;
  int                   descent_;
  int                   indent_left_;
  int                   indent_right_;
  bool                  resizable_;
  CHAlignType           halign_;
  CVAlignType           valign_;
  bool                  border_;
  CHtmlLayoutClearType  clear_;
  RedrawDataList        redraw_datas_;
  CHtmlLayoutSubCell   *sub_cell_;
  SubCellList           sub_cells_;
};

//-----

class CHtmlLayoutSubCell {
 public:
  CHtmlLayoutSubCell(CHtmlLayoutCell *parent);
 ~CHtmlLayoutSubCell();

  void init();

  CHtmlLayoutSubCell *getLeftCell() const { return left_; }
  CHtmlLayoutSubCell *getTopCell () const { return top_ ; }

  int getX() const { return x_; }
  int getY() const { return y_; }

  void setX(int x) { x_ = x; }
  void setY(int y) { y_ = y; }

  int getWidth  () const { return width_  ; }
  int getAscent () const { return ascent_ ; }
  int getDescent() const { return descent_; }
  int getHeight () const { return ascent_ + descent_; }

  void setWidth  (int width  ) { width_   = width  ; }
  void setAscent (int ascent ) { ascent_  = ascent ; }
  void setDescent(int descent) { descent_ = descent; }

  bool getBreakup() const { return breakup_; }

  CHAlignType getAlign() const { return align_; }

  void setAlign(CHAlignType align) { align_ = align; }

  CHtmlLayoutClearType getClear() const { return clear_; }

  void setClear(CHtmlLayoutClearType clear) { clear_ = clear; }

  int getNumRedrawDatas();

  CHtmlLayoutRedrawData *getRedrawData(int i);

  void addRedrawData(CHtmlLayoutRedrawData *redraw_data);

  static CHtmlLayoutSubCell *newCellBelow(CHtmlLayoutMgr *layout, bool breakup);
  static CHtmlLayoutSubCell *newCellRight(CHtmlLayoutMgr *layout, bool breakup);

  void redraw(CHtmlLayoutMgr *layout, int x, int y);

 private:
  void term();

 private:
  typedef std::vector<CHtmlLayoutRedrawData *> RedrawDataList;

  CHtmlLayoutCell      *parent_;
  CHtmlLayoutSubCell   *left_;
  CHtmlLayoutSubCell   *top_;
  int                   x_;
  int                   y_;
  int                   width_;
  int                   ascent_;
  int                   descent_;
  bool                  breakup_;
  CHAlignType           align_;
  CHtmlLayoutClearType  clear_;
  RedrawDataList        redraw_datas_;
};

//------

class CHtmlLayoutRedrawData {
 public:
  CHtmlLayoutRedrawData(CHtmlLayoutCellFormatProc *format_proc,
                        CHtmlLayoutCellRedrawProc *redraw_proc,
                        CHtmlLayoutCellFreeProc *free_proc) :
   format_proc_(format_proc),
   redraw_proc_(redraw_proc),
   free_proc_  (free_proc  ) {
  }

 ~CHtmlLayoutRedrawData() {
    callFreeProc();
  }

  void callFormatProc(CHtmlLayoutMgr *layout) {
    if (format_proc_ != 0)
      format_proc_->execute(layout);
  }

  void callRedrawProc(CHtmlLayoutMgr *layout, int *x, int *y) {
    if (redraw_proc_ != 0)
      redraw_proc_->execute(layout, x, y);
  }

  void callFreeProc() {
    if (free_proc_ != 0)
      free_proc_->execute();
  }

 private:
  CHtmlLayoutCellFormatProc *format_proc_;
  CHtmlLayoutCellRedrawProc *redraw_proc_;
  CHtmlLayoutCellFreeProc   *free_proc_;
};

#endif
