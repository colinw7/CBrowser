#ifndef CBrowserWindowIFace_H
#define CBrowserWindowIFace_H

#include <CBrowserFont.h>
#include <CBrowserImage.h>
#include <CBrowserObjectCSSTagData.h>
#include <CBrowserTypes.h>

#include <CQJWindowIFace.h>

#include <CCSS.h>
#include <CUrl.h>
#include <CFont.h>

#include <QString>

class CHtmlParserTokens;

class CBrowserWindowIFace {
 public:
  using NameValues = std::map<std::string, std::string>;

  struct CSSData {
    CCSS css;
    CUrl url;

    CSSData(const CCSS &css1, const CUrl &url1=CUrl()) :
     css(css1), url(url1) {
    }
  };

  using CSSList = std::vector<CSSData>;

  class JSIFace : public CQJWindowIFace {
   public:
    JSIFace(CBrowserWindowIFace *window) :
     window_(window) {
    }

    CBrowserWindowIFace *window() const { return window_; }

   private:
    CBrowserWindowIFace *window_ { nullptr };
  };

 public:
  CBrowserWindowIFace(CBrowser *browser);

  virtual ~CBrowserWindowIFace();

  //---

  virtual void init();

  virtual void reset();

  //---

  virtual bool isAutoExit() const { return false; }

  virtual CBrowserWindowWidgetIFace *makeWidget() { return nullptr; }

  //---

  virtual CBrowser *browser() const { return browser_; }

  virtual CBrowserDocument *getDocument() const;

  virtual CBrowserWindowWidgetIFace *widget() const;

  virtual CBrowserGraphics *graphics() const;

  virtual CBrowserLayout *getLayout() const;

  virtual CQJWindowIFace *jsIFace();

  //---

  const QString &name() const { return name_; }
  void setName(const QString &s) { name_ = s; }

  virtual QString filename() const { return filename_; }

  virtual const QString &title() const { return title_; }
  virtual void setTitle(const QString &title);

  //---

  const QString &getTarget() const { return windowTarget_; }
  void setTarget(const QString &target);

  //--

  virtual void getMargins(int &left, int &type);
  virtual void setMargins(int left, int top);

  //---

  int getX() const { return bbox_.getXMin(); }
  int getY() const { return bbox_.getYMin(); }

  int getWidth () const { return bbox_.getWidth (); }
  int getHeight() const { return bbox_.getHeight(); }

  //---

  virtual CRGBA getBgColor();
  virtual CRGBA getFgColor();

  virtual CImagePtr getBgImage();

  virtual void setBackgroundImage(const QString &name, bool fixed);

  //---

  virtual bool readFile(const std::string &filename, CHtmlParserTokens &tokens);

  virtual bool setShortcutIcon(const CUrl &url);

  virtual bool downloadFile(const CUrl &, std::string &);

  //---

  virtual CBrowserAnchorLink *getCurrentLink() const;

  virtual void startSourceLink(CBrowserAnchor *anchor);
  virtual void startDestLink  (CBrowserAnchor *anchor);
  virtual void endLink();

  virtual std::string expandDestLink(const std::string &href);

  virtual void clearLinks();

  virtual int getDestLinkPos(const QString &href, int *x, int *y);

  virtual CBrowserAnchorLink *getSourceLink(int x, int y);

  virtual bool hoverLink(int x, int y, std::string &link_name);

  virtual bool activateLink(int x, int y);

  //---

  virtual CImagePtr lookupImage(const CBrowserImageData &imageData, int iwidth, int iheight);

  //---

  CBrowserObject *rootObject() const { return rootObject_; }
  void setRootObject(CBrowserObject *p) { rootObject_ = p; }

  //---

  virtual CBrowserObject *createElement(const std::string &def);

  //---

  virtual void startObject(CBrowserObject *obj, bool add);
  virtual void endObject();

  virtual CBrowserObject *currentObj() const;

  virtual CBrowserObject *getObject(const std::string &id) const;

  virtual CBrowserObject *headObject() const;
  virtual CBrowserObject *bodyObject() const;

  //---

  virtual int getCanvasXOffset() const { return 0; }
  virtual int getCanvasYOffset() const { return 0; }

  virtual int getCanvasWidth() const;
  virtual int getCanvasHeight() const;

  //---

  virtual bool loadCSSFile(const CUrl &url);
  virtual bool loadCSSText(const std::string &filename);

  virtual void selectCSSPattern(const CCSS::StyleData &styleData);

  virtual bool applyStyle(CBrowserObject *obj);

  virtual bool visitStyleData(const CCSS &css, const CCSSTagDataP &tagData);

  virtual void getTagNameValues(CHtmlTag *tag, NameValues &nameValues);

   //---

  virtual void deselectAllObjects();

  virtual void selectSingleObject(CBrowserObject *obj);

  //---

  virtual CFontPtr getFont() const { return font_; }

  virtual void getTextSize(const std::string &text, int *width, int *ascent, int *descent) const;

  virtual void getTextWidth (CFontPtr font, const std::string &text, int *width) const;
  virtual void getTextHeight(CFontPtr font, int *ascent, int *descent) const;

  //---

  virtual void recalc();

  virtual void resize();

  virtual void redraw();

  virtual void propagateSize(int, int) { }

  //---

  virtual void processTokens(const CHtmlParserTokens &tokens);

  virtual void layoutObjects();

  //---

  virtual void parseHAlignOption(const std::string &name, CHAlignType &type);
  virtual void parseVAlignOption(const std::string &name, CVAlignType &type);

  virtual CBrowserFontSize sizeToFontSize(int size) const;

  //---

  virtual void setBaseFontSize(int size);
  virtual int  getBaseFontSize();
  virtual void resetBaseFontSize();
  virtual void increaseBaseFontSize(int d=1);
  virtual void decreaseBaseFontSize(int d=1);

  virtual void setBaseFontStyle();

  //---

  virtual void setFilename(const QString &filename);

  virtual void setDocument(const CUrl &url);

  virtual void outputDocument();

  virtual void drawDocument();

  virtual void gotoDocument(const std::string &text);

  virtual void close() { }

  //---

  virtual void addScript(const std::string &text);
  virtual void addScriptFile(const std::string &filename);

  virtual void runScripts();

  //---

  virtual void draw();

  virtual void scrollTo(int, int) { }

  virtual void drawSelected(int, int, int, int);

  //---

  virtual void mousePress  (int x, int y);
  virtual void mouseMotion (int x, int y);
  virtual void mouseRelease(int x, int y);

  virtual void mouseWheel(int d);

  //---

  virtual void setStatus(const QString &status);

  virtual void saveImage(const QString &filename);

  virtual void print();

  //---

  virtual void addHistory(const CUrl &url);

  virtual void goBack();
  virtual void goForward();

  virtual void addHistoryItem(const CUrl &url);

  //---

  virtual void addHtmlObject(CBrowserObject *obj);

  virtual CJValueP lookupHtmlObject(CBrowserObject *obj) const;

  //---

  const CSSList &cssList() const { return cssList_; }
  void setCssList(const CSSList &v) { cssList_ = v; }

  const CSSData &cssData(int i) {
    assert(i >= 0 && i < int(cssList_.size()));
    return cssList_[i];
  }

  //---

  bool isVisible(int x1, int y1, int x2, int y2);

  //---

  virtual void errorDialog(const QString &msg);

  virtual void displayError(const char *format, ...);
  virtual void displayError(const QString &str);

 protected:
  void createDocument(const CUrl &url);

 protected:
  using WindowList = std::list<CBrowserWindowIFace *>;

  using OutputP   = std::unique_ptr<CBrowserOutput>;
  using DocumentP = std::unique_ptr<CBrowserDocument>;
  using WidgetP   = std::unique_ptr<CBrowserWindowWidgetIFace>;
  using LayoutP   = std::unique_ptr<CBrowserLayout>;

  using LinkMgrP = std::unique_ptr<CBrowserLinkMgr>;
  using FileMgrP = std::unique_ptr<CBrowserFileMgr>;

  using IdObjects = std::map<std::string, CBrowserObject *>;
  using ObjStack  = std::vector<CBrowserObject*>;
  using Objects   = std::vector<CBrowserObject*>;

  using Scripts     = std::vector<std::string>;
  using ScriptFiles = std::vector<std::string>;

  using HtmlObjMap = std::map<CBrowserObject *, CJValueP>;

  using JSIFaceP = std::unique_ptr<JSIFace>;

  using HistoryP = std::unique_ptr<CBrowserHistory>;

 protected:
  static QString    windowTarget_;
  static WindowList windowList_;

  static CBrowserAnchorLink* mouseLink_;

  //---

  CBrowser* browser_ { nullptr };

  QString name_;
  QString filename_;
  QString title_;

  OutputP   output_;
  DocumentP document_;
  WidgetP   widget_;

  CBrowserObject* rootObject_ { nullptr };

  int leftMargin_ { 0 };
  int topMargin_  { 0 };

  CIBBox2D bbox_;

  CFontPtr font_;
  int      baseFontSize_ { 0 };

  LayoutP layout_;

  CSSList cssList_;

  LinkMgrP linkMgr_;
  FileMgrP fileMgr_;

  IdObjects idObjects_;
  ObjStack  objStack_;
  Objects   objects_;

  Scripts     scripts_;
  ScriptFiles scriptFiles_;

  HtmlObjMap htmlObjMap_;

  JSIFaceP jsIFace_;

  HistoryP history_;
};

#endif
