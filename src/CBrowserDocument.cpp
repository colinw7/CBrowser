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

//setBgColor   ("#c0c0c0");
  setBgColor   ("#ffffff");
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
setUrl(const CUrl &url)
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

bool
CBrowserDocument::
read(const CUrl &url)
{
  tokens_.clear();

  url_ = url;

  if      (url_.isFile()) {
    std::string filename = url_.getFile();

    if (! CFile::exists(filename)) {
      window_->displayError("File '%s' does not exist", filename.c_str());
      return false;
    }

    if (! window_->fileMgr()->readFile(filename, tokens_))
      return false;
  }
  else if (url.isHttp()) {
    std::string filename;

    if (! window_->downloadFile(url, filename)) {
      window_->displayError("Failed to download '%s'", url.getUrl().c_str());
      return false;
    }

    if (! CFile::exists(filename)) {
      window_->displayError("File '%s' does not exist", filename.c_str());
      return false;
    }

    if (! window_->fileMgr()->readFile(filename, tokens_)) {
      window_->displayError("Failed to read file '%s'", filename.c_str());
      return false;
    }
  }
  else {
    window_->displayError("Invalid url '%s'", url.getUrl().c_str());
    return false;
  }

  return true;
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
    CRGBName::toHtmlRGBA(color, bgColor_);
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
    CRGBName::toHtmlRGBA(color, fgColor_);
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
    CRGBName::toHtmlRGBA(color, linkColor_);
}

void
CBrowserDocument::
setALinkColor(const std::string &color)
{
  if (color != "")
    CRGBName::toHtmlRGBA(color, alinkColor_);
}

void
CBrowserDocument::
setVLinkColor(const std::string &color)
{
  if (color != "")
    CRGBName::toHtmlRGBA(color, vlinkColor_);
}

void
CBrowserDocument::
addForm(CBrowserForm *form)
{
  forms_.push_back(form);
}
