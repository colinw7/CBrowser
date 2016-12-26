#include <CBrowserSymbol.h>
#include <CBrowserText.h>
#include <CBrowserWindow.h>

CBrowserSymbol::
CBrowserSymbol(CBrowserWindow *window, Type type) :
 CBrowserObject(window, CHtmlTagId::SYMBOL), type_(type), width_(0), ascent_(descent_)
{
  window_->getTextSize("X", &width_, &ascent_, &descent_);
}

CBrowserSymbol::
~CBrowserSymbol()
{
}

void
CBrowserSymbol::
initLayout()
{
  window_->addCellRedrawData(this);
}

void
CBrowserSymbol::
termLayout()
{
}

void
CBrowserSymbol::
format(CHtmlLayoutMgr *)
{
  window_->newSubCellBelow(false);

  window_->updateSubCellHeight(ascent_, descent_);
  window_->updateSubCellWidth (width_);

  //---

  window_->addSubCellRedrawData(this);
}

void
CBrowserSymbol::
draw(CHtmlLayoutMgr *, const CHtmlLayoutRegion &region)
{
  //CHtmlLayoutSubCell *sub_cell = window_->getCurrentSubCell();

  int cx = region.x + region.getWidth()/2;
  int cy = region.y + ascent_/2;

  //---

  int size = std::min(region.getWidth() - 1, ascent_ - 1);

  //---

  switch (type_) {
    case Type::DISC:
      window_->fillCircle(cx, cy, size/2); // x, y, r

      break;
    case Type::CIRCLE:
      window_->drawCircle(cx, cy, size/2); // x, y, r

      break;
    case Type::BLOCK:
      window_->fillRectangle(cx - size/2, cy - size/2, size, size); // x1, y1, x2, y2

      break;
    case Type::SQUARE:
      window_->drawRectangle(cx - size/2, cy - size/2, size, size); // x1, y1, x2, y2

      break;
    default:
      break;
  }

  //---

  //region.x += width_;

  //---

  if (isSelected())
    window_->drawSelected(region.getX(), region.getY(), region.getWidth(), region.getHeight());
}
