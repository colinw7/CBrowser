#include <CBrowserScript.h>
#include <CBrowserWindow.h>
#include <CBrowserCeil.h>
#include <CStrUtil.h>

CBrowserScript::
CBrowserScript(CBrowserWindow *window, const CBrowserScriptData &data) :
 CBrowserObject(window, CHtmlTagId::SCRIPT), data_(data)
{
}

void
CBrowserScript::
init()
{
  if (data_.src != "") {
    if      (CStrUtil::toLower(data_.language) == "ceil") {
      CBrowserCeilInst->runScriptFile(window_, data_.src);
    }
    else if (CStrUtil::toLower(data_.type) == "text/javascript") {
      window_->addScriptFile(data_.src);
    }
  }

  std::vector<std::string> strs = {{ "language", "src", "type" }};

  addProperties(strs);

  CBrowserObject::init();
}

void
CBrowserScript::
setNameValue(const std::string &name, const std::string &value)
{
  std::string lname  = CStrUtil::toLower(name);
//std::string lvalue = CStrUtil::toLower(value);

  if      (lname == "language") {
    data_.language = value;
  }
  else if (lname == "src") {
    data_.src = value;
  }
  else if (lname == "type") {
    data_.type = value;
  }
  else {
    CBrowserObject::setNameValue(name, value);
  }
}

void
CBrowserScript::
initProcess()
{
}

void
CBrowserScript::
termProcess()
{
  if (text_ != "") {
    if      (CStrUtil::casecmp(data_.language, "ceil") == 0) {
      CBrowserCeilInst->runScript(window_, text_);
    }
    else if (CStrUtil::casecmp(data_.type, "text/javascript") == 0) {
      //CQJavaScriptInst->runScript(text_);

      window_->addScript(text_);
    }
  }
}

std::string
CBrowserScript::
propertyValue(int i) const
{
  const std::string &name = propertyName(i);

  if      (name == "language") return CBrowserProperty::toString(data_.language);
  else if (name == "src"     ) return CBrowserProperty::toString(data_.src);
  else if (name == "type"    ) return CBrowserProperty::toString(data_.type);

  return "";
}
