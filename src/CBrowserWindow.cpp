#include <CBrowserWindow.h>
#include <CBrowserDocument.h>
#include <CBrowserWindowWidget.h>
#include <CBrowserScrolledWindow.h>
#include <CBrowserHistory.h>
#include <CBrowserIFace.h>
#include <CBrowserFont.h>
#include <CBrowserMain.h>
#include <CBrowserBreak.h>
#include <CBrowserCanvas.h>
#include <CBrowserImage.h>
#include <CBrowserNamedImage.h>
#include <CBrowserLayout.h>
#include <CBrowserLink.h>
#include <CBrowserFile.h>
#include <CBrowserRule.h>
#include <CBrowserTable.h>
#include <CBrowserText.h>
#include <CBrowserForm.h>
#include <CBrowserJS.h>
#include <CWebGet.h>
#include <CThrow.h>
#include <CRGBName.h>
#include <CFontMgr.h>
#include <CEnv.h>

//------

std::list<CBrowserWindow *> CBrowserWindow::window_list_;
std::string                 CBrowserWindow::window_target_ = "";
CBrowserAnchorLink*         CBrowserWindow::mouse_link_    = nullptr;

CBrowserWindow::
CBrowserWindow(const std::string &filename) :
 output_(this)
{
  init();

  if (filename != "") {
    CUrl url(filename);

    setDocument(url);
  }

  window_list_.push_back(this);
}

CBrowserWindow::
~CBrowserWindow()
{
  window_list_.remove(this);

  document_->freeLinks();

  delete document_;
  delete history_;

  if (window_list_.size() == 0)
    exit(0);
}

void
CBrowserWindow::
init()
{
  iface_   = nullptr;
  swindow_ = nullptr;
  w_       = nullptr;

  reset();
}

void
CBrowserWindow::
reset()
{
  delete document_;
  delete layout_;

  delete linkMgr_;
  delete fileMgr_;

  delete history_;

  //---

  window_target_ = "";

  mouse_link_ = nullptr;

  name_     = "";
  document_ = nullptr;

  leftMargin_ = CBrowserDataConstants::LEFT_MARGIN;
  topMargin_  = CBrowserDataConstants::TOP_MARGIN;

  bbox_ = CIBBox2D(leftMargin_, topMargin_, 2*leftMargin_, 2*topMargin_);

  layout_ = new CBrowserLayout(this);

  linkMgr_ = new CBrowserLinkMgr(this);
  fileMgr_ = new CBrowserFileMgr(this);

  rootObject_ = 0;

  idObjects_   .clear();
  objStack_   .clear();
  objects_    .clear();
  scripts_    .clear();
  scriptFiles_.clear();

  cssList_.clear();

  baseFontSize_ = 3;

  history_ = new CBrowserHistory(this);

  //---

  setBaseFontStyle();
}

void
CBrowserWindow::
setIFace(CBrowserScrolledWindow *swindow)
{
  swindow_ = swindow;
  iface_   = swindow_->iface();
  w_       = swindow_->widget();
}

void
CBrowserWindow::
processTokens(const CHtmlParserTokens &tokens)
{
  output_.processTokens(tokens);
}

void
CBrowserWindow::
layoutObjects()
{
  output_.layoutObjects();
}

void
CBrowserWindow::
setBaseFontSize(int size)
{
  baseFontSize_ = size;

  setBaseFontStyle();
}

int
CBrowserWindow::
getBaseFontSize()
{
  return baseFontSize_;
}

void
CBrowserWindow::
resetBaseFontSize()
{
  baseFontSize_ = 3;

  setBaseFontStyle();
}

void
CBrowserWindow::
increaseBaseFontSize(int d)
{
  baseFontSize_ += d;

  setBaseFontStyle();
}

void
CBrowserWindow::
decreaseBaseFontSize(int d)
{
  baseFontSize_ -= d;

  setBaseFontStyle();
}

void
CBrowserWindow::
setBaseFontStyle()
{
  int size = baseFontSize_;

  CBrowserFontSize fontSize = sizeToFontSize(size);

  int isize = fontSize.value(CScreenUnits(12));

  std::string family = "helvetica";

  font_ = CFontMgrInst->lookupFont(family, CFONT_STYLE_NORMAL, isize);
}

//------

CImagePtr
CBrowserWindow::
lookupImage(const CBrowserImageData &imageData)
{
  CImageFileSrc file(imageData.src);

  CImagePtr image = CImageMgrInst->createImage(file);

  if (! image.isValid())
    return image;

  int width  = image->getWidth ();
  int height = image->getHeight();

  if (imageData.width != -1)
    width = imageData.width;

  if (imageData.height != -1)
    height = imageData.height;

  if (width != int(image->getWidth()) || height != int(image->getHeight())) {
    CImagePtr image1 = image->resize(width, height);

    image = image1;
  }

  return image;
}

//------

void
CBrowserWindow::
startObject(CBrowserObject *obj, bool add)
{
  assert(obj);

//std::cerr << "start obj: "; obj->print(std::cerr); std::cerr << std::endl;
  if (add) {
    CBrowserObject *currentObj = this->currentObj();

    if (currentObj)
      currentObj->addChild(obj);
  }

  objStack_.push_back(obj);

  if (add) {
    if (obj->id() != "")
      idObjects_[obj->id()] = obj;

    objects_.push_back(obj);

    CBrowserJSInst->addHtmlObject(obj);
  }

  //---

  if (add)
    applyStyle(obj);
}

void
CBrowserWindow::
endObject()
{
  assert(! objStack_.empty());

//CBrowserObject *obj = objStack_.back();
//std::cerr << "end obj: "; obj->print(std::cerr); std::cerr << std::endl;

  objStack_.pop_back();
}

CBrowserObject *
CBrowserWindow::
currentObj() const
{
  if (! objStack_.empty())
    return objStack_.back();
  else
    return 0;
}

CBrowserObject *
CBrowserWindow::
getObject(const std::string &id) const
{
  auto p = idObjects_.find(id);

  if (p == idObjects_.end())
    return nullptr;

  return (*p).second;
}

//------

void
CBrowserWindow::
addScript(const std::string &text)
{
  scripts_.push_back(text);
}

void
CBrowserWindow::
addScriptFile(const std::string &filename)
{
  scriptFiles_.push_back(filename);
}

void
CBrowserWindow::
runScripts()
{
  for (const auto &s : scriptFiles_)
    CBrowserJSInst->runScriptFile(this, s);

  for (const auto &s : scripts_)
    CBrowserJSInst->runScript(this, s);

  CBrowserJSInst->onLoad();
}

//------

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
setDocument(const CUrl &url)
{
  reset();

  //---

  filename_ = url.getLocalFile();

  document_ = new CBrowserDocument(this);

  document_->read(url);

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

  document_->freeLinks();

  mouse_link_ = nullptr;

  //---

  processTokens(document_->tokens());

  //---

  layoutObjects();

  //---

  runScripts();

  //---

  resize();
}

void
CBrowserWindow::
recalc()
{
  if (! w_)
    return;

  iface_->setBusy();

  //---

  bbox_ = CIBBox2D(leftMargin_, topMargin_, w_->width() - leftMargin_, w_->height() - topMargin_);

  linkMgr()->clearLinkRects();

  //------

  layout_->layout(rootObject(), bbox_);

  //------

  int w = w_->width ();
  int h = w_->height();

  if (rootObject()) {
    w = rootObject()->contentWidth ();
    h = rootObject()->contentHeight();
  }

  swindow_->setSize(w, h);

  //---

  redraw();

  //---

  iface_->setReady();
}

void
CBrowserWindow::
resize()
{
  recalc();

  //---

  if (window_target_ != "") {
    int x, y;

    if (linkMgr()->getDestLinkPos(window_target_, &x, &y))
      swindow_->scrollTo(x, y);
  }

  //---

  redraw();
}

void
CBrowserWindow::
redraw()
{
  if (! w_)
    return;

  w_->update();

  const QObjectList &objs = w_->children();

  for (int i = 0; i < objs.length(); ++i) {
    QWidget *w = qobject_cast<QWidget *>(objs[i]);

    if (w)
      w->update();
  }
}

int
CBrowserWindow::
getCanvasXOffset() const
{
  return swindow_->getCanvasXOffset();
}

int
CBrowserWindow::
getCanvasYOffset() const
{
  return swindow_->getCanvasYOffset();
}

int
CBrowserWindow::
getCanvasWidth() const
{
  return swindow_->getCanvasWidth();
}

int
CBrowserWindow::
getCanvasHeight() const
{
  return swindow_->getCanvasHeight();
}

void
CBrowserWindow::
drawDocument()
{
  int dx = -getCanvasXOffset();
  int dy = -getCanvasYOffset();

  layout_->render(dx, dy);
}

void
CBrowserWindow::
gotoDocument(const std::string &text)
{
  CUrl url(text);

  setDocument(url);
}

void
CBrowserWindow::
setTitle(const std::string &title)
{
  swindow_->setTitle(title);
}

void
CBrowserWindow::
setMargins(int left, int top)
{
  leftMargin_ = left;
  topMargin_  = top;
}

void
CBrowserWindow::
setBackgroundImage(const std::string &filename, bool fixed)
{
  if (filename == "")
    return;

  if (! CFile::exists(filename)) {
    displayError("Invalid image file '%s'\n", filename.c_str());
    return;
  }

  CImageFileSrc file(filename);

  CImagePtr image = CImageMgrInst->createImage(file);

  if (! image)
    displayError("Illegal Background Image Type %s\n", filename.c_str());

  document_->setBgImage(image, fixed);
}

void
CBrowserWindow::
setTarget(const std::string &target)
{
  window_target_ = target;
}

bool
CBrowserWindow::
loadCSSFile(const CUrl &url)
{
  std::string filename;

  if      (url.isHttp()) {
    if (! downloadFile(url, filename)) {
      displayError("Failed to download '%s'\n", url.getUrl().c_str());
      return false;
    }
  }
  else {
    filename = url.getFile();
  }

  CCSS css;

  if (! css.processFile(filename))
    return false;

  cssList_.push_back(CSSData(css, url));

  return true;
}

bool
CBrowserWindow::
loadCSSText(const std::string &text)
{
  CCSS css;

  if (! css.processLine(text))
    return false;

  cssList_.push_back(CSSData(css));

  return true;
}

//------

bool
CBrowserWindow::
applyStyle(CBrowserObject *obj)
{
  CCSSTagDataP tagData(new CBrowserObjectCSSTagData(obj));

  bool rc = true;

  for (const auto &css : cssList_) {
    if (! visitStyleData(css.css, tagData))
      rc = false;
  }

  return rc;
}

bool
CBrowserWindow::
visitStyleData(const CCSS &css, const CCSSTagDataP &tagData)
{
  bool match = false;

  std::vector<CCSS::SelectorList> selectorListArray;

  css.getSelectors(selectorListArray);

  for (const auto &selectorList : selectorListArray) {
    const CCSS::StyleData &styleData = css.getStyleData(selectorList);

    if (! styleData.checkMatch(tagData))
      continue;

    CBrowserObject *obj = dynamic_cast<CBrowserObjectCSSTagData *>(tagData.get())->obj();

    for (const auto &opt : styleData.getOptions()) {
      obj->setStyleValue(opt.getName(), opt.getValue());
    }

    match = true;
  }

  return match;
}

//------

void
CBrowserWindow::
selectCSSPattern(const CCSS::StyleData &styleData)
{
  deselectAllObjects();

  for (auto &obj : objects_) {
    CCSSTagDataP tagData(new CBrowserObjectCSSTagData(obj));

    if (! styleData.checkMatch(tagData))
      continue;

    obj->setSelected(true);
  }

  redraw();
}

//------

bool
CBrowserWindow::
setShortcutIcon(const CUrl &url)
{
  std::string filename;

  if      (url.isHttp()) {
    if (! downloadFile(url, filename)) {
      displayError("Failed to download '%s'\n", url.getUrl().c_str());
      return false;
    }
  }
  else {
    filename = url.getFile();
  }

  displayError("Shortcut icon '%s'\n", filename.c_str());

  return true;
}

//------

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

  if (CBrowserMainInst->getQuiet())
    return;

  va_start(args, format);

  vfprintf(stderr, format, args);

  va_end(args);
}

bool
CBrowserWindow::
isVisible(int x1, int y1, int x2, int y2)
{
  if (x1 > bbox_.getXMax() || x2 < bbox_.getXMin() ||
      y1 > bbox_.getYMax() || y2 < bbox_.getYMin())
    return false;
  else
    return true;
}

void
CBrowserWindow::
print()
{
  CBrowserObject *root = this->rootObject();

  CBrowserRegion region;

  if (root)
    region = root->calcRegion();

  double xmin = 0;
  double ymin = 0;
  double xmax = region.width () + 2*getLeftMargin();
  double ymax = region.height() + 2*getTopMargin();

  swindow_->print(xmin, ymin, xmax, ymax);
}

void
CBrowserWindow::
saveImage(const std::string &filename)
{
  swindow_->saveImage(filename);
}

void
CBrowserWindow::
goBack()
{
  const CUrl &url = history_->goBack();
  if (! url.isValid()) return;

  setDocument(url);
}

void
CBrowserWindow::
goForward()
{
  const CUrl &url = history_->goForward();
  if (! url.isValid()) return;

  setDocument(url);
}

bool
CBrowserWindow::
hoverLink(int x, int y, std::string &link_name)
{
  CBrowserAnchorLink *link = linkMgr()->getSourceLink(x, y);

  if (link) {
    mouse_link_ = link;

    link_name = link->getTitle();
  }
  else {
    mouse_link_ = nullptr;

    link_name = "";
  }

  setStatus(link_name);

  return link;
}

bool
CBrowserWindow::
activateLink(int x, int y)
{
  CBrowserAnchorLink *link = linkMgr()->getSourceLink(x, y);

  if (link)
    gotoDocument(link->getDest());

  return true;
}

void
CBrowserWindow::
addHistoryItem(const CUrl &item)
{
  iface_->addHistoryItem(item);
}

CRGBA
CBrowserWindow::
getBgColor()
{
  if (document_)
    return document_->getBgColor();

  CRGBA bg;

  CRGBName::toHtmlRGBA("#c0c0c0", bg);

  return bg;
}

CRGBA
CBrowserWindow::
getFgColor()
{
  if (document_)
    return document_->getFgColor();
  else
    return CRGBA();
}

CImagePtr
CBrowserWindow::
getBgImage()
{
  if (document_)
    return document_->getBgImage();
  else
    return CImagePtr();
}

void
CBrowserWindow::
drawImage(int x, int y, const CImagePtr &image)
{
  w_->drawImage(x, y, image);
}

void
CBrowserWindow::
drawImage(int x, int y, const QImage &image)
{
  w_->drawImage(x, y, image);
}

void
CBrowserWindow::
drawRectangle(int x, int y, int w, int h, const CPen &pen)
{
  w_->drawRectangle(x, y, w, h, pen);
}

void
CBrowserWindow::
fillRectangle(int x, int y, int w, int h, const CBrush &brush)
{
  w_->fillRectangle(x, y, w, h, brush);
}

void
CBrowserWindow::
fillPolygon(const std::vector<CIPoint2D> &points, const CBrush &brush)
{
  w_->fillPolygon(points, brush);
}

void
CBrowserWindow::
drawCircle(int x, int y, int r, const CPen &pen)
{
  w_->drawCircle(x, y, r, pen);
}

void
CBrowserWindow::
fillCircle(int x, int y, int r, const CBrush &brush)
{
  w_->fillCircle(x, y, r, brush);
}

void
CBrowserWindow::
drawLine(int x1, int y1, int x2, int y2, const CPen &pen)
{
  w_->drawLine(x1, y1, x2, y2, pen);
}

void
CBrowserWindow::
drawText(int x, int y, const std::string &str, const CPen &pen, const CFontPtr &font)
{
  w_->drawText(x, y, str, pen, font);
}

void
CBrowserWindow::
drawOutline(int x, int y, int width, int height, const CPen &pen)
{
  if (CEnvInst.exists("HTML_OUTLINE"))
    w_->drawOutline(x, y, width, height, pen);
}

void
CBrowserWindow::
drawSelected(int x, int y, int width, int height)
{
  w_->drawOutline(x, y, width, height, CPen(CRGBA(1,0,0)));
}

void
CBrowserWindow::
drawBorder(int x, int y, int width, int height, CBrowserBorderType type)
{
  w_->drawBorder(x, y, width, height, type);
}

void
CBrowserWindow::
drawHRule(int x1, int x2, int y, int height)
{
  w_->drawHRule(x1, x2, y, height);
}

void
CBrowserWindow::
getTextSize(const std::string &text, int *width, int *ascent, int *descent) const
{
  CFontPtr font = getFont();

  *width   = font->getStringWidth(text);
  *ascent  = font->getCharAscent();
  *descent = font->getCharDescent();
}

void
CBrowserWindow::
getTextWidth(CFontPtr font, const std::string &text, int *width) const
{
  *width = font->getStringWidth(text);
}

void
CBrowserWindow::
getTextHeight(CFontPtr font, int *ascent, int *descent) const
{
  *ascent  = font->getCharAscent();
  *descent = font->getCharDescent();
}

void
CBrowserWindow::
errorDialog(const std::string &msg)
{
  iface_->errorDialog(msg);
}

//---

void
CBrowserWindow::
parseHAlignOption(const std::string &name, CHAlignType &align)
{
  std::string lname = CStrUtil::toLower(name);

  if      (lname == "left")
    align = CHALIGN_TYPE_LEFT;
  else if (lname == "center")
    align = CHALIGN_TYPE_CENTER;
  else if (lname == "right")
    align = CHALIGN_TYPE_RIGHT;
  else
    displayError("Illegal Horizontal Alignment '%s'\n", name.c_str());
}

void
CBrowserWindow::
parseVAlignOption(const std::string &name, CVAlignType &align)
{
  std::string lname = CStrUtil::toLower(name);

  if      (lname == "top")
    align = CVALIGN_TYPE_TOP;
  else if (lname == "center")
    align = CVALIGN_TYPE_CENTER;
  else if (lname == "bottom")
    align = CVALIGN_TYPE_BOTTOM;
  else if (lname == "baseline")
    align = CVALIGN_TYPE_BASELINE;
  else
    displayError("Illegal Vertical Alignment %s\n", name.c_str());
}

CBrowserFontSize
CBrowserWindow::
sizeToFontSize(int size) const
{
  if      (size <= 1) return CBrowserFontSize("0.75em");
  else if (size == 2) return CBrowserFontSize("0.83em");
  else if (size == 3) return CBrowserFontSize("1.00em");
  else if (size == 4) return CBrowserFontSize("1.17em");
  else if (size == 5) return CBrowserFontSize("1.50em");
  else if (size == 6) return CBrowserFontSize("2.00em");
  else if (size >= 7) return CBrowserFontSize("3.00em");

  return CBrowserFontSize("1.00em");
}

void
CBrowserWindow::
deselectAllObjects()
{
  selectSingleObject(nullptr);
}

void
CBrowserWindow::
selectSingleObject(CBrowserObject *obj)
{
  bool changed = false;

  for (auto &o : objects_) {
    if (o == obj)
      continue;

    if (o->isSelected()) {
      o->setSelected(false);

      changed = true;
    }
  }

  if (obj) {
    if (! obj->isSelected()) {
      obj->setSelected(true);

      changed = true;
    }
  }

  if (changed)
    redraw();
}

bool
CBrowserWindow::
downloadFile(const CUrl &url, std::string &filename)
{
  CWebGet webget(url);

  webget.setOverwrite(true);
  webget.setDebug    (true);
//webget.setHttpDebug(true);
//webget.setTcpDebug (true);
//webget.setListRefs (false);

  bool follow_links = false;

  try {
    CWebGetUrl web_url(&webget, url);

    if (webget.loadPage(web_url)) {
      if (follow_links && web_url.getProcess())
        webget.processFile(web_url);
    }

    filename = web_url.getFilename();

    return true;
  }
  catch (const char *message) {
    displayError("Download of '%s' failed : '%s'\n", filename.c_str(), message);
  }
  catch (CThrow *error) {
    displayError("Download of '%s' failed : '%s'\n", filename.c_str(), error->message.c_str());
  }

  return false;
}
