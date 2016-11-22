#include <CQJDocument.h>
#include <CBrowserMain.h>
#include <CBrowserWindow.h>
#include <CBrowserIFace.h>
#include <CBrowserJS.h>
#include <CJavaScript.h>

CJObjTypeP CQJDocumentType::type_;

CJObjTypeP
CQJDocumentType::
instance(CJavaScript *js)
{
  if (! type_) {
    type_ = CJObjTypeP(new CQJDocumentType(js));

    js->addObjectType("document", type_);
  }

  return type_;
}

CQJDocumentType::
CQJDocumentType(CJavaScript *js) :
 CJObjType(js, CJToken::Type::Object, "Document")
{
}

//---

CQJDocument::
CQJDocument(CJavaScript *js) :
 CQJObject(js, CQJDocumentType::instance(js))
{
  objType_->addObjFunction(js, "getElementById", objType_);
  objType_->addObjFunction(js, "querySelector" , objType_);
}

CJValueP
CQJDocument::
execNameFn(CJavaScript *js, const std::string &name, const Values &values)
{
  if      (name == "getElementById") {
    if (values.size() == 2) {
      std::string id = values[1]->toString();

      const CBrowserMain::IFaceList ifaceList = CBrowserMainInst->ifaceList();

      for (const auto &i : ifaceList) {
        CBrowserWindow *window = i->getWindow();

        CBrowserObject *obj = window->getObject(id);

        if (! obj)
          return CJValueP();

        CJValueP htmlObj = CBrowserJSInst->lookupHtmlObject(obj);

        return htmlObj;
      }
    }

    return CJValueP();
  }
  else if (name == "querySelector") {
    if (values.size() == 2) {
      std::string id = values[1]->toString();

      const CBrowserMain::IFaceList ifaceList = CBrowserMainInst->ifaceList();

      for (const auto &i : ifaceList) {
        CBrowserWindow *window = i->getWindow();

        CBrowserObject *obj = window->getObject(id);

        if (! obj)
          return CJValueP();

        CJValueP htmlObj = CBrowserJSInst->lookupHtmlObject(obj);

        return htmlObj;
      }
    }

    return CJValueP();
  }

  return CQJObject::execNameFn(js, name, values);
}
