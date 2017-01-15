#ifndef CBrowserColor_H
#define CBrowserColor_H

#include <CRGBName.h>

class CBrowserColor {
 public:
  CBrowserColor() { }

  explicit CBrowserColor(const std::string &str) {
    setString(str);
  }

  explicit CBrowserColor(const CRGBA &c) :
   color_(c) {
    str_ = color_.toString();
  }

  const std::string &string() const { return str_; }
  void setString(const std::string &s) { str_ = s; color_ = CRGBName::toRGBA(str_); }

  bool isValid() const { return (str_ != ""); }

  const CRGBA &color() const { return color_; }

 private:
  std::string str_;
  CRGBA       color_ { 0, 0, 0 };
};

#endif
