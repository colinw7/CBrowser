#include <CBrowserHtmlI.h>

class CBrowserRule {
 private:
  CBrowserWindow                   *window_;
  CHAlignType                       align_;
  int                               shade_;
  int                               size_;
  int                               width_;
  int                               unit_;
  CBrowserFormatProc<CBrowserRule> *format_proc_;
  CBrowserRedrawProc<CBrowserRule> *redraw_proc_;
  CBrowserFreeProc<CBrowserRule>   *free_proc_;

 public:
  CBrowserRule(CBrowserWindow *window, CHAlignType align, int shade,
               int size, int width, int unit);
 ~CBrowserRule();

  void formatProc();
  void redrawProc(int *x, int *y);
  void freeProc();
};

static CHAlignType rule_align = CHALIGN_TYPE_CENTER;
static bool        rule_shade = true;
static int         rule_size  = -1;
static int         rule_width = -1;
static int         rule_unit  = UNIT_PIXEL;

static std::vector<CBrowserRule *> draw_rules;

void
HtmlSetRuleAlign(CHAlignType align)
{
  rule_align = align;
}

void
HtmlSetRuleShade(int shade)
{
  rule_shade = shade;
}

void
HtmlSetRuleSize(int size)
{
  rule_size = size;
}

void
HtmlSetRuleWidth(int width, int unit)
{
  rule_width = width;
  rule_unit  = unit;
}

void
HtmlDrawRule(CBrowserWindow *window)
{
  CHtmlLayoutCell::newCellBelow(window->getLayoutMgr());

  /*-------------*/

  CBrowserRule *draw_rule =
    new CBrowserRule(window, rule_align, rule_shade, rule_size,
                     rule_width, rule_unit);

  draw_rules.push_back(draw_rule);

  /*-------------*/

  CHtmlLayoutCell::newCellBelow(window->getLayoutMgr());
}

CBrowserRule::
CBrowserRule(CBrowserWindow *window, CHAlignType align, int shade,
             int size, int width, int unit) :
 window_(window), align_(align), shade_(shade), size_(size),
 width_(width), unit_(unit) {
  size_ = std::max(size_, 2);

  format_proc_ = new CBrowserFormatProc<CBrowserRule>(this);
  redraw_proc_ = NULL;
  free_proc_   = new CBrowserFreeProc<CBrowserRule>(this);

  window->addCellRedrawData(format_proc_, free_proc_);
}

CBrowserRule::
~CBrowserRule()
{
  delete format_proc_;
  delete redraw_proc_;
  delete free_proc_;
}

void
CBrowserRule::
formatProc()
{
  CHtmlLayoutSubCell::newCellBelow(window_->getLayoutMgr(), false);

  int ascent;

  if (size_ > 14)
    ascent = size_ + 2;
  else
    ascent = 16;

  window_->updateSubCellHeight(ascent, 0);

  if (width_ > 0 && unit_ == UNIT_PIXEL)
    window_->updateSubCellWidth(width_);
  else
    window_->updateSubCellWidth(window_->getCurrentArea()->getWidth());

  /*-------------*/

  delete redraw_proc_;

  redraw_proc_ = new CBrowserRedrawProc<CBrowserRule>(this);

  window_->addSubCellRedrawData(redraw_proc_);
}

void
CBrowserRule::
redrawProc(int *x, int *y)
{
  CHtmlLayoutSubCell *sub_cell = window_->getCurrentSubCell();

  int width;

  if      (width_ == -1)
    width = sub_cell->getWidth();
  else if (unit_ == UNIT_PERCENT)
    width = (int) (width_* sub_cell->getWidth()/100.0);
  else
    width = width_;

  int x1 = *x;

  if      (align_ == CHALIGN_TYPE_CENTER)
    x1 += (window_->getCurrentArea()->getWidth() - width)/2;
  else if (align_ == CHALIGN_TYPE_RIGHT)
    x1 += window_->getCurrentArea()->getWidth() - width;

  int x2 = x1 + width;

  int y1 = *y + sub_cell->getAscent()/2;

  /*---------*/

  if (shade_)
    window_->drawHRule(x1, x2, y1, size_);
  else {
    window_->setForeground(window_->getFg());

    window_->fillRectangle(x1, y1, x2 - x1 + 1, size_);
  }
}

void
CBrowserRule::
freeProc()
{
  delete this;
}
