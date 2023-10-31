#ifndef CBROWSER_MAIN_H
#define CBROWSER_MAIN_H

#include <CBrowserTypes.h>
#include <CUrl.h>
#include <QObject>
#include <string>
#include <list>

class CBrowserIFace;

// main class for browser
class CBrowser : public QObject {
  Q_OBJECT

 public:
  using IFaceList = std::list<CBrowserIFace *>;

 public:
  CBrowser();
 ~CBrowser();

  void init(int argc, char **argv);

  CBrowserIFace *iface() const;
  void setIFace(CBrowserIFace *iface);

  bool getDebug() const { return debug_; }
  void setDebug(bool b);

  bool getQuiet() const { return quiet_; }
  void setQuiet(bool b) { quiet_ = b; }

  bool getUseAlt() const { return useAlt_; }
  void setUseAlt(bool useAlt);

  bool getOldLayout() const { return oldLayout_; }
  void setOldLayout(bool oldLayout_);

  bool getShowBoxes() const { return showBoxes_; }
  void setShowBoxes(bool showBoxes_);

  bool getMouseOver() const { return mouseOver_; }
  void setMouseOver(bool b) { mouseOver_ = b; }

  void setDocument(const CUrl &url);
  void addDocument(const CUrl &url);

  void displayError(const char *format, ...);
  void displayError(const char *format, va_list *args);
  void displayError(const QString &str);

 private slots:
  void exitSlot();

 private:
  CBrowserIFace* iface_ { nullptr };

  bool debug_     { false };
  bool quiet_     { false };
  bool useAlt_    { false };
  bool oldLayout_ { false };
  bool showBoxes_ { false };
  bool mouseOver_ { false };
};

#endif
