#ifndef CBrowserOutline_H
#define CBrowserOutline_H

#include <CBrowserUnitValue.h>
#include <CBrowserColor.h>

class CBrowserOutlineStyle {
 public:
  enum class Type {
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
    INHERIT,
    INVALID
  };

  CBrowserOutlineStyle() { }

  explicit CBrowserOutlineStyle(const std::string &str) :
   str_(str) {
    init();
  }

  bool isValid() const { return type_ != Type::INVALID; }

  const Type &type() const { return type_; }
  void setType(const Type &v) { type_ = v; }

 private:
  void init() {
    type_ = stringToStyle(str_);
  }

  static Type stringToStyle(const std::string &str) {
    std::string lstr = CStrUtil::toLower(str);

    if (lstr == "none"   ) return Type::NONE;
    if (lstr == "dotted" ) return Type::DOTTED;
    if (lstr == "dashed" ) return Type::DASHED;
    if (lstr == "solid"  ) return Type::SOLID;
    if (lstr == "double" ) return Type::DOUBLE;
    if (lstr == "groove" ) return Type::GROOVE;
    if (lstr == "ridge"  ) return Type::RIDGE;
    if (lstr == "inset"  ) return Type::INSET;
    if (lstr == "outset" ) return Type::OUTSET;
    if (lstr == "inherit") return Type::INHERIT;

    return Type::INVALID;
  }

  static std::string styleToString(Type style) {
    if (style == Type::NONE   ) return "none";
    if (style == Type::DOTTED ) return "dotted";
    if (style == Type::DASHED ) return "dashed";
    if (style == Type::SOLID  ) return "solid";
    if (style == Type::DOUBLE ) return "double";
    if (style == Type::GROOVE ) return "groove";
    if (style == Type::RIDGE  ) return "ridge";
    if (style == Type::INSET  ) return "inset";
    if (style == Type::OUTSET ) return "outset";
    if (style == Type::INHERIT) return "inherit";

    return "none";
  }

 private:
  std::string str_;
  Type        type_ { Type::NONE };
};

//------

class CBrowserOutlineWidth {
 public:
  enum class Type {
    INVALID,
    THIN,
    MEDIUM,
    THICK,
    LENGTH,
    INHERIT
  };

 public:
  CBrowserOutlineWidth() { }

  explicit CBrowserOutlineWidth(const std::string &str) :
   str_(str) {
    init();
  }

  explicit CBrowserOutlineWidth(const Type &type) :
   type_(type) {
    str_ = typeToString(type_);
  }

  bool isValid() const { return type_ != Type::INVALID; }

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

class CBrowserOutline {
 public:
  CBrowserOutline() { }

  //---

  const CBrowserColor &color() const { return color_; }
  void setColor(const CBrowserColor &v) { color_ = v; }

  const CBrowserOutlineStyle &style() const { return style_; }
  void setStyle(const CBrowserOutlineStyle &v) { style_ = v; }

  const CBrowserOutlineWidth &width() const { return width_; }
  void setWidth(const CBrowserOutlineWidth &v) { width_ = v; }

  //---

 private:
  CBrowserColor        color_;
  CBrowserOutlineStyle style_;
  CBrowserOutlineWidth width_;
};

#endif
