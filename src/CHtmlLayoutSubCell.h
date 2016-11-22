#ifndef CHtmlLayoutSubCell_H
#define CHtmlLayoutSubCell_H

#include <CHtmlLayoutTypes.h>
#include <CHtmlLayoutVisitor.h>
#include <CAlignType.h>
#include <vector>
#include <iostream>

class CHtmlLayoutCell;
class CHtmlLayoutBox;

class CHtmlLayoutSubCell {
 public:
  typedef std::vector<CHtmlLayoutBox *> Boxes;

 public:
  CHtmlLayoutSubCell(CHtmlLayoutCell *parent);
 ~CHtmlLayoutSubCell();

  void init();

  CHtmlLayoutSubCell *getLeftCell() const { return left_; }
  CHtmlLayoutSubCell *getTopCell () const { return top_ ; }

  int getX() const { return region_.x; }
  void setX(int x) { region_.x = x; }

  int getY() const { return region_.y; }
  void setY(int y) { region_.y = y; }

  int getWidth() const { return region_.width; }
  void setWidth(int width) { region_.width = width; }

  int getAscent() const { return region_.ascent; }
  void setAscent(int ascent) { region_.ascent = ascent; }

  int getDescent() const { return region_.descent; }
  void setDescent(int descent) { region_.descent = descent; }

  int getHeight() const { return region_.ascent + region_.descent; }

  bool getBreakup() const { return breakup_; }
  void setBreakup(bool b) { breakup_ = b; }

  CHAlignType getAlign() const { return align_; }
  void setAlign(CHAlignType align) { align_ = align; }

  CHtmlLayoutClearType getClear() const { return clear_; }
  void setClear(CHtmlLayoutClearType clear) { clear_ = clear; }

  const Boxes &boxes() const { return boxes_; }

  int getNumBoxes();

  CHtmlLayoutBox *getBox(int i);

  void addBox(CHtmlLayoutBox *box);

  static CHtmlLayoutSubCell *newCellBelow(CHtmlLayoutMgr *layout, bool breakup);
  static CHtmlLayoutSubCell *newCellRight(CHtmlLayoutMgr *layout, bool breakup);

  void redraw(CHtmlLayoutMgr *layout, const CHtmlLayoutRegion &region);

  void accept(CHtmlLayoutVisitor &visitor);

  void printSize(std::ostream &os) {
    os << "(" << getX() << "," << getY() << ") " <<
          "(" << getX() + getWidth() << "x" << getY() + getHeight() << ")";
  }

 private:
  void term();

 private:
  CHtmlLayoutCell      *parent_ { nullptr };
  CHtmlLayoutSubCell   *left_ { nullptr };
  CHtmlLayoutSubCell   *top_ { nullptr };
  CHtmlLayoutRegion     region_;
  bool                  breakup_ { false };
  CHAlignType           align_ { CHALIGN_TYPE_NONE };
  CHtmlLayoutClearType  clear_ { CHtmlLayoutClearType::LEFT };
  Boxes                 boxes_;
};

#endif
