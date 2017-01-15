#include <CQJHtmlCollection.h>
#include <CQJHtmlObj.h>
#include <CJavaScript.h>

CJObjTypeP CQJHtmlCollectionType::type_;

CJObjTypeP
CQJHtmlCollectionType::
instance(CJavaScript *js)
{
  if (! type_)
    type_ = CJObjTypeP(new CQJHtmlCollectionType(js));

  return type_;
}

CQJHtmlCollectionType::
CQJHtmlCollectionType(CJavaScript *js) :
 CJObjType(js, CJToken::Type::Object, "HtmlCollection")
{
}

//---

CQJHtmlCollection::
CQJHtmlCollection(CJavaScript *js) :
 CQJObject(js, CQJHtmlCollectionType::instance(js))
{
}

void
CQJHtmlCollection::
addObject(CBrowserObject *obj)
{
  objs_.push_back(obj);
}

CJValueP
CQJHtmlCollection::
indexValue(long i) const
{
  CJValueP value;

  if (i >= 0 && i < int(objs_.size()))
    value = objs_[i]->getJObjValue();

  return value;
}

void
CQJHtmlCollection::
setIndexValue(long i, CJValueP value)
{
  CQJHtmlObj *obj = CJValue::cast<CQJHtmlObj>(value).get();
  assert(obj);

  if (i >= 0 && i < int(objs_.size()))
    objs_[i] = obj->htmlObj();
}

CJValueP
CQJHtmlCollection::
getProperty(CJavaScript *js, const std::string &name) const
{
  if      (name == "length") {
    return js->createNumberValue(long(objs_.size()));
  }
  else {
    for (const auto &obj : objs_) {
      if (obj->getName() == name) {
        return obj->getJObjValue();
      }
    }

    return CJValueP();
  }
}

CJValueP
CQJHtmlCollection::
execNameFn(CJavaScript *, const std::string &, const Values &)
{
  return CJValueP();
}
