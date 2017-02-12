#ifndef CQJHtmlObj_H
#define CQJHtmlObj_H

#include <CBrowserObject.h>
#include <CQJObject.h>

class CQJHtmlObjType : public CJObjType {
 public:
  static CJObjTypeP instance(CJavaScript *js);

  explicit CQJHtmlObjType(CJavaScript *js);

  CJValueP exec(CJavaScript *, const std::string &, const Values &) override {
    return CJValueP();
  }

 private:
  static CJObjTypeP type_;
};

//------

class CQJHtmlObj : public CQJObject {
  Q_OBJECT

 public:
  CQJHtmlObj(CJavaScript *js, CBrowserObject *htmlObj);
  CQJHtmlObj(CJavaScript *js, CBrowserObject *htmlObj, const CJObjTypeP &type);

  virtual ~CQJHtmlObj() { }

  CJValue *dup(CJavaScript *js) const override { return new CQJHtmlObj(js, htmlObj_); }

  CBrowserObject *htmlObj() const { return htmlObj_; }

  void init() override;

  CJValueP getProperty(CJavaScript *js, const std::string &name) const;

  void setProperty(CJavaScript *js, const std::string &name, CJValueP value);

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values);

  void print(std::ostream &os) const override { os << "html"; }

 protected:
  CBrowserObject *htmlObj_ { 0 };
};

#endif
