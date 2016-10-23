#include <CBrowserHtmlI.h>

class CBrowserSymbol {
 private:
  CBrowserWindow                     *window_;
  CBrowserSymbolType                  type_;
  int                                 width_;
  int                                 height_;
  CBrowserFormatProc<CBrowserSymbol> *format_proc_;
  CBrowserRedrawProc<CBrowserSymbol> *redraw_proc_;
  CBrowserFreeProc<CBrowserSymbol>   *free_proc_;

 public:
  CBrowserSymbol(CBrowserWindow *window, CBrowserSymbolType type,
                 int width, int height);
 ~CBrowserSymbol();

  void formatProc();
  void redrawProc(int *x, int *y);
  void freeProc();
};

static std::vector<CBrowserSymbol *> draw_symbols;

void
HtmlDrawSymbol(CBrowserWindow *window, CBrowserSymbolType type)
{
  int width;
  int ascent;
  int descent;

  HtmlGetTextSize(window, "X", &width, &ascent, &descent);

  CBrowserSymbol *draw_symbol =
    new CBrowserSymbol(window, type, width, ascent);

  draw_symbols.push_back(draw_symbol);
}

CBrowserSymbol::
CBrowserSymbol(CBrowserWindow *window, CBrowserSymbolType type,
               int width, int height) :
 window_(window), type_(type), width_(width), height_(height)
{
  format_proc_ = new CBrowserFormatProc<CBrowserSymbol>(this);
  redraw_proc_ = NULL;
  free_proc_   = new CBrowserFreeProc<CBrowserSymbol>(this);

  window->addCellRedrawData(format_proc_, free_proc_);
}

CBrowserSymbol::
~CBrowserSymbol()
{
  delete format_proc_;
  delete redraw_proc_;
  delete free_proc_;
}

void
CBrowserSymbol::
formatProc()
{
  CHtmlLayoutSubCell::newCellBelow(window_->getLayoutMgr(), false);

  window_->updateSubCellHeight(height_, 0);
  window_->updateSubCellWidth (width_);

  /*------------*/

  delete redraw_proc_;

  redraw_proc_ = new CBrowserRedrawProc<CBrowserSymbol>(this);

  window_->addSubCellRedrawData(redraw_proc_);
}

void
CBrowserSymbol::
redrawProc(int *x, int *y)
{
  CHtmlLayoutSubCell *sub_cell = window_->getCurrentSubCell();

  int x1 = *x + width_/2;
  int y1 = *y + sub_cell->getAscent() - height_/2;

  /*------------*/

  int size = std::min(width_ - 1, height_ - 1);

  /*------------*/

  switch (type_) {
    case CBROWSER_SYMBOL_TYPE_DISC:
      window_->fillCircle(x1 - size/2, y1 - size/2, size);

      break;
    case CBROWSER_SYMBOL_TYPE_CIRCLE:
      window_->drawCircle(x1 - size/2, y1 - size/2, size);

      break;
    case CBROWSER_SYMBOL_TYPE_BLOCK:
      window_->fillRectangle(x1 - size/2, y1 - size/2, size, size);

      break;
    case CBROWSER_SYMBOL_TYPE_SQUARE:
      window_->drawRectangle(x1 - size/2, y1 - size/2, size, size);

      break;
    default:
      break;
  }

  /*------------*/

  *x += width_;
}

void
CBrowserSymbol::
freeProc()
{
  delete this;
}
