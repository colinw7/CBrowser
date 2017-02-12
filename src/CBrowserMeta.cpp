#include <CBrowserMeta.h>

CBrowserMeta::
CBrowserMeta(CBrowserWindow *window) :
 CBrowserObject(window, CHtmlTagId::META)
{
}

void
CBrowserMeta::
init()
{
  std::vector<std::string> strs = {{
    "charset", "content", "http-equiv", "scheme" }};

  addProperties(strs);

  CBrowserObject::init();
}

std::string
CBrowserMeta::
propertyValue(int i) const
{
  const std::string &name = propertyName(i);

  if      (name == "charset"   ) return CBrowserProperty::toString(data_.charset);
  else if (name == "content"   ) return CBrowserProperty::toString(data_.content);
  else if (name == "http-equiv") return CBrowserProperty::toString(data_.http_equiv);
  else if (name == "scheme"    ) return CBrowserProperty::toString(data_.scheme);

  return "";
}

void
CBrowserMeta::
setNameValue(const std::string &name, const std::string &value)
{
  std::string lname = CStrUtil::toLower(name);

  if      (lname == "charset") {
    data_.charset = value;
  }
  else if (lname == "content") {
    data_.content = value;
  }
  else if (lname == "http-equiv") {
    data_.http_equiv = value;
  }
  else if (lname == "scheme") {
    data_.scheme = value;
  }
  else {
    CBrowserObject::setNameValue(name, value);
  }
}
