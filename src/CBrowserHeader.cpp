#include <CBrowserHeader.h>
#include <CBrowserWindow.h>
#include <CBrowserFont.h>

namespace {
  int idToInd(CHtmlTagId id) {
    static std::vector<CHtmlTagId> ids =
      {{ CHtmlTagId::H1, CHtmlTagId::H2, CHtmlTagId::H3,
         CHtmlTagId::H4, CHtmlTagId::H3, CHtmlTagId::H4 }};

    for (uint i = 0; i < ids.size(); ++i) {
      if (ids[i] == id)
        return i;
    }

    return -1;
  }
}

CBrowserHeader::
CBrowserHeader(CBrowserWindow *window, CHtmlTagId id) :
 CBrowserObject(window, id), ind_(-1)
{
  ind_ = idToInd(id);

  if      (id == CHtmlTagId::H1) font_.setSize(window_->sizeToFontSize(6));
  else if (id == CHtmlTagId::H2) font_.setSize(window_->sizeToFontSize(5));
  else if (id == CHtmlTagId::H3) font_.setSize(window_->sizeToFontSize(4));
  else if (id == CHtmlTagId::H4) font_.setSize(window_->sizeToFontSize(3));
  else if (id == CHtmlTagId::H5) font_.setSize(window_->sizeToFontSize(2));
  else if (id == CHtmlTagId::H6) font_.setSize(window_->sizeToFontSize(1));

  if      (id == CHtmlTagId::H1) marginRef().setTop(CBrowserUnitValue("0.67em"));
  else if (id == CHtmlTagId::H2) marginRef().setTop(CBrowserUnitValue("0.75em"));
  else if (id == CHtmlTagId::H3) marginRef().setTop(CBrowserUnitValue("0.83em"));
  else if (id == CHtmlTagId::H4) marginRef().setTop(CBrowserUnitValue("1.12em"));
  else if (id == CHtmlTagId::H5) marginRef().setTop(CBrowserUnitValue("1.12em"));
  else if (id == CHtmlTagId::H6) marginRef().setTop(CBrowserUnitValue("1.12em"));

  if      (id == CHtmlTagId::H1) marginRef().setBottom(CBrowserUnitValue("0.67em"));
  else if (id == CHtmlTagId::H2) marginRef().setBottom(CBrowserUnitValue("0.75em"));
  else if (id == CHtmlTagId::H3) marginRef().setBottom(CBrowserUnitValue("0.83em"));
  else if (id == CHtmlTagId::H4) marginRef().setBottom(CBrowserUnitValue("1.12em"));
  else if (id == CHtmlTagId::H5) marginRef().setBottom(CBrowserUnitValue("1.12em"));
  else if (id == CHtmlTagId::H6) marginRef().setBottom(CBrowserUnitValue("1.12em"));

  font_.setBold();
}

CBrowserHeader::
~CBrowserHeader()
{
}

void
CBrowserHeader::
init()
{
  CBrowserObject::init();
}

void
CBrowserHeader::
setNameValue(const std::string &name, const std::string &value)
{
  std::string lname  = CStrUtil::toLower(name);
  std::string lvalue = CStrUtil::toLower(value);

  if (lname == "align") {
    CHAlignType align;

    window_->parseHAlignOption(lvalue, align);

    setHAlign(align);
  }
  else {
    CBrowserObject::setNameValue(name, value);
  }
}
