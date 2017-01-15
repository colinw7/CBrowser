#include <CBrowserParagraph.h>
#include <CBrowserWindow.h>
#include <CBrowserProperty.h>

CBrowserParagraph::
CBrowserParagraph(CBrowserWindow *window, const CBrowserParagraphData &data) :
 CBrowserObject(window, CHtmlTagId::P), data_(data)
{
  marginRef().setTop   (CBrowserUnitValue("1.00em"));
  marginRef().setBottom(CBrowserUnitValue("1.00em"));
}

void
CBrowserParagraph::
init()
{
  std::vector<std::string> strs = {{ "align" }};

  addProperties(strs);

  CBrowserObject::init();
}

void
CBrowserParagraph::
setNameValue(const std::string &name, const std::string &value)
{
  std::string lname = CStrUtil::toLower(name);

  if (lname == "align") {
    CHAlignType align;

    window_->parseHAlignOption(value, align);

    setHAlign(align);
  }
  else
    CBrowserObject::setNameValue(name, value);
}

std::string
CBrowserParagraph::
propertyValue(int i) const
{
  const std::string &name = propertyName(i);

  if (name == "align")
    return CBrowserProperty::toString(halign());
  else
    return CBrowserObject::propertyValue(i);
}
