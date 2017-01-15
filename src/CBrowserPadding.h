#ifndef CBrowserPadding_H
#define CBrowserPadding_H

#include <CBrowserUnitValue.h>

class CBrowserPadding {
 public:
  CBrowserPadding() { }

  const CBrowserUnitValue &left() const { return left_; }
  void setLeft(const CBrowserUnitValue &v) { left_ = v; }

  const CBrowserUnitValue &bottom() const { return bottom_; }
  void setBottom(const CBrowserUnitValue &v) { bottom_ = v; }

  const CBrowserUnitValue &right() const { return right_; }
  void setRight(const CBrowserUnitValue &v) { right_ = v; }

  const CBrowserUnitValue &top() const { return top_; }
  void setTop(const CBrowserUnitValue &v) { top_ = v; }

  int width () const { return left().value() + right ().value(); }
  int height() const { return top ().value() + bottom().value(); }

 private:
  CBrowserUnitValue left_;
  CBrowserUnitValue bottom_;
  CBrowserUnitValue top_;
  CBrowserUnitValue right_;
};

#endif
