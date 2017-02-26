#ifndef CBrowserBoxSizing_H
#define CBrowserBoxSizing_H

class CBrowserBoxSizing {
 public:
  enum class Type {
    CONTENT,
    BORDER,
    INITIAL,
    INHERIT,
    INVALID
  };

 public:
  CBrowserBoxSizing() { }

  explicit CBrowserBoxSizing(const std::string &str) :
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

    if      (lstr == "content-box" )
      type_ = Type::CONTENT;
    else if (lstr == "border-box" )
      type_ = Type::BORDER;
    else if (lstr == "initial")
      type_ = Type::INHERIT;
    else if (lstr == "inherit")
      type_ = Type::INHERIT;
    else
      type_ = Type::INVALID;
  }

 private:
  std::string str_;
  Type        type_ { Type::CONTENT };
};

//---

#include <CBrowserProperty.h>

template<>
class CBrowserPropertyTraits<CBrowserBoxSizing> {
 public:
  std::string toString(const CBrowserBoxSizing &c) {
    return c.string();
  }

  bool fromString(const std::string &str, CBrowserBoxSizing &c) {
    c = CBrowserBoxSizing(str);

    return true;
  }
};

#endif
