#ifndef CBrowserData_H
#define CBrowserData_H

#include <CBrowserClear.h>
#include <CRGBA.h>
#include <map>

struct CBrowserBaseData {
  typedef std::map<std::string,std::string> NameValues;

  std::string id;
  std::string name;
  std::string cclass;
  NameValues  nameValues;
};

//---

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

struct CBrowserBodyData : public CBrowserBaseData {
  std::string text;
  std::string alink { "red" };
  bool        fixed { false };
  std::string link { "blue" };
  std::string vlink { "purple" };
};

//---

struct CBrowserBreakData {
  CBrowserClear::Type clear { CBrowserClear::Type::NONE };
};

//---

struct CBrowserDivData : public CBrowserBaseData {
  std::string style;
};

//---

struct CBrowserCanvasData : public CBrowserBaseData {
  std::string style;
};

//---

struct CBrowserFontData {
  std::string color;
  std::string face;
  int         size  { -1 };
  int         delta { 0 };
  bool        sizeSet { false };
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

struct CBrowserFormData : public CBrowserBaseData {
  CBrowserFormMethodType method { CBrowserFormMethodType::GET };
  std::string            action { "" };
};

struct CBrowserFormInputData : public CBrowserBaseData {
  std::string align;
  bool        checked { false };
  std::string max;
  int         maxlength { -1 };
  std::string min;
  std::string onblur;
  std::string onclick;
  std::string onchange;
  std::string onfocus;
  std::string placeholder;
  std::string required;
  int         size { -1 };
  std::string src;
  std::string step;
  std::string type;
  std::string value;
};

struct CBrowserFormOptionData : public CBrowserBaseData {
  std::string value;
  bool        selected { false };
};

struct CBrowserFormButtonData : public CBrowserFormInputData {
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

struct CBrowserLinkData : public CBrowserBaseData {
  std::string download;
  std::string href;
  std::string methods;
  std::string rel;
  std::string rev;
  std::string target;
  std::string type;
  std::string title;
  std::string url;
};

//---

struct CBrowserOutputListData : public CBrowserBaseData {
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
};

//---

struct CBrowserPreData {
};

//---

struct CBrowserRuleData {
  bool shade = true;
  int  size  = -1;
};

//---

struct CBrowserScriptData {
  std::string language { "" };
  std::string src      { "" };
  std::string type     { "" };
  std::string text     { "" };
};

#endif
