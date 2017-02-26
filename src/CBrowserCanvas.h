#ifndef CBrowserCanvas_H
#define CBrowserCanvas_H

#include <CBrowserObject.h>
#include <CBrowserData.h>
#include <CQJCanvasIFace.h>
#include <CJavaScript.h>
#include <QWidget>
#include <QGradient>
#include <QImage>
#include <QFont>
#include <QColor>
#include <QPen>

class CBrowserCanvas : public CBrowserObject {
 private:
  class IFace : public CQJCanvasIFace {
   public:
    IFace(CBrowserCanvas *canvas) :
     canvas_(canvas) {
    }

    CQJCanvasWidget *canvasWidget() const override {
      return canvas_->canvasWidget();
    }

    int width() const override {
      return canvas_->width().pxValue();
    }

    void setWidth(int w) override {
      return canvas_->setWidth(w);
    }

    int height() const override {
      return canvas_->height().pxValue();
    }

    void setHeight(int h) override {
      return canvas_->setHeight(h);
    }

   private:
    CBrowserCanvas *canvas_ { nullptr };
  };

 public:
  explicit CBrowserCanvas(CBrowserWindow *window);

 ~CBrowserCanvas();

  IFace *iface() { return &iface_; }

  CQJCanvasWidget *canvasWidget() const;

  void setWidth(int w);
  void setHeight(int h);

  void init() override;

  bool layoutChildren() const override { return false; }
  bool renderChildren() const override { return false; }

  void setNameValue(const std::string &name, const std::string &value) override;

  CBrowserRegion calcRegion() const override;

  void show() override;
  void hide() override;

  void draw(const CTextBox &) override;

  void update();

  CQJHtmlObj *createJObj(CJavaScript *js) override;

 private:
  void createWidget();

 private:
  IFace            iface_;
  CQJCanvas*       canvas_ { nullptr };
  CQJCanvasWidget* canvasWidget_ { nullptr };
  CTextBox         region_;
};

#endif
