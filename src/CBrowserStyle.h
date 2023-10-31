#ifndef CBrowserStyle_H
#define CBrowserStyle_H

#include <CBrowserObject.h>
#include <CBrowserData.h>

class CBrowserBStyle : public CBrowserObject {
 public:
  explicit CBrowserBStyle(CBrowserWindowIFace *window);
 ~CBrowserBStyle();
};

//---

class CBrowserBigStyle : public CBrowserObject {
 public:
  explicit CBrowserBigStyle(CBrowserWindowIFace *window);
 ~CBrowserBigStyle();
};

//---

class CBrowserBlinkStyle : public CBrowserObject {
 public:
  explicit CBrowserBlinkStyle(CBrowserWindowIFace *window);
 ~CBrowserBlinkStyle();
};

//---

class CBrowserDfnStyle : public CBrowserObject {
 public:
  explicit CBrowserDfnStyle(CBrowserWindowIFace *window);
 ~CBrowserDfnStyle();
};

//---

class CBrowserCiteStyle : public CBrowserObject {
 public:
  explicit CBrowserCiteStyle(CBrowserWindowIFace *window);
 ~CBrowserCiteStyle();
};

//---

class CBrowserEmStyle : public CBrowserObject {
 public:
  explicit CBrowserEmStyle(CBrowserWindowIFace *window);
 ~CBrowserEmStyle();
};

//---

class CBrowserFontStyleObj : public CBrowserObject {
 public:
  explicit CBrowserFontStyleObj(CBrowserWindowIFace *window);
 ~CBrowserFontStyleObj();

  void init() override;

  void setNameValue(const std::string &name, const std::string &value) override;

 private:
  CBrowserFontData     data_;
  CBrowserSaveFontData saveData_;
};

//---

class CBrowserIStyle : public CBrowserObject {
 public:
  explicit CBrowserIStyle(CBrowserWindowIFace *window);
 ~CBrowserIStyle();
};

//---

class CBrowserSmallStyle : public CBrowserObject {
 public:
  explicit CBrowserSmallStyle(CBrowserWindowIFace *window);
 ~CBrowserSmallStyle();
};

//---

class CBrowserStrikeStyle : public CBrowserObject {
 public:
  explicit CBrowserStrikeStyle(CBrowserWindowIFace *window);
 ~CBrowserStrikeStyle();
};

//---

class CBrowserStrongStyle : public CBrowserObject {
 public:
  explicit CBrowserStrongStyle(CBrowserWindowIFace *window);
 ~CBrowserStrongStyle();
};

//---

class CBrowserSubStyle : public CBrowserObject {
 public:
  explicit CBrowserSubStyle(CBrowserWindowIFace *window);
 ~CBrowserSubStyle();
};

//---

class CBrowserSupStyle : public CBrowserObject {
 public:
  explicit CBrowserSupStyle(CBrowserWindowIFace *window);
 ~CBrowserSupStyle();
};

//---

class CBrowserUStyle : public CBrowserObject {
 public:
  explicit CBrowserUStyle(CBrowserWindowIFace *window);
 ~CBrowserUStyle();
};

//---

class CBrowserVarStyle : public CBrowserObject {
 public:
  explicit CBrowserVarStyle(CBrowserWindowIFace *window);
 ~CBrowserVarStyle();
};

#endif
