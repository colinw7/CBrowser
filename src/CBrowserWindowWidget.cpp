#include <CBrowserWindowWidget.h>
#include <CBrowserScrolledWindow.h>
#include <CBrowserGraphics.h>
#include <CBrowserWindow.h>
#include <CBrowserIFace.h>
#include <CBrowserJS.h>
#include <CQJDocument.h>
#include <CQJWindow.h>
#include <CQJEvent.h>
#include <CEnv.h>
#include <QMouseEvent>

CBrowserWindowWidget::
CBrowserWindowWidget(CBrowserScrolledWindow *window) :
 window_(window)
{
  setObjectName("canvas");

  setFocusPolicy(Qt::StrongFocus);

  setMouseTracking(true);

  graphics_ = new CBrowserGraphics(this);
}

void
CBrowserWindowWidget::
paintEvent(QPaintEvent *)
{
  window_->draw();
}

void
CBrowserWindowWidget::
resizeEvent(QResizeEvent *)
{
  static bool ignore_redraw;

  if (ignore_redraw)
    return;

  ignore_redraw = true;

  //---

  window_->resize();

  //---

  ignore_redraw = false;
}

void
CBrowserWindowWidget::
mousePressEvent(QMouseEvent *e)
{
  int x = e->x();
  int y = e->y();

  window_->mousePress(x, y);
}

void
CBrowserWindowWidget::
mouseMoveEvent(QMouseEvent *e)
{
  int x = e->x();
  int y = e->y();

  window_->mouseMotion(x, y);
}

void
CBrowserWindowWidget::
mouseReleaseEvent(QMouseEvent *e)
{
  int x = e->x();
  int y = e->y();

  window_->mouseRelease(x, y);
}

void
CBrowserWindowWidget::
wheelEvent(QWheelEvent *event)
{
  window_->mouseWheel(event->delta());
}

void
CBrowserWindowWidget::
keyPressEvent(QKeyEvent *e)
{
  CJValueP event(new CQJEvent(CBrowserJSInst->js(), CBrowserJSInst->jsWindow(), e));

  callEventListener("keydown", "onkeydown", event);
}

void
CBrowserWindowWidget::
keyReleaseEvent(QKeyEvent *e)
{
  CJValueP event(new CQJEvent(CBrowserJSInst->js(), CBrowserJSInst->jsWindow(), e));

  callEventListener("keyup", "onkeyup", event);
}

void
CBrowserWindowWidget::
callEventListener(const std::string &name, const std::string &prop, CJValueP event)
{
  CQJObject::EventArgs  args;
  CQJObject::NameValues nameValues;

  args.push_back(event);

  //CQJCanvas *canvas = CBrowserJSInst->jsCanvas()->castP<CQJCanvas>();

  //if (canvas->callEventListener(name, prop, args, nameValues))
  //  return;

  CQJWindow *window = CBrowserJSInst->jsWindow()->castP<CQJWindow>();

  if (window->callEventListener(name, prop, args, nameValues))
    return;

  CQJDocument *document = CBrowserJSInst->jsDocument()->castP<CQJDocument>();

  if (document->callEventListener(name, prop, args, nameValues))
    return;
}

void
CBrowserWindowWidget::
startDoubleBuffer()
{
  graphics_->startDoubleBuffer(width(), height());
}

void
CBrowserWindowWidget::
endDoubleBuffer()
{
  graphics_->endDoubleBuffer();
}

void
CBrowserWindowWidget::
saveImage(const std::string &filename)
{
  QPixmap *pixmap = graphics_->pixmap();

  pixmap->save(filename.c_str());
}

void
CBrowserWindowWidget::
setXDevice()
{
  graphics_->setXDevice();
}

void
CBrowserWindowWidget::
setPSDevice(double xmin, double ymin, double xmax, double ymax)
{
  graphics_->setPSDevice(xmin, ymin, xmax, ymax);
}

void
CBrowserWindowWidget::
clear(const CRGBA &bg)
{
  graphics_->clear(bg);
}

void
CBrowserWindowWidget::
drawImage(int x, int y, const CImagePtr &image)
{
  graphics_->drawImage(x, y, image);
}

void
CBrowserWindowWidget::
drawImage(int x, int y, const QImage &image)
{
  graphics_->drawImage(x, y, image);
}

void
CBrowserWindowWidget::
drawTiledImage(int x, int y, int width, int height, const CImagePtr &image)
{
  graphics_->drawTiledImage(x, y, width, height, image);
}

void
CBrowserWindowWidget::
drawRectangle(int x, int y, int w, int h, const CPen &pen)
{
  graphics_->drawRectangle(x, y, w, h, pen);
}

void
CBrowserWindowWidget::
fillRectangle(int x, int y, int w, int h, const CBrush &brush)
{
  graphics_->fillRectangle(x, y, w, h, brush);
}

void
CBrowserWindowWidget::
fillPolygon(const std::vector<CIPoint2D> &points, const CBrush &brush)
{
  graphics_->fillPolygon(points, brush);
}

void
CBrowserWindowWidget::
drawCircle(int x, int y, int r, const CPen &pen)
{
  graphics_->drawCircle(x, y, r, pen);
}

void
CBrowserWindowWidget::
fillCircle(int x, int y, int r, const CBrush &brush)
{
  graphics_->fillCircle(x, y, r, brush);
}

void
CBrowserWindowWidget::
drawLine(int x1, int y1, int x2, int y2, const CPen &pen)
{
  graphics_->drawLine(x1, y1, x2, y2, pen);
}

void
CBrowserWindowWidget::
drawText(int x, int y, const std::string &str, const CPen &pen, const CFontPtr &font)
{
  graphics_->drawText(x, y, str, pen, font);
}

void
CBrowserWindowWidget::
drawOutline(int x, int y, int width, int height, const CPen &pen)
{
  graphics_->drawOutline(x, y, width, height, pen);
}

void
CBrowserWindowWidget::
drawBorder(int x, int y, int width, int height, CBrowserBorderType type)
{
  CBrowserWindow *window = window_->getWindow();

  graphics_->drawBorder(x, y, width, height, CPen(window->getBgColor()), type);
}

void
CBrowserWindowWidget::
drawHRule(int x1, int x2, int y, int height)
{
  CBrowserWindow *window = window_->getWindow();

  graphics_->drawHRule(x1, x2, y, height, CPen(window->getBgColor()));
}
