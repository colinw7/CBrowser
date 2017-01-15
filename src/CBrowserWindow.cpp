#include <CBrowserWindow.h>
#include <CBrowserDocument.h>
#include <CBrowserHistory.h>
#include <CBrowserIFace.h>
#include <CBrowserWindowWidget.h>
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
#include <CRGBName.h>
#include <CFontMgr.h>
#include <CEnv.h>

std::list<CBrowserWindow *> CBrowserWindow::window_list_;
std::string                 CBrowserWindow::default_font_face_ = "";
std::string                 CBrowserWindow::window_target_     = "";
CBrowserAnchorLink*         CBrowserWindow::mouse_link_        = nullptr;

CBrowserWindow::
CBrowserWindow(const std::string &filename) :
 output_(this)
{
  init();

  if (filename != "")
    setDocument(filename);

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
  //default_font_face_ = "";

  window_target_ = "";

  mouse_link_ = nullptr;

  name_     = "";
  document_ = nullptr;

  iface_ = nullptr;
  w_     = nullptr;

  leftMargin_ = CBrowserDataConstants::LEFT_MARGIN;
  topMargin_  = CBrowserDataConstants::TOP_MARGIN;

  bbox_ = CIBBox2D(leftMargin_, topMargin_, 2*leftMargin_, 2*topMargin_);

  layout_ = new CBrowserLayout(this);

  linkMgr_ = new CBrowserLinkMgr(this);
  fileMgr_ = new CBrowserFileMgr(this);

  baseFontSize_ = 3;

  history_ = new CBrowserHistory(this);

  //---

  loadResources();

  //---

  //startFontFace(default_font_face_);
}

void
CBrowserWindow::
setIFace(CBrowserIFace *iface)
{
  iface_ = iface;
  w_     = iface_->widget();
}

void
CBrowserWindow::
loadResources()
{
  //default_font_face_ = "helvetica";
}

void
CBrowserWindow::
processTokens(const CHtmlParserTokens &tokens)
{
  outputState_ = OutputState::PROCESS_TOKENS;

  output_.processTokens(tokens);

  outputState_ = OutputState::NONE;
}

void
CBrowserWindow::
layoutObjects()
{
  outputState_ = OutputState::LAYOUT_OBJECTS;

  output_.layoutObjects();

  outputState_ = OutputState::NONE;
}

void
CBrowserWindow::
setBaseFontSize(int size)
{
assert(outputState_ != OutputState::LAYOUT_OBJECTS);
  baseFontSize_ = size - 1;

  setBaseFontStyle();
}

int
CBrowserWindow::
getBaseFontSize()
{
  return (baseFontSize_ + 1);
}

void
CBrowserWindow::
resetBaseFontSize()
{
assert(outputState_ != OutputState::LAYOUT_OBJECTS);
  baseFontSize_ = 3;

  setBaseFontStyle();
}

void
CBrowserWindow::
increaseBaseFontSize(int d)
{
assert(outputState_ != OutputState::LAYOUT_OBJECTS);
  baseFontSize_ += d;

  setBaseFontStyle();
}

void
CBrowserWindow::
decreaseBaseFontSize(int d)
{
assert(outputState_ != OutputState::LAYOUT_OBJECTS);
  baseFontSize_ -= d;

  setBaseFontStyle();
}

void
CBrowserWindow::
setBaseFontStyle()
{
  int size = baseFontSize_;

  CBrowserFontSize fontSize = sizeToFontSize(size);

  int isize = fontSize.value(12);

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
    assert(outputState_ != OutputState::LAYOUT_OBJECTS);

    CBrowserObject *currentObj = this->currentObj();

    if (currentObj)
      currentObj->addChild(obj);
  }

  objStack_.push_back(obj);

  if (add) {
    objects_[obj->id()] = obj;

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
  auto p = objects_.find(id);

  if (p == objects_.end())
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
  iface_->setBusy();

  //---

  bbox_ = CIBBox2D(leftMargin_, topMargin_, w_->width() - leftMargin_, w_->height() - topMargin_);

  linkMgr()->deleteLinkRects();

  //------

  layout_->layout(bbox_);

  //------

  int w = w_->width ();
  int h = w_->height();

  if (rootObject()) {
    w = rootObject()->contentWidth ();
    h = rootObject()->contentHeight();
  }

  iface_->setSize(w, h);

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
      iface_->scrollTo(x, y);
  }

  //---

  redraw();
}

void
CBrowserWindow::
redraw()
{
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
  return iface_->getCanvasXOffset();
}

int
CBrowserWindow::
getCanvasYOffset() const
{
  return iface_->getCanvasYOffset();
}

int
CBrowserWindow::
getCanvasWidth() const
{
  return iface_->getCanvasWidth();
}

int
CBrowserWindow::
getCanvasHeight() const
{
  return iface_->getCanvasHeight();
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
  leftMargin_ = left;
  topMargin_  = top;
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
loadCSSFile(const std::string &filename)
{
  if (! css_.processFile(filename))
    return false;

  return processCSSSelectors();
}

bool
CBrowserWindow::
loadCSSText(const std::string &text)
{
  if (! css_.processLine(text))
    return false;

  return processCSSSelectors();
}

bool
CBrowserWindow::
processCSSSelectors()
{
  std::vector<CCSS::Selector> selectors;

  css_.getSelectors(selectors);

  for (const auto &selector : selectors) {
    CCSS::SelectName::Type type = selector.type();

    std::string objName, objType, objClass;

    if      (type == CCSS::SelectName::Type::TYPE) {
      objType = selector.name();
    }
    else if (type == CCSS::SelectName::Type::CLASS) {
      objType  = selector.name();
      objClass = selector.className();
    }
    else if (type == CCSS::SelectName::Type::ID) {
      objName = selector.name();
    }

    if (objName == "" && objType == "" && objClass == "")
      continue;

    //----

    CCSS::SelectName::SubType subType = selector.subType();

    if      (subType == CCSS::SelectName::SubType::CHILD) {
    }
    else if (subType == CCSS::SelectName::SubType::SIBLING) {
    }
    else if (subType == CCSS::SelectName::SubType::PRECEDER) {
    }

    std::string objSel = selector.expr();
    std::string objFn  = selector.function();

    std::string objFnArgs;

    //---

    if (objFn != "") {
      auto p = objFn.find("(");

      if (p != std::string::npos) {
        objFnArgs = objFn.substr(p + 1);
        objFn     = objFn.substr(0, p);

        auto p1 = objFnArgs.find(")");

        if (p1 != std::string::npos)
          objFnArgs = objFnArgs.substr(0, p1);
      }
    }

    //---

    const CCSS::StyleData &cssStyleData = css_.getStyleData(selector);

    //---

    if (objType  == "*") objType  = "";
    if (objClass == "*") objClass = "";

    if      (objName != "")
      addStyleValues(cssData_.getNameStyleData(objName), cssStyleData);
    else if (objType != "" && objClass == "")
      addStyleValues(cssData_.getTypeStyleData(objType), cssStyleData);
    else if (objType == "" && objClass != "")
      addStyleValues(cssData_.getClassStyleData(objClass), cssStyleData);
    else if (objType != "" && objClass  != "")
      addStyleValues(cssData_.getTypeClassStyleData(objType, objClass), cssStyleData);
    else
      addStyleValues(cssData_.getGlobalStyleData(), cssStyleData);
  }

  return true;
}

void
CBrowserWindow::
addStyleValues(CBrowserStyleData &styleData, const CCSS::StyleData &cssStyleData)
{
  for (const auto &option : cssStyleData.getOptions())
    styleData.setValue(option.getName(), option.getValue());
}

//------

namespace {

template<typename VISITOR>
bool
visitStyleData(CBrowserCSSData &cssData, const CBrowserObject *obj, VISITOR &visitor)
{
  if (cssData.hasNameStyleData(obj->id())) {
    CBrowserStyleData &nameStyleData = cssData.getNameStyleData(obj->id());

   if (visitor(CBrowserCSSType::NAME, nameStyleData))
     return true;
  }

  for (const auto &c : obj->getClasses()) {
    if (cssData.hasTypeClassStyleData(obj->typeName(), c)) {
      CBrowserStyleData &typeClassStyleData = cssData.getTypeClassStyleData(obj->typeName(), c);

      if (visitor(CBrowserCSSType::TYPE_CLASS, typeClassStyleData))
        return true;
    }
  }

  if (cssData.hasTypeStyleData(obj->typeName())) {
    CBrowserStyleData &typeStyleData = cssData.getTypeStyleData(obj->typeName());

    if (visitor(CBrowserCSSType::TYPE, typeStyleData))
      return true;
  }

  for (const auto &c : obj->getClasses()) {
    if (cssData.hasClassStyleData(c)) {
      CBrowserStyleData &classStyleData = cssData.getClassStyleData(c);

      if (visitor(CBrowserCSSType::CLASS, classStyleData))
        return true;
    }
  }

  if (cssData.hasGlobalStyleData()) {
    CBrowserStyleData &globalStyleData = cssData.getGlobalStyleData();

    if (visitor(CBrowserCSSType::GLOBAL, globalStyleData))
      return true;
  }

  return false;
}

}

bool
CBrowserWindow::
applyStyle(CBrowserObject *obj)
{
  auto visitor = [&](CBrowserCSSType, CBrowserStyleData &styleData) -> bool {
    for (const auto &nv : styleData.nameValues()) {
      obj->setNameValue(nv.first, nv.second);
    }

    return false;
  };

  return visitStyleData(cssData_, obj, visitor);
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

  iface_->print(xmin, ymin, xmax, ymax);
}

void
CBrowserWindow::
saveImage(const std::string &filename)
{
  iface_->saveImage(filename);
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
  CBrowserAnchorLink *link = linkMgr()->getSourceLink(x, y);

  if (mouse_link_ == link)
    return false;

  if (link) {
    mouse_link_ = link;

    link_name = link->getTitle();
  }
  else {
    mouse_link_ = nullptr;

    link_name = "";
  }

  setStatus(link_name);

  return true;
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
addHistoryItem(const std::string &item)
{
  iface_->addHistoryItem(item);
}

CRGBA
CBrowserWindow::
getBgColor()
{
  if (document_)
    return document_->getBgColor();
  else
    return CRGBA();
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
  if      (size <= 1) return CBrowserFontSize("0.67em");
  else if (size == 2) return CBrowserFontSize("0.83em");
  else if (size == 3) return CBrowserFontSize("1.00em");
  else if (size == 4) return CBrowserFontSize("1.17em");
  else if (size == 5) return CBrowserFontSize("1.50em");
  else if (size == 6) return CBrowserFontSize("2.00em");
  else if (size >= 7) return CBrowserFontSize("3.00em");

  return CBrowserFontSize("1.00em");
}
