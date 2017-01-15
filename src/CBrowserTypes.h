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
class CBrowserAnchorLink;
class CBrowserDocument;
class CBrowserFileMgr;
class CBrowserFontFace;
class CBrowserForm;
class CBrowserFormInput;
class CBrowserFormSelect;
class CBrowserFormOption;
class CBrowserFormTextarea;
class CBrowserFormButton;
class CBrowserFormCheckBox;
class CBrowserFormFileUpload;
class CBrowserFormHidden;
class CBrowserFormImage;
class CBrowserFormInput;
class CBrowserFormPassword;
class CBrowserFormRadio;
class CBrowserFormRange;
class CBrowserFormReset;
class CBrowserFormSubmit;
class CBrowserFormText;
class CBrowserGraphics;
class CBrowserHistory;
class CBrowserIFace;
class CBrowserImage;
class CBrowserLayout;
class CBrowserLinkMgr;
class CBrowserLinkRect;
class CBrowserObject;
class CBrowserRuleData;
class CBrowserTable;
class CBrowserTableRow;
class CBrowserTableCell;
class CBrowserTablePadCell;
class CBrowserTableCaption;
class CBrowserText;
class CBrowserWindow;
class CBrowserWindowWidget;

class CQJCanvas;
class CQJColorWidget;
class CQJDialog;
class CQJHtmlObj;
class CQJObject;

class CJavaScript;

#endif
