#include <CBrowserGraphics.h>
#include <CBrowserIFace.h>
#include <CBrowserWindowWidget.h>
#include <CFontMgr.h>
#include <CPrint.h>

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
CBrowserGraphics(CBrowserWindowWidget *w) :
 w_(w)
{
  current_device_ = CBrowserDeviceType::X;
  print_device_   = nullptr;

  //-----

  renderer_ = new CBrowserRenderer(w_);
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
  current_device_ = CBrowserDeviceType::X;

  if (print_device_) {
    print_device_->term();

    delete print_device_;

    print_device_ = nullptr;
  }
}

void
CBrowserGraphics::
setPSDevice(double xmin, double ymin, double xmax, double ymax)
{
  current_device_ = CBrowserDeviceType::PS;
  print_device_   = new CPrint();

  print_device_->setFilename("/tmp/ps.out");

  print_device_->setSize(xmin, ymin, xmax, ymax);

  print_device_->init();
}

void
CBrowserGraphics::
clear(const CRGBA &bg)
{
  int width  = w_->width ();
  int height = w_->height();

  if      (current_device_ == CBrowserDeviceType::X)
    renderer_->clear(bg);
  else if (current_device_ == CBrowserDeviceType::PS) {
    print_device_->setForeground(bg);

    print_device_->fillRectangle(0, 0, width, height);
  }
}

void
CBrowserGraphics::
drawImage(int x, int y, const CImagePtr &image)
{
  if      (current_device_ == CBrowserDeviceType::X)
    renderer_->drawImage(CIPoint2D(x, y), image);
  else if (current_device_ == CBrowserDeviceType::PS)
    print_device_->drawImage(image, x, y);
}

void
CBrowserGraphics::
drawImage(int x, int y, const QImage &image)
{
  if      (current_device_ == CBrowserDeviceType::X)
    renderer_->drawImage(CIPoint2D(x, y), image);
#if 0
  else if (current_device_ == CBrowserDeviceType::PS)
    print_device_->drawImage(image, x, y);
#endif
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
  if      (current_device_ == CBrowserDeviceType::X)
    renderer_->drawRectangle(CIBBox2D(CIPoint2D(x, y), CISize2D(width, height)), pen);
  else if (current_device_ == CBrowserDeviceType::PS) {
    print_device_->setForeground(pen.getColor());

    print_device_->drawRectangle(x, y, x + width, y + height);
  }
}

void
CBrowserGraphics::
fillRectangle(int x, int y, int width, int height, const CBrush &brush)
{
  if      (current_device_ == CBrowserDeviceType::X)
    renderer_->fillRectangle(CIBBox2D(CIPoint2D(x, y), CISize2D(width, height)), brush);
  else if (current_device_ == CBrowserDeviceType::PS) {
    print_device_->setForeground(brush.getColor());

    print_device_->fillRectangle(x, y, x + width, y + height);
  }
}

void
CBrowserGraphics::
drawCircle(int x, int y, int radius, const CPen &pen)
{
  if      (current_device_ == CBrowserDeviceType::X)
    renderer_->drawCircle(CIPoint2D(x, y), radius, pen);
  else if (current_device_ == CBrowserDeviceType::PS) {
    print_device_->setForeground(pen.getColor());

    print_device_->drawCircle(x, y, radius);
  }
}

void
CBrowserGraphics::
fillCircle(int x, int y, int radius, const CBrush &brush)
{
  if      (current_device_ == CBrowserDeviceType::X)
    renderer_->fillCircle(CIPoint2D(x, y), radius, brush);
  else if (current_device_ == CBrowserDeviceType::PS) {
    print_device_->setForeground(brush.getColor());

    print_device_->fillCircle(x, y, radius);
  }
}

void
CBrowserGraphics::
drawLine(int x1, int y1, int x2, int y2, const CPen &pen)
{
  if      (current_device_ == CBrowserDeviceType::X)
    renderer_->drawLine(CIPoint2D(x1, y1), CIPoint2D(x2, y2), pen);
  else if (current_device_ == CBrowserDeviceType::PS) {
    print_device_->setForeground(pen.getColor());

    print_device_->drawLine(x1, y1, x2, y2);
  }
}

void
CBrowserGraphics::
drawText(int x, int y, const std::string &text, const CPen &pen, const CFontPtr &font)
{
  if      (current_device_ == CBrowserDeviceType::X)
    renderer_->drawText(CIPoint2D(x, y), text, pen, font);
  else if (current_device_ == CBrowserDeviceType::PS) {
    print_device_->setForeground(pen.getColor());
    print_device_->setFont(font);

    print_device_->drawText(x, y, text);
  }
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
