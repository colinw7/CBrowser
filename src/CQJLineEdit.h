#ifndef CQJLineEdit_H
#define CQJLineEdit_H

#include <CBrowserTypes.h>
#include <CJavaScript.h>
#include <QLineEdit>

class CQJLineEdit : public QLineEdit {
  Q_OBJECT

 public:
  CQJLineEdit(CBrowserWindowWidget *w, CBrowserFormInput *input);

 private:
  void keyPressEvent(QKeyEvent *e);
  void keyReleaseEvent(QKeyEvent *e);

  void callEventListener(const std::string &name, const std::string &prop, CJValueP event);

 private slots:
  void changeSlot();

 private:
  CBrowserWindowWidget *w_ { nullptr };
  CBrowserFormInput    *input_ { nullptr };
};

#endif
