#ifndef CBrowserData_H
#define CBrowserData_H

#include <CRGBA.h>

namespace CBrowserDataConstants {
  const int LEFT_MARGIN   = 8;
  const int RIGHT_MARGIN  = 8;
  const int BOTTOM_MARGIN = 8;
  const int TOP_MARGIN    = 8;
};

//---

struct CBrowserBaseFontData {
  int  size     { 0 };
  int  sizeSign { 0 };
  bool sizeSet  { false };
};

//---

struct CBrowserBodyData {
  std::string text;
  std::string bgcolor;
  std::string background;
  std::string alink { "red" };
  bool        fixed { false };
  int         leftmargin { CBrowserDataConstants::LEFT_MARGIN };
  std::string link { "blue" };
  int         topmargin { CBrowserDataConstants::TOP_MARGIN };
  std::string vlink { "purple" };
};

//---

struct CBrowserBreakData {
  CHtmlLayoutClearType clear { CHtmlLayoutClearType::NONE };
};

//---

struct CBrowserDivData {
  std::string id;
  std::string style;
};

//---

struct CBrowserCanvasData {
  std::string id;
  int         width  { -1 };
  int         height { -1 };
  std::string style;
};

//---

struct CBrowserFontData {
  std::string color { "black" };
  std::string face  { "normal" };
  int         size  { -1 };
  int         delta { 1 };
};

class CBrowserSaveFontData {
 public:
  enum class Flag {
    NONE  = 0,
    FACE  = (1<<0),
    COLOR = (1<<1),
    SIZE  = (1<<2)
  };

 public:
  CBrowserSaveFontData(const std::string &face="", const std::string &colorName="",
                       const CRGBA &color=CRGBA(), int size=0) :
   flags_(0), face_(face), colorName_(colorName), color_(color), size_(size) {
  }

  void setFlag(Flag flag) { flags_ = flags_ | uint(flag); }
  bool getFlag(Flag flag) { return (flags_ & uint(flag)); }

  const std::string &getFace     () const { return face_ ; }
  const std::string &getColorName() const { return colorName_; }
  const CRGBA       &getColor    () const { return color_; }
  int                getSize     () const { return size_ ; }

 private:
  uint        flags_ { 0 };
  std::string face_;
  std::string colorName_;
  CRGBA       color_;
  int         size_ { 0 };
};

//---

struct CBrowserFormData {
  std::string            name   { "" };
  CBrowserFormMethodType method { CBrowserFormMethodType::GET };
  std::string            action { "" };
};

struct CBrowserFormInputData {
  std::string align;
  bool        checked { false };
  int         maxlength { -1 };
  std::string id;
  std::string classStr;
  std::string name;
  std::string placeholder;
  std::string onblur;
  std::string onclick;
  std::string onchange;
  std::string onfocus;
  int         size { -1 };
  std::string src;
  std::string type;
  std::string min;
  std::string max;
  std::string step;
  std::string value;
};

struct CBrowserFormOptionData {
  std::string value;
  bool        selected { false };
};

struct CBrowserFormSelectData : public CBrowserFormInputData {
  bool multiple { false };
};

struct CBrowserFormTextareaData : public CBrowserFormInputData {
  std::string wrap;
  int         cols { 40 };
  int         rows { 5 };
};

//---

struct CBrowserHeaderData {
  CHAlignType align { CHALIGN_TYPE_LEFT };
};

//---

struct CBrowserImageData {
  std::string        src;
  CBrowserImageAlign align  { CBrowserImageAlign::BOTTOM };
  int                border { 2 };
  int                width  { -1 };
  int                height { -1 };
  std::string        usemap;
  int                hspace { 2 };
  int                vspace { 2 };
  std::string        alt    { "" };
};

//---

struct CBrowserLinkData {
  std::string download;
  std::string href;
  std::string id;
  std::string methods;
  std::string rel;
  std::string rev;
  std::string target;
  std::string type;
  std::string title;
  std::string url;
};

//---

struct CBrowserOutputListData {
  std::string symbol;
  int         item_num { 1 };
  bool        compact { false };
};

//---

struct CBrowserOutputListItemData {
  std::string symbol;
  int         item_num { 0 };
};

//---

struct CBrowserParagraphData {
  CHAlignType align { CHALIGN_TYPE_LEFT };
};

//---

struct CBrowserPreData {
  int width { -1 };
};

//---

struct CBrowserScriptData {
  std::string language { "" };
  std::string src      { "" };
  std::string type     { "" };
  std::string text     { "" };
};

//---

struct CBrowserTextData {
  CRGBA                 color     { 0, 0, 0, 0 };
  bool                  underline { false };
  bool                  strike    { false };
  CBrowserTextPlaceType place     { CBrowserTextPlaceType::NORMAL };
  bool                  breakup   { true };
  bool                  format    { true };
};

#endif
