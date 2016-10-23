#ifndef CROMAN_NUMBER_H
#define CROMAN_NUMBER_H

#include <string>
#include <iostream>

struct CRomanDigitValue;

class CRomanNumber {
 private:
  int value_;

 public:
  CRomanNumber(int value);
  CRomanNumber(const std::string &str);

  int         getInteger() const;
  std::string getString() const;

  friend std::ostream &operator<<(std::ostream &os, const CRomanNumber &rn);

  friend CRomanNumber operator+(const CRomanNumber &a, const CRomanNumber &b);

  friend CRomanNumber &operator+=(CRomanNumber &a, const CRomanNumber &b) {
    a = a + b;

    return a;
  }

 private:
  std::string toString(int integer) const;
  int         toInteger(const std::string &str) const;

  void addChars(std::string &str, int num, char high_c, char mid_c, char low_c) const;

  int  charToValue(char c) const;
  char valueToChar(int value) const;
};

#endif
