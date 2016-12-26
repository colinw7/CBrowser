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

  bool getUseAlt() const { return use_alt_; }
  void setUseAlt(bool use_alt);

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
  bool      use_alt_ { false };
};

#endif
