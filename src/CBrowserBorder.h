#ifndef CBrowserBorder_H
#define CBrowserBorder_H

#include <CBrowserUnitValue.h>
#include <CBrowserColor.h>

class CBrowserBorder {
 public:
  enum class Style {
    DOTTED,
    DASHED,
    SOLID,
    DOUBLE,
    GROOVE,
    RIDGE,
    INSET,
    OUTSET,
    NONE,
    HIDDEN
  };

 public:
  CBrowserBorder() { }

  const Style &lineStyle() const { return lineStyle_; }
  void setLineStyle(const Style &v) { lineStyle_ = v; }

  const CBrowserUnitValue &lineWidth() const { return lineWidth_; }
  void setLineWidth(const CBrowserUnitValue &v) { lineWidth_ = v; }

  const CBrowserUnitValue &cornerRadius() const { return cornerRadius_; }
  void setCornerRadius(const CBrowserUnitValue &v) { cornerRadius_ = v; }

  const CBrowserColor &lineColor() const { return lineColor_; }
  void setLineColor(const CBrowserColor &v) { lineColor_ = v; }

  //---

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

 public:
  static Style stringToStyle(const std::string &str) {
    std::string lstr = CStrUtil::toLower(str);

    if (str == "dotted") return Style::DOTTED;
    if (str == "dashed") return Style::DASHED;
    if (str == "solid" ) return Style::SOLID;
    if (str == "double") return Style::DOUBLE;
    if (str == "groove") return Style::GROOVE;
    if (str == "ridge" ) return Style::RIDGE;
    if (str == "inset" ) return Style::INSET;
    if (str == "outset") return Style::OUTSET;
    if (str == "hidden") return Style::HIDDEN;

    return Style::NONE;
  }

 private:
  Style             lineStyle_ { Style::NONE };
  CBrowserUnitValue lineWidth_;
  CBrowserUnitValue cornerRadius_;
  CBrowserColor     lineColor_;
  CBrowserUnitValue left_;
  CBrowserUnitValue bottom_;
  CBrowserUnitValue top_;
  CBrowserUnitValue right_;
};

#endif
