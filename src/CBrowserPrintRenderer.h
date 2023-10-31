#ifndef CBrowserPrintRenderer_H
#define CBrowserPrintRenderer_H

#include <CBrowserRenderer.h>

class CPrint;

class CBrowserPrintRenderer : public CBrowserRenderer {
 public:
  explicit CBrowserPrintRenderer(CBrowserWindowWidgetIFace *iface);

 ~CBrowserPrintRenderer() override;

  void init() override;
  void term() override;

  void setSize(int w, int h) override;
  void setRange(double xmin, double ymin, double xmax, double ymax) override;

  void clear(const CRGBA &bg) override;

  void drawRectangle(const CIBBox2D &bbox, const CPen &pen) override;
  void fillRectangle(const CIBBox2D &bbox, const CBrush &brush) override;

  void fillPolygon(const std::vector<CIPoint2D> &points, const CBrush &brush) override;

  void drawCircle(const CIPoint2D &c, int r, const CPen &pen) override;
  void fillCircle(const CIPoint2D &c, int r, const CBrush &brush) override;

  void drawLine(const CIPoint2D &p1, const CIPoint2D &p2, const CPen &pen) override;

  void drawText(const CIPoint2D &p, const std::string &str, const CPen &pen,
                const CFontPtr &font) override;

  void drawImage(const CIPoint2D &p, const CImagePtr &image) override;
  void drawImage(const CIPoint2D &p, const QImage &image) override;

 private:
  CPrint* print_ { nullptr };
  int     w_     { 100 };
  int     h_     { 100 };
};

#endif
