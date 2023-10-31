#ifndef CBrowserWindowWidgetIFace_H
#define CBrowserWindowWidgetIFace_H

#include <QString>

class CBrowser;
class CBrowserGraphics;

class QWidget;
class QCursor;
class QPixmap;

class CBrowserWindowWidgetIFace {
 public:
  CBrowserWindowWidgetIFace(CBrowser *browser);

  virtual ~CBrowserWindowWidgetIFace() = default;

  virtual void init();

  CBrowser *browser() { return browser_; }

  virtual CBrowserGraphics *graphics() const { return graphics_; }

  virtual int windowWidth () { return 100; }
  virtual int windowHeight() { return 100; }

  virtual void updateWidget(bool) { }

  virtual void setCursor(const QCursor &) { }

  virtual void saveImage(const QString &filename);

  virtual void drawPixmap(QPixmap *) { }

 protected:
  CBrowser*         browser_  { nullptr };
  CBrowserGraphics* graphics_ { nullptr };
};

#endif
