#ifndef CBROWSER_MAIN_H
#define CBROWSER_MAIN_H

class CBrowserIFace;

#define CBrowserMainInst CBrowserMain::getInstance()

class CBrowserMain {
 public:
  static CBrowserMain *getInstance();

  void init(int argc, char **argv);

  bool getDebug () const { return debug_  ; }
  bool getUseAlt() const { return use_alt_; }

  void setDebug (bool debug  );
  void setUseAlt(bool use_alt);

  void openDocument(const std::string &fileName);

 private:
  CBrowserMain();

 private:
  typedef std::list<CBrowserIFace *> IFaceList;

  IFaceList ifaces_;
  bool      debug_ { false };
  bool      use_alt_ { false };
};

#endif
