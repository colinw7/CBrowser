#include <CBrowserMeta.h>

CBrowserMeta::
CBrowserMeta(CBrowserWindow *window) :
 CBrowserObject(window, CHtmlTagId::META)
{
}

void
CBrowserMeta::
setNameValue(const std::string &name, const std::string &value)
{
  std::string lname = CStrUtil::toLower(name);

  if      (lname == "charset") {
  }
  else if (lname == "content") {
  }
  else if (lname == "http-equiv") {
  }
  else if (lname == "name") {
  }
  else if (lname == "scheme") {
  }
  else {
    CBrowserObject::setNameValue(name, value);
  }
}
