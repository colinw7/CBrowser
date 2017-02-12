#ifndef CBrowserColor_H
#define CBrowserColor_H

#include <CRGBName.h>
#include <vector>

class CBrowserColorGradient {
 public:
  typedef std::vector<CRGBA> Colors;

  enum class Direction {
    NONE   = 0,
    LEFT   = (1<<0),
    RIGHT  = (1<<1),
    TOP    = (1<<2),
    BOTTOM = (1<<3)
  };

 public:
  CBrowserColorGradient() { }

  uint direction() const { return direction_; }
  void setDirection(uint d) { direction_ = d; }

  const Colors &colors() const { return colors_; }

  void reset() { colors_.clear(); }

  void addColor(const CRGBA &c) { colors_.push_back(c); }

 private:
  uint   direction_ { uint(Direction::LEFT) | uint(Direction::BOTTOM) };
  Colors colors_;
};

class CBrowserColor {
 public:
  enum class Type {
    NONE,
    COLOR,
    GRADIENT
  };

 public:
  CBrowserColor() { }

  explicit CBrowserColor(const std::string &str) :
   str_(str) {
    init();
  }

  explicit CBrowserColor(const CRGBA &c) :
   type_(Type::COLOR), color_(c) {
    str_ = color_.toString();
  }

  const std::string &string() const { return str_; }
  void setString(const std::string &s) { str_ = s; init(); }

  bool isValid() const { return type_ != Type::NONE; }

  const Type &type() const { return type_; }

  const CRGBA &color() const {
    assert(type_ == Type::COLOR);
    return color_;
  }

  const CBrowserColorGradient &gradient() const {
    assert(type_ == Type::GRADIENT);
    return gradient_;
  }

 private:
  void init();

  void addGradientWord(const std::string &str);

 private:
  std::string           str_;
  Type                  type_ { Type::NONE };
  CRGBA                 color_ { 0, 0, 0 };
  CBrowserColorGradient gradient_;
};

//---

#include <CBrowserProperty.h>

template<>
class CBrowserPropertyTraits<CBrowserColor> {
 public:
  std::string toString(const CBrowserColor &c) {
    return c.string();
  }

  bool fromString(const std::string &str, CBrowserColor &c) {
    c = CBrowserColor(str);

    return true;
  }
};

#endif
