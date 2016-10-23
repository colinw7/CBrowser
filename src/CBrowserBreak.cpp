#include <CBrowserHtmlI.h>

CBrowserBreak::
CBrowserBreak(CBrowserWindow *window, CHtmlLayoutClearType clear) :
 window_(window), clear_(clear)
{
  format_proc_ = new CBrowserFormatProc<CBrowserBreak>(this);
  redraw_proc_ = NULL;
  free_proc_   = new CBrowserFreeProc<CBrowserBreak>(this);

  window->addCellRedrawData(format_proc_, free_proc_);
}

CBrowserBreak::
~CBrowserBreak()
{
  delete format_proc_;
  delete redraw_proc_;
  delete free_proc_;
}

void
CBrowserBreak::
formatProc()
{
  CHtmlLayoutSubCell *sub_cell =
    CHtmlLayoutSubCell::newCellBelow(window_->getLayoutMgr(), false);

  sub_cell->setClear(clear_);

  //----

  window_->updateSubCellHeight(0, 0);
  window_->updateSubCellWidth (0);

  //----

  delete redraw_proc_;

  redraw_proc_ = new CBrowserRedrawProc<CBrowserBreak>(this);

  window_->addSubCellRedrawData(redraw_proc_);
}

void
CBrowserBreak::
redrawProc(int *, int *)
{
}

void
CBrowserBreak::
freeProc()
{
  delete this;
}
