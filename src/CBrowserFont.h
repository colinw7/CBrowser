#ifndef CBrowserFont_H
#define CBrowserFont_H

#include <CBrowserTypes.h>
#include <CBrowserUnitValue.h>
#include <CFont.h>

class CBrowserFontFamily {
 public:
  enum class Type {
    NONE,
    INITIAL,
    INHERIT,
    VALUE
  };

 public:
  CBrowserFontFamily() { }

  explicit CBrowserFontFamily(const std::string &str) :
   str_(str) {
    std::string lstr = CStrUtil::toLower(str);

    if      (lstr == "initial") {
      type_ = Type::INITIAL;
    }
    else if (lstr == "inherit") {
      type_ = Type::INHERIT;
    }
    else {
      type_  = Type::VALUE;
      value_ = lstr;
    }
  }

  bool isValid() const { return type_ != Type::NONE; }

  Type type() const { return type_; }
  void setType(const Type &t) { type_ = t; }

  const std::string &value() const { return value_; }

  bool isMonospace() const { return (type_ == Type::VALUE && value_ == "monospace"); }

  bool isSerif() const { return (type_ == Type::VALUE && value_ == "serif"); }
  bool isSansSerif() const { return (type_ == Type::VALUE && value_ == "sans-serif"); }

 private:
  std::string str_;
  Type        type_ { Type::NONE };
  std::string value_;
};

//---

class CBrowserFontSize {
 public:
  enum class Type {
    NONE,
    MEDIUM,
    XX_SMALL,
    X_SMALL,
    SMALL,
    LARGE,
    X_LARGE,
    XX_LARGE,
    SMALLER,
    LARGER,
    VALUE,
    INITIAL,
    INHERIT
  };

 public:
  CBrowserFontSize() { }

  explicit CBrowserFontSize(const std::string &str) :
   str_(str) {
    std::string lstr = CStrUtil::toLower(str);

    if      (lstr == "medium") {
      type_ = Type::MEDIUM;
    }
    else if (lstr == "xx-small") {
      type_ = Type::XX_SMALL;
    }
    else if (lstr == "x-small") {
      type_ = Type::X_SMALL;
    }
    else if (lstr == "small") {
      type_ = Type::SMALL;
    }
    else if (lstr == "large") {
      type_ = Type::LARGE;
    }
    else if (lstr == "x-large") {
      type_ = Type::X_LARGE;
    }
    else if (lstr == "xx-large") {
      type_ = Type::XX_LARGE;
    }
    else if (lstr == "smaller") {
      type_  = Type::SMALLER;
      value_ = CBrowserUnitValue(1);
    }
    else if (lstr == "larger") {
      type_  = Type::LARGER;
      value_ = CBrowserUnitValue(1);
    }
    else if (lstr == "initial") {
      type_ = Type::INITIAL;
    }
    else if (lstr == "inherit") {
      type_ = Type::INHERIT;
    }
    else {
      type_  = Type::VALUE;
      value_ = CBrowserUnitValue(lstr);
    }
  }

  explicit CBrowserFontSize(const CBrowserUnitValue &value) :
    type_(Type::VALUE), value_(value) {
  }

  CBrowserFontSize(const Type &type, int n=1) :
   type_(type), value_(CBrowserUnitValue(n)) {
  }

  int value(int ivalue=defSize()) const {
    if      (type_ == Type::VALUE   ) return value_.value();
    else if (type_ == Type::XX_SMALL) return emSize(0.67);
    else if (type_ == Type::X_SMALL ) return emSize(0.83);
    else if (type_ == Type::SMALL   ) return emSize(1.00);
    else if (type_ == Type::MEDIUM  ) return emSize(1.17);
    else if (type_ == Type::LARGE   ) return emSize(1.50);
    else if (type_ == Type::X_LARGE ) return emSize(2.00);
    else if (type_ == Type::XX_LARGE) return emSize(3.00);
    else if (type_ == Type::SMALLER ) return smaller(value_.value(), ivalue);
    else if (type_ == Type::LARGER  ) return larger (value_.value(), ivalue);
    else if (type_ == Type::INHERIT ) return ivalue;
    else if (type_ == Type::INITIAL ) return defSize();
    else                              return defSize();
  }

  bool isRelative() const {
    return (type_ == Type::SMALLER || type_ == Type::LARGER || type_ == Type::INHERIT);
  }

  bool isValid() const { return type_ != Type::NONE; }

  Type type() const { return type_; }

  void setType(const Type &t) {
    type_ = t;

    if (isRelative())
      value_ = CBrowserUnitValue(1);
  }

  static int defSize() {
    return emSize(1.00);
  }

 private:
  int smaller(int n, int ivalue) const {
    for (int i = 0; i < n; ++i)
      ivalue = 0.8*ivalue;

    return ivalue;
  }

  int larger(int n, int ivalue) const {
    for (int i = 0; i < n; ++i)
      ivalue = 1.2*ivalue;

    return ivalue;
  }

  static int emSize(double s) {
    return CScreenUnits(s, CScreenUnits::Units::EM).pxValue(0);
  }

 private:
  std::string       str_;
  Type              type_  { Type::NONE };
  CBrowserUnitValue value_ { 0.0 };
};

//---

class CBrowserFontSizeAdjust {
 public:
  enum class Type {
    INVALID,
    NONE,
    INITIAL,
    INHERIT,
    NUMBER
  };

 public:
  CBrowserFontSizeAdjust() { }

  explicit CBrowserFontSizeAdjust(const std::string &str) :
   str_(str) {
    std::string lstr = CStrUtil::toLower(str);

    if      (lstr == "none") {
      type_ = Type::NONE;
    }
    else if (lstr == "initial") {
      type_ = Type::INITIAL;
    }
    else if (lstr == "inherit") {
      type_ = Type::INHERIT;
    }
    else if (CStrUtil::isReal(str)) {
      type_  = Type::NUMBER;
      value_ = CStrUtil::toReal(lstr);
    }
  }

  bool isValid() const { return type_ != Type::INVALID; }

  Type type() const { return type_; }
  void setType(const Type &t) { type_ = t; }

 private:
  std::string str_;
  Type        type_  { Type::INVALID };
  double      value_ { 0.0 };
};

//---

class CBrowserFontStretch {
 public:
  enum class Type {
    NONE,
    ULTRA_CONDENSED,
    EXTRA_CONDENSED,
    CONDENSED,
    SEMI_CONDENSED,
    NORMAL,
    SEMI_EXPANDED,
    EXPANDED,
    EXTRA_EXPANDED,
    ULTRA_EXPANDED,
    INITIAL,
    INHERIT
  };

 public:
  CBrowserFontStretch() { }

  explicit CBrowserFontStretch(const std::string &str) :
   str_(str) {
    fromString(str_, type_);
  }

  explicit CBrowserFontStretch(Type type) :
   type_(type) {
    toString(type_, str_);
  }

  static void fromString(const std::string &str, Type &type) {
    std::string lstr = CStrUtil::toLower(str);

    if      (lstr == "ultra-condensed") type = Type::ULTRA_CONDENSED;
    else if (lstr == "extra-condensed") type = Type::EXTRA_CONDENSED;
    else if (lstr == "condensed"      ) type = Type::CONDENSED;
    else if (lstr == "semi-condensed" ) type = Type::SEMI_CONDENSED;
    else if (lstr == "normal"         ) type = Type::NORMAL;
    else if (lstr == "semi-expanded"  ) type = Type::SEMI_EXPANDED;
    else if (lstr == "expanded"       ) type = Type::EXPANDED;
    else if (lstr == "extra-expanded" ) type = Type::EXTRA_EXPANDED;
    else if (lstr == "ultra-expanded" ) type = Type::ULTRA_EXPANDED;
    else if (lstr == "initial"        ) type = Type::INITIAL;
    else if (lstr == "inherit"        ) type = Type::INHERIT;
    else                                type = Type::NONE;
  }

  static void toString(Type type, std::string &str) {
    if      (type == Type::ULTRA_CONDENSED) str = "ultra-condensed";
    else if (type == Type::EXTRA_CONDENSED) str = "extra-condensed";
    else if (type == Type::CONDENSED      ) str = "condensed";
    else if (type == Type::SEMI_CONDENSED ) str = "semi-condensed";
    else if (type == Type::NORMAL         ) str = "normal";
    else if (type == Type::SEMI_EXPANDED  ) str = "semi-expanded";
    else if (type == Type::EXPANDED       ) str = "expanded";
    else if (type == Type::EXTRA_EXPANDED ) str = "extra-expanded";
    else if (type == Type::ULTRA_EXPANDED ) str = "ultra-expanded";
    else if (type == Type::INITIAL        ) str = "initial";
    else if (type == Type::INHERIT        ) str = "inherit";
  }

  bool isValid() const { return type_ != Type::NONE; }

  Type type() const { return type_; }
  void setType(const Type &t) { type_ = t; }

 private:
  std::string str_;
  Type        type_ { Type::NONE };
};

//---

class CBrowserFontStyle {
 public:
  enum class Type {
    NONE,
    NORMAL,
    ITALIC,
    OBLIQUE,
    INITIAL,
    INHERIT
  };

 public:
  CBrowserFontStyle() { }

  explicit CBrowserFontStyle(const std::string &str) :
   str_(str) {
    fromString(str_, type_);
  }

  explicit CBrowserFontStyle(Type type) :
   type_(type) {
    toString(type_, str_);
  }

  static void fromString(const std::string &str, Type &type) {
    std::string lstr = CStrUtil::toLower(str);

    if      (lstr == "normal" ) type = Type::NORMAL;
    else if (lstr == "italic" ) type = Type::ITALIC;
    else if (lstr == "oblique") type = Type::OBLIQUE;
    else if (lstr == "initial") type = Type::INITIAL;
    else if (lstr == "inherit") type = Type::INHERIT;
    else                        type = Type::NONE;
  }

  static void toString(Type type, std::string &str) {
    if      (type == Type::NORMAL ) str = "normal";
    else if (type == Type::ITALIC ) str = "italic";
    else if (type == Type::OBLIQUE) str = "oblique";
    else if (type == Type::INITIAL) str = "initial";
    else if (type == Type::INHERIT) str = "inherit";
  }

  bool isValid() const { return type_ != Type::NONE; }

  Type type() const { return type_; }
  void setType(const Type &t) { type_ = t; }

 private:
  std::string str_;
  Type        type_ { Type::NONE };
};

//---

class CBrowserFontVariant {
 public:
  enum class Type {
    NONE,
    NORMAL,
    SMALL_CAPS,
    INITIAL,
    INHERIT
  };

 public:
  CBrowserFontVariant() { }

  explicit CBrowserFontVariant(const std::string &str) :
   str_(str) {
    std::string lstr = CStrUtil::toLower(str);

    if      (lstr == "normal") {
      type_ = Type::NORMAL;
    }
    else if (lstr == "small-caps") {
      type_ = Type::SMALL_CAPS;
    }
    else if (lstr == "initial") {
      type_ = Type::INITIAL;
    }
    else if (lstr == "inherit") {
      type_ = Type::INHERIT;
    }
  }

  bool isValid() const { return type_ != Type::NONE; }

  Type type() const { return type_; }
  void setType(const Type &t) { type_ = t; }

 private:
  std::string str_;
  Type        type_ { Type::NONE };
};

//---

class CBrowserFontWeight {
 public:
  enum class Type {
    NONE,
    NORMAL,
    BOLD,
    BOLDER,
    LIGHTER,
    INITIAL,
    INHERIT,
    VALUE
  };

 public:
  CBrowserFontWeight() { }

  explicit CBrowserFontWeight(const std::string &str) :
   str_(str) {
    std::string lstr = CStrUtil::toLower(str);

    if      (lstr == "normal") {
      type_ = Type::NORMAL;
    }
    else if (lstr == "bold") {
      type_ = Type::BOLD;
    }
    else if (lstr == "bolder") {
      type_ = Type::BOLDER;
    }
    else if (lstr == "lighter") {
      type_ = Type::LIGHTER;
    }
    else if (lstr == "initial") {
      type_ = Type::INITIAL;
    }
    else if (lstr == "inherit") {
      type_ = Type::INHERIT;
    }
    else if (CStrUtil::isInteger(str)) {
      type_  = Type::VALUE;
      value_ = CStrUtil::toInteger(lstr);
    }
  }

  bool isValid() const { return type_ != Type::NONE; }

  Type type() const { return type_; }
  void setType(const Type &t) { type_ = t; }

  int value(int ivalue=500) const {
    if      (type_ == Type::NORMAL ) return 500;
    else if (type_ == Type::BOLD   ) return 750;
    else if (type_ == Type::BOLDER ) return ivalue + 100;
    else if (type_ == Type::LIGHTER) return ivalue - 100;
    else if (type_ == Type::INITIAL) return ivalue;
    else if (type_ == Type::INHERIT) return 500;
    else if (type_ == Type::VALUE  ) return value_;
    else                             return 500;
  }

 private:
  std::string str_;
  Type        type_  { Type::NONE };
  int         value_ { 0 };
};

//---

class CBrowserFont {
 public:
  CBrowserFont() { }

  const CBrowserFontFamily &family() const { return family_; }
  void setFamily(const CBrowserFontFamily &v) { family_ = v; }

  const CBrowserFontSize &size() const { return size_; }
  void setSize(const CBrowserFontSize &v) { size_ = v; }

  const CBrowserFontSizeAdjust &sizeAdjust() const { return sizeAdjust_; }
  void setSizeAdjust(const CBrowserFontSizeAdjust &v) { sizeAdjust_ = v; }

  const CBrowserFontStretch &stretch() const { return stretch_; }
  void setStretch(const CBrowserFontStretch &v) { stretch_ = v; }

  const CBrowserFontStyle &style() const { return style_; }
  void setStyle(const CBrowserFontStyle &v) { style_ = v; }

  const CBrowserFontVariant &variant() const { return variant_; }
  void setVariant(const CBrowserFontVariant &v) { variant_ = v; }

  const CBrowserFontWeight &weight() const { return weight_; }
  void setWeight(const CBrowserFontWeight &v) { weight_ = v; }

  void setBold  () { weight_.setType(CBrowserFontWeight::Type::BOLD  ); }
  void setItalic() { style_ .setType(CBrowserFontStyle ::Type::ITALIC); }

  void setBig  () { size_.setType(CBrowserFontSize::Type::LARGER); }
  void setSmall() { size_.setType(CBrowserFontSize::Type::SMALLER); }

  bool isUnderline() const { return (fontStyle_ & CFONT_STYLE_UNDERLINE); }
  void setUnderline(bool b=true) {
    if (b) fontStyle_ |=  CFONT_STYLE_UNDERLINE;
    else   fontStyle_ &= ~CFONT_STYLE_UNDERLINE;
  }

  bool isStrike() const { return (fontStyle_ & CFONT_STYLE_STRIKEOUT); }
  void setStrike(bool b) {
    if (b) fontStyle_ |=  CFONT_STYLE_STRIKEOUT;
    else   fontStyle_ &= ~CFONT_STYLE_STRIKEOUT;
  }

  bool isSubscript() const { return (fontStyle_ & CFONT_STYLE_SUBSCRIPT); }
  void setSubscript(bool b) {
    if (b) fontStyle_ |=  CFONT_STYLE_SUBSCRIPT;
    else   fontStyle_ &= ~CFONT_STYLE_SUBSCRIPT;
  }

  bool isSuperscript() const { return (fontStyle_ & CFONT_STYLE_SUPERSCRIPT); }
  void setSuperscript(bool b) {
    if (b) fontStyle_ |=  CFONT_STYLE_SUPERSCRIPT;
    else   fontStyle_ &= ~CFONT_STYLE_SUPERSCRIPT;
  }

  CFontPtr font(const CBrowserObject *obj=0) const;

 private:
  std::string fontFamily(const CBrowserObject *pobj) const;

  int fontSize(const CBrowserObject *obj) const;

  void updateFontStyle(const CBrowserObject *obj, uint &currentStyle) const;

 private:
  CBrowserFontFamily     family_;
  CBrowserFontSize       size_;
  CBrowserFontSizeAdjust sizeAdjust_;
  CBrowserFontStretch    stretch_;
  CBrowserFontStyle      style_;
  CBrowserFontVariant    variant_;
  CBrowserFontWeight     weight_;
  uint                   fontStyle_ { 0 };
};

#endif
