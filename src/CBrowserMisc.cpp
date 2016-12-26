#include <CBrowserMisc.h>

std::string
CBrowserMisc::
integerToAlphabetic(int integer)
{
  std::string str;

  if (integer < 1 || integer > 26)
    str += '?';
  else
    str += char(integer - 1 + 'a');

  return str;
}
