#include <CBrowserIFace.h>
#include <CBrowserWindowWidget.h>
#include <CBrowserWindow.h>
#include <CBrowserMain.h>
#include <CBrowserGraphics.h>
#include <CBrowserJS.h>
#include <CBrowserDomTree.h>
#include <CBrowserObject.h>
#include <CBrowserLayout.h>
#include <CBrowserBox.h>
#include <CQJDocument.h>
#include <CQJWindow.h>
#include <CQJEvent.h>
#include <CQJDialog.h>
#include <CQMenu.h>
#include <CDir.h>
#include <CEnv.h>

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
 CQMainWindow("CBrowser")
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

  frame->setObjectName("frame");

  //------

  w_ = new CBrowserWindowWidget(this);

#ifdef SCROLL_AREA
  QVBoxLayout *vlayout = new QVBoxLayout(frame);
  vlayout->setMargin(0); vlayout->setSpacing(0);

  list_ = new QScrollArea;
  list_->setObjectName("list");

  list_vbar_ = list_->verticalScrollBar  ();
  list_hbar_ = list_->horizontalScrollBar();

  list_->setWidget(w_);

  vlayout->addWidget(list_);
#else
  QGridLayout *grid = new QGridLayout(frame);
  grid->setMargin(0); grid->setSpacing(0);

  list_vbar_ = new QScrollBar(Qt::Vertical);
  list_vbar_->setObjectName("vbar");

  list_hbar_ = new QScrollBar(Qt::Horizontal);
  list_hbar_->setObjectName("hbar");

  grid->addWidget(w_        , 0, 0);
  grid->addWidget(list_vbar_, 0, 1);
  grid->addWidget(list_hbar_, 1, 0);

  connect(list_vbar_, SIGNAL(valueChanged(int)), this, SLOT(vscrollProc()));
  connect(list_hbar_, SIGNAL(valueChanged(int)), this, SLOT(hscrollProc()));
#endif

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

  CQMenuItem *printMenuItem = new CQMenuItem(fileMenu, "&Print");
  printMenuItem->connect(this, SLOT(printProc()));

  CQMenuItem *saveImageMenuItem = new CQMenuItem(fileMenu, "Save &Image");
  saveImageMenuItem->connect(this, SLOT(saveImageProc()));

  CQMenuItem *jsMenuItem = new CQMenuItem(fileMenu, "&JavaScript");
  jsMenuItem->connect(this, SLOT(jsProc()));

  CQMenuItem *domMenuItem = new CQMenuItem(fileMenu, "&DOM");
  domMenuItem->connect(this, SLOT(domProc()));

  CQMenuItem *quitMenuItem = new CQMenuItem(fileMenu, "&Quit");
  quitMenuItem->setShortcut("Ctrl+Q");
  quitMenuItem->connect(this, SLOT(quitProc()));

  //---

  CQMenu *goMenu = new CQMenu(this, "&Go");

  CQMenuItem *backMenuItem = new CQMenuItem(goMenu, "&Back");

  backMenuItem->connect(this, SLOT(goBackProc()));

  CQMenuItem *forwardMenuItem = new CQMenuItem(goMenu, "&Forward");

  forwardMenuItem->connect(this, SLOT(goForwardProc()));

  //---

  CQMenu *viewMenu = new CQMenu(this, "&View");

  CQMenuItem *viewBoxes = new CQMenuItem(viewMenu, "&Boxes");
  viewBoxes->connect(this, SLOT(viewBoxesProc()));

  //---

  historyMenu_ = new CQMenu(this, "&History");

  //---

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

  objLabel_ = new QLabel;

  objLabel_->setObjectName("obj");

  statusBar()->addPermanentWidget(objLabel_ );

  posLabel_ = new QLabel;

  posLabel_->setObjectName("pos");

  statusBar()->addPermanentWidget(posLabel_ );
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
  w_->update();
}

void
CBrowserIFace::
print(double xmin, double ymin, double xmax, double ymax)
{
  w_->setPSDevice(xmin, ymin, xmax, ymax);

  expose();

  w_->setXDevice();
}

void
CBrowserIFace::
saveImage(const std::string &filename)
{
  w_->saveImage(filename);
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
  QMessageBox::warning(w_, "Error", msg.c_str());
}

void
CBrowserIFace::
newProc()
{
  std::string home = CDir::getHome();

  std::string directory = home + "/data/html";

  //---

  QString file =
    QFileDialog::getOpenFileName(this, "Select HTML File", directory.c_str(), "*.html");

  if (file == "")
    return;

  //---

  CBrowserIFace *iface = new CBrowserIFace;

  iface->window_->setDocument(file.toStdString());
}

void
CBrowserIFace::
readProc()
{
  std::string home = CDir::getHome();

  std::string directory = home + "/data/html";

  //---

  QString file =
    QFileDialog::getOpenFileName(this, "Select HTML File", directory.c_str(), "*.html");

  if (file == "")
    return;

  //---

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
saveImageProc()
{
  QString fileName =
    QFileDialog::getSaveFileName(this, "Save Image", "image.png",
                                 "Image Files (*.png *.xpm *.jpg)");

  if (fileName.length())
    window_->saveImage(fileName.toStdString());
}

void
CBrowserIFace::
jsProc()
{
  if (! jsDlg_)
    jsDlg_ = new CQJDialog(CBrowserJSInst->js());

  jsDlg_->show();
}

void
CBrowserIFace::
domProc()
{
  if (! domDlg_)
    domDlg_ = new CBrowserDomTreeDlg(window_);

  domDlg_->show();
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
viewBoxesProc()
{
  CBrowserMainInst->setShowBoxes(! CBrowserMainInst->getShowBoxes());
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

  list_hbar_->setValue(canvas_x_offset_);
  list_vbar_->setValue(canvas_y_offset_);

  canvas_width_  = w_->width ();
  canvas_height_ = w_->height();

  //---

  window_->resize();
}

void
CBrowserIFace::
draw()
{
  setBusy();

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

  setReady();
}

void
CBrowserIFace::
mousePress(int, int)
{
}

void
CBrowserIFace::
mouseMotion(int x, int y)
{
  int x1 = x + canvas_x_offset_;
  int y1 = y + canvas_y_offset_;

  CBrowserBox *box = window_->getLayout()->boxAt(CIPoint2D(x1, y1));

  CBrowserObject *obj = dynamic_cast<CBrowserObject *>(box);

  window_->selectSingleObject(obj);

  if (obj)
    objLabel_->setText(obj->typeName().c_str());
  else
    objLabel_->setText("");

  //---

  std::string link_name;

  if (window_->hoverLink(x1, y1, link_name)) {
    w_->setCursor(Qt::PointingHandCursor);
  }
  else {
    w_->setCursor(Qt::ArrowCursor);
  }

  posLabel_->setText(QString("%1,%2").arg(x1).arg(y1));
}

void
CBrowserIFace::
mouseRelease(int x, int y)
{
  window_->activateLink(x + canvas_x_offset_, y + canvas_y_offset_);
}
