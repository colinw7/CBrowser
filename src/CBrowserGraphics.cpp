#include <CBrowserGraphics.h>
#include <CBrowserWindowWidget.h>
#include <CBrowserScrolledWindow.h>
#include <CBrowserMainWindow.h>
#include <CBrowserMain.h>
#include <CBrowserPrintRenderer.h>

#include <CFontMgr.h>
#include <CPrint.h>

#include <QImage>
#include <QPainter>

CBrowserGraphics::
CBrowserGraphics(CBrowserWindowWidgetIFace *w) :
 w_(w)
{
  setDeviceType(CBrowserDeviceType::X);
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
  setDeviceType(CBrowserDeviceType::X);
}

void
CBrowserGraphics::
setPSDevice(double xmin, double ymin, double xmax, double ymax)
{
  setDeviceType(CBrowserDeviceType::PS);

  auto *printRenderer = dynamic_cast<CBrowserPrintRenderer *>(renderer_);

  printRenderer->setRange(xmin, ymin, xmax, ymax);
}

void
CBrowserGraphics::
setDeviceType(const CBrowserDeviceType &deviceType)
{
  CBrowserRenderer *renderer;

  if (deviceType == CBrowserDeviceType::X)
    renderer = new CBrowserRenderer(w_);
  else
    renderer = new CBrowserPrintRenderer(w_);

  setRenderer(renderer);
}

void
CBrowserGraphics::
setRenderer(CBrowserRenderer *renderer)
{
  if (renderer_)
    renderer_->term();

  delete renderer_;

  renderer_ = renderer;

  renderer_->init();
}

void
CBrowserGraphics::
clear(const CRGBA &bg)
{
  int width  = w_->windowWidth ();
  int height = w_->windowHeight();

  renderer_->setSize(width, height);

  renderer_->clear(bg);
}

void
CBrowserGraphics::
drawImage(int x, int y, const CImagePtr &image)
{
  renderer_->drawImage(CIPoint2D(x, y), image);
}

void
CBrowserGraphics::
drawImage(int x, int y, const QImage &image)
{
  renderer_->drawImage(CIPoint2D(x, y), image);
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
drawRectangle(int x, int y, int width, int height, const CPen &pen)
{
  renderer_->drawRectangle(CIBBox2D(CIPoint2D(x, y), CISize2D(width, height)), pen);
}

void
CBrowserGraphics::
fillRectangle(int x, int y, int width, int height, const CBrush &brush)
{
  renderer_->fillRectangle(CIBBox2D(CIPoint2D(x, y), CISize2D(width, height)), brush);
}

void
CBrowserGraphics::
fillPolygon(const std::vector<CIPoint2D> &points, const CBrush &brush)
{
  renderer_->fillPolygon(points, brush);
}

void
CBrowserGraphics::
drawCircle(int x, int y, int radius, const CPen &pen)
{
  renderer_->drawCircle(CIPoint2D(x, y), radius, pen);
}

void
CBrowserGraphics::
fillCircle(int x, int y, int radius, const CBrush &brush)
{
  renderer_->fillCircle(CIPoint2D(x, y), radius, brush);
}

void
CBrowserGraphics::
drawLine(int x1, int y1, int x2, int y2, const CPen &pen)
{
  renderer_->drawLine(CIPoint2D(x1, y1), CIPoint2D(x2, y2), pen);
}

void
CBrowserGraphics::
drawText(int x, int y, const std::string &text, const CPen &pen, const CFontPtr &font)
{
  renderer_->drawText(CIPoint2D(x, y), text, pen, font);
}

void
CBrowserGraphics::
drawOutline(int x, int y, int width, int height, const CPen &pen)
{
  drawLine(x + width, y         , x + width, y + height, pen);
  drawLine(x + width, y + height, x        , y + height, pen);
  drawLine(x        , y         , x + width, y         , pen);
  drawLine(x        , y + height, x        , y         , pen);
}

void
CBrowserGraphics::
drawBorder(int x, int y, int width, int height, const CPen &pen, CBrowserBorderType type)
{
  CRGBA color = pen.getColor();

  CRGBA color_top    = color.getLightRGBA();
  CRGBA color_bottom = color.getDarkRGBA();

  CPen pen1;

  if (type == CBrowserBorderType::IN)
    pen1 = CPen(color_top);
  else
    pen1 = CPen(color_bottom);

  drawLine(x + width - 1, y             , x + width - 1, y + height - 1, pen1);
  drawLine(x + width - 1, y + height - 1, x            , y + height - 1, pen1);

  if (type == CBrowserBorderType::IN)
    pen1 = CPen(color_bottom);
  else
    pen1 = CPen(color_top);

  drawLine(x, y             , x + width - 1, y, pen1);
  drawLine(x, y + height - 1, x            , y, pen1);
}

void
CBrowserGraphics::
drawHRule(int x1, int x2, int y, int height, const CPen &pen)
{
  CRGBA color = pen.getColor();

  CRGBA color_top    = color.getLightRGBA();
  CRGBA color_bottom = color.getDarkRGBA();

  /*---------*/

  CPen pen1;

  pen1 = CPen(color_bottom);

  drawLine(x1, y, x1, y + height - 1, pen1);

  pen1 = CPen(color_top);

  drawLine(x2, y, x2, y + height - 1, pen1);

  /*---------*/

  x1++;
  x2--;

  pen1 = CPen(color_bottom);

  drawLine(x1, y, x2, y, pen1);

  for (int i = 0; i < height - 2; i++) {
    y++;

    pen1 = CPen(color);

    drawLine(x1, y, x2, y, pen1);
  }

  y++;

  pen1 = CPen(color_top);

  drawLine(x1, y, x2, y, pen1);
}
