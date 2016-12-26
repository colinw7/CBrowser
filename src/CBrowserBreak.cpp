#include <CBrowserBreak.h>
#include <CBrowserWindow.h>

CBrowserBreak::
CBrowserBreak(CBrowserWindow *window, const CBrowserBreakData &data) :
 CBrowserObject(window, CHtmlTagId::BR), data_(data)
{
}

CBrowserBreak::
~CBrowserBreak()
{
}

void
CBrowserBreak::
initLayout()
{
  window_->addCellRedrawData(this);
}

void
CBrowserBreak::
termLayout()
{
}

void
CBrowserBreak::
format(CHtmlLayoutMgr *)
{
  CHtmlLayoutSubCell *sub_cell = window_->newSubCellBelow(false);

  sub_cell->setClear(data_.clear);

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

//------

CBrowserWbr::
CBrowserWbr(CBrowserWindow *window) :
 CBrowserObject(window, CHtmlTagId::WBR)
{
}

CBrowserWbr::
~CBrowserWbr()
{
}

void
CBrowserWbr::
initLayout()
{
}

void
CBrowserWbr::
termLayout()
{
}
