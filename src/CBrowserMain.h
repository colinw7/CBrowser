#ifndef CBROWSER_MAIN_H
#define CBROWSER_MAIN_H

#include <CBrowserTypes.h>
#include <QObject>
#include <string>
#include <list>

#define CBrowserMainInst CBrowserMain::getInstance()

class CBrowserMain : public QObject {
  Q_OBJECT

 public:
  typedef std::list<CBrowserIFace *> IFaceList;

 public:
  static CBrowserMain *getInstance();

 ~CBrowserMain();

  void init(int argc, char **argv);

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

  void openDocument(const std::string &fileName);

  IFaceList ifaceList() const { return ifaces_; }

 private:
  CBrowserMain();

 private slots:
  void exitSlot();

 private:
  IFaceList ifaces_;
  bool      debug_ { false };
  bool      quiet_ { false };
  bool      useAlt_ { false };
  bool      oldLayout_ { false };
  bool      showBoxes_ { false };
};

#endif
