#include <CQJCanvas.h>
#include <CQJCanvasContext2D.h>
#include <CQJImage.h>
#include <CBrowserCanvas.h>
#include <CJavaScript.h>
#include <CStrParse.h>
#include <iostream>

CJObjTypeP CQJCanvasType::type_;

CJObjTypeP
CQJCanvasType::
instance(CJavaScript *js)
{
  if (! type_) {
    type_ = CJObjTypeP(new CQJCanvasType(js));

    js->addObjectType("Canvas", type_);
  }

  return type_;
}

CQJCanvasType::
CQJCanvasType(CJavaScript *js) :
 CJObjType(js, CJToken::Type::Object, "Canvas")
{
}

CJValueP
CQJCanvasType::
exec(CJavaScript *js, const std::string &name, const Values &values)
{
  if (values.size() < 1) {
    js->errorMsg("Invalid number of arguments for " + name);
    return CJValueP();
  }

  CQJCanvas *canvas = values[0]->castP<CQJCanvas>();
  assert(canvas);

  return CJValueP();
}

//------

CQJCanvas::
CQJCanvas(CJavaScript *js, CBrowserCanvas *canvas) :
 CQJHtmlObj(js, canvas, CQJCanvasType::instance(js)), canvas_(canvas)
{
  objType_->addObjFunction(js, "getContext", objType_);

  context2D_ = CJValueP(new CQJCanvasContext2D(js, this));

  updateSize();
}

void
CQJCanvas::
updateSize()
{
  setRealProperty(js_, "width" , canvas_->width ());
  setRealProperty(js_, "height", canvas_->height());
}

CJValueP
CQJCanvas::
execNameFn(CJavaScript *js, const std::string &name, const Values &values)
{
  if (name == "getContext") {
    if (values.size() == 2) {
      std::string id = values[1]->toString();

      if (id == "2d")
        return context2D_;
    }

    return CJValueP();
  }
  else
    return CQJObject::execNameFn(js, name, values);
}