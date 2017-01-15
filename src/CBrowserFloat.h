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
  CBrowserFloat() { }

  CBrowserFloat(const std::string &str) :
   type_(stringToType(str)) {
  }

  const Type &type() const { return type_; }
  void setType(const Type &t) { type_ = t; }

 public:
  static Type stringToType(const std::string &str) {
    std::string lstr = CStrUtil::toLower(str);

    if (str == "none"   ) return Type::NONE;
    if (str == "left"   ) return Type::LEFT;
    if (str == "right"  ) return Type::RIGHT;
    if (str == "initial") return Type::INITIAL;
    if (str == "inherit") return Type::INHERIT;

    return Type::INVALID;
  }

 private:
  Type type_ { Type::INVALID };
};

#endif
