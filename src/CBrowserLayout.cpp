#include <CBrowserLayout.h>
#include <CBrowserWindow.h>
#include <CBrowserBox.h>

CBrowserLayout::
CBrowserLayout(CBrowserWindow *window) :
 window_(window)
{
}

void
CBrowserLayout::
startBox(CBrowserBox *box)
{
  if (! root_)
    root_ = box;

  //---

  CBrowserBox *parent = currentBox();

  boxes_.push_back(box);

  if (parent) {
    box->setParent(parent);

    parent->addChild(box);
  }
}

void
CBrowserLayout::
endBox(CBrowserBox *box)
{
  assert(! boxes_.empty());

  assert(box == currentBox());

  boxes_.pop_back();
}

CBrowserBox *
CBrowserLayout::
currentBox() const
{
  if (boxes_.empty())
    return nullptr;

  return boxes_.back();
}

void
CBrowserLayout::
layout(const CIBBox2D &bbox)
{
  root_->setX(bbox.getXMin());
  root_->setY(bbox.getYMin());

  root_->setSize(bbox.getWidth(), 0);

  root_->layout();
}

void
CBrowserLayout::
render(int dx, int dy)
{
  root_->setHierVisible(true);

  root_->render(dx, dy);
}

CBrowserBox *
CBrowserLayout::
boxAt(const CIPoint2D &p)
{
  CBrowserBox *box  = nullptr;
  double       area = 0.0;

  root_->boxAt(p, box, area);

  return box;
}
