#include <CBrowserWindow.h>
#include <CBrowserWindowWidget.h>
#include <CBrowserScrolledWindow.h>
#include <CBrowserMain.h>
#include <CBrowserGraphics.h>

#include <QApplication>

CBrowserWindow::
CBrowserWindow(CBrowser *browser) :
 CBrowserWindowIFace(browser)
{
  init();
}

CBrowserWindow::
~CBrowserWindow()
{
}

//---

CBrowserWindowWidgetIFace *
CBrowserWindow::
makeWidget()
{
  return new CBrowserWindowWidget(this);
}

//---

void
CBrowserWindow::
setScrolledWindow(CBrowserScrolledWindow *swindow)
{
  swindow_ = swindow;

  auto *widget = dynamic_cast<CBrowserWindowWidget *>(widget_.get());

  widget->setWindow(swindow_);
}

//---

void
CBrowserWindow::
propagateSize(int w, int h)
{
  if (swindow_)
    swindow_->setSize(w, h);
}

void
CBrowserWindow::
redraw()
{
  CBrowserWindowIFace::redraw();

  auto *widget = dynamic_cast<CBrowserWindowWidget *>(this->widget());
  if (! widget) return;

  const auto &objs = widget->children();

  for (int i = 0; i < objs.length(); ++i) {
    auto *w = qobject_cast<QWidget *>(objs[i]);

    if (w)
      w->update();
  }
}

//---

int
CBrowserWindow::
getCanvasXOffset() const
{
  return (swindow_ ? swindow_->getCanvasXOffset() : 0);
}

int
CBrowserWindow::
getCanvasYOffset() const
{
  return (swindow_ ? swindow_->getCanvasYOffset() : 0);
}

int
CBrowserWindow::
getCanvasWidth() const
{
  return (swindow_ ? swindow_->getCanvasWidth() : widget()->windowWidth());
}

int
CBrowserWindow::
getCanvasHeight() const
{
  return (swindow_ ? swindow_->getCanvasHeight() : widget()->windowHeight());
}

//---

void
CBrowserWindow::
scrollTo(int x, int y)
{
  if (swindow_)
    swindow_->scrollTo(x, y);
}
