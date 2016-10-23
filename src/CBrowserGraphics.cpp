#include <CBrowserHtmlI.h>
#include <CQUtil.h>
#include <CFontMgr.h>

#include <QImage>
#include <QPainter>

#define COURIER_N    "Courier"
#define COURIER_B    "Courier-Bold"
#define COURIER_I    "Courier-Oblique"
#define COURIER_BI   "Courier-BoldOblique"
#define HELVETICA_N  "Helvetica"
#define HELVETICA_B  "Helvetica-Bold"
#define HELVETICA_I  "Helvetica-Oblique"
#define HELVETICA_BI "Helvetica-BoldOblique"
#define TIMES_N      "Times-Roman"
#define TIMES_B      "Times-Bold"
#define TIMES_I      "Times-Italic"
#define TIMES_BI     "Times-BoldItalic"

#define ALPHABET_UPPER_LOWER \
  "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"

CBrowserGraphics::
CBrowserGraphics(CBrowserCanvas *canvas) :
 canvas_(canvas)
{
  current_device_ = CBROWSER_DEVICE_TYPE_X;
  print_device_   = NULL;

  //-----

  renderer_ = new CBrowserRenderer(canvas_);
}

CBrowserGraphics::
~CBrowserGraphics()
{
}

void
CBrowserGraphics::
startDoubleBuffer(int width, int height)
{
  renderer_->startDoubleBuffer(width, height);
}

void
CBrowserGraphics::
endDoubleBuffer()
{
  renderer_->endDoubleBuffer();
}

void
CBrowserGraphics::
setXDevice()
{
  current_device_ = CBROWSER_DEVICE_TYPE_X;

  if (print_device_ != NULL) {
    print_device_->term();

    delete print_device_;

    print_device_ = NULL;
  }
}

void
CBrowserGraphics::
setPSDevice(double xmin, double ymin, double xmax, double ymax)
{
  current_device_ = CBROWSER_DEVICE_TYPE_PS;
  print_device_   = new CPrint();

  print_device_->setFilename("/tmp/ps.out");

  print_device_->setSize(xmin, ymin, xmax, ymax);

  print_device_->init();
}

void
CBrowserGraphics::
clear(const CRGBA &bg)
{
  int width  = canvas_->width ();
  int height = canvas_->height();

  if      (current_device_ == CBROWSER_DEVICE_TYPE_X)
    renderer_->clear(bg);
  else if (current_device_ == CBROWSER_DEVICE_TYPE_PS) {
    print_device_->setForeground(bg);

    print_device_->fillRectangle(0, 0, width, height);
  }
}

void
CBrowserGraphics::
drawImage(int x, int y, const CImagePtr &image)
{
  if      (current_device_ == CBROWSER_DEVICE_TYPE_X)
    renderer_->drawImage(CIPoint2D(x, y), image);
  else if (current_device_ == CBROWSER_DEVICE_TYPE_PS)
    print_device_->drawImage(image, x, y);
}

void
CBrowserGraphics::
drawTiledImage(int x, int y, int width, int height, const CImagePtr &image)
{
  for (int y1 = y; y1 < height; y1 += image->getHeight()) {
    for (int x1 = x; x1 < width; x1 += image->getWidth()) {
      drawImage(x1, y1, image);
    }
  }
}

void
CBrowserGraphics::
drawRectangle(int x, int y, int width, int height)
{
  if      (current_device_ == CBROWSER_DEVICE_TYPE_X)
    renderer_->drawRectangle(CIBBox2D(CIPoint2D(x, y), CISize2D(width, height)));
  else if (current_device_ == CBROWSER_DEVICE_TYPE_PS)
    print_device_->drawRectangle(x, y, x + width, y + height);
}

void
CBrowserGraphics::
fillRectangle(int x, int y, int width, int height)
{
  if      (current_device_ == CBROWSER_DEVICE_TYPE_X)
    renderer_->fillRectangle(CIBBox2D(CIPoint2D(x, y), CISize2D(width, height)));
  else if (current_device_ == CBROWSER_DEVICE_TYPE_PS)
    print_device_->fillRectangle(x, y, x + width, y + height);
}

void
CBrowserGraphics::
drawCircle(int x, int y, int radius)
{
  if      (current_device_ == CBROWSER_DEVICE_TYPE_X)
    renderer_->drawCircle(CIPoint2D(x, y), radius);
  else if (current_device_ == CBROWSER_DEVICE_TYPE_PS)
    print_device_->drawCircle(x, y, radius);
}

void
CBrowserGraphics::
fillCircle(int x, int y, int radius)
{
  if      (current_device_ == CBROWSER_DEVICE_TYPE_X)
    renderer_->fillCircle(CIPoint2D(x, y), radius);
  else if (current_device_ == CBROWSER_DEVICE_TYPE_PS)
    print_device_->fillCircle(x, y, radius);
}

void
CBrowserGraphics::
drawLine(int x1, int y1, int x2, int y2)
{
  if      (current_device_ == CBROWSER_DEVICE_TYPE_X)
    renderer_->drawLine(CIPoint2D(x1, y1), CIPoint2D(x2, y2));
  else if (current_device_ == CBROWSER_DEVICE_TYPE_PS)
    print_device_->drawLine(x1, y1, x2, y2);
}

void
CBrowserGraphics::
drawString(int x, int y, const std::string &text)
{
  if      (current_device_ == CBROWSER_DEVICE_TYPE_X)
    renderer_->drawString(CIPoint2D(x, y), text);
  else if (current_device_ == CBROWSER_DEVICE_TYPE_PS)
    print_device_->drawText(x, y, text);
}

void
CBrowserGraphics::
drawOutline(int x, int y, int width, int height, const CRGBA &color)
{
  setForeground(color);

  drawLine(x + width, y         , x + width, y + height);
  drawLine(x + width, y + height, x        , y + height);
  drawLine(x        , y         , x + width, y         );
  drawLine(x        , y + height, x        , y         );
}

void
CBrowserGraphics::
drawBorder(int x, int y, int width, int height, const CRGBA &color, CBrowserBorderType type)
{
  CRGBA color_top    = color.getLightRGBA();
  CRGBA color_bottom = color.getDarkRGBA();

  if (type == CBROWSER_BORDER_TYPE_IN)
    setForeground(color_top);
  else
    setForeground(color_bottom);

  drawLine(x + width - 1, y             , x + width - 1, y + height - 1);
  drawLine(x + width - 1, y + height - 1, x            , y + height - 1);

  if (type == CBROWSER_BORDER_TYPE_IN)
    setForeground(color_bottom);
  else
    setForeground(color_top);

  drawLine(x, y             , x + width - 1, y);
  drawLine(x, y + height - 1, x            , y);
}

void
CBrowserGraphics::
drawHRule(int x1, int x2, int y, int height, const CRGBA &color)
{
  CRGBA color_top    = color.getLightRGBA();
  CRGBA color_bottom = color.getDarkRGBA();

  /*---------*/

  setForeground(color_bottom);

  drawLine(x1, y, x1, y + height - 1);

  setForeground(color_top);

  drawLine(x2, y, x2, y + height - 1);

  /*---------*/

  x1++;
  x2--;

  setForeground(color_bottom);

  drawLine(x1, y, x2, y);

  for (int i = 0; i < height - 2; i++) {
    y++;

    setForeground(color);

    drawLine(x1, y, x2, y);
  }

  y++;

  setForeground(color_top);

  drawLine(x1, y, x2, y);
}

void
CBrowserGraphics::
setForeground(const CRGBA &fg)
{
  if      (current_device_ == CBROWSER_DEVICE_TYPE_X)
    renderer_->setForeground(fg);
  else if (current_device_ == CBROWSER_DEVICE_TYPE_PS)
    print_device_->setForeground(fg.getRGB());
}

void
CBrowserGraphics::
setFont(CFontPtr font)
{
  if      (current_device_ == CBROWSER_DEVICE_TYPE_X)
    renderer_->setFont(font);
  else if (current_device_ == CBROWSER_DEVICE_TYPE_PS)
    print_device_->setFont(font);
}

//------------

CBrowserRenderer::
CBrowserRenderer(CBrowserCanvas *canvas) :
 canvas_(canvas), pixmap_(0), painter_(0)
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

  QPainter painter(canvas_);

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
drawRectangle(const CIBBox2D &bbox)
{
  painter_->setPen(CQUtil::rgbaToColor(fg_));

  painter_->drawRect(CQUtil::toQRect(bbox));
}

void
CBrowserRenderer::
fillRectangle(const CIBBox2D &bbox)
{
  painter_->fillRect(CQUtil::toQRect(bbox), QBrush(CQUtil::rgbaToColor(fg_)));
}

void
CBrowserRenderer::
drawCircle(const CIPoint2D &c, int r)
{
  painter_->setPen(CQUtil::rgbaToColor(fg_));
  painter_->setBrush(Qt::NoBrush);

  painter_->drawEllipse(QRect(c.x - r, c.y - r, 2*r, 2*r));
}

void
CBrowserRenderer::
fillCircle(const CIPoint2D &c, int r)
{
  painter_->setPen(Qt::NoPen);
  painter_->setBrush(CQUtil::rgbaToColor(fg_));

  painter_->drawEllipse(QRect(c.x - r, c.y - r, 2*r, 2*r));
}

void
CBrowserRenderer::
drawLine(const CIPoint2D &p1, const CIPoint2D &p2)
{
  painter_->setPen(CQUtil::rgbaToColor(fg_));

  painter_->drawLine(CQUtil::toQPoint(p1), CQUtil::toQPoint(p2));
}

void
CBrowserRenderer::
drawString(const CIPoint2D &p, const std::string &str)
{
  painter_->setPen (CQUtil::rgbaToColor(fg_));
  painter_->setFont(CQUtil::toQFont(font_));

  painter_->drawText(CQUtil::toQPoint(p), str.c_str());
}

void
CBrowserRenderer::
drawImage(const CIPoint2D &p, const CImagePtr &image)
{
  painter_->drawImage(CQUtil::toQPoint(p), CQUtil::toQImage(image));
}

void
CBrowserRenderer::
setForeground(const CRGBA &fg)
{
  fg_ = fg;
}

void
CBrowserRenderer::
setFont(CFontPtr font)
{
  font_ = font;
}
