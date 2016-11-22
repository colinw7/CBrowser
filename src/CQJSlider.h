#ifndef CQJSlider_H
#define CQJSlider_H

#include <CBrowserTypes.h>
#include <CJavaScript.h>
#include <QSlider>

class CQJSlider : public QSlider {
  Q_OBJECT

 public:
  CQJSlider(CBrowserWindowWidget *w, CBrowserFormInput *input);

  void setRange(double min, double max);
  void setSingleStep(double step);

  double value() const;
  void setValue(double v);

 private:
  void callEventListener(const std::string &name, const std::string &prop, CJValueP event);

 private slots:
  void changeSlot();

 private:
  CBrowserWindowWidget *w_ { nullptr };
  CBrowserFormInput    *input_ { nullptr };

 private:
  double min_ { 0.0 };
  double max_ { 100.0 };
  double step_ { 0.0 };
};

#endif
