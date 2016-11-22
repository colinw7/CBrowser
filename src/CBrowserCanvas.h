#ifndef CBrowserCanvas_H
#define CBrowserCanvas_H

#include <CBrowserObject.h>
#include <CBrowserData.h>
#include <CJavaScript.h>
#include <QWidget>
#include <QGradient>
#include <QImage>
#include <QFont>
#include <QColor>
#include <QPen>

class CBrowserCanvas : public CBrowserObject {
 public:
  CBrowserCanvas(CBrowserWindow *window, const CBrowserCanvasData &data);
 ~CBrowserCanvas();

  CBrowserWindow *window() const { return window_; }

  CBrowserCanvasWidget *canvas() const { return canvas_; }

  int width () const { return data_.width ; }
  int height() const { return data_.height; }

  void format(CHtmlLayoutMgr *) override;

  void draw(CHtmlLayoutMgr *, const CHtmlLayoutRegion &) override;

  void update();

  void createWidget();

 private:
  CBrowserWindow*       window_ { nullptr };
  CBrowserCanvasData    data_;
  CBrowserCanvasWidget* canvas_ { nullptr };
  CHtmlLayoutRegion     region_;
};

#endif
