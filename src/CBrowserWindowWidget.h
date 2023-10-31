#ifndef CBrowserWindowWidget_H
#define CBrowserWindowWidget_H

#include <CBrowserWindowWidgetIFace.h>
#include <CBrowserTypes.h>

#include <CJavaScript.h>
#include <CImageLib.h>
#include <CFont.h>
#include <CRGBA.h>
#include <QWidget>

class CBrowserWindowIFace;

class CBrowserWindowWidget : public QWidget, public CBrowserWindowWidgetIFace {
  Q_OBJECT

 public:
  explicit CBrowserWindowWidget(CBrowserWindowIFace *window);

  CBrowserWindowIFace *window() const { return window_; }
  void setWindow(CBrowserWindowIFace *window) { window_ = window; }

  int windowWidth () override { return QWidget::width (); }
  int windowHeight() override { return QWidget::height(); }

  void paintEvent(QPaintEvent *) override;
  void resizeEvent(QResizeEvent *) override;

  void mousePressEvent(QMouseEvent *e) override;
  void mouseMoveEvent(QMouseEvent *e) override;
  void mouseReleaseEvent(QMouseEvent *e) override;

  void wheelEvent(QWheelEvent *event) override;

  void keyPressEvent(QKeyEvent *e) override;
  void keyReleaseEvent(QKeyEvent *e) override;

  void callEventListener(const std::string &name, const std::string &prop, CJValueP event);

  void updateWidget(bool sync) override;

  void drawPixmap(QPixmap *pixmap) override;

 private:
  CBrowserWindowIFace* window_ { nullptr };
};

#endif
