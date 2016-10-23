#include <CBrowserHtmlI.h>

std::string
HtmlIntegerToAlphabetic(int integer)
{
  std::string str;

  if (integer < 1 || integer > 26)
    str[0] = '?';
  else
    str[0] = integer - 1 + 'a';

  str[1] = '\0';

  return str;
}
