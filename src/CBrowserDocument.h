#ifndef CBrowserDocument_H
#define CBrowserDocument_H

#include <CBrowserTypes.h>
#include <CHtmlParser.h>
#include <CRGBA.h>

class CHtmlTag;
class CHtmlText;

class CBrowserDocument {
 public:
  typedef std::vector<CBrowserForm *> Forms;

 public:
  explicit CBrowserDocument(CBrowserWindow *window);

 ~CBrowserDocument();

  std::string getUrl() const { return url_; }

  void setUrl(const std::string &url);

  CBrowserWindow *getWindow() const { return window_; }

  std::string getTitle() const { return title_; }

  void setTitle(const std::string &title);

  const CRGBA &getBgColor() const { return bgColor_; }
  void setBgColor(const std::string &color);
  void setBgColor(const CRGBA &color);

  const CRGBA &getFgColor() const { return fgColor_; }
  void setFgColor(const std::string &color);

  const CImagePtr &getBgImage() const { return bgImage_; }
  void setBgImage(const CImagePtr &image, bool fixed=false);

  const CRGBA &getLinkColor() const { return link_color_; }
  void setLinkColor(const std::string &color);

  const CRGBA &getALinkColor() const { return alink_color_; }
  void setALinkColor(const std::string &color);

  const CRGBA &getVLinkColor() const { return vlink_color_; }
  void setVLinkColor(const std::string &color);

  void addLink(CBrowserAnchorLink *link);
  uint getNumLinks() const;
  CBrowserAnchorLink *getLink(uint i);

  void addAnchor(CBrowserAnchorLink *anchor);
  uint getNumAnchors() const;
  CBrowserAnchorLink *getAnchor(uint i);
  void freeLinks();

  const CHtmlParserTokens &tokens() const { return tokens_; }

  const Forms &forms() const { return forms_; }
  void addForm(CBrowserForm *form);
//void removeForm(CBrowserForm *form);

  void read(const std::string &url);

 private:
  typedef std::vector<CBrowserAnchorLink *> Links;

  std::string       url_;
  CBrowserWindow*   window_ { nullptr };
  std::string       title_;
  CRGBA             bgColor_;
  CRGBA             fgColor_;
  CImagePtr         bgImage_;
  bool              bgFixed_ { false };
  CRGBA             link_color_;
  CRGBA             alink_color_;
  CRGBA             vlink_color_;
  Links             anchors_;
  Links             links_;
  CHtmlParserTokens tokens_;
  Forms             forms_;
};

#endif
