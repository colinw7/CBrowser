#ifndef CBrowserBorder_H
#define CBrowserBorder_H

#include <CBrowserUnitValue.h>
#include <CBrowserColor.h>

enum class CBrowserBorderStyle {
  NONE,
  HIDDEN,
  DOTTED,
  DASHED,
  SOLID,
  DOUBLE,
  GROOVE,
  RIDGE,
  INSET,
  OUTSET,
  INITIAL,
  INHERIT
};

class CBrowserBorderWidth {
 public:
  enum class Type {
    INVALID,
    NONE,
    THIN,
    MEDIUM,
    THICK,
    LENGTH,
    INHERIT
  };

 public:
  CBrowserBorderWidth() { }

  explicit CBrowserBorderWidth(const std::string &str) :
   str_(str) {
    init();
  }

  explicit CBrowserBorderWidth(const Type &type) :
   type_(type) {
    str_ = typeToString(type_);
  }

  const std::string string() const { return str_; }

  Type type() const { return type_; }

  CBrowserUnitValue value() const {
    if      (type_ == Type::LENGTH)
      return value_;
    else if (type_ == Type::THIN)
      return CBrowserUnitValue(CScreenUnits(2, CScreenUnits::Units::PX));
    else if (type_ == Type::MEDIUM)
      return CBrowserUnitValue(CScreenUnits(4, CScreenUnits::Units::PX));
    else if (type_ == Type::THICK)
      return CBrowserUnitValue(CScreenUnits(6, CScreenUnits::Units::PX));
    else
      return CBrowserUnitValue(CScreenUnits(1, CScreenUnits::Units::PX));
  }

 private:
  void init() {
    type_ = stringToType(str_);

    if (type_ == Type::INVALID) {
      type_  = Type::LENGTH;
      value_ = CBrowserUnitValue(str_);
    }
  }

  static Type stringToType(const std::string &str) {
    std::string lstr = CStrUtil::toLower(str);

    if (lstr == "thin"   ) return Type::THIN;
    if (lstr == "medium" ) return Type::MEDIUM;
    if (lstr == "thick"  ) return Type::THICK;
    if (lstr == "inherit") return Type::INHERIT;

    return Type::INVALID;
  }

  static std::string typeToString(Type type) {
    if (type == Type::NONE   ) return "none";
    if (type == Type::THIN   ) return "thin";
    if (type == Type::MEDIUM ) return "medium";
    if (type == Type::THICK  ) return "thick";
    if (type == Type::INHERIT) return "inherit";

    return "invalid";
  }

 private:
  std::string       str_;
  Type              type_ { Type::MEDIUM };
  CBrowserUnitValue value_;
};

//------

struct CBrowserBorderLine {
  CBrowserBorderLine() { }

  CBrowserBorderWidth width;
  CBrowserUnitValue   left;
  CBrowserBorderStyle style { CBrowserBorderStyle::NONE };
  CBrowserColor       color;

  CPen pen() const {
    CPen pen;

    if (color.type() == CBrowserColor::Type::COLOR)
      pen = CPen(color.color());

    pen.setWidth(width.value().pxValue());

    if      (style == CBrowserBorderStyle::DOTTED)
      pen.setLineDash(CLineDash("2, 2"));
    else if (style == CBrowserBorderStyle::DASHED)
      pen.setLineDash(CLineDash("4, 2"));

    return pen;
  }
};

struct CBrowserBorderCorner {
  CBrowserBorderCorner() { }

  CBrowserUnitValue radius;
};

//------

class CBrowserBorder {
 public:
  CBrowserBorder() { }

  //---

  const CBrowserBorderWidth &topWidth() const { return top_.width; }
  void setTopWidth(const CBrowserBorderWidth &w) { top_.width = w; }
  const CBrowserBorderWidth &leftWidth() const { return left_.width; }
  void setLeftWidth(const CBrowserBorderWidth &w) { left_.width = w; }
  const CBrowserBorderWidth &rightWidth() const { return right_.width; }
  void setRightWidth(const CBrowserBorderWidth &w) { right_.width = w; }
  const CBrowserBorderWidth &bottomWidth() const { return bottom_.width; }
  void setBottomWidth(const CBrowserBorderWidth &w) { bottom_.width = w; }

  void setWidth(const CBrowserBorderWidth &w) {
    setTopWidth   (w);
    setLeftWidth  (w);
    setRightWidth (w);
    setBottomWidth(w);
  }

  //---

  const CBrowserBorderStyle &topStyle() const { return top_.style; }
  void setTopStyle(const CBrowserBorderStyle &s) { top_.style = s; }
  const CBrowserBorderStyle &leftStyle() const { return left_.style; }
  void setLeftStyle(const CBrowserBorderStyle &s) { left_.style = s; }
  const CBrowserBorderStyle &rightStyle() const { return right_.style; }
  void setRightStyle(const CBrowserBorderStyle &s) { right_.style = s; }
  const CBrowserBorderStyle &bottomStyle() const { return bottom_.style; }
  void setBottomStyle(const CBrowserBorderStyle &s) { bottom_.style = s; }

  void setStyle(const CBrowserBorderStyle &s) {
    setTopStyle   (s);
    setLeftStyle  (s);
    setRightStyle (s);
    setBottomStyle(s);
  }

  static bool styleVisible(const CBrowserBorderStyle &s) {
    return s != CBrowserBorderStyle::NONE && s != CBrowserBorderStyle::HIDDEN; }

  bool isTopVisible   () const { return styleVisible(topStyle   ()); }
  bool isLeftVisible  () const { return styleVisible(leftStyle  ()); }
  bool isRightVisible () const { return styleVisible(rightStyle ()); }
  bool isBottomVisible() const { return styleVisible(bottomStyle()); }

  //---

  const CBrowserColor &topColor() const { return top_.color; }
  void setTopColor(const CBrowserColor &c) { top_.color = c; }
  const CBrowserColor &leftColor() const { return left_.color; }
  void setLeftColor(const CBrowserColor &c) { left_.color = c; }
  const CBrowserColor &rightColor() const { return right_.color; }
  void setRightColor(const CBrowserColor &c) { right_.color = c; }
  const CBrowserColor &bottomColor() const { return bottom_.color; }
  void setBottomColor(const CBrowserColor &c) { bottom_.color = c; }

  void setColor(const CBrowserColor &c) {
    setTopColor   (c);
    setLeftColor  (c);
    setRightColor (c);
    setBottomColor(c);
  }
  //---

  const CBrowserUnitValue &topLeftRadius() const { return topLeft_.radius; }
  void setTopLeftRadius(const CBrowserUnitValue &v) { topLeft_.radius = v; }

  const CBrowserUnitValue &topRightRadius() const { return topRight_.radius; }
  void setTopRightRadius(const CBrowserUnitValue &v) { topRight_.radius = v; }

  const CBrowserUnitValue &bottomLeftRadius() const { return bottomLeft_.radius; }
  void setBottomLeftRadius(const CBrowserUnitValue &v) { bottomLeft_.radius = v; }

  const CBrowserUnitValue &bottomRightRadius() const { return bottomRight_.radius; }
  void setBottomRightRadius(const CBrowserUnitValue &v) { bottomRight_.radius = v; }

  //---

  const CBrowserBorderLine &top   () const { return top_; }
  const CBrowserBorderLine &left  () const { return left_; }
  const CBrowserBorderLine &right () const { return right_; }
  const CBrowserBorderLine &bottom() const { return bottom_; }

  const CBrowserBorderCorner &topLeft    () const { return topLeft_; }
  const CBrowserBorderCorner &topRight   () const { return topRight_; }
  const CBrowserBorderCorner &bottomLeft () const { return bottomLeft_; }
  const CBrowserBorderCorner &bottomRight() const { return bottomRight_; }

  //---

  CPen topPen   () const { return top_   .pen(); }
  CPen leftPen  () const { return left_  .pen(); }
  CPen rightPen () const { return right_ .pen(); }
  CPen bottomPen() const { return bottom_.pen(); }

  //---

  double width() const {
    return left().width.value().value().pxValue() + right ().width.value().value().pxValue();
  }

  double height() const {
    return top ().width.value().value().pxValue() + bottom().width.value().value().pxValue();
  }

 public:
  static CBrowserBorderStyle stringToStyle(const std::string &str) {
    std::string lstr = CStrUtil::toLower(str);

    if (lstr == "none"   ) return CBrowserBorderStyle::NONE;
    if (lstr == "hidden" ) return CBrowserBorderStyle::HIDDEN;
    if (lstr == "dotted" ) return CBrowserBorderStyle::DOTTED;
    if (lstr == "dashed" ) return CBrowserBorderStyle::DASHED;
    if (lstr == "solid"  ) return CBrowserBorderStyle::SOLID;
    if (lstr == "double" ) return CBrowserBorderStyle::DOUBLE;
    if (lstr == "groove" ) return CBrowserBorderStyle::GROOVE;
    if (lstr == "ridge"  ) return CBrowserBorderStyle::RIDGE;
    if (lstr == "inset"  ) return CBrowserBorderStyle::INSET;
    if (lstr == "outset" ) return CBrowserBorderStyle::OUTSET;
    if (lstr == "initial") return CBrowserBorderStyle::INITIAL;
    if (lstr == "inherit") return CBrowserBorderStyle::INHERIT;

    return CBrowserBorderStyle::NONE;
  }

  static std::string styleToString(CBrowserBorderStyle style) {
    if (style == CBrowserBorderStyle::NONE   ) return "none";
    if (style == CBrowserBorderStyle::HIDDEN ) return "hidden";
    if (style == CBrowserBorderStyle::DOTTED ) return "dotted";
    if (style == CBrowserBorderStyle::DASHED ) return "dashed";
    if (style == CBrowserBorderStyle::SOLID  ) return "solid";
    if (style == CBrowserBorderStyle::DOUBLE ) return "double";
    if (style == CBrowserBorderStyle::GROOVE ) return "groove";
    if (style == CBrowserBorderStyle::RIDGE  ) return "ridge";
    if (style == CBrowserBorderStyle::INSET  ) return "inset";
    if (style == CBrowserBorderStyle::OUTSET ) return "outset";
    if (style == CBrowserBorderStyle::INITIAL) return "initial";
    if (style == CBrowserBorderStyle::INHERIT) return "inherit";

    return "none";
  }

 private:
  CBrowserBorderLine   top_;
  CBrowserBorderLine   left_;
  CBrowserBorderLine   right_;
  CBrowserBorderLine   bottom_;
  CBrowserBorderCorner topLeft_;
  CBrowserBorderCorner topRight_;
  CBrowserBorderCorner bottomLeft_;
  CBrowserBorderCorner bottomRight_;
};

#endif
