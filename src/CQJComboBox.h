#ifndef CQJComboBox_H
#define CQJComboBox_H

#include <CBrowserTypes.h>
#include <CJavaScript.h>
#include <QComboBox>

class CQJComboBox : public QComboBox {
  Q_OBJECT

 public:
  CQJComboBox(CBrowserWindowWidget *w, CBrowserFormInput *input);

 private slots:
  void currentIndexSlot(int);

 private:
  void callEventListener(const std::string &name, const std::string &prop, CJValueP event);

 private:
  CBrowserWindowWidget *w_ { nullptr };
  CBrowserFormInput    *input_ { nullptr };
};

#endif
