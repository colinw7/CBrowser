#include <CQMainWindow.h>
#include <CImageLib.h>
#include <CFont.h>

#include "CBrowserTypesI.h"

class CBrowserWindow;
class CBrowserCanvas;
class CBrowserGraphics;
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

  CBrowserCanvas *getCanvas() const { return canvas_; }

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
  void quitProc();

  void goBackProc();
  void goForwardProc();

  void hscrollProc();
  void vscrollProc();

 private:
  CBrowserWindow* window_;
  CBrowserCanvas* canvas_;
  QScrollArea*    list_;
  QScrollBar*     list_hbar_;
  QScrollBar*     list_vbar_;
  QLabel*         message_;
  CQMenu*         historyMenu_;
  int             canvas_x_offset_;
  int             canvas_y_offset_;
  int             canvas_width_;
  int             canvas_height_;
};

class CBrowserCanvas : public QWidget {
 public:
  CBrowserCanvas(CBrowserIFace *iface);

  void paintEvent(QPaintEvent *);
  void resizeEvent(QResizeEvent *);

  void mousePressEvent(QMouseEvent *e);
  void mouseMotionEvent(QMouseEvent *e);
  void mouseReleaseEvent(QMouseEvent *e);

  void keyPressEvent(QKeyEvent *e);
  void keyReleaseEvent(QKeyEvent *e);

  void startDoubleBuffer();
  void endDoubleBuffer();

  void setXDevice();
  void setPSDevice(double xmin, double ymin, double xmax, double ymax);

  void clear(const CRGBA &bg);

  void drawImage(int x, int y, const CImagePtr &image);
  void drawRectangle(int x1, int y1, int x2, int y2);
  void fillRectangle(int x1, int y1, int x2, int y2);
  void drawCircle(int x, int y, int r);
  void fillCircle(int x, int y, int r);
  void drawLine(int x1, int y1, int x2, int y2);
  void drawString(int x, int y, const std::string &str);
  void drawOutline(int x, int y, int width, int height, const std::string &color_name);
  void drawBorder(int x, int y, int width, int height, CBrowserBorderType type);
  void drawHRule(int x1, int x2, int y, int height);
  void setForeground(const CRGBA &fg);
  void setFont(CFontPtr font);

 private:
  CBrowserIFace    *iface_ { nullptr };
  CBrowserGraphics *graphics_ { nullptr };
};
