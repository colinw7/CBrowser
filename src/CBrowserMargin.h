#ifndef CBrowserMargin_H
#define CBrowserMargin_H

#include <CBrowserUnitValue.h>

class CBrowserMargin {
 public:
  CBrowserMargin() { }

  explicit CBrowserMargin(const CBrowserUnitValue &v) :
   left_(v), bottom_(v), right_(v), top_(v) {
  }

  const CBrowserUnitValue &left() const { return left_; }
  void setLeft(const CBrowserUnitValue &v) { left_ = v; }

  const CBrowserUnitValue &bottom() const { return bottom_; }
  void setBottom(const CBrowserUnitValue &v) { bottom_ = v; }

  const CBrowserUnitValue &right() const { return right_; }
  void setRight(const CBrowserUnitValue &v) { right_ = v; }

  const CBrowserUnitValue &top() const { return top_; }
  void setTop(const CBrowserUnitValue &v) { top_ = v; }

 private:
  CBrowserUnitValue left_;
  CBrowserUnitValue bottom_;
  CBrowserUnitValue right_;
  CBrowserUnitValue top_;
};

#endif
