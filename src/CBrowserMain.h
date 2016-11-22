#ifndef CBROWSER_MAIN_H
#define CBROWSER_MAIN_H

#include <CBrowserTypes.h>
#include <string>
#include <list>

#define CBrowserMainInst CBrowserMain::getInstance()

class CBrowserMain {
 public:
  typedef std::list<CBrowserIFace *> IFaceList;

 public:
  static CBrowserMain *getInstance();

  void init(int argc, char **argv);

  bool getDebug () const { return debug_  ; }
  bool getUseAlt() const { return use_alt_; }

  void setDebug (bool debug  );
  void setUseAlt(bool use_alt);

  void openDocument(const std::string &fileName);

  IFaceList ifaceList() const { return ifaces_; }

 private:
  CBrowserMain();

 private:
  IFaceList ifaces_;
  bool      debug_ { false };
  bool      use_alt_ { false };
};

#endif
