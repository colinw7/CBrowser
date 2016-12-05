#include <CBrowserBreak.h>
#include <CBrowserWindow.h>

CBrowserBreak::
CBrowserBreak(CBrowserWindow *window, CHtmlLayoutClearType clear) :
 CBrowserObject(CHtmlTagId::BR), window_(window), clear_(clear)
{
}

CBrowserBreak::
~CBrowserBreak()
{
}

void
CBrowserBreak::
format(CHtmlLayoutMgr *)
{
  CHtmlLayoutSubCell *sub_cell = window_->newSubCellBelow(false);

  sub_cell->setClear(clear_);

  //----

  window_->updateSubCellHeight(0, 0);
  window_->updateSubCellWidth (0);

  //----

  window_->addSubCellRedrawData(this);
}

void
CBrowserBreak::
draw(CHtmlLayoutMgr *, const CHtmlLayoutRegion &)
{
}
