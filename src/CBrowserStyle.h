#ifndef CBrowserStyle_H
#define CBrowserStyle_H

#include <CBrowserObject.h>
#include <CBrowserData.h>

class CBrowserBStyle : public CBrowserObject {
 public:
  CBrowserBStyle(CBrowserWindow *window);
 ~CBrowserBStyle();
};

//---

class CBrowserBigStyle : public CBrowserObject {
 public:
  CBrowserBigStyle(CBrowserWindow *window);
 ~CBrowserBigStyle();
};

//---

class CBrowserBlinkStyle : public CBrowserObject {
 public:
  CBrowserBlinkStyle(CBrowserWindow *window);
 ~CBrowserBlinkStyle();
};

//---

class CBrowserDfnStyle : public CBrowserObject {
 public:
  CBrowserDfnStyle(CBrowserWindow *window);
 ~CBrowserDfnStyle();
};

//---

class CBrowserCiteStyle : public CBrowserObject {
 public:
  CBrowserCiteStyle(CBrowserWindow *window);
 ~CBrowserCiteStyle();
};

//---

class CBrowserEmStyle : public CBrowserObject {
 public:
  CBrowserEmStyle(CBrowserWindow *window);
 ~CBrowserEmStyle();
};

//---

class CBrowserFontStyleObj : public CBrowserObject {
 public:
  CBrowserFontStyleObj(CBrowserWindow *window, const CBrowserFontData &data);
 ~CBrowserFontStyleObj();

  void init();

  void setNameValue(const std::string &name, const std::string &value) override;

 private:
  CBrowserFontData     data_;
  CBrowserSaveFontData saveData_;
};

//---

class CBrowserIStyle : public CBrowserObject {
 public:
  CBrowserIStyle(CBrowserWindow *window);
 ~CBrowserIStyle();
};

//---

class CBrowserSmallStyle : public CBrowserObject {
 public:
  CBrowserSmallStyle(CBrowserWindow *window);
 ~CBrowserSmallStyle();
};

//---

class CBrowserStrikeStyle : public CBrowserObject {
 public:
  CBrowserStrikeStyle(CBrowserWindow *window);
 ~CBrowserStrikeStyle();
};

//---

class CBrowserStrongStyle : public CBrowserObject {
 public:
  CBrowserStrongStyle(CBrowserWindow *window);
 ~CBrowserStrongStyle();
};

//---

class CBrowserSubStyle : public CBrowserObject {
 public:
  CBrowserSubStyle(CBrowserWindow *window);
 ~CBrowserSubStyle();
};

//---

class CBrowserSupStyle : public CBrowserObject {
 public:
  CBrowserSupStyle(CBrowserWindow *window);
 ~CBrowserSupStyle();
};

//---

class CBrowserUStyle : public CBrowserObject {
 public:
  CBrowserUStyle(CBrowserWindow *window);
 ~CBrowserUStyle();
};

//---

class CBrowserVarStyle : public CBrowserObject {
 public:
  CBrowserVarStyle(CBrowserWindow *window);
 ~CBrowserVarStyle();
};

#endif
