#include <CBrowserBody.h>
#include <CBrowserWindow.h>
#include <CBrowserDocument.h>

CBrowserBody::
CBrowserBody(CBrowserWindow *window, const CBrowserBodyData &data) :
 CBrowserObject(window, CHtmlTagId::BODY, data), data_(data)
{
  setDisplay(Display::BLOCK);

  setMargin(CBrowserMargin(CBrowserUnitValue("8px")));
}

void
CBrowserBody::
init()
{
  CBrowserObject::init();
}

void
CBrowserBody::
setNameValue(const std::string &name, const std::string &value)
{
  std::string lname  = CStrUtil::toLower(name);
  std::string lvalue = CStrUtil::toLower(value);

  if      (lname == "alink") {
    data_.alink = value;
  }
  else if (lname == "background") {
    background_.setImage(CBrowserBackgroundImage(value));
  }
  else if (lname == "bgproperties") {
    if      (lvalue == "fixed") {
      data_.fixed = true;
    }
    else
      window_->displayError("Illegal 'body' bgproperties value '%s'\n", value.c_str());
  }
  else if (lname == "class") {
    data_.cclass = value;
  }
  else if (lname == "leftmargin") {
    CBrowserUnitValue unit(value);

    if (unit.isValid())
      marginRef().setLeft(unit);
    else
      window_->displayError("Illegal 'body' Value for leftmargin '%s'\n", value.c_str());
  }
  else if (lname == "link") {
    data_.link = value;
  }
  else if (lname == "text") {
    data_.text = value;
  }
  else if (lname == "topmargin") {
    CBrowserUnitValue unit(value);

    if (unit.isValid())
      marginRef().setTop(unit);
    else
      window_->displayError("Illegal 'body' Value for topmargin '%s'\n", value.c_str());
  }
  else if (lname == "vlink") {
    data_.vlink = value;
  }
  else {
    CBrowserObject::setNameValue(name, value);
  }
}

void
CBrowserBody::
initProcess()
{
  CBrowserDocument *document = window_->getDocument();

  if (background().color().isValid())
    document->setBgColor(background().color().color());

  document->setFgColor(data_.text);

  if (background().image().isValid())
    window_->setBackgroundImage(background().image().value(), data_.fixed);

  document->setLinkColor (data_.link );
  document->setALinkColor(data_.alink);
  document->setVLinkColor(data_.vlink);
}

void
CBrowserBody::
termProcess()
{
}
