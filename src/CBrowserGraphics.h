#ifndef CBrowserGraphics_H
#define CBrowserGraphics_H

#include <CBrowserRenderer.h>
#include <CBrowserTypes.h>
#include <CImageLib.h>
#include <CFont.h>
#include <CRGBA.h>
#include <CIBBox2D.h>

class CPrint;

// class to draw graphics elements using current renderer
class CBrowserGraphics {
 public:
  explicit CBrowserGraphics(CBrowserWindowWidgetIFace *w);

 ~CBrowserGraphics();

  CBrowserGraphics(const CBrowserGraphics &) = delete;
  CBrowserGraphics &operator=(const CBrowserGraphics &) = delete;

  //---

  CBrowserWindowWidgetIFace *widget() const { return w_; }

  CBrowserRenderer *renderer() const { return renderer_; }

  const CRGBA &getBg() const { return bg_; }
  const CRGBA &getFg() const { return fg_; }

  void startDoubleBuffer(int width, int height);
  void endDoubleBuffer();

  QPixmap *pixmap() const { return renderer_->pixmap(); }

  void setXDevice();
  void setPSDevice(double xmin, double ymin, double xmax, double ymax);

  void setRenderer(CBrowserRenderer *renderer);

  void clear(const CRGBA &bg);

  void drawImage(int x, int y, const CImagePtr &image);
  void drawImage(int x, int y, const QImage &image);

  void drawTiledImage(int x, int y, int width, int height, const CImagePtr &image);

  void drawRectangle(int x, int y, int w, int h, const CPen &pen);
  void fillRectangle(int x, int y, int w, int h, const CBrush &brush);

  void fillPolygon(const std::vector<CIPoint2D> &points, const CBrush &brush);

  void drawCircle(int x, int y, int r, const CPen &pen);
  void fillCircle(int x, int y, int r, const CBrush &brush);

  void drawLine(int x1, int y1, int x2, int y2, const CPen &pen);

  void drawText(int x, int y, const std::string &str, const CPen &pen, const CFontPtr &font);

  void drawOutline(int x, int y, int width, int height, const CPen &color);

  void drawBorder(int x, int y, int width, int height, const CPen &pen, CBrowserBorderType type);

  void drawHRule(int x1, int x2, int y, int height, const CPen &pen);

 private:
  void setDeviceType(const CBrowserDeviceType &deviceType);

  bool readFontMetrics();

 private:
  CBrowserWindowWidgetIFace* w_        { nullptr };
  CBrowserRenderer*          renderer_ { nullptr };
  CRGBA                      bg_       { 1.0, 1.0, 1.0 };
  CRGBA                      fg_       { 0.0, 0.0, 0.0 };
};

#endif
