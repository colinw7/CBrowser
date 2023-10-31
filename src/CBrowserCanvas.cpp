#include <CBrowserCanvas.h>
#include <CQJCanvasWidget.h>
#include <CQJCanvas.h>
#include <CBrowserWindowWidget.h>
#include <CBrowserMain.h>
#include <CBrowserText.h>
#include <CBrowserLink.h>
#include <CBrowserWindow.h>
#include <CBrowserDocument.h>
#include <cstring>

CBrowserCanvas::
CBrowserCanvas(CBrowserWindowIFace *window) :
 CBrowserObject(window, CHtmlTagId::CANVAS), iface_(this)
{
}

CBrowserCanvas::
~CBrowserCanvas()
{
}

void
CBrowserCanvas::
init()
{
  CBrowserObject::init();
}

void
CBrowserCanvas::
setNameValue(const std::string &name, const std::string &value)
{
  std::string lname  = CStrUtil::toLower(name);

  if      (lname == "width")
    CBrowserObject::setWidth(CBrowserUnitValue(value));
  else if (lname == "height")
    CBrowserObject::setHeight(CBrowserUnitValue(value));
  else
    CBrowserObject::setNameValue(name, value);
}

void
CBrowserCanvas::
setWidth(int w)
{
  CBrowserObject::setWidth(CBrowserUnitValue(w));

  window_->recalc();
}

void
CBrowserCanvas::
setHeight(int h)
{
  CBrowserObject::setHeight(CBrowserUnitValue(h));

  window_->recalc();
}

CBrowserRegion
CBrowserCanvas::
calcRegion() const
{
  int hspace = 0, vspace = 0;

  int width   = this->width ().pxValue() + 2*hspace;
  int ascent  = this->height().pxValue() + vspace;
  int descent = vspace;

  return CBrowserRegion(width, ascent, descent);
}

void
CBrowserCanvas::
show()
{
  if (canvasWidget_)
    canvasWidget_->show();
}

void
CBrowserCanvas::
hide()
{
  if (canvasWidget_)
    canvasWidget_->hide();
}

void
CBrowserCanvas::
draw(const CTextBox &region)
{
  region_ = region;

  if (canvasWidget_) {
    canvasWidget_->move  (region_.x(), region_.y());
    canvasWidget_->resize(width().pxValue(), height().pxValue());
  }
}

CQJCanvasWidget *
CBrowserCanvas::
canvasWidget() const
{
  if (! canvasWidget_) {
    const_cast<CBrowserCanvas *>(this)->createWidget();
  }

  return canvasWidget_;
}

void
CBrowserCanvas::
createWidget()
{
  assert(! canvasWidget_);

  canvasWidget_ = new CQJCanvasWidget(canvas_);

  auto *window1 = dynamic_cast<CBrowserWindow *>(window_);

  if (window1) {
    auto *windowWidget1 = dynamic_cast<CBrowserWindowWidget *>(window1->widget());

    if (windowWidget1)
      canvasWidget_->setParent(windowWidget1);
  }

  canvasWidget_->setObjectName("canvas");

  double w = width ().pxValue();
  double h = height().pxValue();

  canvasWidget_->resize(w, h);

  canvasWidget_->updateSize(w, h);
}

void
CBrowserCanvas::
update()
{
  if (canvasWidget_)
    canvasWidget_->update();
}

CQJHtmlObj *
CBrowserCanvas::
createJObj(CJavaScript *js)
{
  canvas_ = new CQJCanvas(js, iface(), CBrowserObject::iface());

  return canvas_;
}
