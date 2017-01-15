#include <CBrowserRenderer.h>
#include <CBrowserWindowWidget.h>
#include <CQUtil.h>
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
  painter_->setPen(CQUtil::toQPen(pen));

  painter_->drawRect(CQUtil::toQRect(bbox));
}

void
CBrowserRenderer::
fillRectangle(const CIBBox2D &bbox, const CBrush &brush)
{
  painter_->fillRect(CQUtil::toQRect(bbox), CQUtil::toQBrush(brush));
}

void
CBrowserRenderer::
drawCircle(const CIPoint2D &c, int r, const CPen &pen)
{
  painter_->setPen(CQUtil::toQPen(pen));
  painter_->setBrush(Qt::NoBrush);

  painter_->drawEllipse(QRect(c.x - r, c.y - r, 2*r, 2*r));
}

void
CBrowserRenderer::
fillCircle(const CIPoint2D &c, int r, const CBrush &brush)
{
  painter_->setPen(Qt::NoPen);

  painter_->setBrush(CQUtil::toQBrush(brush));

  painter_->drawEllipse(QRect(c.x - r, c.y - r, 2*r, 2*r));
}

void
CBrowserRenderer::
drawLine(const CIPoint2D &p1, const CIPoint2D &p2, const CPen &pen)
{
  painter_->setPen(CQUtil::toQPen(pen));

  painter_->drawLine(CQUtil::toQPoint(p1), CQUtil::toQPoint(p2));
}

void
CBrowserRenderer::
drawText(const CIPoint2D &p, const std::string &str, const CPen &pen, const CFontPtr &font)
{
  painter_->setPen (CQUtil::toQPen(pen));
  painter_->setFont(CQUtil::toQFont(font));

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
  drawImage(p, CQUtil::toQImage(image));
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
