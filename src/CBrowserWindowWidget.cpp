#include <CBrowserWindowWidget.h>
#include <CBrowserMainWindow.h>
#include <CBrowserGraphics.h>
#include <CBrowserWindow.h>

#include <CQJavaScript.h>
#include <CQJDocument.h>
#include <CQJWindow.h>
#include <CQJEvent.h>
#include <CQWidgetUtil.h>

#include <QMouseEvent>
#include <QPainter>

CBrowserWindowWidget::
CBrowserWindowWidget(CBrowserWindowIFace *window) :
 CBrowserWindowWidgetIFace(window->browser()), window_(window)
{
  setObjectName("canvas");

  setFocusPolicy(Qt::StrongFocus);

  setMouseTracking(true);
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
  window_->mouseWheel(CQWidgetUtil::wheelDelta(event));
}

void
CBrowserWindowWidget::
keyPressEvent(QKeyEvent *e)
{
  CJValueP event(new CQJEvent(CQJavaScriptInst->js(), CQJavaScriptInst->jsWindow(), e));

  callEventListener("keydown", "onkeydown", event);
}

void
CBrowserWindowWidget::
keyReleaseEvent(QKeyEvent *e)
{
  CJValueP event(new CQJEvent(CQJavaScriptInst->js(), CQJavaScriptInst->jsWindow(), e));

  callEventListener("keyup", "onkeyup", event);
}

void
CBrowserWindowWidget::
callEventListener(const std::string &name, const std::string &prop, CJValueP event)
{
  CQJObject::EventArgs  args;
  CQJObject::NameValues nameValues;

  args.push_back(event);

  auto window = CQJavaScriptInst->jsWindow();

  if (window->callEventListener(name, prop, args, nameValues))
    return;

  CQJDocumentP document = CQJavaScriptInst->jsDocument();

  if (document->callEventListener(name, prop, args, nameValues))
    return;
}

void
CBrowserWindowWidget::
updateWidget(bool sync)
{
  QWidget::update();

  if (sync)
    qApp->processEvents();
}

void
CBrowserWindowWidget::
drawPixmap(QPixmap *pixmap)
{
  QPainter painter(this);

  painter.drawPixmap(QPoint(0, 0), *pixmap);
}
