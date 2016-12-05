#include <CBrowserJS.h>
#include <CBrowserMain.h>
#include <CBrowserIFace.h>
#include <CBrowserWindow.h>
#include <CBrowserObject.h>
#include <CBrowserCanvas.h>
#include <CBrowserForm.h>
#include <CQJDocument.h>
#include <CQJWindow.h>
#include <CQJCanvas.h>
#include <CQJInput.h>
#include <CQJImage.h>
#include <CQJIntervalFunction.h>
#include <CJavaScript.h>
#include <CStrParse.h>
#include <QMouseEvent>

CBrowserJS *
CBrowserJS::
instance()
{
  static CBrowserJS *inst;

  if (! inst) {
    inst = new CBrowserJS;

    inst->init();
  }

  return inst;
}

CBrowserJS::
CBrowserJS()
{
  js_ = new CJavaScript;

  //js_->setParseDebug(true);
  //js_->setExecDebug(true);
}

void
CBrowserJS::
init()
{
  js_->defineObjectT<CQJImageFunction, CQJImage>(js_, "Image");

  js_->addObjectType("Canvas", CQJCanvasType::instance(js_));

  jsWindow_   = CJValueP(new CQJWindow  (js_));
  jsDocument_ = CJValueP(new CQJDocument(js_));

  js_->setProperty("window"  , jsWindow_);
  js_->setProperty("document", jsDocument_);

  js_->setProperty("setInterval"  , CJValueP(new CQJSetInterval  (js_)));
  js_->setProperty("clearInterval", CJValueP(new CQJClearInterval(js_)));

  //js_->setProperty("requestAnimationFrame", CJValueP(new CQJRequestAnimationFrame(js_)));
}

void
CBrowserJS::
addHtmlObject(CBrowserObject *obj)
{
  CQJHtmlObj *htmlObj = 0;

  if      (obj->type() == CHtmlTagId::CANVAS)
    htmlObj = new CQJCanvas(js_, dynamic_cast<CBrowserCanvas *>(obj));
  else if (obj->type() == CHtmlTagId::INPUT)
    htmlObj = new CQJInput(js_, dynamic_cast<CBrowserFormInput *>(obj));
  else
    htmlObj = new CQJHtmlObj(js_, obj);

  objMap_[obj] = CJValueP(htmlObj);

  addJSObject(htmlObj);

  obj->setJObj(htmlObj);
}

CJValueP
CBrowserJS::
lookupHtmlObject(CBrowserObject *obj) const
{
  auto p = objMap_.find(obj);

  if (p == objMap_.end())
    return nullptr;

  return (*p).second;
}

void
CBrowserJS::
addJSObject(CQJObject *obj)
{
  objects_.insert(obj);
}

void
CBrowserJS::
removeJSObject(CQJObject *obj)
{
  objects_.erase(obj);
}

void
CBrowserJS::
runScript(CBrowserWindow *, const std::string &text)
{
  js_->loadString(text);

  CJValueP value = js_->exec();

  if (value) {
    value->print(std::cout);
    std::cout << std::endl;
  }
}

void
CBrowserJS::
runScriptFile(CBrowserWindow *, const std::string &filename)
{
  js_->loadFile(filename);

  CJValueP value = js_->exec();

  if (value) {
    value->print(std::cout);
    std::cout << std::endl;
  }
}

void
CBrowserJS::
onLoad()
{
  callEventListeners("load", "onload");
}

void
CBrowserJS::
callEventListeners(const std::string &name, const std::string &prop, const EventArgs &args)
{
  for (const auto &o : objects_)
    o->callEventListener(name, prop, args);
}
