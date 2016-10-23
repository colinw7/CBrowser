#include <CBrowserHtmlI.h>
#include <CQMenu.h>
#include <CRGBName.h>

#include <QVBoxLayout>
#include <QScrollArea>
#include <QScrollBar>
#include <QLabel>
#include <QFileDialog>
#include <QMessageBox>
#include <QMouseEvent>
#include <QStatusBar>

CBrowserIFace::
CBrowserIFace() :
 CQMainWindow    ("CBrowser"),
 window_         (NULL),
 canvas_         (NULL),
 list_           (NULL),
 list_hbar_      (NULL),
 list_vbar_      (NULL),
 message_        (NULL),
 historyMenu_    (NULL),
 canvas_x_offset_(0),
 canvas_y_offset_(0),
 canvas_width_   (0),
 canvas_height_  (0)
{
  QWidget::resize(640, 800);
}

CBrowserIFace::
~CBrowserIFace()
{
  delete window_;
}

void
CBrowserIFace::
init()
{
  CQMainWindow::init();

  window_ = new CBrowserWindow();

  window_->setIFace(this);
}

QWidget *
CBrowserIFace::
createCentralWidget()
{
  QWidget *frame = new QWidget;

  QVBoxLayout *vlayout = new QVBoxLayout(frame);

  vlayout->setMargin(0); vlayout->setSpacing(0);

  //------

  list_ = new QScrollArea;

  list_vbar_ = list_->verticalScrollBar  ();
  list_hbar_ = list_->horizontalScrollBar();

  canvas_ = new CBrowserCanvas(this);

  //list_->setWidget(canvas_);

  //vlayout->addWidget(list_);
  vlayout->addWidget(canvas_);

  //------

  return frame;
}

void
CBrowserIFace::
createMenus()
{
  CQMenu *fileMenu = new CQMenu(this, "&File");

  CQMenuItem *newMenuItem = new CQMenuItem(fileMenu, "&New");

  newMenuItem->setShortcut("Ctrl+N");
  newMenuItem->connect(this, SLOT(newProc()));

  CQMenuItem *readMenuItem = new CQMenuItem(fileMenu, "&Read");

  readMenuItem->setShortcut("Ctrl+R");
  readMenuItem->connect(this, SLOT(readProc()));

  CQMenuItem *fileMenuItem = new CQMenuItem(fileMenu, "&Print");

  fileMenuItem->connect(this, SLOT(printProc()));

  CQMenuItem *quitMenuItem = new CQMenuItem(fileMenu, "&Quit");

  quitMenuItem->setShortcut("Ctrl+Q");
  quitMenuItem->connect(this, SLOT(quitProc()));

  CQMenu *goMenu = new CQMenu(this, "&Go");

  CQMenuItem *backMenuItem = new CQMenuItem(goMenu, "&Back");

  backMenuItem->connect(this, SLOT(goBackProc()));

  CQMenuItem *forwardMenuItem = new CQMenuItem(goMenu, "&Forward");

  forwardMenuItem->connect(this, SLOT(goForwardProc()));

  historyMenu_ = new CQMenu(this, "&History");

  CQMenu *helpMenu = new CQMenu(this, "&Help");

  new CQMenuItem(helpMenu, "&Help");
}

void
CBrowserIFace::
createToolBars()
{
}

void
CBrowserIFace::
createStatusBar()
{
  message_ = new QLabel(" ");

  statusBar()->addWidget(message_);
}

void
CBrowserIFace::
addHistoryItem(const std::string &item)
{
  CQMenuItem *menuItem = new CQMenuItem(historyMenu_, "button");

  menuItem->setName(item);
}

void
CBrowserIFace::
setBusy()
{
}

void
CBrowserIFace::
setReady()
{
}

void
CBrowserIFace::
expose()
{
  canvas_->update();
}

void
CBrowserIFace::
print(double xmin, double ymin, double xmax, double ymax)
{
  canvas_->setPSDevice(xmin, ymin, xmax, ymax);

  expose();

  canvas_->setXDevice();
}

void
CBrowserIFace::
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

    width = canvas_width_ - vbar_width;

    int maximum = list_hbar_->maximum();

    if (maximum != width - 1)
      list_hbar_->setMaximum(width - 1);

    list_hbar_->setValue(canvas_x_offset_);

    list_hbar_->setSingleStep(canvas_width_/10);
    list_hbar_->setPageStep(canvas_width_);
  }

  /*-----------*/

  if (vbar_displayed) {
    int hbar_height = 0;

    if (hbar_displayed)
      hbar_height = list_hbar_->height();

    height = canvas_height_ - hbar_height;

    int maximum = list_vbar_->maximum();

    if (maximum != height - 1)
      list_vbar_->setMaximum(height - 1);

    list_vbar_->setValue(canvas_y_offset_);

    list_hbar_->setSingleStep(height/10);
    list_hbar_->setPageStep(height);
  }
}

void
CBrowserIFace::
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
CBrowserIFace::
setTitle(const std::string &title)
{
  setWindowTitle(title.c_str());
}

void
CBrowserIFace::
setStatus(const std::string &status)
{
  if (status != "")
    message_->setText(status.c_str());
  else
    message_->setText(" ");
}

void
CBrowserIFace::
errorDialog(const std::string &msg)
{
  QMessageBox::warning(canvas_, "Error", msg.c_str());
}

void
CBrowserIFace::
newProc()
{
  std::string home = CDir::getHome();

  std::string directory = home + "/data/html";

  /*------------*/

  QString file =
    QFileDialog::getOpenFileName(this, "Select HTML File", directory.c_str(), "*.html");

  if (file == "")
    return;

  /*------------*/

  CBrowserIFace *iface = new CBrowserIFace;

  iface->window_->setDocument(file.toStdString());
}

void
CBrowserIFace::
readProc()
{
  std::string home = CDir::getHome();

  std::string directory = home + "/data/html";

  /*------------*/

  QString file =
    QFileDialog::getOpenFileName(this, "Select HTML File", directory.c_str(), "*.html");

  if (file == "")
    return;

  /*------------*/

  window_->setDocument(file.toStdString());
}

void
CBrowserIFace::
printProc()
{
  window_->print();
}

void
CBrowserIFace::
goBackProc()
{
  window_->goBack();
}

void
CBrowserIFace::
goForwardProc()
{
  window_->goForward();
}

void
CBrowserIFace::
hscrollProc()
{
  canvas_x_offset_ = list_hbar_->value();

  window_->redraw();
}

void
CBrowserIFace::
vscrollProc()
{
  canvas_y_offset_ = list_vbar_->value();

  window_->redraw();
}

void
CBrowserIFace::
quitProc()
{
  close();
}

void
CBrowserIFace::
resize()
{
  canvas_x_offset_ = 0;
  canvas_y_offset_ = 0;

  canvas_width_  = canvas_->width ();
  canvas_height_ = canvas_->height();

  /*---------------*/

  window_->resize();
}

void
CBrowserIFace::
draw()
{
  setBusy();

  canvas_->startDoubleBuffer();

  /*----------------*/

  canvas_->clear(window_->getBg());

  /*
  if (bg_image_ != NULL)
    drawTiledImage(0, 0, pixmap_width_, pixmap_height_, bg_image_);
*/

  /*----------------*/

  window_->drawDocument();

  /*---------------*/

  canvas_->endDoubleBuffer();

  setReady();
}

void
CBrowserIFace::
mouseMotion(int x, int y)
{
  std::string link_name;

  if (window_->hoverLink(x + canvas_x_offset_, y + canvas_y_offset_, link_name)) {
    canvas_->setCursor(Qt::PointingHandCursor);
  }
  else {
    canvas_->setCursor(Qt::ArrowCursor);
  }
}

void
CBrowserIFace::
mouseRelease(int x, int y)
{
  window_->activateLink(x + canvas_x_offset_, y + canvas_y_offset_);
}

//------------

CBrowserCanvas::
CBrowserCanvas(CBrowserIFace *iface) :
 iface_   (iface),
 graphics_(NULL)
{
  graphics_ = new CBrowserGraphics(this);
}

void
CBrowserCanvas::
paintEvent(QPaintEvent *)
{
  iface_->draw();
}

void
CBrowserCanvas::
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
CBrowserCanvas::
mousePressEvent(QMouseEvent *)
{
}

void
CBrowserCanvas::
mouseMotionEvent(QMouseEvent *e)
{
  int x = e->x();
  int y = e->y();

  iface_->mouseMotion(x, y);
}

void
CBrowserCanvas::
mouseReleaseEvent(QMouseEvent *e)
{
  int x = e->x();
  int y = e->y();

  iface_->mouseRelease(x, y);
}

void
CBrowserCanvas::
keyPressEvent(QKeyEvent *)
{
}

void
CBrowserCanvas::
keyReleaseEvent(QKeyEvent *)
{
}

void
CBrowserCanvas::
startDoubleBuffer()
{
  graphics_->startDoubleBuffer(width(), height());
}

void
CBrowserCanvas::
endDoubleBuffer()
{
  graphics_->endDoubleBuffer();
}

void
CBrowserCanvas::
setXDevice()
{
  graphics_->setXDevice();
}

void
CBrowserCanvas::
setPSDevice(double xmin, double ymin, double xmax, double ymax)
{
  graphics_->setPSDevice(xmin, ymin, xmax, ymax);
}

void
CBrowserCanvas::
clear(const CRGBA &bg)
{
  graphics_->clear(bg);
}

void
CBrowserCanvas::
drawImage(int x, int y, const CImagePtr &image)
{
  graphics_->drawImage(x, y, image);
}

void
CBrowserCanvas::
drawRectangle(int x1, int y1, int x2, int y2)
{
  graphics_->drawRectangle(x1, y1, x2, y2);
}

void
CBrowserCanvas::
fillRectangle(int x1, int y1, int x2, int y2)
{
  graphics_->fillRectangle(x1, y1, x2, y2);
}

void
CBrowserCanvas::
drawCircle(int x, int y, int r)
{
  graphics_->drawCircle(x, y, r);
}

void
CBrowserCanvas::
fillCircle(int x, int y, int r)
{
  graphics_->fillCircle(x, y, r);
}

void
CBrowserCanvas::
drawLine(int x1, int y1, int x2, int y2)
{
  graphics_->drawLine(x1, y1, x2, y2);
}

void
CBrowserCanvas::
drawString(int x, int y, const std::string &str)
{
  graphics_->drawString(x, y, str);
}

void
CBrowserCanvas::
drawOutline(int x, int y, int width, int height, const std::string &color_name)
{
  if (CEnvInst.exists("HTML_OUTLINE")) {
    CRGBA color = CRGBName::toRGBA(color_name);

    graphics_->drawOutline(x, y, width, height, color);
  }
}

void
CBrowserCanvas::
drawBorder(int x, int y, int width, int height, CBrowserBorderType type)
{
  CBrowserWindow *window = iface_->getWindow();

  graphics_->drawBorder(x, y, width, height, window->getBg(), type);
}

void
CBrowserCanvas::
drawHRule(int x1, int x2, int y, int height)
{
  CBrowserWindow *window = iface_->getWindow();

  graphics_->drawHRule(x1, x2, y, height, window->getBg());
}

void
CBrowserCanvas::
setForeground(const CRGBA &fg)
{
  graphics_->setForeground(fg);
}

void
CBrowserCanvas::
setFont(CFontPtr font)
{
  graphics_->setFont(font);
}
