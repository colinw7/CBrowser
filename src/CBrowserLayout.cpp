#include <CBrowserLayout.h>
#include <CBrowserWindow.h>
#include <CBrowserBox.h>

CBrowserLayout::
CBrowserLayout(CBrowserWindowIFace *window) :
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

  auto *parent = currentBox();

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
layout(CBrowserBox *root, const CIBBox2D &bbox)
{
  if (! root)
    return;

  root->setX(bbox.getXMin());
  root->setY(bbox.getYMin());

  root->setSize(bbox.getWidth(), 0);

  root->layout();
}

void
CBrowserLayout::
render(int dx, int dy)
{
  if (! root_)
    return;

  root_->setHierVisible(true);

  root_->render(dx, dy);
}

CBrowserBox *
CBrowserLayout::
boxAt(const CIPoint2D &p)
{
  if (! root_)
    return nullptr;

  CBrowserBox *box  = nullptr;
  double       area = 0.0;

  root_->boxAt(p, box, area);

  return box;
}
