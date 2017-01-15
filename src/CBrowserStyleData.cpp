#include <CBrowserStyleData.h>

void
CBrowserStyleData::
setValue(const std::string &name, const std::string &value)
{
  nameValues_[name] = value;
}
