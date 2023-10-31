#ifndef CBrowserWindow_H
#define CBrowserWindow_H

#include <CBrowserWindowIFace.h>

class CBrowserScrolledWindow;

//---

// window for document display
class CBrowserWindow : public CBrowserWindowIFace {
 public:
  explicit CBrowserWindow(CBrowser *browser);

 ~CBrowserWindow() override;

  //---

  bool isAutoExit() const override { return true; }

  //---

  void setScrolledWindow(CBrowserScrolledWindow *swindow);

  CBrowserWindowWidgetIFace *makeWidget() override;

  //---

  void redraw() override;

  void propagateSize(int w, int h) override;

  //---

  int getCanvasXOffset() const override;
  int getCanvasYOffset() const override;

  int getCanvasWidth() const override;
  int getCanvasHeight() const override;

  //---

  void scrollTo(int x, int y) override;

 private:
  CBrowserScrolledWindow* swindow_ { nullptr };
};

#endif
