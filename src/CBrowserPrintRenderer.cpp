#include <CBrowserPrintRenderer.h>

#include <CPrint.h>

CBrowserPrintRenderer::
CBrowserPrintRenderer(CBrowserWindowWidgetIFace *iface) :
 CBrowserRenderer(iface)
{
  print_ = new CPrint;

  print_->setFilename("/tmp/ps.out");
}

CBrowserPrintRenderer::
~CBrowserPrintRenderer()
{
  delete print_;
}

void
CBrowserPrintRenderer::
init()
{
}

void
CBrowserPrintRenderer::
term()
{
  print_->term();
}

void
CBrowserPrintRenderer::
setSize(int w, int h)
{
  w_ = w;
  h_ = h;
}

void
CBrowserPrintRenderer::
setRange(double xmin, double ymin, double xmax, double ymax)
{
  print_->setSize(xmin, ymin, xmax, ymax);

  print_->init();
}

void
CBrowserPrintRenderer::
clear(const CRGBA &bg)
{
  print_->setForeground(bg);

  print_->fillRectangle(0, 0, w_, h_);
}

void
CBrowserPrintRenderer::
drawRectangle(const CIBBox2D &bbox, const CPen &pen)
{
  print_->setForeground(pen.getColor());

  print_->drawRectangle(bbox.getXMin(), bbox.getYMin(), bbox.getXMax(), bbox.getYMax());
}

void
CBrowserPrintRenderer::
fillRectangle(const CIBBox2D &bbox, const CBrush &brush)
{
  print_->setForeground(brush.getColor());

  print_->fillRectangle(bbox.getXMin(), bbox.getYMin(), bbox.getXMax(), bbox.getYMax());
}

void
CBrowserPrintRenderer::
fillPolygon(const std::vector<CIPoint2D> &points, const CBrush &brush)
{
  print_->setForeground(brush.getColor());

  std::vector<double> x, y;

  x.resize(points.size());
  y.resize(points.size());

  for (std::size_t i = 0; i < points.size(); ++i) {
    x[i] = points[i].x;
    y[i] = points[i].y;
  }

  print_->fillPolygon(&x[0], &y[0], x.size());
}

void
CBrowserPrintRenderer::
drawCircle(const CIPoint2D &c, int r, const CPen &pen)
{
  print_->setForeground(pen.getColor());

  print_->drawCircle(c.x, c.y, r);
}

void
CBrowserPrintRenderer::
fillCircle(const CIPoint2D &c, int r, const CBrush &brush)
{
  print_->setForeground(brush.getColor());

  print_->fillCircle(c.x, c.y, r);
}

void
CBrowserPrintRenderer::
drawLine(const CIPoint2D &p1, const CIPoint2D &p2, const CPen &pen)
{
  print_->setForeground(pen.getColor());

  print_->drawLine(p1.x, p1.y, p2.x, p2.y);
}

void
CBrowserPrintRenderer::
drawText(const CIPoint2D &p, const std::string &str, const CPen &pen, const CFontPtr &font)
{
  print_->setForeground(pen.getColor());
  print_->setFont(font);

  print_->drawText(p.x, p.y, str);
}

void
CBrowserPrintRenderer::
drawImage(const CIPoint2D &p, const CImagePtr &image)
{
  print_->drawImage(image, p.x, p.y);
}

void
CBrowserPrintRenderer::
drawImage(const CIPoint2D & /*p*/, const QImage & /*image*/)
{
//print_->drawImage(image, p.x, p.y);
}
