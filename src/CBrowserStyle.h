#ifndef CBrowserStyle_H
#define CBrowserStyle_H

#include <CBrowserObject.h>
#include <CHtmlLayout.h>

class CBrowserBStyle : public CBrowserObject {
 public:
  CBrowserBStyle(CBrowserWindow *window);
 ~CBrowserBStyle();

  CBrowserWindow *getWindow() const { return window_; }

  void draw(CHtmlLayoutMgr *mgr, const CHtmlLayoutRegion &region);

 private:
  CBrowserWindow* window_ { nullptr };
};

//---

class CBrowserBigStyle : public CBrowserObject {
 public:
  CBrowserBigStyle(CBrowserWindow *window);
 ~CBrowserBigStyle();

  CBrowserWindow *getWindow() const { return window_; }

 private:
  CBrowserWindow* window_ { nullptr };
};

//---

class CBrowserBlinkStyle : public CBrowserObject {
 public:
  CBrowserBlinkStyle(CBrowserWindow *window);
 ~CBrowserBlinkStyle();

  CBrowserWindow *getWindow() const { return window_; }

 private:
  CBrowserWindow* window_ { nullptr };
};

//---

class CBrowserCiteStyle : public CBrowserObject {
 public:
  CBrowserCiteStyle(CBrowserWindow *window);
 ~CBrowserCiteStyle();

  CBrowserWindow *getWindow() const { return window_; }

 private:
  CBrowserWindow* window_ { nullptr };
};

//---

class CBrowserEmStyle : public CBrowserObject {
 public:
  CBrowserEmStyle(CBrowserWindow *window);
 ~CBrowserEmStyle();

  CBrowserWindow *getWindow() const { return window_; }

 private:
  CBrowserWindow* window_ { nullptr };
};

//---

class CBrowserIStyle : public CBrowserObject {
 public:
  CBrowserIStyle(CBrowserWindow *window);
 ~CBrowserIStyle();

  CBrowserWindow *getWindow() const { return window_; }

 private:
  CBrowserWindow* window_ { nullptr };
};

//---

class CBrowserSmallStyle : public CBrowserObject {
 public:
  CBrowserSmallStyle(CBrowserWindow *window);
 ~CBrowserSmallStyle();

  CBrowserWindow *getWindow() const { return window_; }

 private:
  CBrowserWindow* window_ { nullptr };
};

//---

class CBrowserStrikeStyle : public CBrowserObject {
 public:
  CBrowserStrikeStyle(CBrowserWindow *window);
 ~CBrowserStrikeStyle();

  CBrowserWindow *getWindow() const { return window_; }

 private:
  CBrowserWindow* window_ { nullptr };
};

//---

class CBrowserStrongStyle : public CBrowserObject {
 public:
  CBrowserStrongStyle(CBrowserWindow *window);
 ~CBrowserStrongStyle();

  CBrowserWindow *getWindow() const { return window_; }

 private:
  CBrowserWindow* window_ { nullptr };
};

//---

class CBrowserSubStyle : public CBrowserObject {
 public:
  CBrowserSubStyle(CBrowserWindow *window);
 ~CBrowserSubStyle();

  CBrowserWindow *getWindow() const { return window_; }

 private:
  CBrowserWindow* window_ { nullptr };
};

//---

class CBrowserSupStyle : public CBrowserObject {
 public:
  CBrowserSupStyle(CBrowserWindow *window);
 ~CBrowserSupStyle();

  CBrowserWindow *getWindow() const { return window_; }

 private:
  CBrowserWindow* window_ { nullptr };
};

//---

class CBrowserUStyle : public CBrowserObject {
 public:
  CBrowserUStyle(CBrowserWindow *window);
 ~CBrowserUStyle();

  CBrowserWindow *getWindow() const { return window_; }

 private:
  CBrowserWindow* window_ { nullptr };
};

//---

class CBrowserVarStyle : public CBrowserObject {
 public:
  CBrowserVarStyle(CBrowserWindow *window);
 ~CBrowserVarStyle();

  CBrowserWindow *getWindow() const { return window_; }

 private:
  CBrowserWindow* window_ { nullptr };
};

#endif
