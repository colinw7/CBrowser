#include <CBrowserDocument.h>
#include <CBrowserWindow.h>
#include <CBrowserLink.h>
#include <CBrowserFile.h>
#include <CBrowserOutput.h>
#include <CBrowserText.h>
#include <CRGBName.h>

CBrowserDocument::
CBrowserDocument(CBrowserWindow *window)
{
  url_    = "";
  window_ = window;
  title_  = "";

  setBgColor   ("#c0c0c0");
  setFgColor   ("#000000");
  setLinkColor ("blue"   );
  setALinkColor("blue"   );
  setVLinkColor("blue"   );
}

CBrowserDocument::
~CBrowserDocument()
{
}

void
CBrowserDocument::
setUrl(const std::string &url)
{
  url_ = url;
}

void
CBrowserDocument::
addLink(CBrowserLink *link)
{
  links_.push_back(link);
}

uint
CBrowserDocument::
getNumLinks() const
{
  return links_.size();
}

CBrowserLink *
CBrowserDocument::
getLink(uint i)
{
  return links_[i];
}

void
CBrowserDocument::
addAnchor(CBrowserLink *anchor)
{
  anchors_.push_back(anchor);
}

uint
CBrowserDocument::
getNumAnchors() const
{
  return anchors_.size();
}

CBrowserLink *
CBrowserDocument::
getAnchor(uint i)
{
  return anchors_[i];
}

void
CBrowserDocument::
freeLinks()
{
  for (auto &link : links_)
    delete link;

  links_.clear();

  for (auto &anchor : anchors_)
    delete anchor;

  anchors_.clear();
}

void
CBrowserDocument::
read(const std::string &url)
{
  tokens_.clear();

  url_ = url;

  HtmlReadURL(window_, url_, tokens_);
}

void
CBrowserDocument::
setTitle(const std::string &title)
{
  title_ = title;

  window_->setTitle(title);
}

void
CBrowserDocument::
setBgColor(const std::string &color)
{
  if (color != "")
    bg_color_ = CRGBName::toRGBA(color);
}

void
CBrowserDocument::
setFgColor(const std::string &color)
{
  if (color != "") {
    fg_color_ = CRGBName::toRGBA(color);

    window_->setFontColor(color);
  }
}

void
CBrowserDocument::
setLinkColor(const std::string &color)
{
  if (color != "")
    link_color_ = CRGBName::toRGBA(color);
}

void
CBrowserDocument::
setALinkColor(const std::string &color)
{
  if (color != "")
    alink_color_ = CRGBName::toRGBA(color);
}

void
CBrowserDocument::
setVLinkColor(const std::string &color)
{
  if (color != "")
    vlink_color_ = CRGBName::toRGBA(color);
}
