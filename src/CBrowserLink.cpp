#include <CBrowserLink.h>
#include <CBrowserWindow.h>
#include <CBrowserDocument.h>
#include <CDir.h>

CBrowserLinkMgr::
CBrowserLinkMgr(CBrowserWindow *window) :
 window_(window)
{
}

void
CBrowserLinkMgr::
startSourceLink(const std::string &dest, const std::string &title)
{
  if (current_link_) {
    fprintf(stderr, "Invalid Link within a Link\n");
    return;
  }

  std::string dest1 = expandDestLink(dest);

  if (dest1 == "")
    dest1 = "????";

  std::string title1 = title;

  if (title1 == "")
    title1 = dest1;

  CBrowserLink *link = new CBrowserLink(SOURCE_LINK, "", dest1, title1);

  window_->getDocument()->addLink(link);

  current_link_ = link;
}

void
CBrowserLinkMgr::
startDestLink(const std::string &name, const std::string &title)
{
  if (current_link_) {
    fprintf(stderr, "Invalid Link within a Link\n");
    return;
  }

  std::string title1 = title;

  if (title1 == "")
    title1 = name;

  CBrowserLink *link = new CBrowserLink(DEST_LINK, name, "", title1);

  window_->getDocument()->addAnchor(link);

  current_link_ = link;
}

void
CBrowserLinkMgr::
endLink()
{
  if (! current_link_)
    return;

  current_link_ = nullptr;
}

CBrowserLink *
CBrowserLinkMgr::
getCurrentLink()
{
  return current_link_;
}

void
CBrowserLinkMgr::
deleteLinkRects()
{
  if (! window_->getDocument())
    return;

  int num_links = window_->getDocument()->getNumLinks();

  for (int i = 0; i < num_links; i++) {
    CBrowserLink *link = window_->getDocument()->getLink(i);

    link->deleteRects();
  }

  int num_anchors = window_->getDocument()->getNumAnchors();

  for (int i = 0; i < num_anchors; i++) {
    CBrowserLink *anchor = window_->getDocument()->getAnchor(i);

    anchor->deleteRects();
  }
}

CBrowserLink *
CBrowserLinkMgr::
getSourceLink(int x, int y)
{
  if (! window_->getDocument())
    return nullptr;

  int num_links = window_->getDocument()->getNumLinks();

  for (int i = 0; i < num_links; i++) {
    CBrowserLink *link = window_->getDocument()->getLink(i);

    if (link->getType() != SOURCE_LINK)
      continue;

    int num_rects = link->getNumRects();

    for (int j = 0; j < num_rects; j++) {
      CBrowserLinkRect *rect = link->getRect(j);

      if (x >= rect->x1 && x <= rect->x2 && y >= rect->y1 && y <= rect->y2)
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
    CBrowserLink *anchor = window_->getDocument()->getAnchor(i);

    if (anchor->getType() != DEST_LINK)
      continue;

    if (anchor->getName() == name)
      continue;

    int num_rects = anchor->getNumRects();

    if (num_rects > 0) {
      CBrowserLinkRect *rect = anchor->getRect(0);

      *x = rect->x1;
      *y = rect->y1;

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

  CStrUtil::skipSpace(dest, &i);

  std::string dest1;

  if     (prefix.substr(0, 4) == "file") {
    if (dest[i] != '/') {
      std::string current_dir = CDir::getCurrent();

      dest1 = current_dir + "/" + dest.substr(i);
    }
    else
      dest1 = dest.substr(i);
  }
  else
    dest1 = dest[i];

  dest1 = CStrUtil::stripSpaces(dest1);

  std::string dest2 = prefix + ":" + dest1;

  return dest2;
}

//------

CBrowserLink::
CBrowserLink(int type, const std::string &name, const std::string &dest,
             const std::string &title) :
 type_(type), name_(name), dest_(dest), title_(title)
{
}

CBrowserLink::
~CBrowserLink()
{
  deleteRects();
}

int
CBrowserLink::
getNumRects()
{
  return rects_.size();
}

CBrowserLinkRect *
CBrowserLink::
getRect(int i)
{
  return rects_[i];
}

void
CBrowserLink::
addRect(int x1, int y1, int x2, int y2)
{
  CBrowserLinkRect *rect = new CBrowserLinkRect;

  rect->x1 = x1;
  rect->y1 = y1;
  rect->x2 = x2;
  rect->y2 = y2;

  rects_.push_back(rect);
}

void
CBrowserLink::
deleteRects()
{
  int num_rects = rects_.size();

  for (int i = 0; i < num_rects; i++)
    delete rects_[i];

  rects_.clear();
}
