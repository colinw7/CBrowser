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
  window_ = window;

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
addLink(CBrowserAnchorLink *link)
{
  links_.push_back(link);
}

uint
CBrowserDocument::
getNumLinks() const
{
  return links_.size();
}

CBrowserAnchorLink *
CBrowserDocument::
getLink(uint i)
{
  return links_[i];
}

void
CBrowserDocument::
addAnchor(CBrowserAnchorLink *anchor)
{
  anchors_.push_back(anchor);
}

uint
CBrowserDocument::
getNumAnchors() const
{
  return anchors_.size();
}

CBrowserAnchorLink *
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

  window_->fileMgr()->readURL(url_, tokens_);
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
    bgColor_ = CRGBName::toRGBA(color);
}

void
CBrowserDocument::
setBgColor(const CRGBA &color)
{
  bgColor_ = color;
}

void
CBrowserDocument::
setFgColor(const std::string &color)
{
  if (color != "")
    fgColor_ = CRGBName::toRGBA(color);
}

void
CBrowserDocument::
setBgImage(const CImagePtr &image, bool fixed)
{
  bgImage_ = image;
  bgFixed_ = fixed;
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

void
CBrowserDocument::
addForm(CBrowserForm *form)
{
  forms_.push_back(form);
}
