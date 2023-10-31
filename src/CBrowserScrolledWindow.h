#ifndef CBrowserScrolledWindow_H
#define CBrowserScrolledWindow_H

#include <CBrowserWindow.h>
#include <CBrowserTypes.h>

#include <CUrl.h>
#include <QFrame>

class CBrowser;
class CBrowserMainWindow;

class QScrollBar;

// scroll window for document window
class CBrowserScrolledWindow : public QFrame, public CBrowserWindowIFace {
  Q_OBJECT

 public:
  explicit CBrowserScrolledWindow(CBrowserMainWindow *mainWindow);

 ~CBrowserScrolledWindow();

  // parent main window
  CBrowserMainWindow *mainWindow() const { return mainWindow_; }

  // child window
  CBrowserWindow *getWindow() const { return window_; }

  //---

  // central widget
  CBrowserWindowWidgetIFace *widget() const override;

  // horizontal scrollbar
  QScrollBar *hbar() const { return list_hbar_; }
  // vertical scrollbar
  QScrollBar *vbar() const { return list_vbar_; }

  //---

  // visible canvas (central widget) size and offset
  int getCanvasWidth() const override { return canvas_width_; }
  void setCanvasWidth(int w) { canvas_width_ = w; }

  int getCanvasHeight() const override { return canvas_height_; }
  void setCanvasHeight(int h) { canvas_height_ = h; }

  int getCanvasXOffset() const override { return canvas_x_offset_; }
  void setCanvasXOffset(int x) { canvas_x_offset_ = x; }

  int getCanvasYOffset() const override { return canvas_y_offset_; }
  void setCanvasYOffset(int y) { canvas_y_offset_ = y; }

  //---

  void setDocument(const CUrl &url) override { window_->setDocument(url); }

  void setSize(int width, int height);

  void scrollTo(int x, int y) override;

  void expose();

  void resize() override;

  void draw() override { window_->draw(); }

  void mousePress  (int x, int y) override;
  void mouseMotion (int x, int y) override;
  void mouseRelease(int x, int y) override;

  void mouseWheel(int d) override;

  void saveImage(const QString &filename) override { return window_->saveImage(filename); }

  void print() override { return window_->print(); }

  void goBack() override { return window_->goBack(); }
  void goForward() override { return window_->goForward(); }

 public slots:
  void hscrollProc();
  void vscrollProc();

 private:
  CBrowserMainWindow* mainWindow_ { nullptr };
  CBrowserWindow*     window_     { nullptr };

  QScrollBar* list_hbar_ { nullptr };
  QScrollBar* list_vbar_ { nullptr };

  int canvas_width_    { 0 };
  int canvas_height_   { 0 };
  int canvas_x_offset_ { 0 };
  int canvas_y_offset_ { 0 };
};

#endif
