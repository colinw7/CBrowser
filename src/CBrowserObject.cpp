#include <CBrowserObject.h>

std::string
CBrowserObject::
typeName() const
{
  switch (type_) {
    case Type::BREAK     : return "break";
    case Type::CANVAS    : return "canvas";
    case Type::FORM_INPUT: return "form_input";
    case Type::IMAGE     : return "image";
    case Type::LABEL     : return "label";
    case Type::RULE      : return "rule";
    case Type::SYMBOL    : return "symbol";
    case Type::TABLE     : return "table";
    case Type::TEXT      : return "text";
  }

  return "";
}
