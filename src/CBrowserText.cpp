#include <CBrowserHtmlI.h>

enum CBrowserTextPlaceType {
  CBROWSER_TEXT_PLACE_TYPE_NORMAL,
  CBROWSER_TEXT_PLACE_TYPE_SUBSCRIPT,
  CBROWSER_TEXT_PLACE_TYPE_SUPERSCRIPT
};

class CBrowserText {
 private:
  CBrowserWindow                   *window_;
  std::string                       text_;
  CFontPtr                          font_;
  CRGBA                             color_;
  bool                              underline_;
  bool                              strike_;
  CBrowserTextPlaceType             place_;
  bool                              breakup_;
  bool                              format_;
  CBrowserLink                     *link_;
  CBrowserFormatProc<CBrowserText> *format_proc_;
  CBrowserRedrawProc<CBrowserText> *redraw_proc_;
  CBrowserFreeProc<CBrowserText>   *free_proc_;

 public:
  CBrowserText(CBrowserWindow *window, const std::string &text);

  CBrowserText(CBrowserWindow *window, const CBrowserText &draw_text, const std::string &text);
 ~CBrowserText();

  CFontPtr getFont() const { return font_; }

  void formatProc();
  void redrawProc(int *x, int *y);
  void freeProc();
};

class CBrowserLabel {
 private:
  CBrowserWindow                    *window_;
  std::string                        text_;
  int                                width_;
  CHAlignType                        align_;
  CFontPtr                           font_;
  CRGBA                              color_;
  CBrowserFormatProc<CBrowserLabel> *format_proc_;
  CBrowserRedrawProc<CBrowserLabel> *redraw_proc_;
  CBrowserFreeProc<CBrowserLabel>   *free_proc_;

 public:
  CBrowserLabel(CBrowserWindow *window, const std::string &text, int width, CHAlignType align);
 ~CBrowserLabel();

  void formatProc();
  void redrawProc(int *x, int *y);
  void freeProc();
};

static CRGBA                 text_color;
static bool                  text_underline = false;
static bool                  text_strike    = false;
static CBrowserTextPlaceType text_place     = CBROWSER_TEXT_PLACE_TYPE_NORMAL;
static bool                  text_breakup   = true;
static bool                  text_format    = true;

std::vector<CBrowserText  *> draw_texts;
std::vector<CBrowserLabel *> draw_labels;

static void HtmlAddDrawText
             (CBrowserWindow *window, CBrowserText *draw_text, const std::string &text);
static void HtmlGetTextBounds
             (CFontPtr, const std::string &, int *, int *, int *);
static void HtmlGetTextWidth
             (CFontPtr, const std::string &, int *);
static void HtmlGetTextHeight
             (CFontPtr, int *, int *);

void
HtmlSetTextColor(const CRGBA &color)
{
  text_color = color;
}

void
HtmlSetTextUnderline(bool flag)
{
  text_underline = flag;
}

void
HtmlSetTextStrike(bool flag)
{
  text_strike = flag;
}

void
HtmlSetTextSubscript(bool flag)
{
  if (flag)
    text_place = CBROWSER_TEXT_PLACE_TYPE_SUBSCRIPT;
  else
    text_place = CBROWSER_TEXT_PLACE_TYPE_NORMAL;
}

void
HtmlSetTextSuperscript(bool flag)
{
  if (flag)
    text_place = CBROWSER_TEXT_PLACE_TYPE_SUPERSCRIPT;
  else
    text_place = CBROWSER_TEXT_PLACE_TYPE_NORMAL;
}

void
HtmlSetTextBreakup(bool flag)
{
  text_breakup = flag;
}

void
HtmlSetTextFormat(bool flag)
{
  text_format = flag;
}

void
HtmlDrawText(CBrowserWindow *window, const std::string &text)
{
  CBrowserText *draw_text = new CBrowserText(window, text);

  draw_texts.push_back(draw_text);
}

CBrowserText::
CBrowserText(CBrowserWindow *window, const std::string &text) :
 window_     (window),
 text_       (text),
 font_       (),
 color_      (text_color),
 underline_  (text_underline),
 strike_     (text_strike),
 place_      (text_place),
 breakup_    (text_breakup),
 format_     (text_format),
 link_       (NULL),
 format_proc_(NULL),
 redraw_proc_(NULL),
 free_proc_  (NULL)
{
  font_ = window->getFont();
  link_ = HtmlGetCurrentLink(window);

  format_proc_ = new CBrowserFormatProc<CBrowserText>(this);
  free_proc_   = new CBrowserFreeProc<CBrowserText>(this);

  window->addCellRedrawData(format_proc_, free_proc_);
}

CBrowserText::
CBrowserText(CBrowserWindow *window, const CBrowserText &draw_text, const std::string &text) :
 window_     (window),
 text_       (text),
 font_       (draw_text.font_),
 color_      (draw_text.color_),
 underline_  (draw_text.underline_),
 strike_     (draw_text.strike_),
 place_      (draw_text.place_),
 breakup_    (draw_text.breakup_),
 format_     (draw_text.format_),
 link_       (draw_text.link_),
 format_proc_(NULL),
 redraw_proc_(NULL),
 free_proc_  (NULL)
{
  redraw_proc_ = new CBrowserRedrawProc<CBrowserText>(this);
  free_proc_   = new CBrowserFreeProc<CBrowserText>(this);

  window->addSubCellRedrawData(redraw_proc_, free_proc_);
}

CBrowserText::
~CBrowserText()
{
  delete format_proc_;
  delete redraw_proc_;
  delete free_proc_;
}

void
CBrowserText::
formatProc()
{
  if      (format_ && breakup_) {
    int i = 0;

    CHtmlLayoutSubCell *sub_cell = window_->getCurrentSubCell();

    if (text_[i] != '\0' && isspace(text_[i])) {
      if (sub_cell != NULL && sub_cell->getLeftCell() != NULL) {
        CHtmlLayoutSubCell::newCellRight(window_->getLayoutMgr(), false);

        HtmlAddDrawText(window_, this, " ");
      }

      while (text_[i] != '\0' && isspace(text_[i]))
        i++;
    }

    while (text_[i] != '\0') {
      int j = i;

      while (text_[i] != '\0' && ! isspace(text_[i]))
        i++;

      if (i - j == 0)
        break;

      CHtmlLayoutSubCell::newCellRight(window_->getLayoutMgr(), true);

      HtmlAddDrawText(window_, this, text_.substr(j, i - j));

      if (text_[i] != '\0' && isspace(text_[i])) {
        CHtmlLayoutSubCell::newCellRight(window_->getLayoutMgr(), false);

        HtmlAddDrawText(window_, this, " ");

        while (text_[i] != '\0' && isspace(text_[i]))
          i++;
      }
    }
  }
  else if (format_) {
    CHtmlLayoutSubCell::newCellRight(window_->getLayoutMgr(), true);

    HtmlAddDrawText(window_, this, text_);
  }
  else {
    int i = 0;

    while (text_[i] != '\0') {
      while (text_[i] == '\n') {
        CHtmlLayoutSubCell::newCellBelow(window_->getLayoutMgr(), false);

        HtmlAddDrawText(window_, this, "");

        i++;
      }

      int j = i;

      while (text_[i] != '\0' && text_[i] != '\n')
        i++;

      if (i > j) {
        CHtmlLayoutSubCell::newCellRight(window_->getLayoutMgr(), false);

        HtmlAddDrawText(window_, this, text_.substr(j, i - j));
      }

      if (text_[i] == '\n') {
        CHtmlLayoutSubCell::newCellBelow(window_->getLayoutMgr(), false);

        HtmlAddDrawText(window_, this, "");

        i++;
      }
    }
  }
}

static void
HtmlAddDrawText(CBrowserWindow *window, CBrowserText *draw_text, const std::string &text)
{
  int ascent;
  int descent;

  HtmlGetTextHeight(draw_text->getFont(), &ascent, &descent);

  window->updateSubCellHeight(ascent, descent);

  int width;

  HtmlGetTextWidth(draw_text->getFont(), text, &width);

  window->updateSubCellWidth(width);

  /*----------*/

  CBrowserText *draw_text1 = new CBrowserText(window, *draw_text, text);

  draw_texts.push_back(draw_text1);
}

void
CBrowserText::
redrawProc(int *x, int *y)
{
  CHtmlLayoutSubCell *sub_cell = window_->getCurrentSubCell();

  window_->setFont(font_);

  window_->setForeground(color_);

  int width;
  int ascent;
  int descent;

  HtmlGetTextBounds(font_, text_, &width, &ascent, &descent);

  int y_offset = 0;

  if      (place_ == CBROWSER_TEXT_PLACE_TYPE_SUBSCRIPT)
    y_offset =  ascent/2;
  else if (place_ == CBROWSER_TEXT_PLACE_TYPE_SUPERSCRIPT)
    y_offset = -ascent/2;

  int x1 = *x;
  int y1 = *y + y_offset + sub_cell->getAscent();

  window_->drawString(x1, y1, text_);

  if (strike_) {
    window_->drawLine(x1        , y1 - ascent/2    , x1 + width, y1 - ascent/2    );
    window_->drawLine(x1        , y1 - ascent/2 + 1, x1 + width, y1 - ascent/2 + 1);
  }

  if (underline_)
    window_->drawLine(x1, y1 + 1, x1 + width, y1 + 1);

  if (link_ != NULL)
    link_->addRect(x1, y1 - sub_cell->getAscent(), x1 + width, y1 + sub_cell->getDescent());

  *x += width;
}

void
CBrowserText::
freeProc()
{
  delete this;
}

void
HtmlDrawLabel(CBrowserWindow *window, const std::string &text, int width, CHAlignType align)
{
  CBrowserLabel *draw_label = new CBrowserLabel(window, text, width, align);

  draw_labels.push_back(draw_label);
}

CBrowserLabel::
CBrowserLabel(CBrowserWindow *window, const std::string &text, int width, CHAlignType align) :
 window_     (window),
 text_       (text),
 width_      (width),
 align_      (align),
 font_       (),
 color_      (text_color),
 format_proc_(NULL),
 redraw_proc_(NULL),
 free_proc_  (NULL)
{
  font_ = window->getFont();

  format_proc_ = new CBrowserFormatProc<CBrowserLabel>(this);
  free_proc_   = new CBrowserFreeProc<CBrowserLabel>(this);

  window->addCellRedrawData(format_proc_, free_proc_);
}

CBrowserLabel::
~CBrowserLabel()
{
  delete format_proc_;
  delete redraw_proc_;
  delete free_proc_;
}

void
CBrowserLabel::
formatProc()
{
  CHtmlLayoutSubCell::newCellRight(window_->getLayoutMgr(), false);

  int ascent  = 0;
  int descent = 0;

  HtmlGetTextHeight(font_, &ascent, &descent);

  window_->updateSubCellHeight(ascent, descent);

  int char_width = 0;

  HtmlGetTextWidth(font_, "X", &char_width);

  window_->updateSubCellWidth(width_*char_width);

  /*---------*/

  delete redraw_proc_;

  redraw_proc_ = new CBrowserRedrawProc<CBrowserLabel>(this);

  window_->addSubCellRedrawData(redraw_proc_);
}

void
CBrowserLabel::
redrawProc(int *x, int *y)
{
  int char_width;

  HtmlGetTextWidth(font_, "X", &char_width);

  int text_width;

  HtmlGetTextWidth(font_, text_, &text_width);

  /*-----------*/

  int x1 = *x;

  if      (align_ == CHALIGN_TYPE_LEFT)
    ;
  else if (align_ == CHALIGN_TYPE_CENTER)
    x1 += (width_*char_width - text_width)/2;
  else if (align_ == CHALIGN_TYPE_RIGHT)
    x1 +=  width_*char_width - text_width;

  CHtmlLayoutSubCell *sub_cell = window_->getCurrentSubCell();

  int y1 = *y + sub_cell->getAscent();

  /*-----------*/

  window_->setFont(font_);

  window_->setForeground(color_);

  window_->drawString(x1, y1, text_);

  /*-----------*/

  *x += width_*char_width;
}

void
CBrowserLabel::
freeProc()
{
  delete this;
}

void
HtmlGetTextSize(CBrowserWindow *window, const std::string &text,
                int *width, int *ascent, int *descent)
{
  CFontPtr font = window->getFont();

  *width   = font->getStringWidth(text);
  *ascent  = font->getCharAscent();
  *descent = font->getCharDescent();
}

static void
HtmlGetTextBounds(CFontPtr font, const std::string &text, int *width, int *ascent, int *descent)
{
  *width   = font->getStringWidth(text);
  *ascent  = font->getCharAscent();
  *descent = font->getCharDescent();
}

static void
HtmlGetTextWidth(CFontPtr font, const std::string &text, int *width)
{
  *width = font->getStringWidth(text);
}

static void
HtmlGetTextHeight(CFontPtr font, int *ascent, int *descent)
{
  *ascent  = font->getCharAscent();
  *descent = font->getCharDescent();
}
