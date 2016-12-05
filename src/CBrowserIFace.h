#ifndef CBrowserIFace_H
#define CBrowserIFace_H

#include <CBrowserTypes.h>
#include <CJavaScript.h>

#include <CQMainWindow.h>
#include <CImageLib.h>
#include <CFont.h>

class CBrowserDomTreeDlg;
class QScrollArea;
class QScrollBar;
class QLabel;
class CQMenu;

class CBrowserIFace : public CQMainWindow {
  Q_OBJECT

 public:
  CBrowserIFace();
 ~CBrowserIFace();

  void init();

  CBrowserWindow *getWindow() const { return window_; }

  CBrowserWindowWidget *widget() const { return w_; }

  int getCanvasXOffset() const { return canvas_x_offset_; }
  int getCanvasYOffset() const { return canvas_y_offset_; }

  int getCanvasWidth () const { return canvas_width_ ; }
  int getCanvasHeight() const { return canvas_height_; }

  void addHistoryItem(const std::string &item);

  void setBusy ();
  void setReady();

  void expose();

  void print(double xmin, double ymin, double xmax, double ymax);

  void setSize(int width, int height);

  void scrollTo(int x, int y);

  void setTitle(const std::string &title);

  void setStatus(const std::string &status);

  void errorDialog(const std::string &msg);

  void resize();
  void draw();

  void mouseMotion(int x, int y);
  void mouseRelease(int x, int y);

 private:
  QWidget *createCentralWidget();

  void createMenus();
  void createToolBars();
  void createStatusBar();

 public slots:
  void newProc();
  void readProc();
  void printProc();
  void jsProc();
  void domProc();
  void quitProc();

  void goBackProc();
  void goForwardProc();

  void hscrollProc();
  void vscrollProc();

 private:
  CBrowserWindow*       window_ { nullptr };
  CBrowserWindowWidget* w_ { nullptr };
  QScrollArea*          list_ { nullptr };
  QScrollBar*           list_hbar_ { nullptr };
  QScrollBar*           list_vbar_ { nullptr };
  QLabel*               message_ { nullptr };
  CQMenu*               historyMenu_ { nullptr };
  int                   canvas_x_offset_ { 0 };
  int                   canvas_y_offset_ { 0 };
  int                   canvas_width_ { 0 };
  int                   canvas_height_ { 0 };
  CQJDialog*            jsDlg_ { nullptr };
  CBrowserDomTreeDlg*   domDlg_ { nullptr };
};

#endif
