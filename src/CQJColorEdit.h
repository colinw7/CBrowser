#ifndef CQJColorEdit_H
#define CQJColorEdit_H

#include <CBrowserTypes.h>
#include <CJavaScript.h>
#include <QLineEdit>

class CQJColorEdit : public QLineEdit {
  Q_OBJECT

  Q_PROPERTY(QColor color READ color WRITE setColor)

 public:
  CQJColorEdit(CBrowserWindowWidget *w, CBrowserFormInput *input);

  void setText(const QString &text);

  const QColor &color() const { return color_; }
  void setColor(const QColor &v);

 private:
  void updateState();

  void emitChange();

  void keyPressEvent  (QKeyEvent *e);
  void keyReleaseEvent(QKeyEvent *e);

  void callEventListener(const std::string &name, const std::string &prop, CJValueP event);

 private slots:
  void focusChanged(QWidget *, QWidget *);
  void changeSlot(const QString &);

 private:
  CBrowserWindowWidget *w_           { nullptr };
  CBrowserFormInput    *input_       { nullptr };
  QColor                color_;
  CQJColorWidget       *colorWidget_ { nullptr };
};

//------

class CQJColorWidget : public QFrame {
 public:
  explicit CQJColorWidget(CQJColorEdit *edit);

  void paintEvent(QPaintEvent *);

  void mousePressEvent(QMouseEvent *e);
  void mouseMoveEvent(QMouseEvent *);
  void mouseReleaseEvent(QMouseEvent *);

  void updateColorFromPos(const QPoint &pos);

 private:
  CQJColorEdit *edit_;
  QRect         palRect_;
  QRect         valueRect_;
};

#endif
