#ifndef CBrowserDocument_H
#define CBrowserDocument_H

#include <CBrowserTypes.h>
#include <CQJDocument.h>
#include <CQJDocumentIFace.h>
#include <CHtmlParser.h>
#include <CImagePtr.h>
#include <CUrl.h>
#include <CRGBA.h>

class CHtmlTag;
class CHtmlText;

class CBrowserDocument {
 public:
  typedef std::vector<CBrowserForm *> Forms;

 private:
  class IFace : public CQJDocumentIFace {
   public:
    IFace(CBrowserDocument *document) :
     document_(document) {
    }

    CJValueP createElement(const std::string &id) const override;

   private:
    CBrowserDocument *document_ { nullptr };
  };

 public:
  explicit CBrowserDocument(CBrowserWindow *window);

 ~CBrowserDocument();

  CQJDocumentIFace *iface() { return &iface_; }

  const CUrl &getUrl() const { return url_; }
  void setUrl(const CUrl &url);

  CBrowserWindow *getWindow() const { return window_; }

  CQJDocumentP document() const { return document_; }
  void setDocument(CQJDocumentP document) { document_ = document; }

  std::string getTitle() const { return title_; }
  void setTitle(const std::string &title);

  const CRGBA &getBgColor() const { return bgColor_; }
  void setBgColor(const std::string &color);
  void setBgColor(const CRGBA &color);

  const CRGBA &getFgColor() const { return fgColor_; }
  void setFgColor(const std::string &color);

  const CImagePtr &getBgImage() const { return bgImage_; }
  void setBgImage(const CImagePtr &image, bool fixed=false);

  const CRGBA &getLinkColor() const { return linkColor_; }
  void setLinkColor(const std::string &color);

  const CRGBA &getALinkColor() const { return alinkColor_; }
  void setALinkColor(const std::string &color);

  const CRGBA &getVLinkColor() const { return vlinkColor_; }
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

  bool read(const CUrl &url);

 private:
  typedef std::vector<CBrowserAnchorLink *> Links;

  IFace             iface_;              // javascript interface
  CUrl              url_;                // url of document
  CBrowserWindow*   window_ { nullptr }; // parent window
  CQJDocumentP      document_;           // javascript document
  std::string       title_;              // title
  CRGBA             bgColor_;            // background color
  CImagePtr         bgImage_;            // background image
  bool              bgFixed_ { false };  // background fixed
  CRGBA             fgColor_;            // foreground color
  CRGBA             linkColor_;          // link color
  CRGBA             alinkColor_;         // link active color
  CRGBA             vlinkColor_;         // link visited color
  Links             anchors_;            // anchors
  Links             links_;              // links
  Forms             forms_;              // forms
  CHtmlParserTokens tokens_;             // html tokens
};

#endif
