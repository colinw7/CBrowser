#include <CBrowserHtmlI.h>
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
  uint num_links = links_.size();

  for (uint i = 0; i < num_links; ++i)
    delete links_[i];

  links_.clear();

  uint num_anchors = anchors_.size();

  for (uint i = 0; i < num_anchors; ++i)
    delete anchors_[i];

  anchors_.clear();
}

void
CBrowserDocument::
addForm(CBrowserForm *form)
{
  forms_.push_back(form);
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
output()
{
  CBrowserOutputTokens(window_, tokens_);
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

    HtmlSetTextColor(fg_color_);
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
