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
 CBrowserObject(CHtmlTagId::CANVAS), window_(window), data_(data)
{
}

CBrowserCanvas::
~CBrowserCanvas()
{
}

void
CBrowserCanvas::
setWidth(int w)
{
  data_.width = w;

  window_->recalc();
}

void
CBrowserCanvas::
setHeight(int h)
{
  data_.height = h;

  window_->recalc();
}

void
CBrowserCanvas::
format(CHtmlLayoutMgr *)
{
  window_->newSubCellRight(true);

  CHtmlLayoutSubCell *sub_cell = window_->getCurrentSubCell();

  sub_cell->setAlign(CHALIGN_TYPE_LEFT);

  //---

  int hspace = 0, vspace = 0;

  window_->updateSubCellHeight(vspace, height() + vspace);
  window_->updateSubCellWidth (width() + 2*hspace);

  //---

  createWidget();

  //---

  window_->addSubCellRedrawData(this);
}

void
CBrowserCanvas::
draw(CHtmlLayoutMgr *, const CHtmlLayoutRegion &region)
{
  region_ = region;

  canvas_->move  (region_.x, region_.y);
  canvas_->resize(data_.width, data_.height);
  //int hspace = 0, vspace = 0;

  //---

  //CHtmlLayoutSubCell *sub_cell = window_->getCurrentSubCell();

  //int x1 = region.x + hspace;
  //int y1 = region.y + vspace;

  //region.x += width() + 2*hspace;
}

void
CBrowserCanvas::
createWidget()
{
  if (! canvas_) {
    canvas_ = new CQJCanvasWidget(this, window_->widget());

    canvas_->setObjectName("canvas");

    canvas_->resize(data_.width, data_.height);

    canvas_->updateSize(data_.width, data_.height);
  }
}

void
CBrowserCanvas::
update()
{
  canvas_->update();
}
