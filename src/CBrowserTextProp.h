#ifndef CBrowserTextProp_H
#define CBrowserTextProp_H

class CBrowserTextAlign {
 public:
  enum class Type {
    NONE,
    LEFT,
    RIGHT,
    CENTER,
    JUSTIFY,
    INITIAL,
    INHERIT
  };

 public:
  explicit CBrowserTextAlign(Type type=Type::NONE) :
    type_(type) {
  }

  explicit CBrowserTextAlign(const std::string &str) :
   str_(str) {
    std::string lstr = CStrUtil::toLower(str);

    if      (lstr == "left") {
      type_ = Type::LEFT;
    }
    else if (lstr == "right") {
      type_ = Type::RIGHT;
    }
    else if (lstr == "center") {
      type_ = Type::CENTER;
    }
    else if (lstr == "justify") {
      type_ = Type::JUSTIFY;
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

class CBrowserTextDecoration {
 public:
  enum class Type {
    INVALID,
    NONE,
    UNDERLINE,
    OVERLINE,
    LINE_THROUGH,
    INITIAL,
    INHERIT
  };

 public:
  explicit CBrowserTextDecoration(Type type=Type::INVALID) :
   type_(type) {
  }

  explicit CBrowserTextDecoration(const std::string &str) :
   str_(str) {
    std::string lstr = CStrUtil::toLower(str);

    if      (lstr == "none") {
      type_ = Type::NONE;
    }
    else if (lstr == "underline") {
      type_ = Type::UNDERLINE;
    }
    else if (lstr == "overline") {
      type_ = Type::OVERLINE;
    }
    else if (lstr == "line-through") {
      type_ = Type::LINE_THROUGH;
    }
    else if (lstr == "initial") {
      type_ = Type::INITIAL;
    }
    else if (lstr == "inherit") {
      type_ = Type::INHERIT;
    }
  }

  bool isValid() const { return type_ != Type::INVALID; }

  Type type() const { return type_; }
  void setType(const Type &t) { type_ = t; }

 private:
  std::string str_;
  Type        type_ { Type::INVALID };
};

//---

class CBrowserTextShadow {
 public:
  enum ColorType {
    INVALID,
    NONE,
    INITIAL,
    INHERIT,
    COLOR
  };

 public:
  explicit CBrowserTextShadow(ColorType colorType=ColorType::INVALID) :
   colorType_(colorType) {
  }

  explicit CBrowserTextShadow(const std::string &str) :
   str_(str) {
    std::vector<std::string> words;

    CStrUtil::toWords(str_, words);

    if (words.size() > 0)
      hShadow_ = CBrowserUnitValue(words[0]);

    if (words.size() > 1)
      vShadow_ = CBrowserUnitValue(words[1]);

    if (words.size() > 2)
      blurRadius_ = CBrowserUnitValue(words[2]);

    if (words.size() > 3) {
      if      (words[3] == "none"   ) { colorType_ = ColorType::NONE; }
      else if (words[3] == "initial") { colorType_ = ColorType::INITIAL; }
      else if (words[3] == "inherit") { colorType_ = ColorType::INHERIT; }
      else                            { color_ = CBrowserColor(words[3]);
                                        colorType_ = ColorType::COLOR; }
    }
  }

  bool isValid() const { return colorType_ != ColorType::INVALID; }

  ColorType colorType() const { return colorType_; }
  void setColorType(const ColorType &t) { colorType_ = t; }

 private:
  std::string str_;
  CBrowserUnitValue hShadow_;
  CBrowserUnitValue vShadow_;
  CBrowserUnitValue blurRadius_;
  CBrowserColor     color_;
  ColorType         colorType_ { ColorType::INVALID };
};

//---

class CBrowserTextVAlign {
 public:
  enum Type {
    INVALID,
    BASELINE,
    LENGTH,
    SUB,
    SUPER,
    TOP,
    TEXT_TOP,
    MIDDLE,
    BOTTOM,
    TEXT_BOTTOM,
    ABS_MIDDLE,
    ABS_BOTTOM,
    INITIAL,
    INHERIT
  };

 public:
  explicit CBrowserTextVAlign(Type type=Type::INVALID) :
   type_(type) {
  }

  explicit CBrowserTextVAlign(const std::string &str) :
   str_(str), type_(Type::INVALID) {
    std::string lstr = CStrUtil::toLower(str);

    if      (lstr == "baseline"   ) type_ = Type::BASELINE;
    else if (lstr == "sub"        ) type_ = Type::SUB;
    else if (lstr == "super"      ) type_ = Type::SUPER;
    else if (lstr == "top"        ) type_ = Type::TOP;
    else if (lstr == "text-top"   ) type_ = Type::TEXT_TOP;
    else if (lstr == "middle"     ) type_ = Type::MIDDLE;
    else if (lstr == "bottom"     ) type_ = Type::BOTTOM;
    else if (lstr == "text-bottom") type_ = Type::TEXT_BOTTOM;
    else if (lstr == "initial"    ) type_ = Type::INITIAL;
    else if (lstr == "inherit"    ) type_ = Type::INHERIT;
    else {
      length_ = CBrowserUnitValue(str);
      type_   = (length_.isValid() ? Type::LENGTH : Type::INVALID);
    }
  }

  bool isValid() const { return type_ != Type::INVALID; }

  Type type() const { return type_; }
  void setType(const Type &t) { type_ = t; }

  const CBrowserUnitValue &length() const { assert(type_ == Type::LENGTH); return length_; }
  void setLength(const CBrowserUnitValue &l) { length_ = l; type_ = Type::LENGTH; }

 private:
  std::string       str_;
  Type              type_ { Type::INVALID };
  CBrowserUnitValue length_;
};

//---

class CBrowserTextProp {
 public:
  CBrowserTextProp() { }

  const CBrowserTextAlign &align() const { return align_; }
  void setAlign(const CBrowserTextAlign &v) { align_ = v; }

  const CBrowserTextDecoration &decoration() const { return decoration_; }
  void setDecoration(const CBrowserTextDecoration &v) { decoration_ = v; }

  const CBrowserTextShadow &shadow() const { return shadow_; }
  void setShadow(const CBrowserTextShadow &v) { shadow_ = v; }

  const CBrowserTextVAlign &verticalAlign() const { return verticalAlign_; }
  void setVerticalAlign(const CBrowserTextVAlign &v) { verticalAlign_ = v; }

 private:
  CBrowserTextAlign      align_;
  CBrowserTextDecoration decoration_;
  CBrowserTextShadow     shadow_;
  CBrowserTextVAlign     verticalAlign_;
};

#endif
