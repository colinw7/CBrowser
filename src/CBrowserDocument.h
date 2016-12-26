#ifndef CBrowserDocument_H
#define CBrowserDocument_H

#include <CBrowserTypes.h>
#include <CHtmlParser.h>
#include <CRGBA.h>

class CHtmlTag;
class CHtmlText;

class CBrowserDocument {
 public:
  CBrowserDocument(CBrowserWindow *window);
 ~CBrowserDocument();

  std::string getUrl() const { return url_; }

  void setUrl(const std::string &url);

  CBrowserWindow *getWindow() const { return window_; }

  std::string getTitle() const { return title_; }

  void setTitle(const std::string &title);

  const CRGBA &getBgColor() const { return bg_color_; }
  const CRGBA &getFgColor() const { return fg_color_; }
  const CRGBA &getLinkColor() const { return link_color_; }
  const CRGBA &getALinkColor() const { return alink_color_; }
  const CRGBA &getVLinkColor() const { return vlink_color_; }

  void setBgColor   (const std::string &color);
  void setFgColor   (const std::string &color);
  void setLinkColor (const std::string &color);
  void setALinkColor(const std::string &color);
  void setVLinkColor(const std::string &color);

  void addLink(CBrowserLink *link);
  uint getNumLinks() const;
  CBrowserLink *getLink(uint i);

  void addAnchor(CBrowserLink *anchor);
  uint getNumAnchors() const;
  CBrowserLink *getAnchor(uint i);
  void freeLinks();

  const CHtmlParserTokens &tokens() const { return tokens_; }

  void read(const std::string &url);

 private:
  typedef std::vector<CBrowserLink *> Links;

  std::string       url_;
  CBrowserWindow*   window_ { nullptr };
  std::string       title_;
  CRGBA             bg_color_;
  CRGBA             fg_color_;
  CRGBA             link_color_;
  CRGBA             alink_color_;
  CRGBA             vlink_color_;
  Links             anchors_;
  Links             links_;
  CHtmlParserTokens tokens_;
};

#endif
