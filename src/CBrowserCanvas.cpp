#include <CBrowserCanvas.h>
#include <CQJCanvasWidget.h>
#include <CBrowserWindowWidget.h>
#include <CBrowserMain.h>
#include <CBrowserText.h>
#include <CBrowserLink.h>
#include <CBrowserWindow.h>
#include <CBrowserDocument.h>
#include <cstring>

CBrowserCanvas::
CBrowserCanvas(CBrowserWindow *window, const CBrowserCanvasData &data) :
 CBrowserObject(window, CHtmlTagId::CANVAS, data), data_(data)
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
}

void
CBrowserCanvas::
setNameValue(const std::string &name, const std::string &value)
{
  std::string lname  = CStrUtil::toLower(name);
//std::string lvalue = CStrUtil::toLower(value);

  if      (lname == "id") {
    data_.id = value;
  }
  else if (lname == "style") {
    data_.style = value;
  }
  else {
    CBrowserObject::setNameValue(name, value);
  }
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

  int width   = this->width ().value() + 2*hspace;
  int ascent  = this->height().value() + vspace;
  int descent = vspace;

  return CBrowserRegion(width, ascent, descent);
}

void
CBrowserCanvas::
draw(const CTextBox &region)
{
  fillBackground(region);

  //---

  region_ = region;

  canvas_->move  (region_.x(), region_.y());
  canvas_->resize(width().value(), height().value());
  //int hspace = 0, vspace = 0;

  //---

  //int x1 = region.x() + hspace;
  //int y1 = region.y() + vspace;

  //region.setX(region.x() + width() + 2*hspace);
}

void
CBrowserCanvas::
createWidget()
{
  if (! canvas_) {
    canvas_ = new CQJCanvasWidget(this, window_->widget());

    canvas_->setObjectName("canvas");

    canvas_->resize(width().value(), height().value());

    canvas_->updateSize(width().value(), height().value());
  }
}

void
CBrowserCanvas::
update()
{
  canvas_->update();
}
