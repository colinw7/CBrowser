#include <CBrowserMainWindow.h>
#include <CBrowserWindowWidget.h>
#include <CBrowserWindow.h>
#include <CBrowserScrolledWindow.h>
#include <CBrowserMain.h>
#include <CBrowserGraphics.h>
#include <CQJavaScript.h>
#include <CBrowserDomTree.h>
#include <CBrowserCSSTree.h>
#include <CBrowserWebView.h>
#include <CBrowserObject.h>
#include <CBrowserLayout.h>
#include <CBrowserBox.h>

#include <CQJDocument.h>
#include <CQJWindow.h>
#include <CQJEvent.h>
#include <CQJDialog.h>
#include <CQMenu.h>
#include <CQToolBar.h>
#include <CDir.h>
#include <CEnv.h>

#include <QVBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QFileDialog>
#include <QMessageBox>
#include <QStatusBar>

#include <svg/dom_svg.h>
#include <svg/css_svg.h>
#include <svg/javascript_svg.h>

CBrowserMainWindow::
CBrowserMainWindow() :
 CQMainWindow("CBrowser")
{
  QWidget::resize(1000, 1200);
}

CBrowserMainWindow::
~CBrowserMainWindow()
{
}

void
CBrowserMainWindow::
init()
{
  CQMainWindow::init();
}

QWidget *
CBrowserMainWindow::
createCentralWidget()
{
  QWidget *widget = new QWidget;

  widget->setObjectName("iface");

  QVBoxLayout *layout = new QVBoxLayout(widget);

  input_ = new QLineEdit;

  input_->setObjectName("input");
  input_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

  layout->addWidget(input_);

  connect(input_, SIGNAL(returnPressed()), this, SLOT(inputSlot()));

  //---

  tab_ = new QTabWidget;

  tab_->setObjectName("tab");
  tab_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  connect(tab_, SIGNAL(currentChanged(int)), this, SLOT(updateTitles()));

  layout->addWidget(tab_);

  //---

  addWindow();

  return widget;
}

CBrowserScrolledWindow *
CBrowserMainWindow::
addWindow()
{
  CBrowserScrolledWindow *window = new CBrowserScrolledWindow(this);

  tab_->addTab(window, "");

  tab_->setCurrentIndex(tab_->count() - 1);

  windows_.push_back(window);

  return window;
}

CBrowserScrolledWindow *
CBrowserMainWindow::
currentWindow() const
{
  int i = tab_->currentIndex();

  if (i >= 0 && i < int(windows_.size()))
    return windows_[i];

  return nullptr;
}

void
CBrowserMainWindow::
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

  jsMenuItem_ = new CQMenuItem(fileMenu, "&JavaScript");
  jsMenuItem_->setIcon(CQPixmapCacheInst->getIcon("JAVASCRIPT"));
  jsMenuItem_->connect(this, SLOT(jsProc()));

  domMenuItem_ = new CQMenuItem(fileMenu, "&DOM");
  domMenuItem_->setIcon(CQPixmapCacheInst->getIcon("DOM"));
  domMenuItem_->connect(this, SLOT(domProc()));

  cssMenuItem_ = new CQMenuItem(fileMenu, "&CSS");
  cssMenuItem_->setIcon(CQPixmapCacheInst->getIcon("CSS"));
  cssMenuItem_->connect(this, SLOT(cssProc()));

  webMenuItem_ = new CQMenuItem(fileMenu, "&Web");
  webMenuItem_->connect(this, SLOT(webProc()));

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
  viewBoxes->setCheckable(true);
  viewBoxes->connect(this, SLOT(viewBoxesProc()));

  CQMenuItem *mouseOver = new CQMenuItem(viewMenu, "&Mouse Over");
  mouseOver->setCheckable(true);
  mouseOver->connect(this, SLOT(mouseOverProc()));

  //---

  historyMenu_ = new CQMenu(this, "&History");

  //---

  CQMenu *helpMenu = new CQMenu(this, "&Help");

  new CQMenuItem(helpMenu, "&Help");
}

void
CBrowserMainWindow::
createToolBars()
{
  toolbar_ = new CQToolBar(this, "Tools");

  toolbar_->addItem(jsMenuItem_);
  toolbar_->addItem(domMenuItem_);
  toolbar_->addItem(cssMenuItem_);
}

void
CBrowserMainWindow::
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
CBrowserMainWindow::
inputSlot()
{
  QString text = input_->text();
  if (text == "") return;

  CUrl url(text.toStdString());

  setDocument(url);
}

void
CBrowserMainWindow::
addHistoryItem(const CUrl &url)
{
  CQMenuItem *menuItem = new CQMenuItem(historyMenu_, "button");

  menuItem->setName(url.getUrl());
}

void
CBrowserMainWindow::
saveImage(const std::string &filename)
{
  CBrowserScrolledWindow *w = currentWindow();
  if (! w) return;

  w->saveImage(filename);
}

void
CBrowserMainWindow::
setTitle(const std::string &title)
{
  setWindowTitle(title.c_str());
}

void
CBrowserMainWindow::
updateTitles()
{
  setWindowTitle("CBrowser");

  int ind = tab_->currentIndex();

  for (int i = 0; i < tab_->count(); ++i) {
    CBrowserScrolledWindow *w = qobject_cast<CBrowserScrolledWindow *>(tab_->widget(i));
    if (! w) continue;

    CBrowserWindow *window = w->getWindow();

    tab_->setTabText(i, window->filename().c_str());

    if (i == ind)
      setWindowTitle(w->title().c_str());
  }
}

void
CBrowserMainWindow::
setStatus(const std::string &status)
{
  if (status != "")
    message_->setText(status.c_str());
  else
    message_->setText(" ");
}

void
CBrowserMainWindow::
errorDialog(const std::string &msg)
{
  QMessageBox::warning(this, "Error", msg.c_str());
}

void
CBrowserMainWindow::
setBusy()
{
}

void
CBrowserMainWindow::
setReady()
{
}

void
CBrowserMainWindow::
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

  CUrl url("file:://" + file.toStdString());

  addDocument(url);
}

void
CBrowserMainWindow::
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

  CUrl url("file://" + file.toStdString());

  setDocument(url);
}

void
CBrowserMainWindow::
addDocument(const CUrl &url)
{
  CBrowserScrolledWindow *window = addWindow();

  window->setDocument(url);

  updateTitles();
}

void
CBrowserMainWindow::
setDocument(const CUrl &url)
{
  CBrowserScrolledWindow *window = currentWindow();
  if (! window) return;

  window->setDocument(url);

  updateTitles();
}

void
CBrowserMainWindow::
printProc()
{
  CBrowserScrolledWindow *window = currentWindow();
  if (! window) return;

  window->print();
}

void
CBrowserMainWindow::
saveImageProc()
{
  QString fileName =
    QFileDialog::getSaveFileName(this, "Save Image", "image.png",
                                 "Image Files (*.png *.xpm *.jpg)");

  if (fileName.length())
    saveImage(fileName.toStdString());
}

void
CBrowserMainWindow::
jsProc()
{
  if (! jsDlg_)
    jsDlg_ = new CQJDialog(CQJavaScriptInst->js());

  jsDlg_->show();
}

void
CBrowserMainWindow::
domProc()
{
  CBrowserScrolledWindow *swindow = currentWindow();
  if (! swindow) return;

  CBrowserWindow *window = swindow->getWindow();

  if (domDlg_ && domDlg_->window() != window) {
    delete domDlg_;

    domDlg_ = nullptr;
  }

  if (! domDlg_)
    domDlg_ = new CBrowserDomTreeDlg(window);

  domDlg_->show();
}

void
CBrowserMainWindow::
cssProc()
{
  CBrowserScrolledWindow *swindow = currentWindow();
  if (! swindow) return;

  CBrowserWindow *window = swindow->getWindow();

  if (cssDlg_ && cssDlg_->window() != window) {
    delete cssDlg_;

    cssDlg_ = nullptr;
  }

  if (! cssDlg_)
    cssDlg_ = new CBrowserCSSTreeDlg(window);

  cssDlg_->show();
}

void
CBrowserMainWindow::
webProc()
{
  if (! webView_)
    webView_ = new CBrowserWebView;

  CBrowserScrolledWindow *swindow = currentWindow();
  if (! swindow) return;

  webView_->setFilename(swindow->filename());

  webView_->show();
}

void
CBrowserMainWindow::
goBackProc()
{
  CBrowserScrolledWindow *window = currentWindow();
  if (! window) return;

  window->goBack();
}

void
CBrowserMainWindow::
goForwardProc()
{
  CBrowserScrolledWindow *window = currentWindow();
  if (! window) return;

  window->goForward();
}

void
CBrowserMainWindow::
viewBoxesProc()
{
  CBrowserMainInst->setShowBoxes(! CBrowserMainInst->getShowBoxes());
}

void
CBrowserMainWindow::
mouseOverProc()
{
  CBrowserMainInst->setMouseOver(! CBrowserMainInst->getMouseOver());
}

void
CBrowserMainWindow::
quitProc()
{
  close();
}
