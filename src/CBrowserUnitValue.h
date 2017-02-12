#ifndef CBrowserUnitValue_H
#define CBrowserUnitValue_H

#include <CScreenUnits.h>
#include <CStrUtil.h>

class CBrowserUnitValue {
 public:
  enum class Type {
    AUTO,
    INITIAL,
    INHERIT,
    VALUE
  };

 public:
  CBrowserUnitValue() { }

  explicit CBrowserUnitValue(const std::string &str) :
   str_(str) {
    int len = str.size();

    if      (str == "auto") {
      type_ = Type::AUTO;
    }
    else if (str == "initial") {
      type_ = Type::INITIAL;
    }
    else if (str == "inherit") {
      type_ = Type::INHERIT;
    }
    else if (str == "none") {
      type_  = Type::VALUE;
      value_ = CScreenUnits(0, CScreenUnits::Units::PX);
    }
    else if (str[len - 1] == '%') {
      std::string value1 = str.substr(0, len - 1);

      if (CStrUtil::isReal(value1))
        value_ = CScreenUnits(CStrUtil::toReal(value1), CScreenUnits::Units::PERCENT);
      else
        std::cerr << "Invalid percent unit value: '" << value1 << "'" << std::endl;
    }
    else if (len > 2 && str[len - 2] == 'p' && str[len - 1] == 'x') {
      std::string value1 = str.substr(0, len - 2);

      if (CStrUtil::isReal(value1))
        value_ = CScreenUnits(CStrUtil::toReal(value1), CScreenUnits::Units::PX);
      else
        std::cerr << "Invalid px unit value: '" << value1 << "'" << std::endl;
    }
    else if (len > 2 && str[len - 2] == 'c' && str[len - 1] == 'm') {
      std::string value1 = str.substr(0, len - 2);

      if (CStrUtil::isReal(value1))
        value_ = CScreenUnits(CStrUtil::toReal(value1), CScreenUnits::Units::CM);
      else
        std::cerr << "Invalid cm unit value: '" << value1 << "'" << std::endl;
    }
    else if (len > 2 && str[len - 2] == 'e' && str[len - 1] == 'm') {
      std::string value1 = str.substr(0, len - 2);

      if (CStrUtil::isReal(value1))
        value_ = CScreenUnits(CStrUtil::toReal(value1), CScreenUnits::Units::EM);
      else
        std::cerr << "Invalid em unit value: '" << value1 << "'" << std::endl;
    }
    else {
      if (CStrUtil::isReal(str))
        value_ = CScreenUnits(CStrUtil::toReal(str), CScreenUnits::Units::PX);
      else
        std::cerr << "Invalid px unit value: '" << str << "'" << std::endl;
    }
  }

  explicit CBrowserUnitValue(double value, CScreenUnits::Units units=CScreenUnits::Units::PX) :
    value_(value, units), type_(Type::VALUE) {
  }

  explicit CBrowserUnitValue(CScreenUnits::Units units, double value=1.0) :
    value_(value, units), type_(Type::VALUE) {
  }

  explicit CBrowserUnitValue(const CScreenUnits &value) :
    value_(value), type_(Type::VALUE) {
  }

  CScreenUnits::Units units() const { return value_.units(); }

  bool isRelative() const {
    return (units() == CScreenUnits::Units::PERCENT);
  }

  CScreenUnits value() const {
    return (isValid() ? value_ : CScreenUnits());
  }

  double pxValue(const CScreenUnits &refValue=CScreenUnits()) const {
    if (isValid())
      return value().pxValue(refValue);
   else
      return 0;
  }

  bool isValid() const { return value_.isValid(); }

 private:
  std::string  str_;
  CScreenUnits value_;
  Type         type_ { Type::VALUE };
};

#endif
