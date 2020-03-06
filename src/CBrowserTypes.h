#ifndef CBROWSER_TYPES_H
#define CBROWSER_TYPES_H

#include <CBrush.h>
#include <CPen.h>
#include <string>
#include <list>

enum class CBrowserBorderType {
  NONE,
  IN,
  OUT
};

enum class CBrowserBorderSide {
  NONE,
  LEFT,
  RIGHT,
  TOP,
  BOTTOM
};

enum class CBrowserImageAlign {
  NONE,
  TOP,
  MIDDLE,
  BOTTOM,
  LEFT,
  RIGHT,
  TEXTTOP,
  ABSMIDDLE,
  ABSBOTTOM
};

enum class CBrowserTextPlaceType {
  NORMAL,
  SUBSCRIPT,
  SUPERSCRIPT
};

enum class CBrowserDeviceType {
  NONE,
  X,
  PS
};

enum class CBrowserFormMethodType {
  NONE,
  GET,
  POST
};

enum class CBrowserFormInputType {
  NONE,
  BUTTON,
  CHECKBOX,
  EMAIL,
  FILE,
  HIDDEN,
  IMAGE,
  MONTH,
  NUMBER,
  PASSWORD_TEXT,
  RADIO_BUTTON,
  RANGE,
  RESET_BUTTON,
  SELECT,
  SUBMIT_BUTTON,
  TEL,
  TEXT,
  TEXTAREA
};

enum class CBrowserFormTextAreaWrapType {
  OFF,
  VIRTUAL,
  PHYSICAL
};

enum class CBrowserCSSType {
  NONE,
  GLOBAL,
  CLASS,
  TYPE,
  TYPE_CLASS,
  NAME
};

//------

class CBrowserCanvas;
class CQJCanvasWidget;

class  CBrowserAnchor;
class  CBrowserAnchorLink;
class  CBrowserDocument;
class  CBrowserFileMgr;
class  CBrowserForm;
class  CBrowserFormInput;
class  CBrowserFormSelect;
class  CBrowserFormOption;
class  CBrowserFormTextarea;
class  CBrowserFormButton;
class  CBrowserFormCheckBox;
class  CBrowserFormFileUpload;
class  CBrowserFormHidden;
class  CBrowserFormImage;
class  CBrowserFormInput;
class  CBrowserFormPassword;
class  CBrowserFormRadio;
class  CBrowserFormRange;
class  CBrowserFormReset;
class  CBrowserFormSubmit;
class  CBrowserFormText;
class  CBrowserGraphics;
class  CBrowserHistory;
class  CBrowserImage;
class  CBrowserLayout;
class  CBrowserLinkMgr;
struct CBrowserLinkRect;
class  CBrowserObject;
struct CBrowserRuleData;
class  CBrowserTable;
class  CBrowserTableRow;
class  CBrowserTableCell;
class  CBrowserTablePadCell;
class  CBrowserTableCaption;
class  CBrowserText;
class  CBrowserWindow;
class  CBrowserWindowWidget;

class CQJCanvas;
class CQJColorWidget;
class CQJDialog;
class CQJHtmlObj;
class CQJObject;

class CJavaScript;

//------

#include <CBrowserProperty.h>

template<>
class CBrowserPropertyTraits<CBrowserImageAlign> {
 public:
  CBrowserPropertyTraits() { }

  std::string toString(const CBrowserImageAlign &align) {
    switch (align) {
      case CBrowserImageAlign::TOP      : return "top";
      case CBrowserImageAlign::MIDDLE   : return "middle";
      case CBrowserImageAlign::BOTTOM   : return "bottom";
      case CBrowserImageAlign::LEFT     : return "left";
      case CBrowserImageAlign::RIGHT    : return "right";
      case CBrowserImageAlign::TEXTTOP  : return "texttop";
      case CBrowserImageAlign::ABSMIDDLE: return "absmiddle";
      case CBrowserImageAlign::ABSBOTTOM: return "absbottom";
      default                           : return "";
    }

    return "";
  }

  bool fromString(const std::string &str, CBrowserImageAlign &align) {
    if (str == "top"      ) { align = CBrowserImageAlign::TOP      ; return true; }
    if (str == "middle"   ) { align = CBrowserImageAlign::MIDDLE   ; return true; }
    if (str == "bottom"   ) { align = CBrowserImageAlign::BOTTOM   ; return true; }
    if (str == "left"     ) { align = CBrowserImageAlign::LEFT     ; return true; }
    if (str == "right"    ) { align = CBrowserImageAlign::RIGHT    ; return true; }
    if (str == "texttop"  ) { align = CBrowserImageAlign::TEXTTOP  ; return true; }
    if (str == "absmiddle") { align = CBrowserImageAlign::ABSMIDDLE; return true; }
    if (str == "absbottom") { align = CBrowserImageAlign::ABSBOTTOM; return true; }

    return false;
  }
};

#endif
