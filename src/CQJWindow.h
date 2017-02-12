#ifndef CQJWindow_H
#define CQJWindow_H

#include <CQJObject.h>

class CQJWindowType : public CJObjType {
 public:
  static CJObjTypeP instance(CJavaScript *js);

  explicit CQJWindowType(CJavaScript *js);

  CJValueP exec(CJavaScript *, const std::string &, const Values &) override {
    return CJValueP();
  }

 private:
  static CJObjTypeP type_;
};

//------

class CQJWindow;
class CQJWindowTimer;
class CBrowserWindow;

typedef std::shared_ptr<CQJWindow> CQJWindowP;

//------

class CQJWindow : public CQJObject {
  Q_OBJECT

 public:
  explicit CQJWindow(CJavaScript *js);

  void init() override;

  CJValue *dup(CJavaScript *js) const override { return new CQJWindow(js); }

  long addTimer(CJFunctionBaseP timerFn, double t);

  void removeTimer(long id);

  void addOneShotTimer(CJFunctionBaseP timerFn, double t);

  CJValueP getProperty(CJavaScript *js, const std::string &name) const;

  void setProperty(CJavaScript *js, const std::string &name, CJValueP value);

  CJValueP execNameFn(CJavaScript *js, const std::string &name, const Values &values);

  void print(std::ostream &os) const override { os << "window"; }

  CBrowserWindow *getWindow() const;

 private:
  typedef std::vector<CQJWindowTimer *> Timers;

  CQJWindowTimer *timer_ { 0 };
  Timers          timers_;
};

#endif
