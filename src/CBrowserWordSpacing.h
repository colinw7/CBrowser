#ifndef CBrowserWordSpacing_H
#define CBrowserWordSpacing_H

class CBrowserWordSpacing {
 public:
  enum class Type {
    LENGTH,
    NORMAL,
    INHERIT,
    INVALID
  };

 public:
  CBrowserWordSpacing() { }

  explicit CBrowserWordSpacing(const std::string &str) :
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

    if      (lstr == "normal" )
      type_ = Type::NORMAL;
    else if (lstr == "inherit")
      type_ = Type::INHERIT;
    else {
      type_   = Type::LENGTH;
      length_ = CBrowserUnitValue(lstr);
    }
  }

 private:
  std::string       str_;
  Type              type_ { Type::NORMAL };
  CBrowserUnitValue length_;
};

//---

#include <CBrowserProperty.h>

template<>
class CBrowserPropertyTraits<CBrowserWordSpacing> {
 public:
  std::string toString(const CBrowserWordSpacing &c) {
    return c.string();
  }

  bool fromString(const std::string &str, CBrowserWordSpacing &c) {
    c = CBrowserWordSpacing(str);

    return true;
  }
};

#endif
