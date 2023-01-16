#include <CBrowserRenderer.h>
#include <CBrowserWindowWidget.h>
#include <CQUtil.h>
#include <CQUtilRGBA.h>
#include <CQUtilGeom.h>
#include <CQPenUtil.h>
#include <CQBrushUtil.h>
#include <CQFontUtil.h>
#include <CQImageUtil.h>
#include <QPainter>

CBrowserRenderer::
CBrowserRenderer(CBrowserWindowWidget *w) :
 w_(w), pixmap_(0), painter_(0)
{
  pixmap_width_  = 0;
  pixmap_height_ = 0;
}

CBrowserRenderer::
~CBrowserRenderer()
{
}

void
CBrowserRenderer::
startDoubleBuffer(int width, int height)
{
  if (width != pixmap_width_ || height != pixmap_height_) {
    pixmap_width_  = width;
    pixmap_height_ = height;

    pixmap_ = new QPixmap(pixmap_width_, pixmap_height_);

    pixmap_->fill(Qt::black);
  }

  if (! painter_)
    painter_ = new QPainter;

  painter_->begin(pixmap_);
}

void
CBrowserRenderer::
endDoubleBuffer()
{
  painter_->end();

  QPainter painter(w_);

  painter.drawPixmap(QPoint(0, 0), *pixmap_);
}

void
CBrowserRenderer::
clear(const CRGBA &bg)
{
  painter_->fillRect(QRect(0, 0, pixmap_width_, pixmap_height_),
                     QBrush(CQUtil::rgbaToColor(bg)));
}

void
CBrowserRenderer::
drawRectangle(const CIBBox2D &bbox, const CPen &pen)
{
  painter_->setPen(CQPenUtil::toQPen(pen));
  painter_->setBrush(Qt::NoBrush);

  painter_->drawRect(CQUtil::toQRect(bbox));
}

void
CBrowserRenderer::
fillRectangle(const CIBBox2D &bbox, const CBrush &brush)
{
  painter_->fillRect(CQUtil::toQRect(bbox), CQBrushUtil::toQBrush(brush));
}

void
CBrowserRenderer::
fillPolygon(const std::vector<CIPoint2D> &points, const CBrush &brush)
{
  std::vector<QPoint> qpoints;

  qpoints.resize(points.size());

  for (std::size_t i = 0; i < points.size(); ++i)
    qpoints[i] = QPoint(points[i].x, points[i].y);

  painter_->setPen(QPen(Qt::NoPen));
  painter_->setBrush(CQBrushUtil::toQBrush(brush));

  painter_->drawPolygon(&qpoints[0], qpoints.size());
}

void
CBrowserRenderer::
drawCircle(const CIPoint2D &c, int r, const CPen &pen)
{
  painter_->setPen(CQPenUtil::toQPen(pen));
  painter_->setBrush(Qt::NoBrush);

  painter_->drawEllipse(QRect(c.x - r, c.y - r, 2*r, 2*r));
}

void
CBrowserRenderer::
fillCircle(const CIPoint2D &c, int r, const CBrush &brush)
{
  painter_->setPen(Qt::NoPen);
  painter_->setBrush(CQBrushUtil::toQBrush(brush));

  painter_->drawEllipse(QRect(c.x - r, c.y - r, 2*r, 2*r));
}

void
CBrowserRenderer::
drawLine(const CIPoint2D &p1, const CIPoint2D &p2, const CPen &pen)
{
  painter_->setPen(CQPenUtil::toQPen(pen));

  painter_->drawLine(CQUtil::toQPoint(p1), CQUtil::toQPoint(p2));
}

void
CBrowserRenderer::
drawText(const CIPoint2D &p, const std::string &str, const CPen &pen, const CFontPtr &font)
{
  painter_->setPen (CQPenUtil::toQPen(pen));
  painter_->setFont(CQFontUtil::toQFont(font));

  QPoint qp = CQUtil::toQPoint(p);

  if      (font->isSubscript())
    qp.setY(qp.y() + font->getCharAscent()/2);
  else if (font->isSuperscript())
    qp.setY(qp.y() - font->getCharAscent()/2);

  painter_->drawText(qp, str.c_str());
}

void
CBrowserRenderer::
drawImage(const CIPoint2D &p, const CImagePtr &image)
{
  drawImage(p, CQImageUtil::toQImage(image));
}

void
CBrowserRenderer::
drawImage(const CIPoint2D &p, const QImage &image)
{
  if (painter_)
    painter_->drawImage(CQUtil::toQPoint(p), image);
}

void
CBrowserRenderer::
setFont(CFontPtr font)
{
  font_ = font;
}
