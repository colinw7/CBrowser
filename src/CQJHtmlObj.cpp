#include <CQJHtmlObj.h>
#include <CJavaScript.h>

CJObjTypeP CQJHtmlObjType::type_;

CJObjTypeP
CQJHtmlObjType::
instance(CJavaScript *js)
{
  if (! type_) {
    type_ = CJObjTypeP(new CQJHtmlObjType(js));

    js->addObjectType("window", type_);
  }

  return type_;
}

CQJHtmlObjType::
CQJHtmlObjType(CJavaScript *js) :
 CJObjType(js, CJToken::Type::Object, "Html")
{
}

//------

CQJHtmlObj::
CQJHtmlObj(CJavaScript *js, CBrowserObject *htmlObj) :
 CQJObject(js, CQJHtmlObjType::instance(js)), htmlObj_(htmlObj)
{
  init();
}

CQJHtmlObj::
CQJHtmlObj(CJavaScript *js, CBrowserObject *htmlObj, const CJObjTypeP &type) :
 CQJObject(js, type), htmlObj_(htmlObj)
{
  init();
}

void
CQJHtmlObj::
init()
{
  objType_->addObjFunction(js_, "appendChild" , objType_);
  objType_->addObjFunction(js_, "insertBefore", objType_);
  objType_->addObjFunction(js_, "getAttribute", objType_);
}

CJValueP
CQJHtmlObj::
getProperty(CJavaScript *js, const std::string &name) const
{
  if      (name == "parentNode") {
    CBrowserObject *parent = htmlObj_->parent();
    if (! parent) return CJValueP();

    CQJHtmlObj *parentObj = htmlObj_->getJObj();
    if (! parentObj) return CJValueP();

    return CJValueP(parentObj);
  }
  else
    return CJObj::getProperty(js, name);
}

void
CQJHtmlObj::
setProperty(CJavaScript *js, const std::string &name, CJValueP value)
{
  CJObj::setProperty(js, name, value);
}

CJValueP
CQJHtmlObj::
execNameFn(CJavaScript *js, const std::string &name, const Values &values)
{
  if      (name == "appendChild") {
    return CJValueP();
  }
  else if (name == "insertBefore") {
    return CJValueP();
  }
  else if (name == "getAttribute") {
    return CJValueP();
  }
  else
    return CQJObject::execNameFn(js, name, values);
}
