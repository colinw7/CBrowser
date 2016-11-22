#define UNIT_NONE    0
#define UNIT_PIXEL   1
#define UNIT_PERCENT 2

#define LEFT_MARGIN   8
#define RIGHT_MARGIN  8
#define BOTTOM_MARGIN 8
#define TOP_MARGIN    8

class CBrowserWindow {
 public:
  CBrowserWindow(const std::string &filename);
  CBrowserWindow();
 ~CBrowserWindow();

  void setIFace(CBrowserIFace *iface);

  CBrowserCanvas *getCanvas() const { return canvas_; }

  CBrowserDocument *getDocument() const { return document_; }

  int getX() const { return x_; }
  int getY() const { return y_; }

  int getWidth () const { return width_ ; }
  int getHeight() const { return height_; }

  int getLeftMargin() const { return left_margin_; }
  int getTopMargin () const { return top_margin_ ; }

  CHtmlLayoutMgr *getLayoutMgr() const { return layout_mgr_; }

  CHtmlLayoutArea *getAreaData() const { return area_data_; }

  void setHAlign(CHAlignType halign);
  void setVAlign(CVAlignType valign);
  void setAlign(CHAlignType halign, CVAlignType valign);

  void startArea(CHtmlLayoutArea *area_data);
  void endArea();

  void startFontFace(const std::string &face);
  void endFontFace();

  std::string getCurrentFontFace() const;

  void setCurrentFontFace(const std::string &face);

  CBrowserFontFace *lookupFontFace(const std::string &face);
  CBrowserFontFace *loadFontFace(const std::string &face);

  void        setBaseFontSize(int size);
  int         getBaseFontSize();
  void        resetBaseFontSize();
  void        increaseBaseFontSize();
  void        decreaseBaseFontSize();
  void        setFontSize(int size);
  int         getFontSize();
  void        resetFontSize();
  void        increaseFontSize();
  void        decreaseFontSize();
  void        setFontColor(const std::string &color);
  std::string getFontColor();
  void        startBold();
  void        endBold();
  void        startItalic();
  void        endItalic();
  void        setFontStyle();

  void freeFonts();

  CFontPtr getFont() const { return font_; }

  CHtmlLayoutArea    *getCurrentArea();
  CHtmlLayoutCell    *getCurrentCell();
  CHtmlLayoutSubCell *getCurrentSubCell();

  void updateSubCellWidth(int width);
  void updateSubCellHeight(int ascent, int descent);

  void addCellRedrawData(CHtmlLayoutCellFormatProc *format_proc,
                         CHtmlLayoutCellFreeProc *free_proc);

  void addSubCellRedrawData(CHtmlLayoutCellRedrawProc *redraw_proc);

  void addSubCellRedrawData(CHtmlLayoutCellRedrawProc *redraw_proc,
                            CHtmlLayoutCellFreeProc *free_proc);

  void close();

  void setName(const std::string &name);

  void setDocument(const std::string &filename);

  void outputDocument();

  void resize();
  void redraw();

  void drawDocument();

  void setTitle(const std::string &title);
  void setMargins(int, int);
  void setBackgroundImage(const std::string &name, bool fixed);
  void setTarget(const std::string &target);
  void indentLeft(int indent);
  void indentRight(int indent);
  void newLine();
  void skipLine();
  void setStatus(const std::string &status);

  void displayError(const char *format, ...);

  bool isVisible(int x1, int y1, int x2, int y2);

  void print();

  void goBack();
  void goForward();

  bool hoverLink(int x, int y, std::string &link_name);

  bool activateLink(int x, int y);

  void addHistoryItem(const std::string &item);

  CRGBA getBg();
  CRGBA getFg();

  void drawImage(int x, int y, const CImagePtr &image);
  void drawRectangle(int x1, int y1, int x2, int y2);
  void fillRectangle(int x1, int y1, int x2, int y2);
  void drawCircle(int x, int y, int r);
  void fillCircle(int x, int y, int r);
  void drawLine(int x1, int y1, int x2, int y2);
  void drawString(int x, int y, const std::string &str);
  void drawOutline(int x, int y, int width, int height, const std::string &color_name);
  void drawBorder(int x, int y, int width, int height, CBrowserBorderType type);
  void drawHRule(int x1, int x2, int y, int height);

  void setForeground(const CRGBA &fg);
  void setFont(CFontPtr font);

  void errorDialog(const std::string &msg);

 private:
  void init();

  void loadResources();

  void setPixmapSize(int width, int height);

  void recalc();

  void redrawArea();

  void gotoDocument(const std::string &filename);

 private:
  typedef std::list<CBrowserWindow *>     WindowList;
  typedef std::vector<CBrowserFontFace *> FontFaces;

  static WindowList    window_list_;
  static std::string   default_font_face_;
  static bool          ignore_redraw_;
  static std::string   window_target_;
  static CBrowserLink *mouse_link_;

  std::string          name_;
  CBrowserDocument    *document_ { nullptr };

  CBrowserIFace       *iface_ { nullptr };
  CBrowserCanvas      *canvas_ { nullptr };

  int                  x_ { 0 };
  int                  y_ { 0 };
  int                  width_ { 0 };
  int                  height_ { 0 };
  int                  left_margin_ { 0 };
  int                  top_margin_ { 0 };

  CImagePtr            bg_image_;
  bool                 bg_fixed_ { false };

  CHtmlLayoutMgr      *layout_mgr_ { nullptr };
  CHtmlLayoutArea     *area_data_ { nullptr };

  CFontPtr             font_;
  FontFaces            font_face_stack_;
  FontFaces            font_face_list_;
  CBrowserFontFace    *current_font_face_ { nullptr };
  int                  base_font_size_ { 0 };
  CRGBA                current_font_color_;
  int                  current_font_style_ { 0 };
  int                  current_font_size_ { 0 };

  CBrowserHistory     *history_ { nullptr };
};
