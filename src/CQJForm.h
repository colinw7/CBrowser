#ifndef CQJForm_H
#define CQJForm_H

#include <CQJHtmlObj.h>
#include <CQJObject.h>

class CQJFormType : public CJObjType {
 public:
  static CJObjTypeP instance(CJavaScript *js);

  CQJFormType(CJavaScript *js);

  CJValueP exec(CJavaScript *, const std::string &, const Values &) override {
    return CJValueP();
  }

 private:
  static CJObjTypeP type_;
};

//------

class CQJForm : public CQJHtmlObj {
  Q_OBJECT

 public:
  CQJForm(CJavaScript *js, CBrowserForm *form);

  CJValue *dup(CJavaScript *js) const override { return new CQJForm(js, form_); }

  CBrowserForm *form() const { return form_; }

  CJValueP getProperty(CJavaScript *js, const std::string &name) const;

  void setProperty(CJavaScript *js, const std::string &name, CJValueP value);

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values);

  void print(std::ostream &os) const override { os << "form"; }

 protected:
  CBrowserForm *form_ { nullptr };
};

#endif
