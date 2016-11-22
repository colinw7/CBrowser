#include <CBrowserWindowWidget.h>
#include <CBrowserGraphics.h>
#include <CBrowserWindow.h>
#include <CBrowserIFace.h>
#include <CBrowserJS.h>
#include <CQJDocument.h>
#include <CQJWindow.h>
#include <CQJEvent.h>
#include <CRGBName.h>
#include <CEnv.h>
#include <QMouseEvent>

CBrowserWindowWidget::
CBrowserWindowWidget(CBrowserIFace *iface) :
 iface_(iface)
{
  setObjectName("canvas");

  setFocusPolicy(Qt::StrongFocus);

  graphics_ = new CBrowserGraphics(this);
}

void
CBrowserWindowWidget::
paintEvent(QPaintEvent *)
{
  iface_->draw();
}

void
CBrowserWindowWidget::
resizeEvent(QResizeEvent *)
{
  static bool ignore_redraw;

  if (ignore_redraw)
    return;

  ignore_redraw = true;

  /*---------------*/

  iface_->resize();

  /*---------------*/

  ignore_redraw = false;
}

void
CBrowserWindowWidget::
mousePressEvent(QMouseEvent *)
{
}

void
CBrowserWindowWidget::
mouseMotionEvent(QMouseEvent *e)
{
  int x = e->x();
  int y = e->y();

  iface_->mouseMotion(x, y);
}

void
CBrowserWindowWidget::
mouseReleaseEvent(QMouseEvent *e)
{
  int x = e->x();
  int y = e->y();

  iface_->mouseRelease(x, y);
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
drawRectangle(int x1, int y1, int x2, int y2)
{
  graphics_->drawRectangle(x1, y1, x2, y2);
}

void
CBrowserWindowWidget::
fillRectangle(int x1, int y1, int x2, int y2)
{
  graphics_->fillRectangle(x1, y1, x2, y2);
}

void
CBrowserWindowWidget::
drawCircle(int x, int y, int r)
{
  graphics_->drawCircle(x, y, r);
}

void
CBrowserWindowWidget::
fillCircle(int x, int y, int r)
{
  graphics_->fillCircle(x, y, r);
}

void
CBrowserWindowWidget::
drawLine(int x1, int y1, int x2, int y2)
{
  graphics_->drawLine(x1, y1, x2, y2);
}

void
CBrowserWindowWidget::
drawString(int x, int y, const std::string &str)
{
  graphics_->drawString(x, y, str);
}

void
CBrowserWindowWidget::
drawOutline(int x, int y, int width, int height, const std::string &color_name)
{
  if (CEnvInst.exists("HTML_OUTLINE")) {
    CRGBA color = CRGBName::toRGBA(color_name);

    graphics_->drawOutline(x, y, width, height, color);
  }
}

void
CBrowserWindowWidget::
drawOutline(int x, int y, int width, int height, const CRGBA &c)
{
  if (CEnvInst.exists("HTML_OUTLINE")) {
    graphics_->drawOutline(x, y, width, height, c);
  }
}

void
CBrowserWindowWidget::
drawBorder(int x, int y, int width, int height, CBrowserBorderType type)
{
  CBrowserWindow *window = iface_->getWindow();

  graphics_->drawBorder(x, y, width, height, window->getBg(), type);
}

void
CBrowserWindowWidget::
drawHRule(int x1, int x2, int y, int height)
{
  CBrowserWindow *window = iface_->getWindow();

  graphics_->drawHRule(x1, x2, y, height, window->getBg());
}

void
CBrowserWindowWidget::
setForeground(const CRGBA &fg)
{
  graphics_->setForeground(fg);
}

void
CBrowserWindowWidget::
setFont(CFontPtr font)
{
  graphics_->setFont(font);
}
