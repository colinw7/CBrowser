#ifndef CBrowserScrolledWindow_H
#define CBrowserScrolledWindow_H

#include <CBrowserTypes.h>

#include <QFrame>

class QScrollBar;

class CBrowserScrolledWindow : public QFrame {
  Q_OBJECT

 public:
  CBrowserScrolledWindow(CBrowserIFace *iface);

 ~CBrowserScrolledWindow();

  CBrowserIFace *iface() const { return iface_; }

  CBrowserWindow *getWindow() const { return window_; }

  CBrowserWindowWidget *widget() const { return w_; }

  const std::string &title() const { return title_; }
  void setTitle(const std::string &v);

  int getCanvasWidth () const { return canvas_width_ ; }
  int getCanvasHeight() const { return canvas_height_; }

  int getCanvasXOffset() const { return canvas_x_offset_; }
  int getCanvasYOffset() const { return canvas_y_offset_; }

  void setSize(int width, int height);

  void setDocument(const std::string &filename);

  void print();

  void saveImage(const std::string &filename);

  void scrollTo(int x, int y);

  void expose();

  void goBack();
  void goForward();

  void print(double xmin, double ymin, double xmax, double ymax);

  void resize();
  void draw();

  void mousePress  (int x, int y);
  void mouseMotion (int x, int y);
  void mouseRelease(int x, int y);

  void mouseWheel(int d);

 public slots:
  void hscrollProc();
  void vscrollProc();

 private:
  CBrowserIFace*        iface_ { nullptr };
  CBrowserWindow*       window_ { nullptr };
  CBrowserWindowWidget* w_ { nullptr };
  QScrollBar*           list_hbar_ { nullptr };
  QScrollBar*           list_vbar_ { nullptr };
  std::string           title_;
  int                   canvas_width_ { 0 };
  int                   canvas_height_ { 0 };
  int                   canvas_x_offset_ { 0 };
  int                   canvas_y_offset_ { 0 };
};

#endif
