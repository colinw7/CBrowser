#include <CBrowserParagraph.h>
#include <CBrowserWindow.h>
#include <CBrowserProperty.h>

CBrowserParagraph::
CBrowserParagraph(CBrowserWindow *window, const CBrowserParagraphData &data) :
 CBrowserObject(window, CHtmlTagId::P), data_(data)
{
  std::vector<std::string> strs = {{ "align" }};

  setProperties(strs);
}

std::string
CBrowserParagraph::
propertyValue(int i) const
{
  if (i == 0) return CBrowserProperty::toString(data_.align);

  return "";
}

void
CBrowserParagraph::
initLayout()
{
  window_->skipLine();

  window_->setAlign(data_.align, CVALIGN_TYPE_TOP);
}

void
CBrowserParagraph::
termLayout()
{
  window_->newLine();

  window_->setAlign(CHALIGN_TYPE_LEFT, CVALIGN_TYPE_TOP);
}
