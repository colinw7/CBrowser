#ifndef CBROWSER_MAIN_H
#define CBROWSER_MAIN_H

#include <CBrowserTypes.h>
#include <CUrl.h>
#include <QObject>
#include <string>
#include <list>

class CBrowserMainWindow;

#define CBrowserMainInst CBrowserMain::getInstance()

class CBrowserMain : public QObject {
  Q_OBJECT

 public:
  typedef std::list<CBrowserMainWindow *> IFaceList;

 public:
  static CBrowserMain *getInstance();

 ~CBrowserMain();

  void init(int argc, char **argv);

  CBrowserMainWindow *iface() const;

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

 private:
  CBrowserMain();

 private slots:
  void exitSlot();

 private:
  CBrowserMainWindow* iface_ { nullptr };
  bool                debug_ { false };
  bool                quiet_ { false };
  bool                useAlt_ { false };
  bool                oldLayout_ { false };
  bool                showBoxes_ { false };
  bool                mouseOver_ { false };
};

#endif
