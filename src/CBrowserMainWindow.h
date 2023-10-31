#ifndef CBrowserMainWindow_H
#define CBrowserMainWindow_H

#include <CBrowserTypes.h>
#include <CBrowserIFace.h>
#include <CQMainWindow.h>

class CBrowser;
class CBrowserWindowIFace;
class CBrowserDomTreeDlg;
class CBrowserCSSTreeDlg;
class CBrowserWebView;

class CQMenu;
class CQToolBar;
class CQMenuItem;
class QLineEdit;
class QScrollArea;
class QScrollBar;
class QLabel;

// Main Window with interface elements, and document scrolled window
class CBrowserMainWindow : public CQMainWindow, public CBrowserIFace {
  Q_OBJECT

 public:
  CBrowserMainWindow(CBrowser *browser);
 ~CBrowserMainWindow();

  void init();

  // add new document (and window)
  void addDocument(const CUrl &url) override;
  // add new document (in current window)
  void setDocument(const CUrl &url) override;

  // add new window
  CBrowserWindowIFace *addWindow();
  // get current window
  CBrowserWindowIFace *currentWindow() const;

  void addHistoryItem(const CUrl &url) override;

  void show() override { CQMainWindow::show(); }

  void saveImage(const QString &filename);

  void setTitle (const QString &title ) override;
  void setStatus(const QString &status) override;

  void errorDialog(const QString &msg) override;

  void setBusy () override;
  void setReady() override;

  void setObjText(const QString &text) override;
  void setPosText(const QString &text) override;

 private:
  QWidget *createCentralWidget() override;

  void createMenus() override;
  void createToolBars() override;
  void createStatusBar() override;

  QLabel* objLabel() const { return objLabel_; }
  QLabel* posLabel() const { return posLabel_; }

 public slots:
  void inputSlot();

  void updateTitles() override;

  void newProc();
  void readProc();
  void printProc();
  void saveImageProc();
  void jsProc();
  void domProc();
  void cssProc();
  void webProc();

  void goBackProc();
  void goForwardProc();

  void viewBoxesProc();
  void mouseOverProc();

  void quitProc();

 private:
  using Windows = std::vector<CBrowserWindowIFace *>;

  Windows windows_;

  QLineEdit*          input_       { nullptr };
  QTabWidget*         tab_         { nullptr };
  QLabel*             message_     { nullptr };
  CQMenu*             historyMenu_ { nullptr };
  QLabel*             objLabel_    { nullptr };
  QLabel*             posLabel_    { nullptr };
  CQJDialog*          jsDlg_       { nullptr };
  CBrowserDomTreeDlg* domDlg_      { nullptr };
  CBrowserCSSTreeDlg* cssDlg_      { nullptr };
  CBrowserWebView*    webView_     { nullptr };
  CQToolBar*          toolbar_     { nullptr };
  CQMenuItem*         jsMenuItem_  { nullptr };
  CQMenuItem*         domMenuItem_ { nullptr };
  CQMenuItem*         cssMenuItem_ { nullptr };
  CQMenuItem*         webMenuItem_ { nullptr };
};

#endif
