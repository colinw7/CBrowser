#ifndef CBrowserOverflow_H
#define CBrowserOverflow_H

class CBrowserOverflow {
 public:
  enum class Type {
    VISIBLE,
    HIDDEN,
    SCROLL,
    AUTO,
    INHERIT,
    INVALID
  };

 public:
  CBrowserOverflow() { }

  explicit CBrowserOverflow(const std::string &str) :
   str_(str) {
    init();
  }

  const std::string &string() const { return str_; }
  void setString(const std::string &s) { str_ = s; init(); }

  bool isValid() const { return type_ != Type::INVALID; }

  const Type &type() const { return type_; }

 private:
  void init() {
    std::string lstr = CStrUtil::toLower(str_);

    if      (lstr == "visible") type_ = Type::VISIBLE;
    else if (lstr == "hidden" ) type_ = Type::HIDDEN;
    else if (lstr == "scroll" ) type_ = Type::SCROLL;
    else if (lstr == "auto"   ) type_ = Type::AUTO;
    else if (lstr == "inherit") type_ = Type::INHERIT;
    else                        type_ = Type::INVALID;
  }

 private:
  std::string str_;
  Type        type_ { Type::INVALID };
};

//---

#include <CBrowserProperty.h>

template<>
class CBrowserPropertyTraits<CBrowserOverflow> {
 public:
  std::string toString(const CBrowserOverflow &c) {
    return c.string();
  }

  bool fromString(const std::string &str, CBrowserOverflow &c) {
    c = CBrowserOverflow(str);

    return true;
  }
};

#endif
