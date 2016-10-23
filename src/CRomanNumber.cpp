#include <CRomanNumber.h>
#include <sys/types.h>

using std::string;
using std::ostream;

struct CRomanDigitValue {
  char c;
  int  value;
};

static CRomanDigitValue
roman_digit_values[] = {
  { 'm', 1000, },
  { 'd',  500, },
  { 'c',  100, },
  { 'l',   50, },
  { 'x',   10, },
  { 'v',    5, },
  { 'i',    1, },
};

static uint num_roman_digit_values =
  sizeof(roman_digit_values)/sizeof(CRomanDigitValue);

CRomanNumber::
CRomanNumber(int value) :
 value_(value)
{
  int max_value = 4*roman_digit_values[0].value;

  if (value_ <= 0 || value_ >= max_value)
    throw "Invalid Value";
}

CRomanNumber::
CRomanNumber(const string &str)
{
  value_ = toInteger(str);
}

int
CRomanNumber::
getInteger() const
{
  return value_;
}

string
CRomanNumber::
getString() const
{
  return toString(value_);
}

string
CRomanNumber::
toString(int integer) const
{
  int char_value = roman_digit_values[0].value;

  string str;

  int i = -2;

  while (char_value >= 1) {
    int num = integer/char_value;

    if (i < 0)
      addChars(str, num,
               '\0',
               '\0',
               roman_digit_values[i + 2].c);
    else
      addChars(str, num,
               roman_digit_values[i + 0].c,
               roman_digit_values[i + 1].c,
               roman_digit_values[i + 2].c);

    i += 2;

    integer -= num*char_value;

    char_value /= 10;
  }

  return str;
}

void
CRomanNumber::
addChars(string &str, int num, char high_c, char mid_c, char low_c) const
{
  if      (num == 9) {
    str += low_c;
    str += high_c;
  }
  else if (num > 5) {
    str += mid_c;

    for (int i = 0; i < num - 5; i++)
      str += low_c;
  }
  else if (num == 5)
    str += mid_c;
  else if (num == 4) {
    str += low_c;
    str += mid_c;
  }
  else {
    for (int i = 0; i < num; i++)
      str += low_c;
  }
}

int
CRomanNumber::
toInteger(const string &str) const
{
  int integer = 0;

  int last_value = 0;

  for (int i = 0; i < (int) str.size(); i++) {
    int value = charToValue(str[i]);

    if (last_value != 0 && last_value < value)
      integer -= last_value;
    else
      integer += last_value;

    last_value = value;
  }

  integer += last_value;

  return integer;
}

int
CRomanNumber::
charToValue(char c) const
{
  if (isupper(c))
    c = tolower(c);

  for (uint i = 0; i < num_roman_digit_values; i++)
    if (roman_digit_values[i].c == c)
      return roman_digit_values[i].value;

  throw "Invalid char";

  return 0;
}

char
CRomanNumber::
valueToChar(int value) const
{
  for (uint i = 0; i < num_roman_digit_values; i++)
    if (roman_digit_values[i].value == value)
      return roman_digit_values[i].c;

  throw "Invalid value";

  return '\0';
}

ostream &
operator<<(ostream &os, const CRomanNumber &rn)
{
  os << rn.getString();

  return os;
}

CRomanNumber
operator+(const CRomanNumber &a, const CRomanNumber &b)
{
  return CRomanNumber(a.value_ + b.value_);
}
