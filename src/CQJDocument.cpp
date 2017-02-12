#include <CQJDocument.h>
#include <CQJHtmlCollection.h>
#include <CQJHtmlObj.h>
#include <CBrowserMain.h>
#include <CBrowserWindow.h>
#include <CBrowserDocument.h>
#include <CBrowserForm.h>
#include <CBrowserIFace.h>
#include <CBrowserScrolledWindow.h>
#include <CBrowserOutputTag.h>
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
  objType_->addObjFunction(js, "createElement"         , objType_);
  objType_->addObjFunction(js, "createTextNode"        , objType_);
  objType_->addObjFunction(js, "getElementById"        , objType_);
  objType_->addObjFunction(js, "getElementsByClassName", objType_);
  objType_->addObjFunction(js, "getElementsByTagName"  , objType_);
  objType_->addObjFunction(js, "querySelector"         , objType_);
}

CJValueP
CQJDocument::
getProperty(CJavaScript *js, const std::string &name) const
{
  if      (name == "forms") {
    CQJHtmlCollection *col = new CQJHtmlCollection(js);

    CBrowserWindow *window = getWindow();

    CBrowserDocument *document = window->getDocument();

    for (auto &form : document->forms()) {
      CBrowserForm *form1 = const_cast<CBrowserForm *>(form);

      col->addObject(form1);
    }

    return CJValueP(col);
  }
  else
    return CQJObject::getProperty(js, name);
}

CJValueP
CQJDocument::
execNameFn(CJavaScript *js, const std::string &name, const Values &values)
{
  if      (name == "createElement") {
    if (values.size() == 2) {
      std::string id = values[1]->toString();

      const CHtmlTagDef &def = CHtmlTagDefLookupInst->lookup(id);

      if (def.getId() == CHtmlTagId::NONE)
        return CJValueP();

      CBrowserOutputTagBase *tag = CBrowserOutputTagMgrInst->getTag(def.getId());

      CBrowserWindow *window = getWindow();

      CBrowserObject *obj = tag->start(window, nullptr);
      if (! obj) return CJValueP();

      window->startObject(obj, /*add*/true);

      CJValueP htmlObj = CBrowserJSInst->lookupHtmlObject(obj);

      return htmlObj;
    }

    return CJValueP();
  }
  else if (name == "createTextNode") {
  }
  else if (name == "getElementById") {
    if (values.size() == 2) {
      std::string id = values[1]->toString();

      CBrowserWindow *window = getWindow();

      CBrowserObject *obj = window->getObject(id);
      if (! obj) return CJValueP();

      CJValueP htmlObj = CBrowserJSInst->lookupHtmlObject(obj);

      return htmlObj;
    }

    return CJValueP();
  }
  else if (name == "getElementsByClassName") {
    CQJHtmlCollection *col = new CQJHtmlCollection(js);

    if (values.size() == 2) {
      std::string name = values[1]->toString();

      for (const auto &obj : CBrowserJSInst->objects()) {
        CQJHtmlObj     *obj1 = dynamic_cast<CQJHtmlObj *>(obj);
        CBrowserObject *obj2 = (obj1 ? obj1->htmlObj() : nullptr);

        if (obj2 && obj2->getClass() == name)
          col->addObject(obj2);
      }
    }

    return CJValueP(col);
  }
  else if (name == "getElementsByTagName") {
    CQJHtmlCollection *col = new CQJHtmlCollection(js);

    if (values.size() == 2) {
      std::string name = values[1]->toString();

      for (const auto &obj : CBrowserJSInst->objects()) {
        CQJHtmlObj     *obj1 = dynamic_cast<CQJHtmlObj *>(obj);
        CBrowserObject *obj2 = (obj1 ? obj1->htmlObj() : nullptr);

        if (obj2 && obj2->typeName() == name)
          col->addObject(obj2);
      }
    }

    return CJValueP(col);
  }
  else if (name == "querySelector") {
    if (values.size() == 2) {
      std::string id = values[1]->toString();

      CBrowserWindow *window = getWindow();

      CBrowserObject *obj = window->getObject(id);
      if (! obj) return CJValueP();

      CJValueP htmlObj = CBrowserJSInst->lookupHtmlObject(obj);

      return htmlObj;
    }

    return CJValueP();
  }

  return CQJObject::execNameFn(js, name, values);
}

CBrowserWindow *
CQJDocument::
getWindow() const
{
  CBrowserIFace *iface = CBrowserMainInst->iface();
  if (! iface) return nullptr;

  CBrowserScrolledWindow *swindow = iface->currentWindow();
  if (! swindow) return nullptr;

  return swindow->getWindow();
}
