#ifndef CBrowserShadow_H
#define CBrowserShadow_H

class CBrowserShadow {
 public:
  enum class Type {
    NONE,
    COLORS,
    INSET,
    INITIAL,
    INHERIT,
    INVALID
  };

 public:
  CBrowserShadow() { }

  explicit CBrowserShadow(const std::string &str) :
   str_(str) {
    init();
  }

  const std::string &string() const { return str_; }
  void setString(const std::string &s) { str_ = s; init(); }

  bool isValid() const { return type_ != Type::INVALID; }

  const Type &type() const { return type_; }

 private:
  void init() {
    type_ = Type::INVALID;

    std::string lstr = CStrUtil::toLower(str_);

    if      (lstr == "none")
      type_ = Type::NONE;
    else if (lstr == "inset")
      type_ = Type::INSET;
    else if (lstr == "initial")
      type_ = Type::INITIAL;
    else if (lstr == "inherit")
      type_ = Type::INHERIT;
    else {
      std::vector<std::string> words;

      CStrUtil::toWords(lstr, words);

      if (words.size() > 0)
        hShadow_ = CBrowserUnitValue(words[0]);

      if (words.size() > 1)
        vShadow_ = CBrowserUnitValue(words[1]);

      if (words.size() > 2)
        blur_ = CBrowserUnitValue(words[2]);

      if (words.size() > 3)
        spread_ = CBrowserUnitValue(words[3]);

      if (words.size() > 4)
        color_ = CBrowserColor(words[4]);

      type_ = Type::COLORS;
    }
  }

 private:
  std::string       str_;
  Type              type_ { Type::NONE };
  CBrowserUnitValue hShadow_;
  CBrowserUnitValue vShadow_;
  CBrowserUnitValue blur_;
  CBrowserUnitValue spread_;
  CBrowserColor     color_;
};

//---

#include <CBrowserProperty.h>

template<>
class CBrowserPropertyTraits<CBrowserShadow> {
 public:
  std::string toString(const CBrowserShadow &c) {
    return c.string();
  }

  bool fromString(const std::string &str, CBrowserShadow &c) {
    c = CBrowserShadow(str);

    return true;
  }
};

#endif
