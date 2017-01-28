#ifndef CBrowserClear_H
#define CBrowserClear_H

#include <CStrUtil.h>

class CBrowserClear {
 public:
  enum class Type {
    INVALID,
    NONE,
    LEFT,
    RIGHT,
    BOTH,
    INITIAL,
    INHERIT
  };

 public:
  CBrowserClear() { }

  explicit CBrowserClear(const std::string &str) :
   type_(stringToType(str)) {
  }

  const Type &type() const { return type_; }
  void setType(const Type &t) { type_ = t; }

 public:
  static Type stringToType(const std::string &str) {
    std::string lstr = CStrUtil::toLower(str);

    if (lstr == "none"   ) return Type::NONE;
    if (lstr == "left"   ) return Type::LEFT;
    if (lstr == "right"  ) return Type::RIGHT;
    if (lstr == "both"   ) return Type::BOTH;
    if (lstr == "initial") return Type::INITIAL;
    if (lstr == "inherit") return Type::INHERIT;

    return Type::INVALID;
  }

 private:
  Type type_ { Type::INVALID };
};

#endif
