#include <CBrowserLink.h>
#include <CBrowserWindow.h>
#include <CBrowserDocument.h>
#include <CBrowserProperty.h>
#include <CUrl.h>
#include <CDir.h>

CBrowserLinkMgr::
CBrowserLinkMgr(CBrowserWindowIFace *window) :
 window_(window)
{
}

void
CBrowserLinkMgr::
startSourceLink(CBrowserAnchor *anchor)
{
  if (currentLink_) {
    fprintf(stderr, "Invalid Link within a Link\n");
    return;
  }

  std::string href1 = expandDestLink(anchor->href());

  if (href1 == "")
    href1 = "????";

  std::string title1 = anchor->title();

  if (title1 == "")
    title1 = href1;

  CBrowserAnchorLink *link =
    new CBrowserAnchorLink(CBrowserAnchorLink::Type::SOURCE, "", href1, title1);

  window_->getDocument()->addLink(link);

  currentLink_ = link;
}

void
CBrowserLinkMgr::
startDestLink(CBrowserAnchor *anchor)
{
  if (currentLink_) {
    fprintf(stderr, "Invalid Link within a Link\n");
    return;
  }

  std::string title1 = anchor->title();

  if (title1 == "")
    title1 = anchor->id();

  CBrowserAnchorLink *link =
    new CBrowserAnchorLink(CBrowserAnchorLink::Type::DEST, anchor->id(), "", title1);

  window_->getDocument()->addAnchor(link);

  currentLink_ = link;
}

void
CBrowserLinkMgr::
endLink()
{
  if (! currentLink_)
    return;

  currentLink_ = nullptr;
}

CBrowserAnchorLink *
CBrowserLinkMgr::
getCurrentLink()
{
  return currentLink_;
}

void
CBrowserLinkMgr::
clearLinkRects()
{
  if (! window_->getDocument())
    return;

  int num_links = window_->getDocument()->getNumLinks();

  for (int i = 0; i < num_links; i++) {
    CBrowserAnchorLink *link = window_->getDocument()->getLink(i);

    link->clearRects();
  }

  int num_anchors = window_->getDocument()->getNumAnchors();

  for (int i = 0; i < num_anchors; i++) {
    CBrowserAnchorLink *anchor = window_->getDocument()->getAnchor(i);

    anchor->clearRects();
  }
}

CBrowserAnchorLink *
CBrowserLinkMgr::
getSourceLink(int x, int y)
{
  if (! window_->getDocument())
    return nullptr;

  int num_links = window_->getDocument()->getNumLinks();

  for (int i = 0; i < num_links; i++) {
    CBrowserAnchorLink *link = window_->getDocument()->getLink(i);

    if (link->getType() != CBrowserAnchorLink::Type::SOURCE)
      continue;

    int num_rects = link->getNumRects();

    for (int j = 0; j < num_rects; j++) {
      const CBrowserLinkRect &rect = link->getRect(j);

      if (x >= rect.x1 && x <= rect.x2 && y >= rect.y1 && y <= rect.y2)
        return link;
    }
  }

  return nullptr;
}

int
CBrowserLinkMgr::
getDestLinkPos(const std::string &name, int *x, int *y)
{
  if (! window_->getDocument())
    return false;

  int num_links = window_->getDocument()->getNumAnchors();

  for (int i = 0; i < num_links; i++) {
    CBrowserAnchorLink *anchor = window_->getDocument()->getAnchor(i);

    if (anchor->getType() != CBrowserAnchorLink::Type::DEST)
      continue;

    if (anchor->getName() == name)
      continue;

    int num_rects = anchor->getNumRects();

    if (num_rects > 0) {
      const CBrowserLinkRect &rect = anchor->getRect(0);

      *x = rect.x1;
      *y = rect.y1;

      return true;
    }
  }

  return false;
}

std::string
CBrowserLinkMgr::
expandDestLink(const std::string &dest) const
{
  int len = dest.size();

  int i = 0;

  CStrUtil::skipSpace(dest, &i);

  // get prefix
  int j = i;

  while (i < len && dest[i] != ':')
    i++;

  std::string prefix;

  if (dest[i] == ':') {
    prefix = dest.substr(j, i - j);

    prefix = CStrUtil::stripSpaces(prefix);

    i++;
  }
  else {
    prefix = "file";

    i = 0;
  }

  //---

  CStrUtil::skipSpace(dest, &i);

  std::string dest2;

  if     (prefix == "file") {
    std::string dest1;

    if (dest[i] != '/') {
      std::string current_dir = CDir::getCurrent();

      dest1 = current_dir + "/" + dest.substr(i);
    }
    else
      dest1 = dest.substr(i);

    dest2 = prefix + "://" + dest1;
  }
  else
    dest2 = dest;

  dest2 = CStrUtil::stripSpaces(dest2);

  return dest2;
}

//------

CBrowserAnchorLink::
CBrowserAnchorLink(Type type, const std::string &name, const std::string &dest,
                   const std::string &title) :
 type_(type), name_(name), dest_(dest), title_(title)
{
}

CBrowserAnchorLink::
~CBrowserAnchorLink()
{
}

int
CBrowserAnchorLink::
getNumRects()
{
  return rects_.size();
}

const CBrowserLinkRect &
CBrowserAnchorLink::
getRect(int i)
{
  return rects_[i];
}

void
CBrowserAnchorLink::
addRect(int x1, int y1, int x2, int y2)
{
  rects_.push_back(CBrowserLinkRect(x1, y1, x2, y2));
}

void
CBrowserAnchorLink::
clearRects()
{
  rects_.clear();
}

//------

CBrowserAnchor::
CBrowserAnchor(CBrowserWindowIFace *window) :
 CBrowserObject(window, CHtmlTagId::A)
{
  setDisplay(CBrowserObject::Display::INLINE);

  setForeground(CBrowserColor(window_->getDocument()->getLinkColor()));

  textProp_.setDecoration(CBrowserTextDecoration("underline"));

  // cursor: auto
}

CBrowserAnchor::
~CBrowserAnchor()
{
}

void
CBrowserAnchor::
setNameValue(const std::string &name, const std::string &value)
{
  std::string lname = CStrUtil::toLower(name);

  if      (lname == "charset") {
  }
  else if (lname == "coords") {
  }
  else if (lname == "download") {
    data_.download = value;
  }
  else if (lname == "href") {
    data_.href = value;
  }
  else if (lname == "hreflang") {
  }
  else if (lname == "itemprop") {
  }
  else if (lname == "media") {
  }
  else if (lname == "methods") {
    data_.methods = value;
  }
  else if (lname == "rel") {
    data_.rel = value;
  }
  else if (lname == "rev") {
    data_.rev = value;
  }
  else if (lname == "shape") {
  }
  else if (lname == "target") {
    data_.target = value;
  }
  else if (lname == "type") {
    data_.type = value;
  }
  else if (lname == "url") {
    data_.url = value;
  }
  else {
    CBrowserObject::setNameValue(name, value);
  }
}

void
CBrowserAnchor::
init()
{
  std::vector<std::string> strs = {{
    "download", "href", "methods", "rel", "rev", "target", "title", "url" }};

  addProperties(strs);

  //---

  if (href() == "" && id() == "")
    window_->displayError("No 'href' or 'name' specified for 'a' Tag'\n");

  CBrowserObject::init();
}

void
CBrowserAnchor::
initProcess()
{
  if (href() != "") {
    window_->startSourceLink(this);
  }
  else
    window_->startDestLink(this);
}

void
CBrowserAnchor::
termProcess()
{
  window_->endLink();
}

std::string
CBrowserAnchor::
propertyValue(int i) const
{
  const std::string &name = propertyName(i);

  if      (name == "download") return CBrowserProperty::toString(data_.download);
  else if (name == "href"    ) return CBrowserProperty::toString(data_.href);
  else if (name == "methods" ) return CBrowserProperty::toString(data_.methods);
  else if (name == "rel"     ) return CBrowserProperty::toString(data_.rel);
  else if (name == "rev"     ) return CBrowserProperty::toString(data_.rev);
  else if (name == "target"  ) return CBrowserProperty::toString(data_.target);
  else if (name == "url"     ) return CBrowserProperty::toString(data_.url);

  return CBrowserObject::propertyValue(i);
}

//------

CBrowserLink::
CBrowserLink(CBrowserWindowIFace *window) :
 CBrowserObject(window, CHtmlTagId::LINK)
{
}

CBrowserLink::
~CBrowserLink()
{
}

void
CBrowserLink::
init()
{
  std::vector<std::string> strs = {{
    "rel", "href", "type", "media" }};

  addProperties(strs);

  CBrowserObject::init();
}

std::string
CBrowserLink::
propertyValue(int i) const
{
  const std::string &name = propertyName(i);

  if      (name == "rel"  ) return CBrowserProperty::toString(data_.rel);
  else if (name == "href" ) return CBrowserProperty::toString(data_.href);
  else if (name == "type" ) return CBrowserProperty::toString(data_.type);
  else if (name == "media") return CBrowserProperty::toString(data_.media);

  return "";
}

void
CBrowserLink::
setNameValue(const std::string &name, const std::string &value)
{
  std::string lname  = CStrUtil::toLower(name);
  std::string lvalue = CStrUtil::toLower(value);

  if      (lname == "rel") {
    data_.rel = value;
  }
  else if (lname == "href") {
    data_.href = value;
  }
  else if (lname == "type") {
    data_.type = value;
  }
  else if (lname == "media") {
    data_.media = value;
  }
  else {
    CBrowserObject::setNameValue(name, value);
  }
}

void
CBrowserLink::
initProcess()
{
  if      (data_.rel == "stylesheet") {
    std::string href = window_->expandDestLink(data_.href);

    if (href == "")
      return;

    CUrl url(href);

    if (! window_->loadCSSFile(url))
      return;
  }
  else if (data_.rel == "shortcut icon") {
     std::string href = window_->expandDestLink(data_.href);

    if (href == "")
      return;

    CUrl url(href);

    if (! window_->setShortcutIcon(url))
      return;
  }
  else {
    window_->displayError("Invalid link rel '%s'", data_.rel.c_str());
  }
}

void
CBrowserLink::
termProcess()
{
}
