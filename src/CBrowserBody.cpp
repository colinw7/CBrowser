#include <CBrowserBody.h>
#include <CBrowserWindow.h>
#include <CBrowserDocument.h>

CBrowserBody::
CBrowserBody(CBrowserWindowIFace *window) :
 CBrowserObject(window, CHtmlTagId::BODY)
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
  auto *document = window_->getDocument();

  if (background().color().isValid())
    document->setBgColor(background().color().color());

  document->setFgColor(data_.text);

  if (background().image().isValid()) {
    auto imageName = background().image().value();

    window_->setBackgroundImage(QString::fromStdString(imageName), data_.fixed);
  }

  document->setLinkColor (data_.link );
  document->setALinkColor(data_.alink);
  document->setVLinkColor(data_.vlink);
}

void
CBrowserBody::
termProcess()
{
}
