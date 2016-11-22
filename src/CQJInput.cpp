#include <CQJInput.h>
#include <CBrowserForm.h>
#include <CJavaScript.h>

CJObjTypeP CQJInputType::type_;

CJObjTypeP
CQJInputType::
instance(CJavaScript *js)
{
  if (! type_) {
    type_ = CJObjTypeP(new CQJInputType(js));

    js->addObjectType("input", type_);
  }

  return type_;
}

CQJInputType::
CQJInputType(CJavaScript *js) :
 CJObjType(js, CJToken::Type::Object, "Input")
{
}

//------

CQJInput::
CQJInput(CJavaScript *js, CBrowserFormInput *input) :
 CQJHtmlObj(js, input, CQJInputType::instance(js)), input_(input)
{
}

CJValueP
CQJInput::
getProperty(CJavaScript *js, const std::string &name) const
{
  if (name == "value") {
    switch (input_->getType()) {
      case CBrowserFormInputType::CHECKBOX: {
        return CJValueP();
      }
      case CBrowserFormInputType::FILE: {
        return CJValueP();
      }
      case CBrowserFormInputType::HIDDEN: {
        return CJValueP();
      }
      case CBrowserFormInputType::IMAGE: {
        return CJValueP();
      }
      case CBrowserFormInputType::PASSWORD_TEXT: {
        return CJValueP();
      }
      case CBrowserFormInputType::RADIO_BUTTON: {
        return CJValueP();
      }
      case CBrowserFormInputType::RANGE: {
        CBrowserFormRange *range = dynamic_cast<CBrowserFormRange *>(input_);

        return js->createNumberValue(long(range->value()));
      }
      case CBrowserFormInputType::RESET_BUTTON: {
        return CJValueP();
      }
      case CBrowserFormInputType::SELECT: {
        CBrowserFormSelect *select = dynamic_cast<CBrowserFormSelect *>(input_);

        return js->createStringValue(select->value());
      }
      case CBrowserFormInputType::SUBMIT_BUTTON: {
        return CJValueP();
      }
      case CBrowserFormInputType::TEXT: {
        CBrowserFormText *text = dynamic_cast<CBrowserFormText *>(input_);

        return js->createStringValue(text->text());
      }
      case CBrowserFormInputType::TEXTAREA: {
        return CJValueP();
      }
      default:
        break;
    }
  }

  return CJObj::getProperty(js, name);
}

void
CQJInput::
setProperty(CJavaScript *js, const std::string &name, CJValueP value)
{
  CJObj::setProperty(js, name, value);
}

CJValueP
CQJInput::
execNameFn(CJavaScript *js, const std::string &name, const Values &values)
{
  return CQJObject::execNameFn(js, name, values);
}
