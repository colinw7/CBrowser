#ifndef CBrowserFloat_H
#define CBrowserFloat_H

class CBrowserFloat {
 public:
  enum class Type {
    INVALID,
    NONE,
    LEFT,
    RIGHT,
    INITIAL,
    INHERIT
  };

 public:
  explicit CBrowserFloat(Type type=Type::INVALID) :
   type_(type) {
  }

  explicit CBrowserFloat(const std::string &str) :
   type_(stringToType(str)) {
  }

  const Type &type() const { return type_; }
  void setType(const Type &t) { type_ = t; }

  std::string string() const { return typeToString(type_); }

 public:
  static Type stringToType(const std::string &str) {
    std::string lstr = CStrUtil::toLower(str);

    if (lstr == "none"   ) return Type::NONE;
    if (lstr == "left"   ) return Type::LEFT;
    if (lstr == "right"  ) return Type::RIGHT;
    if (lstr == "initial") return Type::INITIAL;
    if (lstr == "inherit") return Type::INHERIT;

    return Type::INVALID;
  }

  static std::string typeToString(Type type) {
    if (type == Type::NONE   ) return "none";
    if (type == Type::LEFT   ) return "left";
    if (type == Type::RIGHT  ) return "right";
    if (type == Type::INITIAL) return "initial";
    if (type == Type::INHERIT) return "inherit";

    return "invalid";
  }

 private:
  Type type_ { Type::INVALID };
};

//---

#include <CBrowserProperty.h>

template<>
class CBrowserPropertyTraits<CBrowserFloat> {
 public:
  std::string toString(const CBrowserFloat &c) {
    return c.string();
  }

  bool fromString(const std::string &str, CBrowserFloat &c) {
    c = CBrowserFloat(str);

    return true;
  }
};

#endif
