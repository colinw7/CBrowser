#ifndef CQJInput_H
#define CQJInput_H

#include <CQJHtmlObj.h>
#include <CQJObject.h>

class CQJInputType : public CJObjType {
 public:
  static CJObjTypeP instance(CJavaScript *js);

  CQJInputType(CJavaScript *js);

  CJValueP exec(CJavaScript *, const std::string &, const Values &) override {
    return CJValueP();
  }

 private:
  static CJObjTypeP type_;
};

//------

class CQJInput : public CQJHtmlObj {
  Q_OBJECT

 public:
  CQJInput(CJavaScript *js, CBrowserFormInput *input);

  CJValue *dup(CJavaScript *js) const override { return new CQJInput(js, input_); }

  CBrowserFormInput *input() const { return input_; }

  CJValueP getProperty(CJavaScript *js, const std::string &name) const;

  void setProperty(CJavaScript *js, const std::string &name, CJValueP value);

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values);

  void print(std::ostream &os) const override { os << "input"; }

 protected:
  CBrowserFormInput *input_ { nullptr };
};

#endif
