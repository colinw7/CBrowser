#ifndef CBrowserStyle_H
#define CBrowserStyle_H

#include <CBrowserObject.h>
#include <CBrowserData.h>
#include <CHtmlLayout.h>

class CBrowserBStyle : public CBrowserObject {
 public:
  CBrowserBStyle(CBrowserWindow *window);
 ~CBrowserBStyle();

  void initProcess() override;
  void termProcess() override;
};

//---

class CBrowserBigStyle : public CBrowserObject {
 public:
  CBrowserBigStyle(CBrowserWindow *window);
 ~CBrowserBigStyle();

  void initProcess() override;
  void termProcess() override;
};

//---

class CBrowserBlinkStyle : public CBrowserObject {
 public:
  CBrowserBlinkStyle(CBrowserWindow *window);
 ~CBrowserBlinkStyle();

  void initProcess() override;
  void termProcess() override;
};

//---

class CBrowserDfnStyle : public CBrowserObject {
 public:
  CBrowserDfnStyle(CBrowserWindow *window);
 ~CBrowserDfnStyle();

  void initProcess() override;
  void termProcess() override;
};

//---

class CBrowserCiteStyle : public CBrowserObject {
 public:
  CBrowserCiteStyle(CBrowserWindow *window);
 ~CBrowserCiteStyle();

  void initProcess() override;
  void termProcess() override;
};

//---

class CBrowserEmStyle : public CBrowserObject {
 public:
  CBrowserEmStyle(CBrowserWindow *window);
 ~CBrowserEmStyle();

  void initProcess() override;
  void termProcess() override;
};

//---

class CBrowserFontStyle : public CBrowserObject {
 public:
  CBrowserFontStyle(CBrowserWindow *window, const CBrowserFontData &data);
 ~CBrowserFontStyle();

  void initProcess() override;
  void termProcess() override;

 private:
  CBrowserFontData     data_;
  CBrowserSaveFontData saveData_;
};

//---

class CBrowserIStyle : public CBrowserObject {
 public:
  CBrowserIStyle(CBrowserWindow *window);
 ~CBrowserIStyle();

  void initProcess() override;
  void termProcess() override;
};

//---

class CBrowserSmallStyle : public CBrowserObject {
 public:
  CBrowserSmallStyle(CBrowserWindow *window);
 ~CBrowserSmallStyle();

  void initProcess() override;
  void termProcess() override;
};

//---

class CBrowserStrikeStyle : public CBrowserObject {
 public:
  CBrowserStrikeStyle(CBrowserWindow *window);
 ~CBrowserStrikeStyle();

  void initProcess() override;
  void termProcess() override;
};

//---

class CBrowserStrongStyle : public CBrowserObject {
 public:
  CBrowserStrongStyle(CBrowserWindow *window);
 ~CBrowserStrongStyle();

  void initProcess() override;
  void termProcess() override;
};

//---

class CBrowserSubStyle : public CBrowserObject {
 public:
  CBrowserSubStyle(CBrowserWindow *window);
 ~CBrowserSubStyle();

  void initProcess() override;
  void termProcess() override;
};

//---

class CBrowserSupStyle : public CBrowserObject {
 public:
  CBrowserSupStyle(CBrowserWindow *window);
 ~CBrowserSupStyle();

  void initProcess() override;
  void termProcess() override;
};

//---

class CBrowserUStyle : public CBrowserObject {
 public:
  CBrowserUStyle(CBrowserWindow *window);
 ~CBrowserUStyle();

  void initProcess() override;
  void termProcess() override;
};

//---

class CBrowserVarStyle : public CBrowserObject {
 public:
  CBrowserVarStyle(CBrowserWindow *window);
 ~CBrowserVarStyle();

  void initProcess() override;
  void termProcess() override;
};

#endif
