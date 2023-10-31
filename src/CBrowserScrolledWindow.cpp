#include <CBrowserScrolledWindow.h>
#include <CBrowserMainWindow.h>
#include <CBrowserWindowWidget.h>
#include <CBrowserWindow.h>
#include <CBrowserMain.h>
#include <CBrowserLayout.h>
#include <CBrowserBox.h>
#include <CBrowserObject.h>
#include <CBrowserGraphics.h>

#include <CQJavaScript.h>

#include <QApplication>
#include <QGridLayout>
#include <QScrollBar>
#include <QLabel>

CBrowserScrolledWindow::
CBrowserScrolledWindow(CBrowserMainWindow *mainWindow) :
 QFrame(mainWindow), CBrowserWindowIFace(mainWindow->browser()), mainWindow_(mainWindow)
{
  setObjectName("swindow");

  //---

  window_ = new CBrowserWindow(mainWindow->browser());

  window_->setScrolledWindow(this);

  auto *windowWidget = dynamic_cast<CBrowserWindowWidget *>(window_->widget());

  //---

  CQJavaScriptInst->setWindowIFace(window_->jsIFace());

  CQJavaScriptInst->init();

  //---

  auto *grid = new QGridLayout(this);
  grid->setMargin(0); grid->setSpacing(0);

  list_vbar_ = new QScrollBar(Qt::Vertical);
  list_vbar_->setObjectName("vbar");

  list_hbar_ = new QScrollBar(Qt::Horizontal);
  list_hbar_->setObjectName("hbar");

  grid->addWidget(windowWidget, 0, 0);
  grid->addWidget(list_vbar_  , 0, 1);
  grid->addWidget(list_hbar_  , 1, 0);

  connect(list_vbar_, SIGNAL(valueChanged(int)), this, SLOT(vscrollProc()));
  connect(list_hbar_, SIGNAL(valueChanged(int)), this, SLOT(hscrollProc()));
}

CBrowserScrolledWindow::
~CBrowserScrolledWindow()
{
  delete window_;
}

CBrowserWindowWidgetIFace *
CBrowserScrolledWindow::
widget() const
{
  return window_->widget();
}

void
CBrowserScrolledWindow::
setSize(int width, int height)
{
  bool hbar_displayed = false;
  bool vbar_displayed = false;

  if (width > getCanvasWidth())
    hbar_displayed = true;

  if (height > getCanvasHeight())
    vbar_displayed = true;

  int vbar_width  = (vbar_displayed ? vbar()->width () : 0);
  int hbar_height = (hbar_displayed ? hbar()->height() : 0);

  if (hbar_displayed) {
    //int width1 = getCanvasWidth() - vbar_width;
    int width1 = width - vbar_width;

    int maximum = hbar()->maximum();

    if (maximum != width1 - 1)
      hbar()->setRange(0, width1 - getCanvasWidth() - 1);

    hbar()->setValue(getCanvasXOffset());

    hbar()->setSingleStep(getCanvasWidth()/10);
    hbar()->setPageStep(getCanvasWidth());
  }

  hbar()->setVisible(hbar_displayed);

  //---

  if (vbar_displayed) {
    //int height1 = getCanvasHeight() - hbar_height;
    int height1 = height - hbar_height;

    int maximum = vbar()->maximum();

    if (maximum != height1 - 1)
      vbar()->setRange(0, height1 - getCanvasHeight() - 1);

    vbar()->setValue(getCanvasYOffset());

    vbar()->setSingleStep(getCanvasHeight()/10);
    vbar()->setPageStep(getCanvasHeight());
  }

  vbar()->setVisible(vbar_displayed);

  //widget()->resize(width, height);
}

void
CBrowserScrolledWindow::
scrollTo(int, int y)
{
  int minimum     = vbar()->minimum();
  int maximum     = vbar()->maximum();
  int slider_size = vbar()->pageStep();

  if      (y < minimum)
    y = minimum;
  else if (y > maximum - slider_size)
    y = maximum - slider_size;

  vbar()->setValue(y);
}

void
CBrowserScrolledWindow::
hscrollProc()
{
  setCanvasXOffset(hbar()->value());

  window_->redraw();
}

void
CBrowserScrolledWindow::
vscrollProc()
{
  setCanvasYOffset(vbar()->value());

  window_->redraw();
}

void
CBrowserScrolledWindow::
expose()
{
  widget()->updateWidget(/*sync*/false);
}

void
CBrowserScrolledWindow::
resize()
{
  setCanvasXOffset(0);
  setCanvasYOffset(0);

  hbar()->setValue(getCanvasXOffset());
  vbar()->setValue(getCanvasYOffset());

  setCanvasWidth (widget()->windowWidth ());
  setCanvasHeight(widget()->windowHeight());

  //---

  window_->resize();
}

void
CBrowserScrolledWindow::
mousePress(int, int)
{
}

void
CBrowserScrolledWindow::
mouseMotion(int x, int y)
{
  int x1 = x + getCanvasXOffset();
  int y1 = y + getCanvasYOffset();

  if (browser()->getMouseOver()) {
    auto *box = window_->getLayout()->boxAt(CIPoint2D(x1, y1));

    auto *obj = dynamic_cast<CBrowserObject *>(box);

    window_->selectSingleObject(obj);

    if (obj)
      mainWindow_->setObjText(QString::fromStdString(obj->hierTypeName()));
    else
      mainWindow_->setObjText("");
  }

  //---

  std::string link_name;

  if (window_->hoverLink(x1, y1, link_name)) {
    widget()->setCursor(Qt::PointingHandCursor);
  }
  else {
    widget()->setCursor(Qt::ArrowCursor);
  }

  mainWindow_->setPosText(QString("%1,%2").arg(x1).arg(y1));
}

void
CBrowserScrolledWindow::
mouseRelease(int x, int y)
{
  int x1 = x + getCanvasXOffset();
  int y1 = y + getCanvasYOffset();

  window_->mouseRelease(x1, y1);
}

void
CBrowserScrolledWindow::
mouseWheel(int d)
{
  int d1 = d;

  vbar()->setValue(vbar()->value() - d1);
}
