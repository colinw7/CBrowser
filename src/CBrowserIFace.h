#ifndef CBrowserIFace_H
#define CBrowserIFace_H

#include <CBrowserTypes.h>
#include <CQMainWindow.h>

class CBrowserScrolledWindow;
class CBrowserDomTreeDlg;
class QLineEdit;
class QScrollArea;
class QScrollBar;
class QLabel;
class CQMenu;
class CQToolBar;
class CQMenuItem;

class CBrowserIFace : public CQMainWindow {
  Q_OBJECT

 public:
  CBrowserIFace();
 ~CBrowserIFace();

  void init();

  CBrowserScrolledWindow *addWindow();

  CBrowserScrolledWindow *currentWindow() const;

  QLabel* objLabel() const { return objLabel_; }
  QLabel* posLabel() const { return posLabel_; }

  void addHistoryItem(const std::string &item);

  void addDocument(const std::string &filename);
  void setDocument(const std::string &filename);

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
  CQToolBar*          toolbar_ { nullptr };
  CQMenuItem*         jsMenuItem_ { nullptr };
  CQMenuItem*         domMenuItem_ { nullptr };
};

#endif
