#ifndef CBrowserWindowWidget_H
#define CBrowserWindowWidget_H

#include <CBrowserTypes.h>
#include <CJavaScript.h>
#include <CImageLib.h>
#include <CFont.h>
#include <CRGBA.h>
#include <QWidget>

class CBrowserWindowWidget : public QWidget {
 public:
  CBrowserWindowWidget(CBrowserIFace *iface);

  void paintEvent(QPaintEvent *);
  void resizeEvent(QResizeEvent *);

  void mousePressEvent(QMouseEvent *e);
  void mouseMotionEvent(QMouseEvent *e);
  void mouseReleaseEvent(QMouseEvent *e);

  void keyPressEvent(QKeyEvent *e);
  void keyReleaseEvent(QKeyEvent *e);

  void callEventListener(const std::string &name, const std::string &prop, CJValueP event);

  void startDoubleBuffer();
  void endDoubleBuffer();

  void saveImage(const std::string &filename);

  void setXDevice();
  void setPSDevice(double xmin, double ymin, double xmax, double ymax);

  void clear(const CRGBA &bg);

  void drawImage(int x, int y, const CImagePtr &image);
  void drawImage(int x, int y, const QImage &image);

  void drawRectangle(int x1, int y1, int x2, int y2);
  void fillRectangle(int x1, int y1, int x2, int y2);

  void drawCircle(int x, int y, int r);
  void fillCircle(int x, int y, int r);

  void drawLine(int x1, int y1, int x2, int y2);
  void drawString(int x, int y, const std::string &str);
  void drawOutline(int x, int y, int width, int height, const std::string &color_name);
  void drawOutline(int x, int y, int width, int height, const CRGBA &c);
  void drawBorder(int x, int y, int width, int height, CBrowserBorderType type);
  void drawHRule(int x1, int x2, int y, int height);

  void setForeground(const CRGBA &fg);
  void setFont(CFontPtr font);

 private:
  CBrowserIFace    *iface_ { nullptr };
  CBrowserGraphics *graphics_ { nullptr };
};

#endif
