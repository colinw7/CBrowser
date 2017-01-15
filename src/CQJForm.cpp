#include <CQJForm.h>
#include <CQJHtmlCollection.h>
#include <CBrowserForm.h>
#include <CJavaScript.h>

CJObjTypeP CQJFormType::type_;

CJObjTypeP
CQJFormType::
instance(CJavaScript *js)
{
  if (! type_) {
    type_ = CJObjTypeP(new CQJFormType(js));

    js->addObjectType("form", type_);
  }

  return type_;
}

CQJFormType::
CQJFormType(CJavaScript *js) :
 CJObjType(js, CJToken::Type::Object, "Form")
{
}

//------

CQJForm::
CQJForm(CJavaScript *js, CBrowserForm *form) :
 CQJHtmlObj(js, form, CQJFormType::instance(js)), form_(form)
{
}

CJValueP
CQJForm::
getProperty(CJavaScript *js, const std::string &name) const
{
  CQJHtmlCollection *col = new CQJHtmlCollection(js);

  for (const auto &input : form_->inputs()) {
    if (input->getName() == name)
      col->addObject(input);
  }

  return CJValueP(col);
}

void
CQJForm::
setProperty(CJavaScript *js, const std::string &name, CJValueP value)
{
  CJObj::setProperty(js, name, value);
}

CJValueP
CQJForm::
execNameFn(CJavaScript *js, const std::string &name, const Values &values)
{
  return CQJObject::execNameFn(js, name, values);
}
