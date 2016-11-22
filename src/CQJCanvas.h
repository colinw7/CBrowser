#ifndef CQJCanvas_H
#define CQJCanvas_H

#include <CQJHtmlObj.h>

class CQJCanvasType : public CJObjType {
 public:
  static CJObjTypeP instance(CJavaScript *js);

  CQJCanvasType(CJavaScript *js);

  CJValueP exec(CJavaScript *, const std::string &, const Values &) override;

 private:
  static CJObjTypeP type_;
};

//------

class CQJCanvas : public CQJHtmlObj {
  Q_OBJECT

 public:
  CQJCanvas(CJavaScript *js, CBrowserCanvas *canvas);

  CJValue *dup(CJavaScript *js) const override { return new CQJCanvas(js, canvas_); }

  CBrowserCanvas *canvas() const { return canvas_; }

  void updateSize();

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values) override;

  void print(std::ostream &os) const override { os << "canvas"; }

 private:
  CBrowserCanvas* canvas_ { nullptr };
  CJValueP        context2D_;
};

#endif
