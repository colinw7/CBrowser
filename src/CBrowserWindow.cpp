#include <CBrowserHtmlI.h>
#include <CRGBName.h>

std::list<CBrowserWindow *>  CBrowserWindow::window_list_;
std::string                  CBrowserWindow::default_font_face_ = "";
std::string                  CBrowserWindow::window_target_     = "";
CBrowserLink                *CBrowserWindow::mouse_link_        = NULL;

CBrowserWindow::
CBrowserWindow(const std::string &filename) :
  name_              (""),
  document_          (NULL),
  iface_             (NULL),
  canvas_            (NULL),
  x_                 (0),
  y_                 (0),
  width_             (0),
  height_            (0),
  left_margin_       (0),
  top_margin_        (0),
  bg_image_          (),
  bg_fixed_          (false),
  layout_mgr_        (NULL),
  area_data_         (NULL),
  font_              (),
  font_face_stack_   (),
  font_face_list_    (),
  current_font_face_ (NULL),
  base_font_size_    (0),
  current_font_color_(0,0,0),
  current_font_style_(0),
  current_font_size_ (0),
  history_           (NULL)
{
  init();

  setDocument(filename);

  window_list_.push_back(this);
}

CBrowserWindow::
CBrowserWindow()
{
  init();

  window_list_.push_back(this);
}

CBrowserWindow::
~CBrowserWindow()
{
  window_list_.remove(this);

  HtmlFreeLinks(this);

  freeFonts();

  delete area_data_;
  delete document_;
  delete history_;

  if (window_list_.size() == 0)
    exit(0);
}

void
CBrowserWindow::
init()
{
  default_font_face_ = "";

  window_target_ = "";

  mouse_link_ = NULL;

  name_     = "";
  document_ = NULL;

  iface_  = NULL;
  canvas_ = NULL;

  x_      = LEFT_MARGIN;
  y_      = TOP_MARGIN;
  width_  = x_;
  height_ = y_;

  left_margin_ = LEFT_MARGIN;
  top_margin_  = TOP_MARGIN;

  bg_image_ = CImagePtr();
  bg_fixed_ = false;

  layout_mgr_ = new CHtmlLayoutMgr();
  area_data_  = new CHtmlLayoutArea();

  current_font_face_  = NULL;
  base_font_size_     = 3;
  current_font_style_ = CFONT_STYLE_NORMAL;
  current_font_size_  = 3;

  history_ = new CBrowserHistory(this);

  /*-------------*/

  loadResources();

  /*-------------*/

  startFontFace(default_font_face_);
}

void
CBrowserWindow::
setIFace(CBrowserIFace *iface)
{
  iface_  = iface;
  canvas_ = iface_->getCanvas();
}

void
CBrowserWindow::
loadResources()
{
  default_font_face_ = "helvetica";
}

void
CBrowserWindow::
startArea(CHtmlLayoutArea *area_data)
{
  layout_mgr_->startArea(area_data);
}

void
CBrowserWindow::
endArea()
{
  layout_mgr_->endArea();
}

void
CBrowserWindow::
startFontFace(const std::string &face)
{
  if (current_font_face_ != NULL)
    font_face_stack_.push_back(current_font_face_);

  current_font_face_ = lookupFontFace(face);

  if (current_font_face_ == NULL)
    return;

  base_font_size_     = 3;
  current_font_size_  = 3;
  current_font_style_ = CFONT_STYLE_NORMAL;

  setFontStyle();
}

void
CBrowserWindow::
endFontFace()
{
  if (font_face_stack_.empty())
    return;

  current_font_face_ = font_face_stack_[font_face_stack_.size() - 1];

  font_face_stack_.pop_back();

  if (current_font_face_ == NULL)
    return;

  setFontStyle();
}

std::string
CBrowserWindow::
getCurrentFontFace() const
{
  if (current_font_face_ == NULL)
    return "helvetica";

  return current_font_face_->getFace();
}

void
CBrowserWindow::
setCurrentFontFace(const std::string &face)
{
  current_font_face_ = lookupFontFace(face);
}

CBrowserFontFace *
CBrowserWindow::
lookupFontFace(const std::string &face)
{
  CBrowserFontFace *font_face = NULL;

  std::vector<CBrowserFontFace *>::iterator pfont1 = font_face_list_.begin();
  std::vector<CBrowserFontFace *>::iterator pfont2 = font_face_list_.end  ();

  for ( ; pfont1 != pfont2; ++pfont1)
    if ((*pfont1)->getFace() == face)
      return *pfont1;

  font_face = loadFontFace(face);

  return font_face;
}

CBrowserFontFace *
CBrowserWindow::
loadFontFace(const std::string &face)
{
  CBrowserFontFace *font_face = new CBrowserFontFace(this, face);

  font_face_list_.push_back(font_face);

  return font_face;
}

void
CBrowserWindow::
setBaseFontSize(int size)
{
  base_font_size_ = size - 1;

  setFontStyle();
}

int
CBrowserWindow::
getBaseFontSize()
{
  return (base_font_size_ + 1);
}

void
CBrowserWindow::
resetBaseFontSize()
{
  base_font_size_ = 3;

  setFontStyle();
}

void
CBrowserWindow::
increaseBaseFontSize()
{
  base_font_size_++;

  setFontStyle();
}

void
CBrowserWindow::
decreaseBaseFontSize()
{
  base_font_size_--;

  setFontStyle();
}

void
CBrowserWindow::
setFontSize(int size)
{
  current_font_size_ = size - 1;

  setFontStyle();
}

int
CBrowserWindow::
getFontSize()
{
  return(current_font_size_ + 1);
}

void
CBrowserWindow::
resetFontSize()
{
  current_font_size_ = 3;

  setFontStyle();
}

void
CBrowserWindow::
increaseFontSize()
{
  current_font_size_++;

  setFontStyle();
}

void
CBrowserWindow::
decreaseFontSize()
{
  current_font_size_--;

  setFontStyle();
}

void
CBrowserWindow::
setFontColor(const std::string &colorName)
{
  current_font_color_ = CRGBName::toRGBA(colorName);

  HtmlSetTextColor(current_font_color_);
}

std::string
CBrowserWindow::
getFontColor()
{
  return current_font_color_.stringEncode();
}

void
CBrowserWindow::
startBold()
{
  current_font_style_ |= CFONT_STYLE_BOLD;

  setFontStyle();
}

void
CBrowserWindow::
endBold()
{
  current_font_style_ &= ~CFONT_STYLE_BOLD;

  setFontStyle();
}

void
CBrowserWindow::
startItalic()
{
  current_font_style_ |= CFONT_STYLE_ITALIC;

  setFontStyle();
}

void
CBrowserWindow::
endItalic()
{
  current_font_style_ &= ~CFONT_STYLE_ITALIC;

  setFontStyle();
}

void
CBrowserWindow::
setFontStyle()
{
  if (current_font_face_ == NULL)
    return;

  int size = current_font_size_ + base_font_size_ - 3;

  if      (size > 6)
    size = 6;
  else if (size < 0)
    size = 0;

  if      (current_font_style_ == CFONT_STYLE_NORMAL)
    font_ = current_font_face_->getNormalFont(size);
  else if (current_font_style_ == CFONT_STYLE_BOLD)
    font_ = current_font_face_->getBoldFont(size);
  else if (current_font_style_ == CFONT_STYLE_ITALIC)
    font_ = current_font_face_->getItalicFont(size);
  else if (current_font_style_ == CFONT_STYLE_BOLD_ITALIC)
    font_ = current_font_face_->getBoldItalicFont(size);
}

void
CBrowserWindow::
freeFonts()
{
  std::vector<CBrowserFontFace *>::iterator pface1 = font_face_list_.begin();
  std::vector<CBrowserFontFace *>::iterator pface2 = font_face_list_.end  ();

  for ( ; pface1 != pface2; ++pface1)
    delete (*pface1);
}

CHtmlLayoutArea *
CBrowserWindow::
getCurrentArea()
{
  return layout_mgr_->getCurrentArea();
}

CHtmlLayoutCell *
CBrowserWindow::
getCurrentCell()
{
  return getCurrentArea()->getCurrentCell();
}

CHtmlLayoutSubCell *
CBrowserWindow::
getCurrentSubCell()
{
  return getCurrentCell()->getCurrentSubCell();
}

void
CBrowserWindow::
updateSubCellWidth(int width)
{
  CHtmlLayoutCell *redraw_cell = getCurrentCell();

  redraw_cell->updateSubCellWidth(width);
}

void
CBrowserWindow::
updateSubCellHeight(int ascent, int descent)
{
  CHtmlLayoutCell *redraw_cell = getCurrentCell();

  redraw_cell->updateSubCellHeight(ascent, descent);
}

void
CBrowserWindow::
addCellRedrawData(CHtmlLayoutCellFormatProc *format_proc,
                  CHtmlLayoutCellFreeProc *free_proc)
{
  CHtmlLayoutCell *cell = getCurrentCell();

  CHtmlLayoutRedrawData *redraw_data =
    new CHtmlLayoutRedrawData(format_proc,
                              (CHtmlLayoutCellRedrawProc *) NULL,
                              free_proc);

  cell->addRedrawData(redraw_data);
}

void
CBrowserWindow::
addSubCellRedrawData(CHtmlLayoutCellRedrawProc *redraw_proc)
{
  CHtmlLayoutSubCell *sub_cell = getCurrentSubCell();

  CHtmlLayoutRedrawData *redraw_data =
    new CHtmlLayoutRedrawData((CHtmlLayoutCellFormatProc *) NULL,
                              redraw_proc,
                              (CHtmlLayoutCellFreeProc *) NULL);

  if (sub_cell)
    sub_cell->addRedrawData(redraw_data);
}

void
CBrowserWindow::
addSubCellRedrawData(CHtmlLayoutCellRedrawProc *redraw_proc,
                     CHtmlLayoutCellFreeProc *free_proc)
{
  CHtmlLayoutSubCell *sub_cell = getCurrentSubCell();

  CHtmlLayoutRedrawData *redraw_data =
    new CHtmlLayoutRedrawData((CHtmlLayoutCellFormatProc *) NULL,
                              redraw_proc,
                              free_proc);

  if (sub_cell)
    sub_cell->addRedrawData(redraw_data);
}

void
CBrowserWindow::
close()
{
}

void
CBrowserWindow::
setName(const std::string &name)
{
  name_ = name;
}

void
CBrowserWindow::
setDocument(const std::string &filename)
{
  delete document_;

  document_ = new CBrowserDocument(this);

  document_->read(filename);

  outputDocument();

  if (! history_->goTo(document_->getUrl()))
    history_->addUrl(document_->getUrl());
}

void
CBrowserWindow::
outputDocument()
{
  if (! document_)
    return;

  HtmlFreeLinks(this);

  /*-------------*/

  layout_mgr_->init();

  area_data_->init();

  /*-------------*/

  layout_mgr_->startArea(area_data_);

  /*-------------*/

  CHtmlLayoutCell::newCellBelow(layout_mgr_);

  /*-------------*/

  mouse_link_ = NULL;

  /*-------------*/

  startFontFace(default_font_face_);

  setFontColor("#000000");

  /*-------------*/

  document_->output();

  /*----------------*/

  layout_mgr_->endArea();

  /*----------------*/

  resize();
}

void
CBrowserWindow::
recalc()
{
  iface_->setBusy();

  /*---------------*/

  x_      = left_margin_;
  y_      = top_margin_;
  width_  = canvas_->width () - 2*left_margin_;
  height_ = canvas_->height() - 2*top_margin_;

  HtmlDeleteLinkRects(this);

  /*----------------*/

  area_data_->setX     (left_margin_);
  area_data_->setY     (top_margin_ );
  area_data_->setWidth (canvas_->width () - 2*left_margin_);
  area_data_->setHeight(canvas_->height() - 2*top_margin_ );

  area_data_->format(layout_mgr_);

  /*----------------*/

  iface_->setSize(area_data_->getWidth () + 2*left_margin_,
                  area_data_->getHeight() + 2*top_margin_);

  /*----------------*/

  redraw();

  /*----------------*/

  iface_->setReady();
}

void
CBrowserWindow::
resize()
{
  recalc();

  /*----------------*/

  if (window_target_ != "") {
    int x;
    int y;

    if (HtmlGetDestLinkPos(this, window_target_, &x, &y))
      iface_->scrollTo(x, y);
  }

  /*----------------*/

  redraw();
}

void
CBrowserWindow::
redraw()
{
  canvas_->update();
}

void
CBrowserWindow::
drawDocument()
{
  area_data_->setX(left_margin_ - iface_->getCanvasXOffset());
  area_data_->setY(top_margin_  - iface_->getCanvasYOffset());

  area_data_->redraw(layout_mgr_);
}

void
CBrowserWindow::
gotoDocument(const std::string &url)
{
  setDocument(url);
}

void
CBrowserWindow::
setTitle(const std::string &title)
{
  iface_->setTitle(title);
}

void
CBrowserWindow::
setMargins(int left, int top)
{
  left_margin_ = left;
  top_margin_  = top;
}

void
CBrowserWindow::
setBackgroundImage(const std::string &filename, bool fixed)
{
  if (filename == "")
    return;

  CImageFileSrc file(filename);

  CImagePtr image = CImageMgrInst->createImage(file);

  if (! image)
    fprintf(stderr, "Illegal Backgound Image Type %s\n", filename.c_str());

  bg_image_ = image;
  bg_fixed_ = fixed;
}

void
CBrowserWindow::
setTarget(const std::string &target)
{
  window_target_ = target;
}

void
CBrowserWindow::
indentLeft(int offset)
{
  CHtmlLayoutArea *area = getCurrentArea();

  area->setIndentLeft(area->getIndentLeft () + offset*8);
}

void
CBrowserWindow::
indentRight(int offset)
{
  CHtmlLayoutArea *area = getCurrentArea();

  area->setIndentRight(area->getIndentRight() + offset*8);
}

void
CBrowserWindow::
newLine()
{
  CHtmlLayoutCell::newCellBelow(layout_mgr_);
}

void
CBrowserWindow::
skipLine()
{
  CHtmlLayoutCell::newCellBelow(layout_mgr_);

  HtmlDrawLabel(this, " ", 1, CHALIGN_TYPE_LEFT);

  CHtmlLayoutCell::newCellBelow(layout_mgr_);
}

void
CBrowserWindow::
setAlign(CHAlignType halign, CVAlignType valign)
{
  layout_mgr_->setAlign(halign, valign);
}

void
CBrowserWindow::
setStatus(const std::string &status)
{
  iface_->setStatus(status);
}

void
CBrowserWindow::
displayError(const char *format, ...)
{
  va_list args;

  if (! CBrowserMainInst->getDebug())
    return;

  va_start(args, format);

  vfprintf(stderr, format, args);

  va_end(args);
}

bool
CBrowserWindow::
isVisible(int x1, int y1, int x2, int y2)
{
  if (x1 > x_ + width_  || x2 < x_ ||
      y1 > y_ + height_ || y2 < y_)
    return false;
  else
    return true;
}

void
CBrowserWindow::
print()
{
  double xmin = 0;
  double ymin = 0;
  double xmax = getAreaData()->getWidth () + 2*getLeftMargin();
  double ymax = getAreaData()->getHeight() + 2*getTopMargin();

  iface_->print(xmin, ymin, xmax, ymax);
}

void
CBrowserWindow::
goBack()
{
  std::string url = history_->goBack();

  if (url == "")
    return;

  setDocument(url);
}

void
CBrowserWindow::
goForward()
{
  std::string url = history_->goForward();

  if (url == "")
    return;

  setDocument(url);
}

bool
CBrowserWindow::
hoverLink(int x, int y, std::string &link_name)
{
  CBrowserLink *link = HtmlGetSourceLink(this, x, y);

  if (mouse_link_ == link)
    return false;

  if (link != NULL) {
    mouse_link_ = link;

    link_name = link->getTitle();
  }
  else {
    mouse_link_ = NULL;

    link_name = "";
  }

  setStatus(link_name);

  return true;
}

bool
CBrowserWindow::
activateLink(int x, int y)
{
  CBrowserLink *link = HtmlGetSourceLink(this, x, y);

  if (link != NULL)
    gotoDocument(link->getDest());

  return true;
}

void
CBrowserWindow::
addHistoryItem(const std::string &item)
{
  iface_->addHistoryItem(item);
}

CRGBA
CBrowserWindow::
getBg()
{
  if (document_)
    return document_->getBgColor();
  else
    return CRGBA();
}

CRGBA
CBrowserWindow::
getFg()
{
  if (document_)
    return document_->getFgColor();
  else
    return CRGBA();
}

void
CBrowserWindow::
drawImage(int x, int y, const CImagePtr &image)
{
  canvas_->drawImage(x, y, image);
}

void
CBrowserWindow::
drawRectangle(int x1, int y1, int x2, int y2)
{
  canvas_->drawRectangle(x1, y1, x2, y2);
}

void
CBrowserWindow::
fillRectangle(int x1, int y1, int x2, int y2)
{
  canvas_->fillRectangle(x1, y1, x2, y2);
}

void
CBrowserWindow::
drawCircle(int x, int y, int r)
{
  canvas_->drawCircle(x, y, r);
}

void
CBrowserWindow::
fillCircle(int x, int y, int r)
{
  canvas_->fillCircle(x, y, r);
}

void
CBrowserWindow::
drawLine(int x1, int y1, int x2, int y2)
{
  canvas_->drawLine(x1, y1, x2, y2);
}

void
CBrowserWindow::
drawString(int x, int y, const std::string &str)
{
  canvas_->drawString(x, y, str);
}

void
CBrowserWindow::
drawOutline(int x, int y, int width, int height, const std::string &color_name)
{
  if (CEnvInst.exists("HTML_OUTLINE"))
    canvas_->drawOutline(x, y, width, height, color_name);
}

void
CBrowserWindow::
drawBorder(int x, int y, int width, int height, CBrowserBorderType type)
{
  canvas_->drawBorder(x, y, width, height, type);
}

void
CBrowserWindow::
drawHRule(int x1, int x2, int y, int height)
{
  canvas_->drawHRule(x1, x2, y, height);
}

void
CBrowserWindow::
setForeground(const CRGBA &fg)
{
  canvas_->setForeground(fg);
}

void
CBrowserWindow::
setFont(CFontPtr font)
{
  canvas_->setFont(font);
}

void
CBrowserWindow::
errorDialog(const std::string &msg)
{
  iface_->errorDialog(msg);
}
