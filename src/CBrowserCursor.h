#ifndef CBrowserCursor_H
#define CBrowserCursor_H

class CBrowserCursor {
 public:
  enum class Type {
    NONE,
    URL,
    AUTO,
    DEFAULT,
    POINTER,
    CROSSHAIR,
    MOVE,
    E_RESIZE,
    NE_RESIZE,
    NW_RESIZE,
    N_RESIZE,
    SE_RESIZE,
    SW_RESIZE,
    S_RESIZE,
    W_RESIZE,
    TEXT,
    WAIT,
    HELP,
    PROGRESS,
    INHERIT,
    INVALID
  };

 public:
  CBrowserCursor() { }

  explicit CBrowserCursor(const std::string &str) :
   str_(str) {
    init();
  }

  const std::string &string() const { return str_; }
  void setString(const std::string &s) { str_ = s; init(); }

  bool isValid() const { return type_ != Type::NONE; }

  const Type &type() const { return type_; }

 private:
  void init() {
    std::string lstr = CStrUtil::toLower(str_);

    if      (lstr == "none"     ) type_ = Type::NONE;
    else if (lstr == "auto"     ) type_ = Type::AUTO;
    else if (lstr == "default"  ) type_ = Type::DEFAULT;
    else if (lstr == "pointer"  ) type_ = Type::POINTER;
    else if (lstr == "crosshair") type_ = Type::CROSSHAIR;
    else if (lstr == "move"     ) type_ = Type::MOVE;
    else if (lstr == "e-resize" ) type_ = Type::E_RESIZE;
    else if (lstr == "ne-resize") type_ = Type::NE_RESIZE;
    else if (lstr == "nw-resize") type_ = Type::NW_RESIZE;
    else if (lstr == "n-resize" ) type_ = Type::N_RESIZE;
    else if (lstr == "se-resize") type_ = Type::SE_RESIZE;
    else if (lstr == "sw-resize") type_ = Type::SW_RESIZE;
    else if (lstr == "s-resize" ) type_ = Type::S_RESIZE;
    else if (lstr == "w-resize" ) type_ = Type::W_RESIZE;
    else if (lstr == "text"     ) type_ = Type::TEXT;
    else if (lstr == "wait"     ) type_ = Type::WAIT;
    else if (lstr == "help"     ) type_ = Type::HELP;
    else if (lstr == "progress" ) type_ = Type::PROGRESS;
    else if (lstr == "inherit"  ) type_ = Type::INHERIT;
    else                          type_ = Type::INVALID;
  }

 private:
  std::string str_;
  Type        type_ { Type::NONE };
};

//---

#include <CBrowserProperty.h>

template<>
class CBrowserPropertyTraits<CBrowserCursor> {
 public:
  std::string toString(const CBrowserCursor &c) {
    return c.string();
  }

  bool fromString(const std::string &str, CBrowserCursor &c) {
    c = CBrowserCursor(str);

    return true;
  }
};

#endif
