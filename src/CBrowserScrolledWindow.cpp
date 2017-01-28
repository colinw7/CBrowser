#include <CBrowserScrolledWindow.h>
#include <CBrowserIFace.h>
#include <CBrowserWindowWidget.h>
#include <CBrowserWindow.h>
#include <CBrowserMain.h>
#include <CBrowserLayout.h>
#include <CBrowserBox.h>
#include <CBrowserObject.h>

#include <QGridLayout>
#include <QScrollBar>
#include <QLabel>

CBrowserScrolledWindow::
CBrowserScrolledWindow(CBrowserIFace *iface) :
 QFrame(iface), iface_(iface)
{
  window_ = new CBrowserWindow();

  //---

  QGridLayout *grid = new QGridLayout(this);
  grid->setMargin(0); grid->setSpacing(0);

  w_ = new CBrowserWindowWidget(this);

  list_vbar_ = new QScrollBar(Qt::Vertical);
  list_vbar_->setObjectName("vbar");

  list_hbar_ = new QScrollBar(Qt::Horizontal);
  list_hbar_->setObjectName("hbar");

  grid->addWidget(w_        , 0, 0);
  grid->addWidget(list_vbar_, 0, 1);
  grid->addWidget(list_hbar_, 1, 0);

  connect(list_vbar_, SIGNAL(valueChanged(int)), this, SLOT(vscrollProc()));
  connect(list_hbar_, SIGNAL(valueChanged(int)), this, SLOT(hscrollProc()));

  //---

  window_->setIFace(this);
}

CBrowserScrolledWindow::
~CBrowserScrolledWindow()
{
  delete window_;
}

void
CBrowserScrolledWindow::
setTitle(const std::string &title)
{
  title_ = title;

  iface_->updateTitles();
}

void
CBrowserScrolledWindow::
setSize(int width, int height)
{
  bool hbar_displayed = false;
  bool vbar_displayed = false;

  if (width > canvas_width_)
    hbar_displayed = true;

  if (height > canvas_height_)
    vbar_displayed = true;

  if (hbar_displayed) {
    int vbar_width = 0;

    if (vbar_displayed)
      vbar_width = list_vbar_->width();

    //int width1 = canvas_width_ - vbar_width;
    int width1 = width - vbar_width;

    int maximum = list_hbar_->maximum();

    if (maximum != width1 - 1)
      list_hbar_->setRange(0, width1 - canvas_width_ - 1);

    list_hbar_->setValue(canvas_x_offset_);

    list_hbar_->setSingleStep(canvas_width_/10);
    list_hbar_->setPageStep(canvas_width_);
  }

  //---

  if (vbar_displayed) {
    int hbar_height = 0;

    if (hbar_displayed)
      hbar_height = list_hbar_->height();

    //int height1 = canvas_height_ - hbar_height;
    int height1 = height - hbar_height;

    int maximum = list_vbar_->maximum();

    if (maximum != height1 - 1)
      list_vbar_->setRange(0, height1 - canvas_height_ - 1);

    list_vbar_->setValue(canvas_y_offset_);

    list_vbar_->setSingleStep(canvas_height_/10);
    list_vbar_->setPageStep(canvas_height_);
  }

  //w_->resize(width, height);
}

void
CBrowserScrolledWindow::
setDocument(const std::string &filename)
{
  window_->setDocument(filename);
}

void
CBrowserScrolledWindow::
print()
{
  window_->print();
}

void
CBrowserScrolledWindow::
saveImage(const std::string &filename)
{
  window_->saveImage(filename);
}

void
CBrowserScrolledWindow::
scrollTo(int, int y)
{
  int minimum     = list_vbar_->minimum();
  int maximum     = list_vbar_->maximum();
  int slider_size = list_vbar_->pageStep();

  if      (y < minimum)
    y = minimum;
  else if (y > maximum - slider_size)
    y = maximum - slider_size;

  list_vbar_->setValue(y);
}

void
CBrowserScrolledWindow::
hscrollProc()
{
  canvas_x_offset_ = list_hbar_->value();

  window_->redraw();
}

void
CBrowserScrolledWindow::
vscrollProc()
{
  canvas_y_offset_ = list_vbar_->value();

  window_->redraw();
}

void
CBrowserScrolledWindow::
expose()
{
  w_->update();
}

void
CBrowserScrolledWindow::
goBack()
{
  window_->goBack();
}

void
CBrowserScrolledWindow::
goForward()
{
  window_->goForward();
}

void
CBrowserScrolledWindow::
print(double xmin, double ymin, double xmax, double ymax)
{
  w_->setPSDevice(xmin, ymin, xmax, ymax);

  expose();

  w_->setXDevice();
}

void
CBrowserScrolledWindow::
resize()
{
  canvas_x_offset_ = 0;
  canvas_y_offset_ = 0;

  list_hbar_->setValue(canvas_x_offset_);
  list_vbar_->setValue(canvas_y_offset_);

  canvas_width_  = w_->width ();
  canvas_height_ = w_->height();

  //---

  window_->resize();
}

void
CBrowserScrolledWindow::
draw()
{
  iface_->setBusy();

  w_->startDoubleBuffer();

  //---

  w_->clear(window_->getBgColor());

  CImagePtr bg_image = window_->getBgImage();

  if (bg_image.isValid())
    w_->drawTiledImage(0, 0, w_->width(), w_->height(), bg_image);

  //---

  window_->drawDocument();

  //---

  w_->endDoubleBuffer();

  iface_->setReady();
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
  int x1 = x + canvas_x_offset_;
  int y1 = y + canvas_y_offset_;

  if (CBrowserMainInst->getMouseOver()) {
    CBrowserBox *box = window_->getLayout()->boxAt(CIPoint2D(x1, y1));

    CBrowserObject *obj = dynamic_cast<CBrowserObject *>(box);

    window_->selectSingleObject(obj);

    if (obj)
      iface_->objLabel()->setText(obj->hierTypeName().c_str());
    else
      iface_->objLabel()->setText("");
  }

  //---

  std::string link_name;

  if (window_->hoverLink(x1, y1, link_name)) {
    w_->setCursor(Qt::PointingHandCursor);
  }
  else {
    w_->setCursor(Qt::ArrowCursor);
  }

  iface_->posLabel()->setText(QString("%1,%2").arg(x1).arg(y1));
}

void
CBrowserScrolledWindow::
mouseRelease(int x, int y)
{
  window_->activateLink(x + canvas_x_offset_, y + canvas_y_offset_);
}

void
CBrowserScrolledWindow::
mouseWheel(int d)
{
  int d1 = d;

  list_vbar_->setValue(list_vbar_->value() - d1);
}
