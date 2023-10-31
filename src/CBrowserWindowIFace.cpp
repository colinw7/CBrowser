#include <CBrowserWindowIFace.h>
#include <CBrowserDocument.h>
#include <CBrowserOutput.h>
#include <CBrowserLayout.h>
#include <CBrowserGraphics.h>
#include <CBrowserMain.h>
#include <CBrowserHistory.h>
#include <CBrowserIFace.h>
#include <CBrowserLink.h>
#include <CBrowserFile.h>
#include <CBrowserOutputTag.h>
#include <CBrowserWindowWidgetIFace.h>

#include <CQJavaScript.h>
#include <CQJHtmlObj.h>
#include <CHtmlCSSTagData.h>

#include <CWebGet.h>
#include <CFontMgr.h>
#include <CThrow.h>

QString                         CBrowserWindowIFace::windowTarget_;
CBrowserWindowIFace::WindowList CBrowserWindowIFace::windowList_;
CBrowserAnchorLink*             CBrowserWindowIFace::mouseLink_ = nullptr;

//---

CBrowserWindowIFace::
CBrowserWindowIFace(CBrowser *browser) :
 browser_(browser)
{
  output_ = std::make_unique<CBrowserOutput>(this);

  leftMargin_ = CBrowserDataConstants::LEFT_MARGIN;
  topMargin_  = CBrowserDataConstants::TOP_MARGIN;

  //---

  windowList_.push_back(this);
}

CBrowserWindowIFace::
~CBrowserWindowIFace()
{
  windowList_.remove(this);

  getDocument()->freeLinks();

  if (windowList_.empty() && isAutoExit())
    exit(0);
}

//---

void
CBrowserWindowIFace::
init()
{
  widget_ = WidgetP(makeWidget());

  widget_->init();

  jsIFace_ = std::make_unique<JSIFace>(this);

  reset();
}

void
CBrowserWindowIFace::
reset()
{
  baseFontSize_ = 3;

  document_ = DocumentP();

  setName("");

  setTarget("");

  mouseLink_  = nullptr;
  rootObject_ = nullptr;

  //---

  int left, top;
  getMargins(left, top);

  bbox_ = CIBBox2D(left, top, 2*left, 2*top);

  //---

  layout_ = std::make_unique<CBrowserLayout>(this);

  //---

  linkMgr_ = std::make_unique<CBrowserLinkMgr>(this);
  fileMgr_ = std::make_unique<CBrowserFileMgr>(this);

  //---

  idObjects_.clear();
  objStack_ .clear();
  objects_  .clear();

  //---

  cssList_.clear();

  //---

  scripts_    .clear();
  scriptFiles_.clear();

  //---

  history_ = std::make_unique<CBrowserHistory>(this);

  //---

  setBaseFontStyle();
}

void
CBrowserWindowIFace::
recalc()
{
  auto *iface = browser()->iface();

  iface->setBusy();

  //---

  auto *widget = this->widget();

  int w = (widget ? widget->windowWidth () : 100);
  int h = (widget ? widget->windowHeight() : 100);

  int left, top;
  getMargins(left, top);

  bbox_ = CIBBox2D(left, top, w - left, h - top);

  clearLinks();

  //------

  getLayout()->layout(rootObject(), bbox_);

  //------

  if (rootObject()) {
    w = rootObject()->contentWidth ();
    h = rootObject()->contentHeight();
  }

  propagateSize(w, h);

  //---

  redraw();

  //---

  iface->setReady();
}

void
CBrowserWindowIFace::
resize()
{
  recalc();

  //---

  if (getTarget() != "") {
    int x, y;

    if (getDestLinkPos(getTarget(), &x, &y))
      scrollTo(x, y);
  }

  //---

  redraw();
}

void
CBrowserWindowIFace::
redraw()
{
  auto *widget = this->widget();
  if (! widget) return;

  widget->updateWidget(/*sync*/false);
}

//---

CBrowserDocument *
CBrowserWindowIFace::
getDocument() const
{
  return document_.get();
}

CBrowserWindowWidgetIFace *
CBrowserWindowIFace::
widget() const
{
  return widget_.get();
}

CBrowserGraphics *
CBrowserWindowIFace::
graphics() const
{
  auto *widget = this->widget();

  return (widget ? widget->graphics() : nullptr);
}

CBrowserLayout *
CBrowserWindowIFace::
getLayout() const
{
  return layout_.get();
}

CQJWindowIFace *
CBrowserWindowIFace::
jsIFace()
{
  return jsIFace_.get();
}

//---

void
CBrowserWindowIFace::
setTitle(const QString &title)
{
  title_ = title;

  auto *iface = browser()->iface();

  iface->setTitle(title);
}

void
CBrowserWindowIFace::
setStatus(const QString &status)
{
  auto *iface = browser()->iface();

  iface->setStatus(status);
}

//---

void
CBrowserWindowIFace::
setTarget(const QString &target)
{
  windowTarget_ = target;
}

//---

void
CBrowserWindowIFace::
setMargins(int left, int top)
{
  leftMargin_ = left;
  topMargin_  = top;
}

void
CBrowserWindowIFace::
getMargins(int &left, int &top)
{
  left = leftMargin_;
  top  = topMargin_;
}

//---

CRGBA
CBrowserWindowIFace::
getBgColor()
{
  if (document_)
    return document_->getBgColor();

  CRGBA bg;

  CRGBName::toHtmlRGBA("#c0c0c0", bg);

  return bg;
}

CRGBA
CBrowserWindowIFace::
getFgColor()
{
  if (! document_) return CRGBA();

  return document_->getFgColor();
}

CImagePtr
CBrowserWindowIFace::
getBgImage()
{
  if (! document_) return CImagePtr();

  return document_->getBgImage();
}

void
CBrowserWindowIFace::
setBackgroundImage(const QString &filename, bool fixed)
{
  if (filename == "")
    return;

  if (! CFile::exists(filename.toStdString())) {
    displayError("Invalid image file '" + filename + "'\n");
    return;
  }

  CImageFileSrc file(filename.toStdString());

  auto image = CImageMgrInst->createImage(file);

  if (! image)
    displayError("Illegal Background Image Type '" + filename + "'\n");

  document_->setBgImage(image, fixed);
}

//---

bool
CBrowserWindowIFace::
readFile(const std::string &filename, CHtmlParserTokens &tokens)
{
  return fileMgr_->readFile(filename, tokens);
}

//---

CBrowserAnchorLink *
CBrowserWindowIFace::
getCurrentLink() const
{
  return linkMgr_->getCurrentLink();
}

void
CBrowserWindowIFace::
startSourceLink(CBrowserAnchor *anchor)
{
  linkMgr_->startSourceLink(anchor);
}

void
CBrowserWindowIFace::
startDestLink(CBrowserAnchor *anchor)
{
  linkMgr_->startDestLink(anchor);
}

void
CBrowserWindowIFace::
endLink()
{
  linkMgr_->endLink();
}

std::string
CBrowserWindowIFace::
expandDestLink(const std::string &href)
{
  return linkMgr_->expandDestLink(href);
}

void
CBrowserWindowIFace::
clearLinks()
{
  linkMgr_->clearLinkRects();
}

int
CBrowserWindowIFace::
getDestLinkPos(const QString &href, int *x, int *y)
{
  return linkMgr_->getDestLinkPos(href.toStdString(), x, y);
}

CBrowserAnchorLink *
CBrowserWindowIFace::
getSourceLink(int x, int y)
{
  return linkMgr_->getSourceLink(x, y);
}

bool
CBrowserWindowIFace::
hoverLink(int x, int y, std::string &linkName)
{
  auto *link = getSourceLink(x, y);

  if (link) {
    mouseLink_ = link;
    linkName   = link->getTitle();
  }
  else {
    mouseLink_ = nullptr;
    linkName   = "";
  }

  setStatus(QString::fromStdString(linkName));

  return link;
}

bool
CBrowserWindowIFace::
activateLink(int x, int y)
{
  auto *link = getSourceLink(x, y);

  if (link)
    gotoDocument(link->getDest());

  return true;
}

//---

void
CBrowserWindowIFace::
setFilename(const QString &filename)
{
  filename_ = filename;

  CUrl url(filename.toStdString());

  setDocument(url);
}

void
CBrowserWindowIFace::
setDocument(const CUrl &url)
{
  reset();

  //---

  filename_ = QString::fromStdString(url.getLocalFile());

  createDocument(url);

  outputDocument();

  //---

  addHistory(getDocument()->getUrl());
}

void
CBrowserWindowIFace::
outputDocument()
{
  if (! getDocument())
    return;

  getDocument()->freeLinks();

  mouseLink_ = nullptr;

  //---

  processTokens(getDocument()->tokens());

  //---

  layoutObjects();

  //---

  runScripts();

  //---

  resize();
}

void
CBrowserWindowIFace::
createDocument(const CUrl &url)
{
  document_ = std::make_unique<CBrowserDocument>(this);

  document_->read(url);

  auto jsDocument = CQJavaScriptInst->jsDocument();

  document_->setDocument(jsDocument);

  if (jsDocument)
    jsDocument->setIFace(document_->iface());
}

//---

void
CBrowserWindowIFace::
drawDocument()
{
  int dx = -getCanvasXOffset();
  int dy = -getCanvasYOffset();

  getLayout()->render(dx, dy);
}

void
CBrowserWindowIFace::
gotoDocument(const std::string &text)
{
  CUrl url(text);

  setDocument(url);
}

void
CBrowserWindowIFace::
draw()
{
  auto *iface = browser()->iface();

  iface->setBusy();

  auto *widget = this->widget();

  int w = (widget ? widget->windowWidth () : 100);
  int h = (widget ? widget->windowHeight() : 100);

  auto *graphics = this->graphics();
  if (! graphics) return;

  graphics->startDoubleBuffer(w, h);

  //---

  graphics->clear(getBgColor());

  auto bg_image = getBgImage();

  if (bg_image.isValid())
    graphics->drawTiledImage(0, 0, w, h, bg_image);

  //---

  drawDocument();

  //---

  graphics->endDoubleBuffer();

  iface->setReady();
}

void
CBrowserWindowIFace::
drawSelected(int x, int y, int width, int height)
{
  auto *graphics = this->graphics();

  graphics->drawOutline(x, y, width, height, CPen(CRGBA(1, 0, 0)));
}

//---

bool
CBrowserWindowIFace::
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

bool
CBrowserWindowIFace::
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

//---

void
CBrowserWindowIFace::
processTokens(const CHtmlParserTokens &tokens)
{
  output_->processTokens(tokens);
}

void
CBrowserWindowIFace::
layoutObjects()
{
  output_->layoutObjects();
}

//---

CImagePtr
CBrowserWindowIFace::
lookupImage(const CBrowserImageData &imageData, int iwidth, int iheight)
{
  CImageFileSrc file(imageData.src);

  CImagePtr image = CImageMgrInst->createImage(file);

  if (! image.isValid())
    return image;

  int width  = image->getWidth ();
  int height = image->getHeight();

  if (iwidth != -1)
    width = iwidth;

  if (iheight != -1)
    height = iheight;

  if (width != int(image->getWidth()) || height != int(image->getHeight())) {
    CImagePtr image1 = image->resize(width, height);

    image = image1;
  }

  return image;
}

//---

void
CBrowserWindowIFace::
setBaseFontSize(int size)
{
  baseFontSize_ = size;

  setBaseFontStyle();
}

int
CBrowserWindowIFace::
getBaseFontSize()
{
  return baseFontSize_;
}

void
CBrowserWindowIFace::
resetBaseFontSize()
{
  baseFontSize_ = 3;

  setBaseFontStyle();
}

void
CBrowserWindowIFace::
increaseBaseFontSize(int d)
{
  baseFontSize_ += d;

  setBaseFontStyle();
}

void
CBrowserWindowIFace::
decreaseBaseFontSize(int d)
{
  baseFontSize_ -= d;

  setBaseFontStyle();
}

void
CBrowserWindowIFace::
setBaseFontStyle()
{
  int size = baseFontSize_;

  auto fontSize = sizeToFontSize(size);

  int isize = fontSize.value(CScreenUnits(12));

  std::string family = "helvetica";

  font_ = CFontMgrInst->lookupFont(family, CFONT_STYLE_NORMAL, isize);
}

//---

void
CBrowserWindowIFace::
getTextSize(const std::string &text, int *width, int *ascent, int *descent) const
{
  auto font = getFont();

  *width   = font->getStringWidth(text);
  *ascent  = font->getCharAscent();
  *descent = font->getCharDescent();
}

void
CBrowserWindowIFace::
getTextWidth(CFontPtr font, const std::string &text, int *width) const
{
  *width = font->getStringWidth(text);
}

void
CBrowserWindowIFace::
getTextHeight(CFontPtr font, int *ascent, int *descent) const
{
  *ascent  = font->getCharAscent();
  *descent = font->getCharDescent();
}

//---

void
CBrowserWindowIFace::
parseHAlignOption(const std::string &name, CHAlignType &align)
{
  auto lname = CStrUtil::toLower(name);

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
CBrowserWindowIFace::
parseVAlignOption(const std::string &name, CVAlignType &align)
{
  auto lname = CStrUtil::toLower(name);

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
CBrowserWindowIFace::
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

//---

void
CBrowserWindowIFace::
addHistoryItem(const CUrl &item)
{
  auto *iface = browser()->iface();

  iface->addHistoryItem(item);
}

//---

CBrowserObject *
CBrowserWindowIFace::
createElement(const std::string &id)
{
  const auto &def = CHtmlTagDefLookupInst->lookup(id);

  if (def.getId() == CHtmlTagId::NONE)
    return nullptr;

  auto *tag = CBrowserOutputTagMgrInst->getTag(def.getId());

  auto *obj = tag->start(this, nullptr);
  if (! obj) return nullptr;

  startObject(obj, /*add*/true);

  return obj;
}

void
CBrowserWindowIFace::
startObject(CBrowserObject *obj, bool add)
{
  assert(obj);

//std::cerr << "start obj: "; obj->print(std::cerr); std::cerr << "\n";
  if (add) {
    auto *currentObj = this->currentObj();

    if (currentObj)
      currentObj->addChild(obj);
  }

  objStack_.push_back(obj);

  if (add) {
    if (obj->id() != "")
      idObjects_[obj->id()] = obj;

    objects_.push_back(obj);

    //---

    addHtmlObject(obj);
  }

  //---

  if (add)
    applyStyle(obj);
}

void
CBrowserWindowIFace::
endObject()
{
  assert(! objStack_.empty());

//CBrowserObject *obj = objStack_.back();
//std::cerr << "end obj: "; obj->print(std::cerr); std::cerr << "\n";

  objStack_.pop_back();
}

CBrowserObject *
CBrowserWindowIFace::
currentObj() const
{
  if (! objStack_.empty())
    return objStack_.back();
  else
    return nullptr;
}

CBrowserObject *
CBrowserWindowIFace::
getObject(const std::string &id) const
{
  auto p = idObjects_.find(id);

  if (p == idObjects_.end())
    return nullptr;

  return (*p).second;
}

CBrowserObject *
CBrowserWindowIFace::
headObject() const
{
  auto *root = rootObject();
  if (! root) return nullptr;

  for (const auto &c : root->children())
    if (c->type() == CHtmlTagId::HEAD)
      return c;

  return nullptr;
}

CBrowserObject *
CBrowserWindowIFace::
bodyObject() const
{
  auto *root = rootObject();
  if (! root) return nullptr;

  for (const auto &c : root->children())
    if (c->type() == CHtmlTagId::BODY)
      return c;

  return nullptr;
}

//---

int
CBrowserWindowIFace::
getCanvasWidth() const
{
  return (widget() ? widget()->windowWidth() : 100);
}

int
CBrowserWindowIFace::
getCanvasHeight() const
{
  return (widget() ? widget()->windowHeight() : 100);
}

//---

bool
CBrowserWindowIFace::
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
CBrowserWindowIFace::
loadCSSText(const std::string &text)
{
  CCSS css;

  if (! css.processLine(text))
    return false;

  cssList_.push_back(CSSData(css));

  return true;
}

void
CBrowserWindowIFace::
selectCSSPattern(const CCSS::StyleData &styleData)
{
  deselectAllObjects();

  for (auto &obj : objects_) {
    auto tagData = std::make_shared<CBrowserObjectCSSTagData>(obj);

    if (! styleData.checkMatch(tagData))
      continue;

    obj->setSelected(true);
  }

  redraw();
}

//---

bool
CBrowserWindowIFace::
applyStyle(CBrowserObject *obj)
{
  auto tagData = std::make_shared<CBrowserObjectCSSTagData>(obj);

  bool rc = true;

  for (const auto &css : cssList_) {
    if (! visitStyleData(css.css, tagData))
      rc = false;
  }

  return rc;
}

bool
CBrowserWindowIFace::
visitStyleData(const CCSS &css, const CCSSTagDataP &tagData)
{
  bool match = false;

  std::vector<CCSS::SelectorList> selectorListArray;

  css.getSelectors(selectorListArray);

  for (const auto &selectorList : selectorListArray) {
    const auto &styleData = css.getStyleData(selectorList);

    if (! styleData.checkMatch(tagData))
      continue;

    auto *obj = dynamic_cast<CBrowserObjectCSSTagData *>(tagData.get())->obj();

    for (const auto &opt : styleData.getOptions()) {
      obj->setStyleValue(opt.getName(), opt.getValue());
    }

    match = true;
  }

  return match;
}

void
CBrowserWindowIFace::
getTagNameValues(CHtmlTag *tag, NameValues &nameValues)
{
  auto cssTagData = std::make_shared<CHtmlCSSTagData>(tag);

  for (const auto &css : cssList_) {
    std::vector<CCSS::SelectorList> selectorListArray;

    css.css.getSelectors(selectorListArray);

    for (const auto &selectorList : selectorListArray) {
      const auto &styleData = css.css.getStyleData(selectorList);

      if (! styleData.checkMatch(cssTagData))
        continue;

      for (const auto &opt : styleData.getOptions()) {
        nameValues[opt.getName()] = opt.getValue();
      }
    }
  }
}

//---

void
CBrowserWindowIFace::
deselectAllObjects()
{
  selectSingleObject(nullptr);
}

void
CBrowserWindowIFace::
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

//---

void
CBrowserWindowIFace::
addScript(const std::string &text)
{
  scripts_.push_back(text);
}

void
CBrowserWindowIFace::
addScriptFile(const std::string &filename)
{
  scriptFiles_.push_back(filename);
}

void
CBrowserWindowIFace::
runScripts()
{
  for (const auto &s : scriptFiles_)
    CQJavaScriptInst->runScriptFile(s);

  for (const auto &s : scripts_)
    CQJavaScriptInst->runScript(s);

  CQJavaScriptInst->onLoad();
}

//---

void
CBrowserWindowIFace::
addHtmlObject(CBrowserObject *obj)
{
  auto *js = CQJavaScriptInst->js();

  auto *htmlObj = obj->createJObj(js);

  CJValueP value(htmlObj); // first create of shared pointer

  htmlObjMap_[obj] = value;

  CQJavaScriptInst->addHtmlObject(htmlObj);

  obj->setJObj(htmlObj);
}

CJValueP
CBrowserWindowIFace::
lookupHtmlObject(CBrowserObject *obj) const
{
  auto p = htmlObjMap_.find(obj);

  if (p == htmlObjMap_.end())
    return nullptr;

  return (*p).second;
}

//---

void
CBrowserWindowIFace::
addHistory(const CUrl &url)
{
  if (! history_->goTo(url))
    history_->addUrl(url);
}

void
CBrowserWindowIFace::
goBack()
{
  const auto &url = history_->goBack();
  if (! url.isValid()) return;

  setDocument(url);
}

void
CBrowserWindowIFace::
goForward()
{
  const auto &url = history_->goForward();
  if (! url.isValid()) return;

  setDocument(url);
}

//---

bool
CBrowserWindowIFace::
isVisible(int x1, int y1, int x2, int y2)
{
  if (x1 > bbox_.getXMax() || x2 < bbox_.getXMin() ||
      y1 > bbox_.getYMax() || y2 < bbox_.getYMin())
    return false;
  else
    return true;
}

//---

void
CBrowserWindowIFace::
mousePress(int, int)
{
}

void
CBrowserWindowIFace::
mouseMotion(int, int)
{
  // TODO
}

void
CBrowserWindowIFace::
mouseRelease(int x, int y)
{
  activateLink(x, y);
}

void
CBrowserWindowIFace::
mouseWheel(int)
{
}

//---

void
CBrowserWindowIFace::
saveImage(const QString &filename)
{
  widget()->saveImage(filename);
}

void
CBrowserWindowIFace::
print()
{
  auto *root = this->rootObject();

  CIBBox2D box;

  if (root)
    box = root->layoutBBox();

  int left, top;
  getMargins(left, top);

  double xmin = box.getXMin() - left;
  double ymin = box.getYMin() - top;
  double xmax = box.getXMax() + left;
  double ymax = box.getYMax() + top;

  auto *widget   = this->widget();
  auto *graphics = this->graphics();

  graphics->setPSDevice(xmin, ymin, xmax, ymax);

  widget->updateWidget(/*sync*/true);

  graphics->setXDevice();

  widget->updateWidget(/*sync*/false);
}

//---

void
CBrowserWindowIFace::
errorDialog(const QString &msg)
{
  auto *iface = browser()->iface();

  iface->errorDialog(msg);
}

void
CBrowserWindowIFace::
displayError(const char *format, ...)
{
  va_list args;

  va_start(args, format);

  browser()->displayError(format, &args);

  va_end(args);
}

void
CBrowserWindowIFace::
displayError(const QString &str)
{
  browser()->displayError(str);
}
