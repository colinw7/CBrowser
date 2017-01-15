#ifndef CBrowserRenderer_H
#define CBrowserRenderer_H

#include <CBrowserTypes.h>
#include <CImageLib.h>
#include <CFont.h>
#include <CIBBox2D.h>
#include <CRGBA.h>

class QImage;
class QPixmap;
class QPainter;

class CBrowserRenderer {
 public:
  CBrowserRenderer(CBrowserWindowWidget *w);

  virtual ~CBrowserRenderer();

  virtual void startDoubleBuffer(int width, int height);
  virtual void endDoubleBuffer  ();

  QPixmap *pixmap() const { return pixmap_; }

  virtual void clear(const CRGBA &bg);

  virtual void drawRectangle(const CIBBox2D &bbox, const CPen &pen);
  virtual void fillRectangle(const CIBBox2D &bbox, const CBrush &brush);

  virtual void drawCircle(const CIPoint2D &c, int r, const CPen &pen);
  virtual void fillCircle(const CIPoint2D &c, int r, const CBrush &brush);

  virtual void drawLine(const CIPoint2D &p1, const CIPoint2D &p2, const CPen &pen);
  virtual void drawText(const CIPoint2D &p, const std::string &str, const CPen &pen,
                        const CFontPtr &font);

  virtual void drawImage(const CIPoint2D &p, const CImagePtr &image);
  virtual void drawImage(const CIPoint2D &p, const QImage &image);

  virtual void setFont(CFontPtr font);

 private:
  CBrowserWindowWidget* w_ { nullptr };
  QPixmap*              pixmap_ { nullptr };
  int                   pixmap_width_ { 0 };
  int                   pixmap_height_ { 0 };
  QPainter*             painter_ { nullptr };
  CFontPtr              font_;
};

#endif
