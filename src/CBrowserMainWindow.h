#ifndef CBrowserMainWindow_H
#define CBrowserMainWindow_H

#include <CBrowserTypes.h>
#include <CQMainWindow.h>
#include <CUrl.h>

class CBrowserScrolledWindow;
class CBrowserDomTreeDlg;
class CBrowserCSSTreeDlg;
class CBrowserWebView;
class QLineEdit;
class QScrollArea;
class QScrollBar;
class QLabel;
class CQMenu;
class CQToolBar;
class CQMenuItem;

class CBrowserMainWindow : public CQMainWindow {
  Q_OBJECT

 public:
  CBrowserMainWindow();
 ~CBrowserMainWindow();

  void init();

  CBrowserScrolledWindow *addWindow();

  CBrowserScrolledWindow *currentWindow() const;

  QLabel* objLabel() const { return objLabel_; }
  QLabel* posLabel() const { return posLabel_; }

  void addHistoryItem(const CUrl &url);

  void addDocument(const CUrl &url);
  void setDocument(const CUrl &url);

  void saveImage(const std::string &filename);

  void setTitle(const std::string &title);

  void setStatus(const std::string &status);

  void errorDialog(const std::string &msg);

  void setBusy ();
  void setReady();

 private:
  QWidget *createCentralWidget();

  void createMenus();
  void createToolBars();
  void createStatusBar();

 public slots:
  void inputSlot();

  void updateTitles();

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
  typedef std::vector<CBrowserScrolledWindow *> Windows;

  QLineEdit*          input_ { nullptr };
  QTabWidget*         tab_ { nullptr };
  Windows             windows_;
  QLabel*             message_ { nullptr };
  CQMenu*             historyMenu_ { nullptr };
  QLabel*             objLabel_ { nullptr };
  QLabel*             posLabel_ { nullptr };
  CQJDialog*          jsDlg_ { nullptr };
  CBrowserDomTreeDlg* domDlg_ { nullptr };
  CBrowserCSSTreeDlg* cssDlg_ { nullptr };
  CBrowserWebView*    webView_ { nullptr };
  CQToolBar*          toolbar_ { nullptr };
  CQMenuItem*         jsMenuItem_ { nullptr };
  CQMenuItem*         domMenuItem_ { nullptr };
  CQMenuItem*         cssMenuItem_ { nullptr };
  CQMenuItem*         webMenuItem_ { nullptr };
};

#endif
