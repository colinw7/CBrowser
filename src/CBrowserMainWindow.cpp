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
CBrowserMainWindow(CBrowser *browser) :
 CQMainWindow("CBrowser"), CBrowserIFace(browser)
{
  QWidget::resize(1000, 1200);

  browser->setIFace(this);
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
  auto *widget = new QWidget;

  widget->setObjectName("iface");

  auto *layout = new QVBoxLayout(widget);

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

  (void) addWindow();

  return widget;
}

CBrowserWindowIFace *
CBrowserMainWindow::
addWindow()
{
  auto *swindow = new CBrowserScrolledWindow(this);

  tab_->addTab(swindow, "");

  tab_->setCurrentIndex(tab_->count() - 1);

  windows_.push_back(swindow);

  return swindow;
}

CBrowserWindowIFace *
CBrowserMainWindow::
currentWindow() const
{
  int i = tab_->currentIndex();

  if (i >= 0 && i < int(windows_.size())) {
    auto *swindow = dynamic_cast<CBrowserScrolledWindow *>(windows_[i]);

    return swindow->getWindow();
  }

  return nullptr;
}

void
CBrowserMainWindow::
createMenus()
{
  auto *fileMenu = new CQMenu(this, "&File");

  auto *newMenuItem = new CQMenuItem(fileMenu, "&New");
  newMenuItem->setShortcut("Ctrl+N");
  newMenuItem->connect(this, SLOT(newProc()));

  auto *readMenuItem = new CQMenuItem(fileMenu, "&Read");
  readMenuItem->setShortcut("Ctrl+R");
  readMenuItem->connect(this, SLOT(readProc()));

  auto *printMenuItem = new CQMenuItem(fileMenu, "&Print");
  printMenuItem->connect(this, SLOT(printProc()));

  auto *saveImageMenuItem = new CQMenuItem(fileMenu, "Save &Image");
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

  auto *quitMenuItem = new CQMenuItem(fileMenu, "&Quit");
  quitMenuItem->setShortcut("Ctrl+Q");
  quitMenuItem->connect(this, SLOT(quitProc()));

  //---

  auto *goMenu = new CQMenu(this, "&Go");

  auto *backMenuItem = new CQMenuItem(goMenu, "&Back");

  backMenuItem->connect(this, SLOT(goBackProc()));

  auto *forwardMenuItem = new CQMenuItem(goMenu, "&Forward");

  forwardMenuItem->connect(this, SLOT(goForwardProc()));

  //---

  auto *viewMenu = new CQMenu(this, "&View");

  auto *viewBoxes = new CQMenuItem(viewMenu, "&Boxes");
  viewBoxes->setCheckable(true);
  viewBoxes->connect(this, SLOT(viewBoxesProc()));

  auto *mouseOver = new CQMenuItem(viewMenu, "&Mouse Over");
  mouseOver->setCheckable(true);
  mouseOver->connect(this, SLOT(mouseOverProc()));

  //---

  historyMenu_ = new CQMenu(this, "&History");

  //---

  auto *helpMenu = new CQMenu(this, "&Help");

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
  auto text = input_->text();
  if (text == "") return;

  CUrl url(text.toStdString());

  setDocument(url);
}

void
CBrowserMainWindow::
addHistoryItem(const CUrl &url)
{
  auto *menuItem = new CQMenuItem(historyMenu_, "button");

  menuItem->setName(url.getUrl());
}

void
CBrowserMainWindow::
saveImage(const QString &filename)
{
  auto *window = currentWindow();
  if (! window) return;

  window->saveImage(filename);
}

void
CBrowserMainWindow::
setTitle(const QString &title)
{
  setWindowTitle(title);
}

void
CBrowserMainWindow::
updateTitles()
{
  setWindowTitle("CBrowser");

  int ind = tab_->currentIndex();

  for (int i = 0; i < tab_->count(); ++i) {
    auto *swindow = qobject_cast<CBrowserScrolledWindow *>(tab_->widget(i));
    if (! swindow) continue;

    auto *window = swindow->getWindow();

    tab_->setTabText(i, window->filename());

    if (i == ind)
      setWindowTitle(window->title());
  }
}

void
CBrowserMainWindow::
setStatus(const QString &status)
{
  if (status != "")
    message_->setText(status);
  else
    message_->setText(" ");
}

void
CBrowserMainWindow::
errorDialog(const QString &msg)
{
  QMessageBox::warning(this, "Error", msg);
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
setObjText(const QString &text)
{
  objLabel()->setText(text);
}

void
CBrowserMainWindow::
setPosText(const QString &text)
{
  posLabel()->setText(text);
}

void
CBrowserMainWindow::
newProc()
{
  auto home = CDir::getHome();

  auto directory = home + "/data/html";

  //---

  auto file =
    QFileDialog::getOpenFileName(this, "Select HTML File", directory.c_str(), "*.html");

  if (file == "")
    return;

  //---

  CUrl url("file:://" + file.toStdString());

  (void) addDocument(url);
}

void
CBrowserMainWindow::
readProc()
{
  auto home = CDir::getHome();

  std::string directory = home + "/data/html";

  //---

  auto file =
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
  (void) addWindow();

  setDocument(url);
}

void
CBrowserMainWindow::
setDocument(const CUrl &url)
{
  auto *window = currentWindow();
  if (! window) return;

  window->setDocument(url);

  updateTitles();
}

void
CBrowserMainWindow::
printProc()
{
  auto *window = currentWindow();
  if (! window) return;

  window->print();
}

void
CBrowserMainWindow::
saveImageProc()
{
  auto fileName =
    QFileDialog::getSaveFileName(this, "Save Image", "image.png",
                                 "Image Files (*.png *.xpm *.jpg)");

  if (fileName.length())
    saveImage(fileName);
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
  auto *window = currentWindow();
  if (! window) return;

  if (domDlg_ && domDlg_->window() != window) {
    delete domDlg_;

    domDlg_ = nullptr;
  }

  if (! domDlg_) {
    auto *window1 = dynamic_cast<CBrowserWindow *>(window);

    if (! window1) {
      auto *swindow = dynamic_cast<CBrowserScrolledWindow *>(window);

      window1 = (swindow ? swindow->getWindow() : nullptr);
    }

    domDlg_ = new CBrowserDomTreeDlg(window1);
  }

  domDlg_->show();
}

void
CBrowserMainWindow::
cssProc()
{
  auto *window = currentWindow();
  if (! window) return;

  if (cssDlg_ && cssDlg_->window() != window) {
    delete cssDlg_;

    cssDlg_ = nullptr;
  }

  if (! cssDlg_) {
    auto *window1 = dynamic_cast<CBrowserWindow *>(window);

    if (! window1) {
      auto *swindow = dynamic_cast<CBrowserScrolledWindow *>(window);

      window1 = (swindow ? swindow->getWindow() : nullptr);
    }

    cssDlg_ = new CBrowserCSSTreeDlg(window1);
  }

  cssDlg_->show();
}

void
CBrowserMainWindow::
webProc()
{
  if (! webView_)
    webView_ = new CBrowserWebView;

  auto *window = currentWindow();
  if (! window) return;

  webView_->setFilename(window->filename().toStdString());

  webView_->show();
}

void
CBrowserMainWindow::
goBackProc()
{
  auto *window = currentWindow();
  if (! window) return;

  window->goBack();
}

void
CBrowserMainWindow::
goForwardProc()
{
  auto *window = currentWindow();
  if (! window) return;

  window->goForward();
}

void
CBrowserMainWindow::
viewBoxesProc()
{
  browser()->setShowBoxes(! browser()->getShowBoxes());
}

void
CBrowserMainWindow::
mouseOverProc()
{
  browser()->setMouseOver(! browser()->getMouseOver());
}

void
CBrowserMainWindow::
quitProc()
{
  close();
}
