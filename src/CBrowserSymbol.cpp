#include <CBrowserSymbol.h>
#include <CBrowserText.h>
#include <CBrowserWindow.h>

CBrowserSymbol::
CBrowserSymbol(CBrowserWindow *window, CBrowserSymbolType type, int width, int height) :
 CBrowserObject(CHtmlTagId::SYMBOL), window_(window), type_(type), width_(width), height_(height)
{
}

CBrowserSymbol::
~CBrowserSymbol()
{
}

void
CBrowserSymbol::
format(CHtmlLayoutMgr *)
{
  window_->newSubCellBelow(false);

  window_->updateSubCellHeight(height_, 0);
  window_->updateSubCellWidth (width_);

  /*------------*/

  window_->addSubCellRedrawData(this);
}

void
CBrowserSymbol::
draw(CHtmlLayoutMgr *, const CHtmlLayoutRegion &region)
{
  CHtmlLayoutSubCell *sub_cell = window_->getCurrentSubCell();

  int x1 = region.x + width_/2;
  int y1 = region.y + sub_cell->getAscent() - height_/2;

  /*------------*/

  int size = std::min(width_ - 1, height_ - 1);

  /*------------*/

  switch (type_) {
    case CBrowserSymbolType::DISC:
      window_->fillCircle(x1 - size/2, y1 - size/2, size);

      break;
    case CBrowserSymbolType::CIRCLE:
      window_->drawCircle(x1 - size/2, y1 - size/2, size);

      break;
    case CBrowserSymbolType::BLOCK:
      window_->fillRectangle(x1 - size/2, y1 - size/2, size, size);

      break;
    case CBrowserSymbolType::SQUARE:
      window_->drawRectangle(x1 - size/2, y1 - size/2, size, size);

      break;
    default:
      break;
  }

  /*------------*/

  //region.x += width_;
}
