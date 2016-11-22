#ifndef CBROWSER_TYPES_H
#define CBROWSER_TYPES_H

#include <CHtmlLayout.h>
#include <string>
#include <list>

#define UNIT_NONE    0
#define UNIT_PIXEL   1
#define UNIT_PERCENT 2

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

enum class CBrowserSymbolType {
  NONE,
  DISC,
  CIRCLE,
  BLOCK,
  SQUARE
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

//------

class CBrowserCanvas;
class CBrowserCanvasWidget;
class CBrowserDocument;
class CBrowserFontFace;
class CBrowserFormMgr;
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
class CBrowserLink;
class CBrowserLinkMgr;
class CBrowserLinkRect;
class CBrowserObject;
class CBrowserRuleData;
class CBrowserTableMgr;
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
