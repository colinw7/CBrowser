#ifndef CBrowserProperty_H
#define CBrowserProperty_H

#include <CAlignType.h>

template<typename T>
class CBrowserPropertyTraits {
 public:
  CBrowserPropertyTraits() { }

  std::string toString(const T &t) { return std::to_string(t); }
  bool fromString(const std::string &, T &) { return false; }
};

template<>
class CBrowserPropertyTraits<std::string> {
 public:
  std::string toString(const std::string &str) { return str; }

  bool fromString(const std::string &str, std::string &value) { value = str; return true; }
};

//---

template<>
class CBrowserPropertyTraits<CHAlignType> {
 public:
  CBrowserPropertyTraits() { }

  std::string toString(const CHAlignType &align) {
    switch (align) {
      case CHALIGN_TYPE_LEFT   : return "left";
      case CHALIGN_TYPE_CENTER : return "center";
      case CHALIGN_TYPE_RIGHT  : return "right";
      case CHALIGN_TYPE_JUSTIFY: return "justify";
      case CHALIGN_TYPE_INSIDE : return "inside";
      case CHALIGN_TYPE_OUTSIDE: return "outside";
      default                  : return "";
    }

    return "";
  }

  bool fromString(const std::string &str, CHAlignType &align) {
    if (str == "left"   ) { align = CHALIGN_TYPE_LEFT   ; return true; }
    if (str == "center" ) { align = CHALIGN_TYPE_CENTER ; return true; }
    if (str == "right"  ) { align = CHALIGN_TYPE_RIGHT  ; return true; }
    if (str == "justify") { align = CHALIGN_TYPE_JUSTIFY; return true; }
    if (str == "inside" ) { align = CHALIGN_TYPE_INSIDE ; return true; }
    if (str == "outside") { align = CHALIGN_TYPE_OUTSIDE; return true; }

    return false;
  }
};

//---

template<>
class CBrowserPropertyTraits<CVAlignType> {
 public:
  CBrowserPropertyTraits() { }

  std::string toString(const CVAlignType &align) {
    switch (align) {
      case CVALIGN_TYPE_TOP     : return "top";
      case CVALIGN_TYPE_CENTER  : return "center";
      case CVALIGN_TYPE_BOTTOM  : return "bottom";
      case CVALIGN_TYPE_BASELINE: return "baseline";
      default                   : return "";
    }

    return "";
  }

  bool fromString(const std::string &str, CVAlignType &align) {
    if (str == "top"      ) { align = CVALIGN_TYPE_TOP     ; return true; }
    if (str == "center"   ) { align = CVALIGN_TYPE_CENTER  ; return true; }
    if (str == "bottom"   ) { align = CVALIGN_TYPE_BOTTOM  ; return true; }
    if (str == "baseline" ) { align = CVALIGN_TYPE_BASELINE; return true; }

    return false;
  }
};

//---

class CBrowserProperty {
 public:
  explicit CBrowserProperty(const std::string &name) :
   name_(name) {
  }

  virtual ~CBrowserProperty() { }

  virtual std::string value() = 0;

  template<typename T>
  static std::string toString(const T &t) {
    CBrowserPropertyTraits<T> traits;

    return traits.toString(t);
  }

  template<typename T>
  static bool fromString(const std::string &str, T &t) {
    CBrowserPropertyTraits<T> traits;

    return traits.fromString(str, t);
  }

 private:
  std::string name_;
};

#endif
