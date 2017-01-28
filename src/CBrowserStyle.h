#ifndef CBrowserStyle_H
#define CBrowserStyle_H

#include <CBrowserObject.h>
#include <CBrowserData.h>

class CBrowserBStyle : public CBrowserObject {
 public:
  explicit CBrowserBStyle(CBrowserWindow *window);
 ~CBrowserBStyle();
};

//---

class CBrowserBigStyle : public CBrowserObject {
 public:
  explicit CBrowserBigStyle(CBrowserWindow *window);
 ~CBrowserBigStyle();
};

//---

class CBrowserBlinkStyle : public CBrowserObject {
 public:
  explicit CBrowserBlinkStyle(CBrowserWindow *window);
 ~CBrowserBlinkStyle();
};

//---

class CBrowserDfnStyle : public CBrowserObject {
 public:
  explicit CBrowserDfnStyle(CBrowserWindow *window);
 ~CBrowserDfnStyle();
};

//---

class CBrowserCiteStyle : public CBrowserObject {
 public:
  explicit CBrowserCiteStyle(CBrowserWindow *window);
 ~CBrowserCiteStyle();
};

//---

class CBrowserEmStyle : public CBrowserObject {
 public:
  explicit CBrowserEmStyle(CBrowserWindow *window);
 ~CBrowserEmStyle();
};

//---

class CBrowserFontStyleObj : public CBrowserObject {
 public:
  explicit CBrowserFontStyleObj(CBrowserWindow *window);
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
  explicit CBrowserIStyle(CBrowserWindow *window);
 ~CBrowserIStyle();
};

//---

class CBrowserSmallStyle : public CBrowserObject {
 public:
  explicit CBrowserSmallStyle(CBrowserWindow *window);
 ~CBrowserSmallStyle();
};

//---

class CBrowserStrikeStyle : public CBrowserObject {
 public:
  explicit CBrowserStrikeStyle(CBrowserWindow *window);
 ~CBrowserStrikeStyle();
};

//---

class CBrowserStrongStyle : public CBrowserObject {
 public:
  explicit CBrowserStrongStyle(CBrowserWindow *window);
 ~CBrowserStrongStyle();
};

//---

class CBrowserSubStyle : public CBrowserObject {
 public:
  explicit CBrowserSubStyle(CBrowserWindow *window);
 ~CBrowserSubStyle();
};

//---

class CBrowserSupStyle : public CBrowserObject {
 public:
  explicit CBrowserSupStyle(CBrowserWindow *window);
 ~CBrowserSupStyle();
};

//---

class CBrowserUStyle : public CBrowserObject {
 public:
  explicit CBrowserUStyle(CBrowserWindow *window);
 ~CBrowserUStyle();
};

//---

class CBrowserVarStyle : public CBrowserObject {
 public:
  explicit CBrowserVarStyle(CBrowserWindow *window);
 ~CBrowserVarStyle();
};

#endif
