enum CBrowserDeviceType {
  CBROWSER_DEVICE_TYPE_NONE,
  CBROWSER_DEVICE_TYPE_X,
  CBROWSER_DEVICE_TYPE_PS
};

class CBrowserCanvas;
class QImage;
class QPixmap;
class QPainter;

class CBrowserRenderer {
 public:
  CBrowserRenderer(CBrowserCanvas *canvas);

  virtual ~CBrowserRenderer();

  virtual void startDoubleBuffer(int width, int height);
  virtual void endDoubleBuffer  ();

  virtual void clear(const CRGBA &bg);

  virtual void drawRectangle(const CIBBox2D &bbox);
  virtual void fillRectangle(const CIBBox2D &bbox);
  virtual void drawCircle(const CIPoint2D &c, int r);
  virtual void fillCircle(const CIPoint2D &c, int r);
  virtual void drawLine(const CIPoint2D &p1, const CIPoint2D &p2);
  virtual void drawString(const CIPoint2D &p, const std::string &str);
  virtual void drawImage(const CIPoint2D &p, const CImagePtr &image);

  virtual void setForeground(const CRGBA &fg);
  virtual void setFont(CFontPtr font);

 private:
  CBrowserCanvas* canvas_ { 0 };
  QPixmap*        pixmap_ { 0 };
  int             pixmap_width_ { 0 };
  int             pixmap_height_ { 0 };
  QPainter*       painter_ { 0 };
  CRGBA           fg_;
  CFontPtr        font_;
};

//------

class CBrowserGraphics {
 public:
  CBrowserGraphics(CBrowserCanvas *canvas);
 ~CBrowserGraphics();

  const CRGBA &getBg() const { return bg_; }
  const CRGBA &getFg() const { return fg_; }

  void startDoubleBuffer(int width, int height);
  void endDoubleBuffer();

  void setXDevice();
  void setPSDevice(double xmin, double ymin, double xmax, double ymax);

  void clear(const CRGBA &bg);
  void drawImage(int x, int y, const CImagePtr &image);
  void drawTiledImage(int x, int y, int width, int height, const CImagePtr &image);
  void drawRectangle(int x1, int y1, int x2, int y2);
  void fillRectangle(int x1, int y1, int x2, int y2);
  void drawCircle(int x, int y, int r);
  void fillCircle(int x, int y, int r);
  void drawLine(int x1, int y1, int x2, int y2);
  void drawString(int x, int y, const std::string &str);

  void drawOutline(int x, int y, int width, int height, const CRGBA &color);

  void drawBorder(int x, int y, int width, int height, const CRGBA &color,
                  CBrowserBorderType type);

  void drawHRule(int x1, int x2, int y, int height, const CRGBA &color);

  void setForeground(const CRGBA &fg);
  void setFont(CFontPtr font);

 private:
  bool readFontMetrics();

 private:
  CBrowserCanvas*    canvas_ { 0 };
  CBrowserRenderer*  renderer_ { 0 };
  CRGBA              bg_;
  CRGBA              fg_;
  CBrowserDeviceType current_device_;
  CPrint*            print_device_ { 0 };
};
