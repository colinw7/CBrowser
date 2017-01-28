#ifndef CBrowserWindowWidget_H
#define CBrowserWindowWidget_H

#include <CBrowserTypes.h>
#include <CJavaScript.h>
#include <CImageLib.h>
#include <CFont.h>
#include <CRGBA.h>
#include <QWidget>

class CBrowserScrolledWindow;

class CBrowserWindowWidget : public QWidget {
 public:
  explicit CBrowserWindowWidget(CBrowserScrolledWindow *window);

  void paintEvent(QPaintEvent *) override;
  void resizeEvent(QResizeEvent *) override;

  void mousePressEvent(QMouseEvent *e) override;
  void mouseMoveEvent(QMouseEvent *e) override;
  void mouseReleaseEvent(QMouseEvent *e) override;

  void wheelEvent(QWheelEvent *event);

  void keyPressEvent(QKeyEvent *e) override;
  void keyReleaseEvent(QKeyEvent *e) override;

  void callEventListener(const std::string &name, const std::string &prop, CJValueP event);

  void startDoubleBuffer();
  void endDoubleBuffer();

  void saveImage(const std::string &filename);

  void setXDevice();
  void setPSDevice(double xmin, double ymin, double xmax, double ymax);

  void clear(const CRGBA &bg);

  void drawImage(int x, int y, const CImagePtr &image);
  void drawImage(int x, int y, const QImage &image);

  void drawTiledImage(int x, int y, int width, int height, const CImagePtr &image);

  void drawRectangle(int x, int y, int w, int h, const CPen &pen);
  void fillRectangle(int x, int y, int w, int h, const CBrush &brush);

  void drawCircle(int x, int y, int r, const CPen &pen);
  void fillCircle(int x, int y, int r, const CBrush &brush);

  void drawLine(int x1, int y1, int x2, int y2, const CPen &pen);

  void drawText(int x, int y, const std::string &str, const CPen &pen, const CFontPtr &font);

  void drawOutline(int x, int y, int width, int height, const CPen &c);

  void drawBorder(int x, int y, int width, int height, CBrowserBorderType type);

  void drawHRule(int x1, int x2, int y, int height);

 private:
  CBrowserScrolledWindow *window_ { nullptr };
  CBrowserGraphics       *graphics_ { nullptr };
};

#endif
