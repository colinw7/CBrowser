#ifndef CBrowserWindow_H
#define CBrowserWindow_H

#include <CBrowserTypes.h>
#include <CBrowserOutput.h>
#include <CBrowserData.h>
#include <CBrowserFont.h>
#include <CBrowserObjectCSSTagData.h>
#include <CQJWindow.h>
#include <CQJWindowIFace.h>
#include <CQJDocument.h>
#include <CHtmlParser.h>
#include <CUrl.h>
#include <CFont.h>
#include <QImage>

class CBrowserScrolledWindow;

//---

class CBrowserWindow {
 public:
  enum class OutputState {
    NONE,
    PROCESS_TOKENS,
    LAYOUT_OBJECTS
  };

 public:
  typedef std::map<std::string, std::string> NameValues;

  struct CSSData {
    CCSS css;
    CUrl url;

    CSSData(const CCSS &css1, const CUrl &url1=CUrl()) :
     css(css1), url(url1) {
    }
  };

  typedef std::vector<CSSData> CSSList;

 private:
  class IFace : public CQJWindowIFace {
   public:
    IFace(CBrowserWindow *window) :
     window_(window) {
    }

    CBrowserWindow *window() const { return window_; }

   private:
    CBrowserWindow *window_ { nullptr };
  };

 public:
  explicit CBrowserWindow(const std::string &filename="");

 ~CBrowserWindow();

  CQJWindowIFace *iface() { return &iface_; }

  void setScrolledWindow(CBrowserScrolledWindow *swindow);

  CBrowserWindowWidget *widget() const { return w_; }

  CBrowserDocument *getDocument() const { return document_; }

  void setWindow(CQJWindowP window) { window_ = window; }

  const std::string &filename() const { return filename_; }
  void setFilename(const std::string &s) { filename_ = s; }

  int getX() const { return bbox_.getXMin(); }
  int getY() const { return bbox_.getYMin(); }

  int getWidth () const { return bbox_.getWidth (); }
  int getHeight() const { return bbox_.getHeight(); }

  int getLeftMargin() const { return leftMargin_; }
  int getTopMargin () const { return topMargin_ ; }

  CBrowserLayout *getLayout() const { return layout_; }

  CBrowserLinkMgr *linkMgr() const { return linkMgr_; }

  CBrowserFileMgr *fileMgr() const { return fileMgr_; }

  //---

  void setBaseFontSize(int size);
  int  getBaseFontSize();
  void resetBaseFontSize();
  void increaseBaseFontSize(int d=1);
  void decreaseBaseFontSize(int d=1);

  void setBaseFontStyle();

  //---

  const CSSList &cssList() const { return cssList_; }
  void setCssList(const CSSList &v) { cssList_ = v; }

  //---

  CFontPtr getFont() const { return font_; }

  //---

  CImagePtr lookupImage(const CBrowserImageData &imageData, int iwidth, int iheight);

  //---

  CBrowserObject *rootObject() const { return rootObject_; }
  void setRootObject(CBrowserObject *p) { rootObject_ = p; }

  //---

  CBrowserObject *createElement(const std::string &id);

  //---

  void startObject(CBrowserObject *obj, bool add);
  void endObject();

  //---

  void addHtmlObject(CBrowserObject *obj);
  CJValueP lookupHtmlObject(CBrowserObject *htmlObj) const;

  //---

  CBrowserObject *currentObj() const;

  void addObject(CBrowserObject *obj);

  //---

  CBrowserObject *getObject(const std::string &id) const;

  CBrowserObject *headObject() const;
  CBrowserObject *bodyObject() const;

  //---

  void addScript(const std::string &text);
  void addScriptFile(const std::string &filename);

  void runScripts();

  //---

  void close();

  void setName(const std::string &name);

  void setDocument(const CUrl &url);

  void outputDocument();

  void processTokens(const CHtmlParserTokens &tokens);

  void layoutObjects();

  void resize();
  void redraw();

  int getCanvasXOffset() const;
  int getCanvasYOffset() const;

  int getCanvasWidth() const;
  int getCanvasHeight() const;

  void drawDocument();

  void setTitle(const std::string &title);
  void setMargins(int, int);
  void setBackgroundImage(const std::string &name, bool fixed);
  void setTarget(const std::string &target);

  //---

  bool loadCSSFile(const CUrl &url);
  bool loadCSSText(const std::string &filename);

  bool applyStyle(CBrowserObject *obj);

  bool visitStyleData(const CCSS &css, const CCSSTagDataP &tagData);

  void getTagNameValues(CHtmlTag *tag, NameValues &nameValues);

  void selectCSSPattern(const CCSS::StyleData &styleData);

  //---

  bool setShortcutIcon(const CUrl &url);

  //---

  void setStatus(const std::string &status);

  void displayError(const char *format, ...);

  bool isVisible(int x1, int y1, int x2, int y2);

  void print();
  void saveImage(const std::string &filename);

  void goBack();
  void goForward();

  bool hoverLink(int x, int y, std::string &link_name);

  bool activateLink(int x, int y);

  void addHistoryItem(const CUrl &item);

  CRGBA getBgColor();
  CRGBA getFgColor();

  CImagePtr getBgImage();

  void drawImage(int x, int y, const CImagePtr &image);
  void drawImage(int x, int y, const QImage &image);

  void drawRectangle(int x, int y, int w, int h, const CPen &pen);
  void fillRectangle(int x, int y, int w, int h, const CBrush &brush);

  void fillPolygon(const std::vector<CIPoint2D> &points, const CBrush &brush);

  void drawCircle(int x, int y, int r, const CPen &pen);
  void fillCircle(int x, int y, int r, const CBrush &brush);

  void drawLine(int x1, int y1, int x2, int y2, const CPen &pen);

  void drawText(int x, int y, const std::string &str, const CPen &pen, const CFontPtr &font);

  void drawOutline(int x, int y, int width, int height, const CPen &pen);

  void drawSelected(int x, int y, int width, int height);

  void drawBorder(int x, int y, int width, int height, CBrowserBorderType type);

  void drawHRule(int x1, int x2, int y, int height);

  void getTextSize(const std::string &text, int *width, int *ascent, int *descent) const;
  void getTextWidth (CFontPtr font, const std::string &text, int *width) const;
  void getTextHeight(CFontPtr font, int *ascent, int *descent) const;

  void errorDialog(const std::string &msg);

  void recalc();

  //---

  void parseHAlignOption(const std::string &name, CHAlignType &align);
  void parseVAlignOption(const std::string &name, CVAlignType &align);

  //---

  CBrowserFontSize sizeToFontSize(int size) const;

  //---

  void deselectAllObjects();

  void selectSingleObject(CBrowserObject *obj);

  //---

  bool downloadFile(const CUrl &url, std::string &filename);

 private:
  void init();

  void reset();

  void setPixmapSize(int width, int height);

  void redrawArea();

  void gotoDocument(const std::string &filename);

 private:
  typedef std::list<CBrowserWindow *>             WindowList;
  typedef std::map<std::string, CBrowserObject *> IdObjects;
  typedef std::vector<CBrowserObject*>            ObjStack;
  typedef std::vector<CBrowserObject*>            Objects;
  typedef std::vector<std::string>                Scripts;
  typedef std::vector<std::string>                ScriptFiles;
  typedef std::map<CBrowserObject *, CJValueP>    ObjMap;

  static WindowList          window_list_;
  static std::string         window_target_;
  static CBrowserAnchorLink* mouse_link_;

  IFace                   iface_;
  std::string             name_;
  std::string             filename_;
  CBrowserDocument*       document_ { nullptr };
  CQJWindowP              window_;

  CBrowserScrolledWindow* swindow_ { nullptr };
  CBrowserWindowWidget*   w_ { nullptr };

  CIBBox2D                bbox_;
  int                     leftMargin_ { 0 };
  int                     topMargin_ { 0 };

  CBrowserLayout*         layout_ { nullptr };

  CBrowserLinkMgr*        linkMgr_ { nullptr };
  CBrowserFileMgr*        fileMgr_ { nullptr };

  CBrowserObject*         rootObject_ { nullptr };
  IdObjects               idObjects_;
  ObjStack                objStack_;
  Objects                 objects_;
  ObjMap                  objMap_;
  Scripts                 scripts_;
  ScriptFiles             scriptFiles_;

  CBrowserOutput          output_;

  CSSList                 cssList_;

  CFontPtr                font_;
  int                     baseFontSize_ { 0 };

  CBrowserHistory*        history_ { nullptr };
};

#endif
