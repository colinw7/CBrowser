#include <CBrowserOutput.h>
#include <CBrowserBreak.h>
#include <CBrowserWindow.h>
#include <CBrowserText.h>
#include <CBrowserLabel.h>
#include <CBrowserDocument.h>
#include <CBrowserLink.h>
#include <CBrowserTable.h>
#include <CBrowserForm.h>
#include <CBrowserRule.h>
#include <CBrowserImage.h>
#include <CBrowserCanvas.h>
#include <CBrowserMisc.h>
#include <CBrowserSymbol.h>
#include <CBrowserMain.h>
#include <CBrowserCeil.h>
#include <CBrowserJS.h>
#include <CHtmlLib.h>
#include <CRomanNumber.h>
#include <CRGBName.h>

enum class CBrowserOutputListType {
  NONE,
  UL,
  OL,
  DIR,
  MENU,
  DL
};

enum class CBrowserOutputFontFlag {
  NONE  = 0,
  FACE  = (1<<0),
  COLOR = (1<<1),
  SIZE  = (1<<2)
};

typedef void (*CBrowserOutputProc)
                (CBrowserWindow *window, CHtmlTag *tag);

//------

class CBrowserOutputData {
 public:
  CHtmlTagId         id;
  CBrowserOutputProc start_proc;
  CBrowserOutputProc end_proc;
};

//------

class CBrowserOutputList {
 public:
  CBrowserOutputList(CBrowserOutputListType type) :
   type_(type), symbol_(""), item_num_(1), compact_(false) {
  }

  CBrowserOutputListType getType() const { return type_; }

  const std::string &getSymbol() const { return symbol_; }

  void setSymbol(const std::string &symbol) { symbol_ = symbol; }

  int  getItemNum() const { return item_num_; }
  void setItemNum(int item_num) { item_num_ = item_num; }

  bool getCompact() const { return compact_; }
  void setCompact(bool compact) { compact_ = compact; }

 private:
  CBrowserOutputListType type_ { CBrowserOutputListType::NONE };
  std::string            symbol_;
  int                    item_num_ { 0 };
  bool                   compact_ { false };
};

//------

class CBrowserOutputFont {
  static std::vector<CBrowserOutputFont *> font_stack_;

 public:
  CBrowserOutputFont(const std::string &face, const std::string &color, int size) :
   flags_(0), face_(face), color_(color), size_(size) {
    font_stack_.push_back(this);
  }

  void setFlag(CBrowserOutputFontFlag flag) { flags_ = flags_ | uint(flag); }
  bool getFlag(CBrowserOutputFontFlag flag) { return (flags_ & uint(flag)); }

  const std::string &getFace () const { return face_ ; }
  const std::string &getColor() const { return color_; }
  int                getSize () const { return size_ ; }

  static void startFont(CBrowserOutputFont *font) {
    font_stack_.push_back(font);
  }

  static CBrowserOutputFont *endFont() {
    if (font_stack_.empty())
      return nullptr;

    CBrowserOutputFont *font = font_stack_[font_stack_.size() - 1];

    font_stack_.pop_back();

    return font;
  }

 private:
  uint        flags_ { 0 };
  std::string face_;
  std::string color_;
  int         size_ { 0 };
};

//------

struct CBrowserScriptData {
  bool        active   = false;
  std::string language = "";
  std::string type     = "";
  std::string text     = "";

  void reset() {
    active   = false;
    language = "";
    type     = "";
    text     = "";
  }
};

struct CBrowserTextData {
  CRGBA                 color;
  bool                  underline = false;
  bool                  strike    = false;
  CBrowserTextPlaceType place     = CBrowserTextPlaceType::NORMAL;
  bool                  breakup   = true;
  bool                  format    = true;
};

static CBrowserOutputList                *current_list    = nullptr;
static std::vector<CBrowserOutputList *>  list_stack;
static CBrowserScriptData                 scriptData;
static CBrowserTextData                   textData;
static bool                               in_title        = false;
static bool                               in_option       = false;
static std::string                        option_text     = "";
static bool                               in_textarea     = false;
static std::string                        textarea_text   = "";
static bool                               in_dd           = false;
static bool                               in_dt           = false;

std::vector<CBrowserOutputFont *> CBrowserOutputFont::font_stack_;

std::vector<CBrowserBreak *> breakers;

//------

static void CBrowserOutputAStartTag         (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputAEndTag           (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputAddressStartTag   (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputAddressEndTag     (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputBStartTag         (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputBEndTag           (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputBasefontStartTag  (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputBasefontEndTag    (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputBigStartTag       (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputBigEndTag         (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputBlinkStartTag     (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputBlinkEndTag       (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputBlockquoteStartTag(CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputBlockquoteEndTag  (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputBodyStartTag      (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputBodyEndTag        (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputBrTag             (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputCanvasStartTag    (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputCanvasEndTag      (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputCaptionStartTag   (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputCaptionEndTag     (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputCenterStartTag    (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputCenterEndTag      (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputCiteStartTag      (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputCiteEndTag        (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputCodeStartTag      (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputCodeEndTag        (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputDdStartTag        (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputDdEndTag          (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputDfnStartTag       (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputDfnEndTag         (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputDirStartTag       (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputDirEndTag         (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputDivStartTag       (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputDivEndTag         (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputDlStartTag        (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputDlEndTag          (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputDtStartTag        (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputDtEndTag          (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputEmStartTag        (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputEmEndTag          (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputFontStartTag      (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputFontEndTag        (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputFormStartTag      (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputFormEndTag        (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputH1StartTag        (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputH1EndTag          (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputH2StartTag        (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputH2EndTag          (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputH3StartTag        (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputH3EndTag          (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputH4StartTag        (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputH4EndTag          (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputH5StartTag        (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputH5EndTag          (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputH6StartTag        (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputH6EndTag          (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputHeadStartTag      (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputHeadEndTag        (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputHrTag             (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputHtmlStartTag      (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputHtmlEndTag        (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputIStartTag         (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputIEndTag           (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputImgTag            (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputInputTag          (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputKbdStartTag       (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputKbdEndTag         (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputLiStartTag        (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputLiEndTag          (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputMenuStartTag      (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputMenuEndTag        (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputNobrStartTag      (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputNobrEndTag        (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputOlStartTag        (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputOlEndTag          (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputOptionStartTag    (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputOptionEndTag      (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputPStartTag         (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputPEndTag           (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputPreStartTag       (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputPreEndTag         (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputSampStartTag      (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputSampEndTag        (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputScriptStartTag    (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputScriptEndTag      (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputSelectStartTag    (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputSelectEndTag      (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputSmallStartTag     (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputSmallEndTag       (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputStrikeStartTag    (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputStrikeEndTag      (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputStrongStartTag    (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputStrongEndTag      (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputSubStartTag       (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputSubEndTag         (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputSupStartTag       (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputSupEndTag         (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputTableStartTag     (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputTableEndTag       (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputTdStartTag        (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputTdEndTag          (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputTextareaStartTag  (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputTextareaEndTag    (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputThStartTag        (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputThEndTag          (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputTitleStartTag     (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputTitleEndTag       (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputTrStartTag        (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputTrEndTag          (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputTtStartTag        (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputTtEndTag          (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputUStartTag         (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputUEndTag           (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputUlStartTag        (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputUlEndTag          (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputVarStartTag       (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputVarEndTag         (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputWbrTag            (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputXmpStartTag       (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputXmpEndTag         (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputSkipStartTag      (CBrowserWindow *, CHtmlTag *);
static void CBrowserOutputSkipEndTag        (CBrowserWindow *, CHtmlTag *);

static void CBrowserOutputNewLine (CBrowserWindow *);
static void CBrowserOutputSkipLine(CBrowserWindow *);

//------

static CBrowserOutputData
output_datas[] = {
  { CHtmlTagId::A         , CBrowserOutputAStartTag         , CBrowserOutputAEndTag          },
  { CHtmlTagId::ADDRESS   , CBrowserOutputAddressStartTag   , CBrowserOutputAddressEndTag    },
  { CHtmlTagId::APPLET    , nullptr                         , nullptr                        },
  { CHtmlTagId::AREA      , nullptr                         , nullptr                        },
  { CHtmlTagId::B         , CBrowserOutputBStartTag         , CBrowserOutputBEndTag          },
  { CHtmlTagId::BASE      , nullptr                         , nullptr                        },
  { CHtmlTagId::BASEFONT  , CBrowserOutputBasefontStartTag  , CBrowserOutputBasefontEndTag   },
  { CHtmlTagId::BGSOUND   , nullptr                         , nullptr                        },
  { CHtmlTagId::BIG       , CBrowserOutputBigStartTag       , CBrowserOutputBigEndTag        },
  { CHtmlTagId::BLINK     , CBrowserOutputBlinkStartTag     , CBrowserOutputBlinkEndTag      },
  { CHtmlTagId::BLOCKQUOTE, CBrowserOutputBlockquoteStartTag, CBrowserOutputBlockquoteEndTag },
  { CHtmlTagId::BODY      , CBrowserOutputBodyStartTag      , CBrowserOutputBodyEndTag       },
  { CHtmlTagId::BR        , CBrowserOutputBrTag             , nullptr                        },
  { CHtmlTagId::CANVAS    , CBrowserOutputCanvasStartTag    , CBrowserOutputCanvasEndTag     },
  { CHtmlTagId::CAPTION   , CBrowserOutputCaptionStartTag   , CBrowserOutputCaptionEndTag    },
  { CHtmlTagId::CENTER    , CBrowserOutputCenterStartTag    , CBrowserOutputCenterEndTag     },
  { CHtmlTagId::CITE      , CBrowserOutputCiteStartTag      , CBrowserOutputCiteEndTag       },
  { CHtmlTagId::CODE      , CBrowserOutputCodeStartTag      , CBrowserOutputCodeEndTag       },
  { CHtmlTagId::DD        , CBrowserOutputDdStartTag        , CBrowserOutputDdEndTag         },
  { CHtmlTagId::DFN       , CBrowserOutputDfnStartTag       , CBrowserOutputDfnEndTag        },
  { CHtmlTagId::DIR       , CBrowserOutputDirStartTag       , CBrowserOutputDirEndTag        },
  { CHtmlTagId::DIV       , CBrowserOutputDivStartTag       , CBrowserOutputDivEndTag        },
  { CHtmlTagId::DL        , CBrowserOutputDlStartTag        , CBrowserOutputDlEndTag         },
  { CHtmlTagId::DT        , CBrowserOutputDtStartTag        , CBrowserOutputDtEndTag         },
  { CHtmlTagId::EM        , CBrowserOutputEmStartTag        , CBrowserOutputEmEndTag         },
  { CHtmlTagId::FONT      , CBrowserOutputFontStartTag      , CBrowserOutputFontEndTag       },
  { CHtmlTagId::FORM      , CBrowserOutputFormStartTag      , CBrowserOutputFormEndTag       },
  { CHtmlTagId::FRAME     , nullptr                         , nullptr                        },
  { CHtmlTagId::FRAMESET  , nullptr                         , nullptr                        },
  { CHtmlTagId::H1        , CBrowserOutputH1StartTag        , CBrowserOutputH1EndTag         },
  { CHtmlTagId::H2        , CBrowserOutputH2StartTag        , CBrowserOutputH2EndTag         },
  { CHtmlTagId::H3        , CBrowserOutputH3StartTag        , CBrowserOutputH3EndTag         },
  { CHtmlTagId::H4        , CBrowserOutputH4StartTag        , CBrowserOutputH4EndTag         },
  { CHtmlTagId::H5        , CBrowserOutputH5StartTag        , CBrowserOutputH5EndTag         },
  { CHtmlTagId::H6        , CBrowserOutputH6StartTag        , CBrowserOutputH6EndTag         },
  { CHtmlTagId::HEAD      , CBrowserOutputHeadStartTag      , CBrowserOutputHeadEndTag       },
  { CHtmlTagId::HR        , CBrowserOutputHrTag             , nullptr                        },
  { CHtmlTagId::HTML      , CBrowserOutputHtmlStartTag      , CBrowserOutputHtmlEndTag       },
  { CHtmlTagId::I         , CBrowserOutputIStartTag         , CBrowserOutputIEndTag          },
  { CHtmlTagId::IMG       , CBrowserOutputImgTag            , nullptr                        },
  { CHtmlTagId::INPUT     , CBrowserOutputInputTag          , CBrowserOutputSkipEndTag       },
  { CHtmlTagId::ISINDEX   , nullptr                         , nullptr                        },
  { CHtmlTagId::KBD       , CBrowserOutputKbdStartTag       , CBrowserOutputKbdEndTag        },
  { CHtmlTagId::LI        , CBrowserOutputLiStartTag        , CBrowserOutputLiEndTag         },
  { CHtmlTagId::LINK      , nullptr                         , nullptr                        },
  { CHtmlTagId::LISTING   , nullptr                         , nullptr                        },
  { CHtmlTagId::MAP       , nullptr                         , nullptr                        },
  { CHtmlTagId::MARQUEE   , nullptr                         , nullptr                        },
  { CHtmlTagId::MENU      , CBrowserOutputMenuStartTag      , CBrowserOutputMenuEndTag       },
  { CHtmlTagId::META      , CBrowserOutputSkipStartTag      , nullptr                        },
  { CHtmlTagId::NEXTID    , nullptr                         , nullptr                        },
  { CHtmlTagId::NOBR      , CBrowserOutputNobrStartTag      , CBrowserOutputNobrEndTag       },
  { CHtmlTagId::NOFRAMES  , nullptr                         , nullptr                        },
  { CHtmlTagId::OL        , CBrowserOutputOlStartTag        , CBrowserOutputOlEndTag         },
  { CHtmlTagId::OPTION    , CBrowserOutputOptionStartTag    , CBrowserOutputOptionEndTag     },
  { CHtmlTagId::P         , CBrowserOutputPStartTag         , CBrowserOutputPEndTag          },
  { CHtmlTagId::PARAM     , nullptr                         , nullptr                        },
  { CHtmlTagId::PLAINTEXT , nullptr                         , nullptr                        },
  { CHtmlTagId::PRE       , CBrowserOutputPreStartTag       , CBrowserOutputPreEndTag        },
  { CHtmlTagId::S         , CBrowserOutputStrikeStartTag    , CBrowserOutputStrikeEndTag     },
  { CHtmlTagId::SAMP      , CBrowserOutputSampStartTag      , CBrowserOutputSampEndTag       },
  { CHtmlTagId::SCRIPT    , CBrowserOutputScriptStartTag    , CBrowserOutputScriptEndTag     },
  { CHtmlTagId::SELECT    , CBrowserOutputSelectStartTag    , CBrowserOutputSelectEndTag     },
  { CHtmlTagId::SMALL     , CBrowserOutputSmallStartTag     , CBrowserOutputSmallEndTag      },
  { CHtmlTagId::SPAN      , nullptr                         , nullptr                        },
  { CHtmlTagId::STRIKE    , CBrowserOutputStrikeStartTag    , CBrowserOutputStrikeEndTag     },
  { CHtmlTagId::STRONG    , CBrowserOutputStrongStartTag    , CBrowserOutputStrongEndTag     },
  { CHtmlTagId::STYLE     , nullptr                         , nullptr                        },
  { CHtmlTagId::SUB       , CBrowserOutputSubStartTag       , CBrowserOutputSubEndTag        },
  { CHtmlTagId::SUP       , CBrowserOutputSupStartTag       , CBrowserOutputSupEndTag        },
  { CHtmlTagId::TABLE     , CBrowserOutputTableStartTag     , CBrowserOutputTableEndTag      },
  { CHtmlTagId::TD        , CBrowserOutputTdStartTag        , CBrowserOutputTdEndTag         },
  { CHtmlTagId::TEXTAREA  , CBrowserOutputTextareaStartTag  , CBrowserOutputTextareaEndTag   },
  { CHtmlTagId::TH        , CBrowserOutputThStartTag        , CBrowserOutputThEndTag         },
  { CHtmlTagId::TITLE     , CBrowserOutputTitleStartTag     , CBrowserOutputTitleEndTag      },
  { CHtmlTagId::TR        , CBrowserOutputTrStartTag        , CBrowserOutputTrEndTag         },
  { CHtmlTagId::TT        , CBrowserOutputTtStartTag        , CBrowserOutputTtEndTag         },
  { CHtmlTagId::U         , CBrowserOutputUStartTag         , CBrowserOutputUEndTag          },
  { CHtmlTagId::UL        , CBrowserOutputUlStartTag        , CBrowserOutputUlEndTag         },
  { CHtmlTagId::VAR       , CBrowserOutputVarStartTag       , CBrowserOutputVarEndTag        },
  { CHtmlTagId::WBR       , CBrowserOutputWbrTag            , nullptr                        },
  { CHtmlTagId::XMP       , CBrowserOutputXmpStartTag       , CBrowserOutputXmpEndTag        },
  { CHtmlTagId::COMMENT   , nullptr                         , nullptr                        },
};

static uint num_output_datas = sizeof(output_datas)/sizeof(CBrowserOutputData);

CBrowserOutputData *
CBrowserGetOutputData(CHtmlTagId id)
{
  typedef std::map<CHtmlTagId,CBrowserOutputData *> TagOutputData;

  static TagOutputData tagOutputData;

  if (tagOutputData.empty()) {
    for (uint i = 0; i < num_output_datas; ++i)
      tagOutputData[output_datas[i].id] = &output_datas[i];
  }

  auto p = tagOutputData.find(id);

  if (p == tagOutputData.end())
    return nullptr;

  return (*p).second;
}

//------

void
CBrowserOutputTokens(CBrowserWindow *window, CHtmlParserTokens &tokens)
{
  typedef std::vector<CHtmlTag *> TagStack;

  TagStack tagStack;

  CBrowserOutputInit(window);

  for (int i = 0; i < tokens.size(); ++i) {
    const CHtmlToken *t = tokens[i];

    if      (t->isTag()) {
      CHtmlTag *tag = t->getTag();

      if      (tag->isStartTag())
        tagStack.push_back(tag);
      else if (tag->isEndTag()) {
        if (! tagStack.empty())
          tagStack.pop_back();
      }

      CBrowserOutputTag(window, tag);
    }
    else if (t->isText()) {
      CHtmlText *text = t->getText();

      CHtmlTag *currentTag = (! tagStack.empty() ? tagStack.back() : nullptr);

      if (currentTag && currentTag->getTagDef().getId() == CHtmlTagId::CANVAS)
        continue;

      CBrowserOutputText(window, text);
    }
  }

  CBrowserOutputTerm();
}

void
CBrowserOutputInit(CBrowserWindow *window)
{
  textData.color = window->currentFontColor();
}

void
CBrowserOutputTerm()
{
}

void
CBrowserOutputTag(CBrowserWindow *window, CHtmlTag *tag)
{
  const CHtmlTagDef &data = tag->getTagDef();

  CHtmlTagId id = data.getId();

  CBrowserOutputData *output_data = CBrowserGetOutputData(id);

  if (! output_data) {
    window->displayError("Tag %s not found\n", tag->getName().c_str());
    return;
  }

  if (tag->isStartTag()) {
    if (output_data->start_proc)
      (*output_data->start_proc)(window, tag);
    else
      window->displayError("Start Tag %s not implemented\n", tag->getName().c_str());
  }
  else  {
    if (output_data->end_proc)
      (*output_data->end_proc)(window, tag);
    else
      window->displayError("End Tag %s not implemented\n", tag->getName().c_str());
  }
}

void
CBrowserOutputText(CBrowserWindow *window, CHtmlText *text)
{
  std::string str = text->getText();

  if      (in_title)
    window->getDocument()->setTitle(str);
  else if (scriptData.active) {
    scriptData.text += str;
  }
  else if (in_option)
    option_text += str;
  else if (in_textarea)
    textarea_text += str;
  else
    window->addText(str, textData.color, textData.underline, textData.strike,
                    textData.place, textData.breakup, textData.format);
}

static void
CBrowserOutputAStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int         i;
  std::string href;
  std::string name;
  std::string title;

  /*-------------*/

  int num_options = tag->getNumOptions();

  for (i = 0; i < num_options; i++) {
    const CHtmlTagOption *option = tag->getOption(i);

    std::string option_name = option->getName();

    if      (CStrUtil::casecmp(option_name, "href") == 0)
      href = option->getValue();
    else if (CStrUtil::casecmp(option_name, "methods") == 0)
      ;
    else if (CStrUtil::casecmp(option_name, "name") == 0)
      name = option->getValue();
    else if (CStrUtil::casecmp(option_name, "rel") == 0)
      ;
    else if (CStrUtil::casecmp(option_name, "rev") == 0)
      ;
    else if (CStrUtil::casecmp(option_name, "target") == 0)
      ;
    else if (CStrUtil::casecmp(option_name, "title") == 0)
      title = option->getValue();
    else if (CStrUtil::casecmp(option_name, "urn") == 0)
      ;
    else
      window->displayError("Illegal 'a' Option Name '%s'\n", option_name.c_str());
  }

  /*-------------*/

  if (href == "" && name == "") {
    window->displayError("No 'href' or 'name' specified for A Tag'\n");
    return;
  }

  if (href != "") {
    window->linkMgr()->startSourceLink(href, title);

    textData.color = window->getDocument()->getLinkColor();

    textData.underline = true;
  }
  else
    window->linkMgr()->startDestLink(name, title);
}

static void
CBrowserOutputAEndTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  textData.underline = false;

  textData.color = window->getDocument()->getFgColor();

  window->linkMgr()->endLink();
}

static void
CBrowserOutputAddressStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  window->startItalic();
}

static void
CBrowserOutputAddressEndTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  window->endItalic();
}

static void
CBrowserOutputBStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  window->startBold();
}

static void
CBrowserOutputBEndTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  window->endBold();
}

static void
CBrowserOutputBasefontStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int size;
  int size_flag;

  /*-------------*/

  int error_code = 1;

  int num_options = tag->getNumOptions();

  for (int i = 0; i < num_options; i++) {
    const CHtmlTagOption *option = tag->getOption(i);

    std::string option_name = option->getName();

    if (CStrUtil::casecmp(option_name, "size") == 0) {
      std::string value = option->getValue();

      if      (value[0] == '+')
        size_flag = 1;
      else if (value[0] == '-')
        size_flag = -1;
      else
        size_flag = 0;

      if (CStrUtil::isInteger(option->getValue()))
        size = CStrUtil::toInteger(option->getValue());
      else
        window->displayError("Illegal 'basefont' Value for Size %s\n", option->getValue().c_str());
    }
    else
      window->displayError("Illegal 'basefont' Option Name '%s'\n", option_name.c_str());
  }

  /*-------------*/

  if (error_code == 0) {
    if      (size_flag == 1) {
      for (int i = 0; i < size; i++)
        window->increaseBaseFontSize();
    }
    else if (size_flag == -1) {
      for (int i = 0; i < -size; i++)
        window->decreaseBaseFontSize();
    }
    else
      window->setBaseFontSize(size);
  }
  else
    window->resetBaseFontSize();
}

static void
CBrowserOutputBasefontEndTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  window->resetFontSize();
}

static void
CBrowserOutputBigStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  window->increaseFontSize();
}

static void
CBrowserOutputBigEndTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  window->decreaseFontSize();
}

static void
CBrowserOutputBlinkStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  window->startBold();
}

static void
CBrowserOutputBlinkEndTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  window->endBold();
}

static void
CBrowserOutputBlockquoteStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  window->indentLeft (2);
  window->indentRight(2);

  CBrowserOutputSkipLine(window);

  window->startItalic();
}

static void
CBrowserOutputBlockquoteEndTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  window->endItalic();

  window->indentLeft (-2);
  window->indentRight(-2);

  CBrowserOutputSkipLine(window);
}

static void
CBrowserOutputBodyStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  std::string text;
  std::string bgcolor;
  std::string background;

  /*-------------*/

  std::string alink      = "red";
  bool        fixed      = false;
  int         leftmargin = LEFT_MARGIN;
  std::string link       = "blue";
  int         topmargin  = TOP_MARGIN;
  std::string vlink      = "purple";

  int num_options = tag->getNumOptions();

  for (int i = 0; i < num_options; i++) {
    const CHtmlTagOption *option = tag->getOption(i);

    std::string option_name = option->getName();

    if      (CStrUtil::casecmp(option_name, "alink") == 0)
      alink = option->getValue();
    else if (CStrUtil::casecmp(option_name, "background") == 0)
      background = option->getValue();
    else if (CStrUtil::casecmp(option_name, "bgcolor") == 0)
      bgcolor = option->getValue();
    else if (CStrUtil::casecmp(option_name, "bgproperties") == 0) {
      if      (CStrUtil::casecmp(option->getValue(), "fixed") == 0)
        fixed = true;
      else
        window->displayError("Illegal 'body' bgproperties value %s\n",
                             option->getValue().c_str());
    }
    else if (CStrUtil::casecmp(option_name, "leftmargin") == 0) {
      if (CStrUtil::isInteger(option->getValue()))
        leftmargin = CStrUtil::toInteger(option->getValue());
      else {
        window->displayError("Illegal 'body' Value for leftmargin %s\n",
                             option->getValue().c_str());

        leftmargin = LEFT_MARGIN;
      }
    }
    else if (CStrUtil::casecmp(option_name, "link") == 0)
      link = option->getValue();
    else if (CStrUtil::casecmp(option_name, "text") == 0)
      text = option->getValue();
    else if (CStrUtil::casecmp(option_name, "topmargin") == 0) {
      if (CStrUtil::isInteger(option->getValue()))
        topmargin = CStrUtil::toInteger(option->getValue());
      else {
        window->displayError("Illegal 'body' Value for leftmargin %s\n",
                             option->getValue().c_str());

        topmargin = TOP_MARGIN;
      }
    }
    else if (CStrUtil::casecmp(option_name, "vlink") == 0)
      vlink = option->getValue();
    else
      window->displayError("Illegal 'body' Option Name '%s'\n", option_name.c_str());
  }

  /*-------------*/

  window->getDocument()->setBgColor(bgcolor);
  window->getDocument()->setFgColor(text   );

  window->setBackgroundImage(background, fixed);

  window->getDocument()->setLinkColor (link);
  window->getDocument()->setALinkColor(alink);
  window->getDocument()->setVLinkColor(vlink);

  window->setMargins(leftmargin, topmargin);
}

static void
CBrowserOutputBodyEndTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());
}

static void
CBrowserOutputBrTag(CBrowserWindow *window, CHtmlTag *tag)
{
  CHtmlLayoutClearType clear = CHtmlLayoutClearType::NONE;

  int num_options = tag->getNumOptions();

  for (int i = 0; i < num_options; i++) {
    const CHtmlTagOption *option = tag->getOption(i);

    std::string option_name = option->getName();

    if (CStrUtil::casecmp(option_name, "clear") == 0) {
      if      (CStrUtil::casecmp(option->getValue(), "left" ) == 0)
        clear = CHtmlLayoutClearType::LEFT;
      else if (CStrUtil::casecmp(option->getValue(), "right") == 0)
        clear = CHtmlLayoutClearType::RIGHT;
      else if (CStrUtil::casecmp(option->getValue(), "all"  ) == 0)
        clear = CHtmlLayoutClearType::ALL;
      else
        window->displayError("Illegal 'br' Clear %s\n", option->getValue().c_str());
    }
    else
      window->displayError("Illegal 'br' Option Name '%s'\n", option_name.c_str());
  }

  /*-------------*/

  window->addBreak(clear);
}

static void
CBrowserOutputCanvasStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserCanvasData data;

  int num_options = tag->getNumOptions();

  for (int i = 0; i < num_options; i++) {
    const CHtmlTagOption *option = tag->getOption(i);

    std::string option_name = option->getName();

    if      (CStrUtil::casecmp(option_name, "id") == 0) {
      data.id = option->getValue();
    }
    else if (CStrUtil::casecmp(option_name, "width") == 0) {
      if (CStrUtil::isInteger(option->getValue()))
        data.width = CStrUtil::toInteger(option->getValue());
      else {
        window->displayError("Illegal 'canvas' Value for Width %s\n", option->getValue().c_str());
        data.width = -1;
      }
    }
    else if (CStrUtil::casecmp(option_name, "height") == 0) {
      if (CStrUtil::isInteger(option->getValue()))
        data.height = CStrUtil::toInteger(option->getValue());
      else {
        window->displayError("Illegal 'canvas' Value for Height %s\n", option->getValue().c_str());
        data.height = -1;
      }
    }
    else
      window->displayError("Illegal 'canvas' Option Name '%s'\n", option_name.c_str());
  }

  window->addCanvas(data);

  CBrowserOutputSkipLine(window);
}

static void
CBrowserOutputCanvasEndTag(CBrowserWindow *, CHtmlTag *)
{
}

static void
CBrowserOutputCaptionStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserTableCaptionData captionData;

  int num_options = tag->getNumOptions();

  for (int i = 0; i < num_options; i++) {
    const CHtmlTagOption *option = tag->getOption(i);

    std::string option_name = option->getName();

    if      (CStrUtil::casecmp(option_name, "align") == 0) {
      if      (CStrUtil::casecmp(option->getValue(), "left") == 0)
        captionData.halign = CHALIGN_TYPE_LEFT;
      else if (CStrUtil::casecmp(option->getValue(), "center") == 0)
        captionData.halign = CHALIGN_TYPE_CENTER;
      else if (CStrUtil::casecmp(option->getValue(), "right") == 0)
        captionData.halign = CHALIGN_TYPE_RIGHT;
      else if (CStrUtil::casecmp(option->getValue(), "top") == 0)
        captionData.valign = CVALIGN_TYPE_TOP;
      else if (CStrUtil::casecmp(option->getValue(), "bottom") == 0)
        captionData.valign = CVALIGN_TYPE_BOTTOM;
      else
        window->displayError("Illegal %s Alignment %s\n", "Table Caption",
                             option->getValue().c_str());
    }
    else if (CStrUtil::casecmp(option_name, "valign") == 0) {
      if      (CStrUtil::casecmp(option->getValue(), "top") == 0)
        captionData.valign = CVALIGN_TYPE_TOP;
      else if (CStrUtil::casecmp(option->getValue(), "bottom") == 0)
        captionData.valign = CVALIGN_TYPE_BOTTOM;
      else
        window->displayError("Illegal %s Vertical Alignment %s\n", "Table Caption",
                             option->getValue().c_str());
    }
    else
      window->displayError("Illegal 'caption' Option Name '%s'\n", option_name.c_str());
  }

  //---

  window->tableMgr()->startTableCaption(captionData);
}

static void
CBrowserOutputCaptionEndTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  //---

  window->tableMgr()->endTableCaption();
}

static void
CBrowserOutputCenterStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  CBrowserOutputSkipLine(window);

  window->setAlign(CHALIGN_TYPE_CENTER, CVALIGN_TYPE_TOP);
}

static void
CBrowserOutputCenterEndTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  CBrowserOutputSkipLine(window);

  window->setAlign(CHALIGN_TYPE_LEFT, CVALIGN_TYPE_TOP);
}

static void
CBrowserOutputCiteStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  window->startItalic();
}

static void
CBrowserOutputCiteEndTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  window->endItalic();
}

static void
CBrowserOutputCodeStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  window->startFontFace("courier");
}

static void
CBrowserOutputCodeEndTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  window->endFontFace();
}

static void
CBrowserOutputDdStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  if (in_dt)
    CBrowserOutputDtEndTag(window, nullptr);

  if (in_dd)
    CBrowserOutputDdEndTag(window, nullptr);

  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  window->indentLeft(4);

  CBrowserOutputNewLine(window);

  in_dd = true;
}

static void
CBrowserOutputDdEndTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (tag && num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  window->indentLeft(-4);

  CBrowserOutputNewLine(window);

  in_dd = false;
}

static void
CBrowserOutputDfnStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  window->startItalic();
}

static void
CBrowserOutputDfnEndTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  window->endItalic();
}

static void
CBrowserOutputDirStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  if (current_list)
    list_stack.push_back(current_list);

  current_list = new CBrowserOutputList(CBrowserOutputListType::DIR);

  /*-------------*/

  int num_options = tag->getNumOptions();

  for (int i = 0; i < num_options; i++) {
    const CHtmlTagOption *option = tag->getOption(i);

    std::string option_name = option->getName();

    if      (CStrUtil::casecmp(option_name, "compact") == 0) {
      current_list->setCompact(true);

      std::string value = option->getValue();

      if (value[0] != '\0')
        window->displayError("No Value needed for Compact\n");
    }
    else if (CStrUtil::casecmp(option_name, "type") == 0)
      current_list->setSymbol(option->getValue());
    else
      window->displayError("Illegal Dir Option Name '%s'\n", option_name.c_str());
  }

  /*-------------*/

  window->indentLeft(2);

  if (list_stack.empty() || ! current_list->getCompact())
    CBrowserOutputSkipLine(window);
  else
    CBrowserOutputNewLine(window);
}

static void
CBrowserOutputDirEndTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  window->indentLeft(-2);

  if (list_stack.empty())
    CBrowserOutputSkipLine(window);
  else
    CBrowserOutputNewLine(window);

  /*-------------*/

  delete current_list;

  if (! list_stack.empty()) {
    current_list = list_stack[list_stack.size() - 1];

    list_stack.pop_back();
  }
  else
    current_list = nullptr;
}

static void
CBrowserOutputDivStartTag(CBrowserWindow *, CHtmlTag *)
{
}

static void
CBrowserOutputDivEndTag(CBrowserWindow *, CHtmlTag *)
{
}

static void
CBrowserOutputDlStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  if (current_list)
    list_stack.push_back(current_list);

  current_list = new CBrowserOutputList(CBrowserOutputListType::DL);

  /*-------------*/

  int num_options = tag->getNumOptions();

  for (int i = 0; i < num_options; i++) {
    const CHtmlTagOption *option = tag->getOption(i);

    std::string option_name = option->getName();

    if      (CStrUtil::casecmp(option_name, "compact") == 0) {
      current_list->setCompact(true);

      std::string value = option->getValue();

      if (value[0] != '\0')
        window->displayError("No Value needed for Compact\n");
    }
    else
      window->displayError("Illegal Dl Option Name '%s'\n", option_name.c_str());
  }

  /*-------------*/

  window->indentLeft(2);

  if (list_stack.empty() || ! current_list->getCompact())
    CBrowserOutputSkipLine(window);
  else
    CBrowserOutputNewLine(window);
}

static void
CBrowserOutputDlEndTag(CBrowserWindow *window, CHtmlTag *tag)
{
  if (in_dt)
    CBrowserOutputDtEndTag(window, nullptr);

  if (in_dd)
    CBrowserOutputDdEndTag(window, nullptr);

  /*-------------*/

  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  window->indentLeft(-2);

  if (list_stack.empty())
    CBrowserOutputSkipLine(window);
  else
    CBrowserOutputNewLine(window);

  /*-------------*/

  delete current_list;

  if (! list_stack.empty()) {
    current_list = list_stack[list_stack.size() - 1];

    list_stack.pop_back();
  }
  else
    current_list = nullptr;
}

static void
CBrowserOutputDtStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  if (in_dt)
    CBrowserOutputDtEndTag(window, nullptr);

  if (in_dd)
    CBrowserOutputDdEndTag(window, nullptr);

  /*-------------*/

  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  window->startBold();

  in_dt = true;
}

static void
CBrowserOutputDtEndTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (tag && num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  window->endBold();

  in_dt = false;
}

static void
CBrowserOutputEmStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  window->startItalic();
}

static void
CBrowserOutputEmEndTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  window->endItalic();
}

static void
CBrowserOutputFontStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  std::string color = window->getFontColor();
  std::string face  = window->getCurrentFontFace();
  int         size  = window->getFontSize();

  CBrowserOutputFont *font = new CBrowserOutputFont(face, color, size);

  CBrowserOutputFont::startFont(font);

  /*-------------*/

  size = -1;

  int delta = 1;

  int num_options = tag->getNumOptions();

  for (int i = 0; i < num_options; i++) {
    const CHtmlTagOption *option = tag->getOption(i);

    std::string option_name = option->getName();

    if      (CStrUtil::casecmp(option_name, "color") == 0)
      color = option->getValue();
    else if (CStrUtil::casecmp(option_name, "face") == 0)
      face = option->getValue();
    else if (CStrUtil::casecmp(option_name, "size") == 0) {
      std::string value = option->getValue();

      if      (value[0] == '+')
        delta =  1;
      else if (value[0] == '-')
        delta = -1;
      else
        delta =  0;

      if (CStrUtil::isInteger(option->getValue()))
        size = CStrUtil::toInteger(option->getValue());
      else {
        window->displayError("Illegal Value for Size %s\n", option->getValue().c_str());

        size  = -1;
        delta = 1;
      }
    }
    else
      window->displayError("Illegal Font Option Name '%s'\n", option_name.c_str());
  }

  /*-------------*/

  if (face != "") {
    window->setCurrentFontFace(face);

    font->setFlag(CBrowserOutputFontFlag::FACE);
  }

  if (color != "") {
    window->setFontColor(color);

    font->setFlag(CBrowserOutputFontFlag::COLOR);
  }

  int base_size = window->getBaseFontSize();

  if (delta != 0)
    size += base_size;

  if (size > 0) {
    window->setFontSize(size);

    font->setFlag(CBrowserOutputFontFlag::SIZE);
  }
}

static void
CBrowserOutputFontEndTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  /*-------------*/

  CBrowserOutputFont *font = CBrowserOutputFont::endFont();

  if (! font) {
    window->displayError("<font> ... </font> mismatch\n");
    return;
  }

  if (font->getFlag(CBrowserOutputFontFlag::FACE))
    window->setCurrentFontFace(font->getFace());

  if (font->getFlag(CBrowserOutputFontFlag::COLOR))
    window->setFontColor(font->getColor());

  if (font->getFlag(CBrowserOutputFontFlag::SIZE))
    window->setFontSize(font->getSize());

  /*-------------*/

  delete font;
}

static void
CBrowserOutputFormStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  std::string name   = "";
  int         method = FORM_METHOD_GET;
  std::string action = "";

  int num_options = tag->getNumOptions();

  for (int i = 0; i < num_options; i++) {
    const CHtmlTagOption *option = tag->getOption(i);

    std::string option_name = option->getName();

    if      (CStrUtil::casecmp(option_name, "action") == 0)
      action = option->getValue();
    else if (CStrUtil::casecmp(option_name, "enctype") == 0)
      ;
    else if (CStrUtil::casecmp(option_name, "method") == 0) {
      if      (CStrUtil::casecmp(option->getValue(), "get" ) == 0)
        method = FORM_METHOD_GET;
      else if (CStrUtil::casecmp(option->getValue(), "post") == 0)
        method = FORM_METHOD_POST;
      else
        window->displayError("Illegal form method %s\n", option->getValue().c_str());
    }
    else if (CStrUtil::casecmp(option_name, "name") == 0)
      name = option->getValue();
    else
      window->displayError("Illegal %s Option Name '%s'\n",
                           tag->getName().c_str(), option_name.c_str());
  }

  //----

  window->getDocument()->formMgr()->startForm(name, method, action);
}

static void
CBrowserOutputFormEndTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  //----

  window->getDocument()->formMgr()->endForm();
}

static void
CBrowserOutputH1StartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  CHAlignType align = CHALIGN_TYPE_LEFT;

  int num_options = tag->getNumOptions();

  for (int i = 0; i < num_options; i++) {
    const CHtmlTagOption *option = tag->getOption(i);

    std::string option_name = option->getName();

    if (CStrUtil::casecmp(option_name, "align") == 0) {
      if      (CStrUtil::casecmp(option->getValue(), "left") == 0)
        align = CHALIGN_TYPE_LEFT;
      else if (CStrUtil::casecmp(option->getValue(), "center") == 0)
        align = CHALIGN_TYPE_CENTER;
      else if (CStrUtil::casecmp(option->getValue(), "right") == 0)
        align = CHALIGN_TYPE_RIGHT;
      else
        window->displayError("Illegal Header Alignment %s\n", option->getValue().c_str());
    }
    else
      window->displayError("Illegal H1 Option Name '%s'\n", option_name.c_str());
  }

  /*-----------*/

  CBrowserOutputSkipLine(window);

  window->setAlign(align, CVALIGN_TYPE_TOP);

  window->setFontSize(6);

  window->startBold();
}

static void
CBrowserOutputH1EndTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  /*-----------*/

  window->endBold();

  window->resetFontSize();

  CBrowserOutputSkipLine(window);

  window->setAlign(CHALIGN_TYPE_LEFT, CVALIGN_TYPE_TOP);
}

static void
CBrowserOutputH2StartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  CHAlignType align = CHALIGN_TYPE_LEFT;

  int num_options = tag->getNumOptions();

  for (int i = 0; i < num_options; i++) {
    const CHtmlTagOption *option = tag->getOption(i);

    std::string option_name = option->getName();

    if (CStrUtil::casecmp(option_name, "align") == 0) {
      if      (CStrUtil::casecmp(option->getValue(), "left") == 0)
        align = CHALIGN_TYPE_LEFT;
      else if (CStrUtil::casecmp(option->getValue(), "center") == 0)
        align = CHALIGN_TYPE_CENTER;
      else if (CStrUtil::casecmp(option->getValue(), "right") == 0)
        align = CHALIGN_TYPE_RIGHT;
      else
        window->displayError("Illegal Header Alignment %s\n", option->getValue().c_str());
    }
    else
      window->displayError("Illegal H2 Option Name '%s'\n", option_name.c_str());
  }

  /*-----------*/

  CBrowserOutputSkipLine(window);

  window->setAlign(align, CVALIGN_TYPE_TOP);

  window->setFontSize(5);

  window->startBold();
}

static void
CBrowserOutputH2EndTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  /*-----------*/

  window->endBold();

  window->resetFontSize();

  CBrowserOutputSkipLine(window);

  window->setAlign(CHALIGN_TYPE_LEFT, CVALIGN_TYPE_TOP);
}

static void
CBrowserOutputH3StartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  CHAlignType align = CHALIGN_TYPE_LEFT;

  int num_options = tag->getNumOptions();

  for (int i = 0; i < num_options; i++) {
    const CHtmlTagOption *option = tag->getOption(i);

    std::string option_name = option->getName();

    if (CStrUtil::casecmp(option_name, "align") == 0) {
      if      (CStrUtil::casecmp(option->getValue(), "left") == 0)
        align = CHALIGN_TYPE_LEFT;
      else if (CStrUtil::casecmp(option->getValue(), "center") == 0)
        align = CHALIGN_TYPE_CENTER;
      else if (CStrUtil::casecmp(option->getValue(), "right") == 0)
        align = CHALIGN_TYPE_RIGHT;
      else
        window->displayError("Illegal Header Alignment %s\n", option->getValue().c_str());
    }
    else
      window->displayError("Illegal H3 Option Name '%s'\n", option_name.c_str());
  }

  /*-----------*/

  CBrowserOutputSkipLine(window);

  window->setAlign(align, CVALIGN_TYPE_TOP);

  window->setFontSize(4);

  window->startBold();
}

static void
CBrowserOutputH3EndTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  /*-----------*/

  window->endBold();

  window->resetFontSize();

  CBrowserOutputSkipLine(window);

  window->setAlign(CHALIGN_TYPE_LEFT, CVALIGN_TYPE_TOP);
}

static void
CBrowserOutputH4StartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  CHAlignType align = CHALIGN_TYPE_LEFT;

  int num_options = tag->getNumOptions();

  for (int i = 0; i < num_options; i++) {
    const CHtmlTagOption *option = tag->getOption(i);

    std::string option_name = option->getName();

    if (CStrUtil::casecmp(option_name, "align") == 0) {
      if      (CStrUtil::casecmp(option->getValue(), "left") == 0)
        align = CHALIGN_TYPE_LEFT;
      else if (CStrUtil::casecmp(option->getValue(), "center") == 0)
        align = CHALIGN_TYPE_CENTER;
      else if (CStrUtil::casecmp(option->getValue(), "right") == 0)
        align = CHALIGN_TYPE_RIGHT;
      else
        window->displayError("Illegal Header Alignment %s\n", option->getValue().c_str());
    }
    else
      window->displayError("Illegal H4 Option Name '%s'\n", option_name.c_str());
  }

  /*-----------*/

  CBrowserOutputSkipLine(window);

  window->setAlign(align, CVALIGN_TYPE_TOP);

  window->setFontSize(3);

  window->startBold();
}

static void
CBrowserOutputH4EndTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  /*-----------*/

  window->endBold();

  window->resetFontSize();

  CBrowserOutputSkipLine(window);

  window->setAlign(CHALIGN_TYPE_LEFT, CVALIGN_TYPE_TOP);
}

static void
CBrowserOutputH5StartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  CHAlignType align = CHALIGN_TYPE_LEFT;

  int num_options = tag->getNumOptions();

  for (int i = 0; i < num_options; i++) {
    const CHtmlTagOption *option = tag->getOption(i);

    std::string option_name = option->getName();

    if (CStrUtil::casecmp(option_name, "align") == 0) {
      if      (CStrUtil::casecmp(option->getValue(), "left") == 0)
        align = CHALIGN_TYPE_LEFT;
      else if (CStrUtil::casecmp(option->getValue(), "center") == 0)
        align = CHALIGN_TYPE_CENTER;
      else if (CStrUtil::casecmp(option->getValue(), "right") == 0)
        align = CHALIGN_TYPE_RIGHT;
      else
        window->displayError("Illegal Header Alignment %s\n", option->getValue().c_str());
    }
    else
      window->displayError("Illegal H5 Option Name '%s'\n", option_name.c_str());
  }

  /*-----------*/

  CBrowserOutputSkipLine(window);

  window->setAlign(align, CVALIGN_TYPE_TOP);

  window->setFontSize(2);

  window->startBold();
}

static void
CBrowserOutputH5EndTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  /*-----------*/

  window->endBold();

  window->resetFontSize();

  CBrowserOutputSkipLine(window);

  window->setAlign(CHALIGN_TYPE_LEFT, CVALIGN_TYPE_TOP);
}

static void
CBrowserOutputH6StartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  CHAlignType align = CHALIGN_TYPE_LEFT;

  int num_options = tag->getNumOptions();

  for (int i = 0; i < num_options; i++) {
    const CHtmlTagOption *option = tag->getOption(i);

    std::string option_name = option->getName();

    if (CStrUtil::casecmp(option_name, "align") == 0) {
      if      (CStrUtil::casecmp(option->getValue(), "left") == 0)
        align = CHALIGN_TYPE_LEFT;
      else if (CStrUtil::casecmp(option->getValue(), "center") == 0)
        align = CHALIGN_TYPE_CENTER;
      else if (CStrUtil::casecmp(option->getValue(), "right") == 0)
        align = CHALIGN_TYPE_RIGHT;
      else
        window->displayError("Illegal Header Alignment %s\n", option->getValue().c_str());
    }
    else
      window->displayError("Illegal H6 Option Name '%s'\n", option_name.c_str());
  }

  /*-----------*/

  CBrowserOutputSkipLine(window);

  window->setAlign(align, CVALIGN_TYPE_TOP);

  window->setFontSize(1);

  window->startBold();
}

static void
CBrowserOutputH6EndTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  /*-----------*/

  window->endBold();

  window->resetFontSize();

  CBrowserOutputSkipLine(window);

  window->setAlign(CHALIGN_TYPE_LEFT, CVALIGN_TYPE_TOP);
}

static void
CBrowserOutputHeadStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());
}

static void
CBrowserOutputHeadEndTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());
}

static void
CBrowserOutputHrTag(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserRuleData ruleData;

  int num_options = tag->getNumOptions();

  for (int i = 0; i < num_options; i++) {
    const CHtmlTagOption *option = tag->getOption(i);

    std::string option_name = option->getName();

    if      (CStrUtil::casecmp(option_name, "align") == 0) {
      if      (CStrUtil::casecmp(option->getValue(), "left"  ) == 0)
        ruleData.align = CHALIGN_TYPE_LEFT;
      else if (CStrUtil::casecmp(option->getValue(), "center" ) == 0)
        ruleData.align = CHALIGN_TYPE_CENTER;
      else if (CStrUtil::casecmp(option->getValue(), "right" ) == 0)
        ruleData.align = CHALIGN_TYPE_RIGHT;
      else
        window->displayError("Illegal Align %s\n", option->getValue().c_str());
    }
    else if (CStrUtil::casecmp(option_name, "noshade") == 0) {
      ruleData.shade = false;

      std::string value = option->getValue();

      if (value[0] != '\0')
        window->displayError("No Value needed for Noshade\n");
    }
    else if (CStrUtil::casecmp(option_name, "size") == 0) {
      if (CStrUtil::isInteger(option->getValue()))
        ruleData.size = CStrUtil::toInteger(option->getValue());
      else {
        window->displayError("Illegal Value for Size %s\n", option->getValue().c_str());
        ruleData.size = -1;
      }
    }
    else if (CStrUtil::casecmp(option_name, "width") == 0) {
      uint len = option->getValue().size();

      std::string value = option->getValue();

      if (value[len - 1] == '%') {
        ruleData.unit = UNIT_PERCENT;

        std::string value = value.substr(0, len - 1);

        if (CStrUtil::isInteger(option->getValue()))
          ruleData.width = CStrUtil::toInteger(value);
        else {
          window->displayError("Illegal Value for Width %s\n", option->getValue().c_str());
          ruleData.width = -1;
        }
      }
      else {
        ruleData.unit = UNIT_PIXEL;

        if (CStrUtil::isInteger(option->getValue()))
          ruleData.width = CStrUtil::toInteger(option->getValue());
        else {
          window->displayError("Illegal Value for Width %s\n", option->getValue().c_str());
          ruleData.width = -1;
        }
      }
    }
    else
      window->displayError("Illegal 'hr' Option Name '%s'\n", option_name.c_str());
  }

  window->addRule(ruleData);
}

static void
CBrowserOutputHtmlStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  for (int i = 0; i < num_options; i++) {
    const CHtmlTagOption *option = tag->getOption(i);

    std::string option_name = option->getName();

    if      (CStrUtil::casecmp(option_name, "lang") == 0) {
    }
    else
      window->displayError("Illegal 'html' Option Name '%s'\n", option_name.c_str());
  }
}

static void
CBrowserOutputHtmlEndTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());
}

static void
CBrowserOutputIStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  window->startItalic();
}

static void
CBrowserOutputIEndTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  window->endItalic();
}

static void
CBrowserOutputImgTag(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserImageData imageData;

  /*-----------*/

  int num_options = tag->getNumOptions();

  for (int i = 0; i < num_options; i++) {
    const CHtmlTagOption *option = tag->getOption(i);

    std::string option_name = option->getName();

    if      (CStrUtil::casecmp(option_name, "align") == 0) {
      if      (CStrUtil::casecmp(option->getValue(), "top"      ) == 0)
        imageData.align = CBrowserImageAlign::TOP;
      else if (CStrUtil::casecmp(option->getValue(), "middle"   ) == 0)
        imageData.align = CBrowserImageAlign::MIDDLE;
      else if (CStrUtil::casecmp(option->getValue(), "bottom"   ) == 0)
        imageData.align = CBrowserImageAlign::BOTTOM;
      else if (CStrUtil::casecmp(option->getValue(), "left"     ) == 0)
        imageData.align = CBrowserImageAlign::LEFT;
      else if (CStrUtil::casecmp(option->getValue(), "right"    ) == 0)
        imageData.align = CBrowserImageAlign::RIGHT;
      else if (CStrUtil::casecmp(option->getValue(), "texttop"  ) == 0)
        imageData.align = CBrowserImageAlign::TEXTTOP;
      else if (CStrUtil::casecmp(option->getValue(), "absmiddle") == 0)
        imageData.align = CBrowserImageAlign::ABSMIDDLE;
      else if (CStrUtil::casecmp(option->getValue(), "absbottom") == 0)
        imageData.align = CBrowserImageAlign::ABSBOTTOM;
      else
        window->displayError("Illegal Image Align %s\n", option->getValue().c_str());
    }
    else if (CStrUtil::casecmp(option_name, "alt") == 0) {
      imageData.alt = option->getValue();
    }
    else if (CStrUtil::casecmp(option_name, "border") == 0) {
      if (CStrUtil::isInteger(option->getValue()))
        imageData.border = CStrUtil::toInteger(option->getValue());
      else {
        window->displayError("Illegal Value for Border %s\n", option->getValue().c_str());
        imageData.border = 2;
      }
    }
    else if (CStrUtil::casecmp(option_name, "controls") == 0) {
    }
    else if (CStrUtil::casecmp(option_name, "dynsrc") == 0) {
    }
    else if (CStrUtil::casecmp(option_name, "height") == 0) {
      if (CStrUtil::isInteger(option->getValue()))
        imageData.height = CStrUtil::toInteger(option->getValue());
      else {
        window->displayError("Illegal Value for Height %s\n", option->getValue().c_str());
        imageData.height = -1;
      }
    }
    else if (CStrUtil::casecmp(option_name, "hspace") == 0) {
      if (CStrUtil::isInteger(option->getValue()))
        imageData.hspace = CStrUtil::toInteger(option->getValue());
      else {
        window->displayError("Illegal Value for HSpace %s\n", option->getValue().c_str());
        imageData.hspace = 2;
      }
    }
    else if (CStrUtil::casecmp(option_name, "ismap") == 0) {
      //ismap = true;

      std::string value = option->getValue();

      if (value[0] != '\0')
        window->displayError("No Value needed for IsMap\n");
    }
    else if (CStrUtil::casecmp(option_name, "loop") == 0) {
    }
    else if (CStrUtil::casecmp(option_name, "lowsrc") == 0) {
    }
    else if (CStrUtil::casecmp(option_name, "src") == 0) {
      imageData.src = option->getValue();
    }
    else if (CStrUtil::casecmp(option_name, "start") == 0) {
    }
    else if (CStrUtil::casecmp(option_name, "usemap") == 0) {
      imageData.usemap = option->getValue();
    }
    else if (CStrUtil::casecmp(option_name, "vspace") == 0) {
      if (CStrUtil::isInteger(option->getValue()))
        imageData.vspace = CStrUtil::toInteger(option->getValue());
      else {
        window->displayError("Illegal Value for VSpace %s\n", option->getValue().c_str());
        imageData.vspace = 2;
      }
    }
    else if (CStrUtil::casecmp(option_name, "width") == 0) {
      if (CStrUtil::isInteger(option->getValue()))
        imageData.width = CStrUtil::toInteger(option->getValue());
      else {
        window->displayError("Illegal Value for Width %s\n", option->getValue().c_str());
        imageData.width = -1;
      }
    }
    else
      window->displayError("Illegal Img Option Name '%s'\n", option_name.c_str());
  }

  if (imageData.src == "") {
    window->displayError("No Image Source Specified\n");
    return;
  }

  /*-----------*/

  window->addImage(imageData);
}

static void
CBrowserOutputInputTag(CBrowserWindow *window, CHtmlTag *tag)
{
  std::string align;
  bool        checked = false;
  int         maxlength = -1;
  std::string id;
  std::string classStr;
  std::string name;
  std::string placeholder;
  std::string onchange;
  std::string onclick;
  int         size = -1;
  std::string src;
  std::string type;
  std::string min;
  std::string max;
  std::string step;
  std::string value;

  /*-----------*/

  int num_options = tag->getNumOptions();

  for (int i = 0; i < num_options; i++) {
    const CHtmlTagOption *option = tag->getOption(i);

    std::string option_name = option->getName();

    if      (CStrUtil::casecmp(option_name, "align") == 0)
      align = option->getValue();
    else if (CStrUtil::casecmp(option_name, "checked") == 0)
      checked = true;
    else if (CStrUtil::casecmp(option_name, "maxlength") == 0) {
      if (CStrUtil::isInteger(option->getValue()))
        maxlength = CStrUtil::toInteger(option->getValue());
      else {
        window->displayError("Illegal Value '%s' for '%s'\n",
                             option->getValue().c_str(), option_name.c_str());
        maxlength = -1;
      }
    }
    else if (CStrUtil::casecmp(option_name, "id") == 0)
      id = option->getValue();
    else if (CStrUtil::casecmp(option_name, "class") == 0)
      classStr = option->getValue();
    else if (CStrUtil::casecmp(option_name, "name") == 0)
      name = option->getValue();
    else if (CStrUtil::casecmp(option_name, "placeholder") == 0)
      placeholder = option->getValue();
    else if (CStrUtil::casecmp(option_name, "onchange") == 0)
      onchange = option->getValue();
    else if (CStrUtil::casecmp(option_name, "onclick") == 0)
      onclick = option->getValue();
    else if (CStrUtil::casecmp(option_name, "size") == 0) {
      if (CStrUtil::isInteger(option->getValue()))
        size = CStrUtil::toInteger(option->getValue());
      else {
        window->displayError("Illegal Value '%s' for '%s'\n",
                             option->getValue().c_str(), option_name.c_str());
        size = -1;
      }
    }
    else if (CStrUtil::casecmp(option_name, "src") == 0)
      src = option->getValue();
    else if (CStrUtil::casecmp(option_name, "type") == 0)
      type = option->getValue();
    else if (CStrUtil::casecmp(option_name, "min") == 0)
      min = option->getValue();
    else if (CStrUtil::casecmp(option_name, "max") == 0)
      max = option->getValue();
    else if (CStrUtil::casecmp(option_name, "step") == 0)
      step = option->getValue();
    else if (CStrUtil::casecmp(option_name, "value") == 0)
      value = option->getValue();
    else
      window->displayError("Illegal '%s' Option Name '%s'\n",
                           tag->getName().c_str(), option_name.c_str());
  }

  if (type == "")
    type = "text";

  /*-----------*/

  if (onclick != "")
    window->getDocument()->formMgr()->setInputOnClick(onclick);

  if (onchange != "")
    window->getDocument()->formMgr()->setInputOnChange(onchange);

  CBrowserFormInput *input;

  if      (CStrUtil::casecmp(type, "button"  ) == 0)
    input = window->getDocument()->formMgr()->addButtonInput(name, value);
  else if (CStrUtil::casecmp(type, "checkbox") == 0)
    input = window->getDocument()->formMgr()->addCheckboxInput(name, value, checked);
  else if (CStrUtil::casecmp(type, "file"    ) == 0)
    input = window->getDocument()->formMgr()->addFileInput(name, value, size, maxlength);
  else if (CStrUtil::casecmp(type, "hidden"  ) == 0)
    input = window->getDocument()->formMgr()->addHiddenInput(name, value);
  else if (CStrUtil::casecmp(type, "image"   ) == 0)
    input = window->getDocument()->formMgr()->addImageInput(name, src, align);
  else if (CStrUtil::casecmp(type, "password") == 0)
    input = window->getDocument()->formMgr()->addPasswordInput(name, size, maxlength);
  else if (CStrUtil::casecmp(type, "radio"   ) == 0)
    input = window->getDocument()->formMgr()->addRadioInput(name, value, checked);
  else if (CStrUtil::casecmp(type, "range"   ) == 0)
    input = window->getDocument()->formMgr()->addRangeInput(name, value, min, max, step);
  else if (CStrUtil::casecmp(type, "reset"   ) == 0)
    input = window->getDocument()->formMgr()->addResetInput(name, value);
  else if (CStrUtil::casecmp(type, "submit"  ) == 0)
    input = window->getDocument()->formMgr()->addSubmitInput(name, value);
  else if (CStrUtil::casecmp(type, "text"    ) == 0)
    input = window->getDocument()->formMgr()->addTextInput(name, value, classStr, size,
                                                           maxlength, placeholder);
  else {
    window->displayError("Invalid Input Type '%s'\n", type.c_str());
    return;
  }

  window->addObject(id, input);
}

static void
CBrowserOutputKbdStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  window->startFontFace("courier");
}

static void
CBrowserOutputKbdEndTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  window->endFontFace();
}

static void
CBrowserOutputLiStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  for (int i = 0; i < num_options; i++) {
    const CHtmlTagOption *option = tag->getOption(i);

    std::string option_name = option->getName();

    if      (CStrUtil::casecmp(option_name, "type") == 0)
      current_list->setSymbol(option->getValue());
    else if (CStrUtil::casecmp(option_name, "value") == 0) {
      if (CStrUtil::isInteger(option->getValue())) {
        int item_num = CStrUtil::toInteger(option->getValue());

        current_list->setItemNum(item_num);
      }
      else
        window->displayError("Illegal Value for Value %s\n", option->getValue().c_str());

    }
    else
      window->displayError("Illegal Li Option Name '%s'\n", option_name.c_str());
  }

  /*-------------*/

  if (! current_list->getCompact())
    CBrowserOutputSkipLine(window);
  else
    CBrowserOutputNewLine(window);

  if      (current_list && current_list->getType() == CBrowserOutputListType::OL) {
    std::string text;

    if      (current_list->getSymbol() == "")
      text = CStrUtil::toString(current_list->getItemNum());
    else if (current_list->getSymbol() == "A") {
      text = CBrowserMisc::integerToAlphabetic(current_list->getItemNum());

      CStrUtil::toUpper(text);
    }
    else if (current_list->getSymbol() == "a") {
      text = CBrowserMisc::integerToAlphabetic(current_list->getItemNum());

      CStrUtil::toLower(text);
    }
    else if (current_list->getSymbol() == "I") {
      CRomanNumber roman(current_list->getItemNum());

      text = roman.getString();

      CStrUtil::toUpper(roman.getString());
    }
    else if (current_list->getSymbol() == "i") {
      CRomanNumber roman(current_list->getItemNum());

      text = roman.getString();

      CStrUtil::toLower(text);
    }
    else if (current_list->getSymbol() == "1")
      text = CStrUtil::toString(current_list->getItemNum());
    else
      text = CStrUtil::toString(current_list->getItemNum());

    window->addLabel(text, 4, CHALIGN_TYPE_RIGHT, textData.color);

    window->addLabel(". ", 2, CHALIGN_TYPE_LEFT, textData.color);
  }
  else if (current_list &&
           (current_list->getType() == CBrowserOutputListType::UL  ||
            current_list->getType() == CBrowserOutputListType::DIR ||
            current_list->getType() == CBrowserOutputListType::MENU)) {
    if      (current_list->getSymbol() == "") {
      int num = list_stack.size();

      num %= 4;

      CBrowserSymbolType type = CBrowserSymbolType::CIRCLE;

      if      (num == 0) type = CBrowserSymbolType::DISC;
      else if (num == 1) type = CBrowserSymbolType::CIRCLE;
      else if (num == 2) type = CBrowserSymbolType::BLOCK;
      else               type = CBrowserSymbolType::SQUARE;

      window->addSymbol(type);
    }
    else if (CStrUtil::casecmp(current_list->getSymbol(), "disc"  ) == 0)
      window->addSymbol(CBrowserSymbolType::DISC);
    else if (CStrUtil::casecmp(current_list->getSymbol(), "circle") == 0)
      window->addSymbol(CBrowserSymbolType::CIRCLE);
    else if (CStrUtil::casecmp(current_list->getSymbol(), "square") == 0)
      window->addSymbol(CBrowserSymbolType::SQUARE);
    else
      window->addSymbol(CBrowserSymbolType::DISC);

    window->addLabel(" ", 1, CHALIGN_TYPE_LEFT, textData.color);
  }

  current_list->setItemNum(current_list->getItemNum() + 1);
}

static void
CBrowserOutputLiEndTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());
}

static void
CBrowserOutputMenuStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  if (current_list)
    list_stack.push_back(current_list);

  current_list = new CBrowserOutputList(CBrowserOutputListType::MENU);

  /*-------------*/

  int num_options = tag->getNumOptions();

  for (int i = 0; i < num_options; i++) {
    const CHtmlTagOption *option = tag->getOption(i);

    std::string option_name = option->getName();

    if      (CStrUtil::casecmp(option_name, "compact") == 0) {
      current_list->setCompact(true);

      std::string value = option->getValue();

      if (value[0] != '\0')
        window->displayError("No Value needed for Compact\n");
    }
    else if (CStrUtil::casecmp(option_name, "type") == 0)
      current_list->setSymbol(option->getValue());
    else
      window->displayError("Illegal Menu Option Name '%s'\n", option_name.c_str());
  }

  /*-------------*/

  window->indentLeft(2);

  if (list_stack.empty() || ! current_list->getCompact())
    CBrowserOutputSkipLine(window);
  else
    CBrowserOutputNewLine(window);
}

static void
CBrowserOutputMenuEndTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  window->indentLeft(-2);

  if (list_stack.empty())
    CBrowserOutputSkipLine(window);
  else
    CBrowserOutputNewLine(window);

  /*-------------*/

  delete current_list;

  if (! list_stack.empty()) {
    current_list = list_stack[list_stack.size() - 1];

    list_stack.pop_back();
  }
  else
    current_list = nullptr;
}

static void
CBrowserOutputNobrStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  textData.breakup = false;
}

static void
CBrowserOutputNobrEndTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  textData.breakup = true;
}

static void
CBrowserOutputOlStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  if (current_list)
    list_stack.push_back(current_list);

  current_list = new CBrowserOutputList(CBrowserOutputListType::OL);

  /*-------------*/

  int num_options = tag->getNumOptions();

  for (int i = 0; i < num_options; i++) {
    const CHtmlTagOption *option = tag->getOption(i);

    std::string option_name = option->getName();

    if      (CStrUtil::casecmp(option_name, "compact") == 0) {
      current_list->setCompact(true);

      std::string value = option->getValue();

      if (value[0] != '\0')
        window->displayError("No Value needed for Compact\n");
    }
    else if (CStrUtil::casecmp(option_name, "start") == 0) {
      if (CStrUtil::isInteger(option->getValue())) {
        int item_num = CStrUtil::toInteger(option->getValue());

        current_list->setItemNum(item_num);
      }
      else {
        window->displayError("Illegal Value for Value %s\n", option->getValue().c_str());

        current_list->setItemNum(1);
      }
    }
    else if (CStrUtil::casecmp(option_name, "type") == 0)
      current_list->setSymbol(option->getValue());
    else
      window->displayError("Illegal Ol Option Name '%s'\n", option_name.c_str());
  }

  /*-------------*/

  window->indentLeft(2);

  if (list_stack.empty() || ! current_list->getCompact())
    CBrowserOutputSkipLine(window);
  else
    CBrowserOutputNewLine(window);
}

static void
CBrowserOutputOlEndTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  window->indentLeft(-2);

  if (list_stack.empty())
    CBrowserOutputSkipLine(window);
  else
    CBrowserOutputNewLine(window);

  /*-------------*/

  delete current_list;

  if (! list_stack.empty()) {
    current_list = list_stack[list_stack.size() - 1];

    list_stack.pop_back();
  }
  else
    current_list = nullptr;
}

static void
CBrowserOutputOptionStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  std::string value;

  if (in_option)
    CBrowserOutputOptionEndTag(window, nullptr);

  /*-------------*/

  bool selected = false;

  int num_options = tag->getNumOptions();

  for (int i = 0; i < num_options; i++) {
    const CHtmlTagOption *option = tag->getOption(i);

    std::string option_name = option->getName();

    if      (CStrUtil::casecmp(option_name, "selected") == 0)
      selected = true;
    else if (CStrUtil::casecmp(option_name, "value") == 0)
      value = option->getValue();
    else
      window->displayError("Illegal %s Option '%s'\n", tag->getName().c_str(),
                           option_name.c_str());
  }

  /*-------------*/

  in_option   = true;
  option_text = "";

  window->getDocument()->formMgr()->startOption(value, selected);
}

static void
CBrowserOutputOptionEndTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (tag && num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  /*-------------*/

  window->getDocument()->formMgr()->endOption(option_text);

  in_option   = false;
  option_text = "";
}

static void
CBrowserOutputPStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  CHAlignType align = CHALIGN_TYPE_LEFT;

  int num_options = tag->getNumOptions();

  for (int i = 0; i < num_options; i++) {
    const CHtmlTagOption *option = tag->getOption(i);

    std::string option_name = option->getName();

    if (CStrUtil::casecmp(option_name, "align") == 0) {
      if      (CStrUtil::casecmp(option->getValue(), "left") == 0)
        align = CHALIGN_TYPE_LEFT;
      else if (CStrUtil::casecmp(option->getValue(), "center") == 0)
        align = CHALIGN_TYPE_CENTER;
      else if (CStrUtil::casecmp(option->getValue(), "right") == 0)
        align = CHALIGN_TYPE_RIGHT;
      else
        window->displayError("Illegal Paragraph Alignment %s\n", option->getValue().c_str());
    }
    else
      window->displayError("Illegal P Option Name '%s'\n", option_name.c_str());
  }

  /*-----------*/

  CBrowserOutputSkipLine(window);

  window->setAlign(align, CVALIGN_TYPE_TOP);
}

static void
CBrowserOutputPEndTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  /*-----------*/

  CBrowserOutputNewLine(window);

  window->setAlign(CHALIGN_TYPE_LEFT, CVALIGN_TYPE_TOP);
}

static void
CBrowserOutputPreStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int width = -1;

  int num_options = tag->getNumOptions();

  for (int i = 0; i < num_options; i++) {
    const CHtmlTagOption *option = tag->getOption(i);

    std::string option_name = option->getName();

    if (CStrUtil::casecmp(option_name, "width") == 0) {
      if (CStrUtil::isInteger(option->getValue()))
        width = CStrUtil::toInteger(option->getValue());
      else {
        window->displayError("Illegal Value for Width %s\n", option->getValue().c_str());
        width = -1;
      }
    }
    else
      window->displayError("Illegal Pre Option Name '%s'\n", option_name.c_str());
  }

  if (CBrowserMainInst->getDebug())
    std::cerr << width << std::endl;

  /*-----------*/

  CBrowserOutputSkipLine(window);

  window->startFontFace("courier");

  textData.breakup = false;
  textData.format  = false;
}

static void
CBrowserOutputPreEndTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  /*-----------*/

  textData.breakup = true;
  textData.format  = true;

  window->endFontFace();

  CBrowserOutputSkipLine(window);
}

static void
CBrowserOutputSampStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  window->startFontFace("courier");
}

static void
CBrowserOutputSampEndTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  window->endFontFace();
}

static void
CBrowserOutputScriptStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  std::string language;
  std::string src;
  std::string type;

  int num_options = tag->getNumOptions();

  for (int i = 0; i < num_options; i++) {
    const CHtmlTagOption *option = tag->getOption(i);

    std::string option_name = option->getName();

    if      (CStrUtil::casecmp(option_name, "language") == 0)
      language = option->getValue();
    else if (CStrUtil::casecmp(option_name, "src") == 0)
      src = option->getValue();
    else if (CStrUtil::casecmp(option_name, "type") == 0)
      type = option->getValue();
    else
      window->displayError("Illegal %s Option '%s'\n",
                           tag->getName().c_str(), option_name.c_str());
  }

  if (src != "") {
    if (CStrUtil::casecmp(language, "ceil") == 0)
      CBrowserCeilInst->runScriptFile(window, src);
  }
  else {
    scriptData.active   = true;
    scriptData.language = language;
    scriptData.type     = type;
    scriptData.text     = "";
  }
}

static void
CBrowserOutputScriptEndTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  if (! scriptData.active)
    return;

  scriptData.active = false;

  if (scriptData.text != "") {
    if      (CStrUtil::casecmp(scriptData.language, "ceil") == 0) {
      CBrowserCeilInst->runScript(window, scriptData.text);
    }
    else if (CStrUtil::casecmp(scriptData.type, "text/javascript") == 0) {
      //CBrowserJSInst->runScript(window, scriptData.text);

      window->addScript(scriptData.text);
    }
  }

  scriptData.reset();
}

static void
CBrowserOutputSelectStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  bool        multiple = false;
  std::string id;
  std::string name;
  int         size = 1;
  std::string onchange;

  int num_options = tag->getNumOptions();

  for (int i = 0; i < num_options; i++) {
    const CHtmlTagOption *option = tag->getOption(i);

    std::string option_name = option->getName();

    if      (CStrUtil::casecmp(option_name, "multiple") == 0)
      multiple = true;
    else if (CStrUtil::casecmp(option_name, "id") == 0)
      id = option->getValue();
    else if (CStrUtil::casecmp(option_name, "name") == 0)
      name = option->getValue();
    else if (CStrUtil::casecmp(option_name, "size") == 0) {
      if (CStrUtil::isInteger(option->getValue()))
        size = CStrUtil::toInteger(option->getValue());
      else {
        window->displayError("Illegal Value '%s' for '%s'\n",
                             option->getValue().c_str(), option_name.c_str());
        size = 1;
      }
    }
    else if (CStrUtil::casecmp(option_name, "onchange") == 0)
      onchange = option->getValue();
    else
      window->displayError("Illegal '%s' Option '%s'\n",
                           tag->getName().c_str(), option_name.c_str());
  }

  if (onchange != "")
    window->getDocument()->formMgr()->setInputOnChange(onchange);

  CBrowserFormSelect *select =
    window->getDocument()->formMgr()->startSelect(name, size, multiple);

  window->addObject(id, select);
}

static void
CBrowserOutputSelectEndTag(CBrowserWindow *window, CHtmlTag *tag)
{
  if (in_option)
    CBrowserOutputOptionEndTag(window, nullptr);

  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  window->getDocument()->formMgr()->endSelect();
}

static void
CBrowserOutputSmallStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  window->decreaseFontSize();
}

static void
CBrowserOutputSmallEndTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  window->increaseFontSize();
}

static void
CBrowserOutputStrikeStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  textData.strike = true;
}

static void
CBrowserOutputStrikeEndTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  textData.strike = false;
}

static void
CBrowserOutputStrongStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  window->startBold();
}

static void
CBrowserOutputStrongEndTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  window->endBold();
}

static void
CBrowserOutputSubStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  textData.place = CBrowserTextPlaceType::SUBSCRIPT;
}

static void
CBrowserOutputSubEndTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  textData.place = CBrowserTextPlaceType::NORMAL;
}

static void
CBrowserOutputSupStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  textData.place = CBrowserTextPlaceType::SUPERSCRIPT;
}

static void
CBrowserOutputSupEndTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  textData.place = CBrowserTextPlaceType::NORMAL;
}

static void
CBrowserOutputTableStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserTableData tableData;

  tableData.hspace       = 0;
  tableData.vspace       = 0;
  tableData.cell_padding = 1;
  tableData.cell_spacing = 2;

  int num_options = tag->getNumOptions();

  for (int i = 0; i < num_options; i++) {
    const CHtmlTagOption *option = tag->getOption(i);

    std::string option_name = option->getName();

    if      (CStrUtil::casecmp(option_name, "align") == 0) {
      if      (CStrUtil::casecmp(option->getValue(), "left") == 0)
        tableData.halign = CHALIGN_TYPE_LEFT;
      else if (CStrUtil::casecmp(option->getValue(), "right") == 0)
        tableData.halign = CHALIGN_TYPE_RIGHT;
      else
        window->displayError("Illegal Table Alignment %s\n", option->getValue().c_str());
    }
    else if (CStrUtil::casecmp(option_name, "bgcolor") == 0) {
      if (option->getValue() != "")
        tableData.background_color = CRGBName::toRGBA(option->getValue());
    }
    else if (CStrUtil::casecmp(option_name, "border") == 0) {
      std::string value = option->getValue();

      if (value != "") {
        if (CStrUtil::isInteger(option->getValue()))
          tableData.border = (CStrUtil::toInteger(option->getValue()) != 0);
        else {
          window->displayError("Illegal Value for border %s\n", option->getValue().c_str());

          tableData.border = false;
        }
      }
      else
        tableData.border = true;
    }
    else if (CStrUtil::casecmp(option_name, "bordercolor") == 0) {
      if (option->getValue() != "")
        tableData.border_color = CRGBName::toRGBA(option->getValue());
    }
    else if (CStrUtil::casecmp(option_name, "bordercolorlight") == 0) {
      if (option->getValue() != "")
        tableData.border_color_light = CRGBName::toRGBA(option->getValue());
    }
    else if (CStrUtil::casecmp(option_name, "bordercolordark") == 0) {
      if (option->getValue() != "")
        tableData.border_color_dark = CRGBName::toRGBA(option->getValue());
    }
    else if (CStrUtil::casecmp(option_name, "cellpadding") == 0) {
      if (CStrUtil::isInteger(option->getValue()))
        tableData.cell_padding = CStrUtil::toInteger(option->getValue());
      else {
        window->displayError("Illegal Value for cellpadding %s\n", option->getValue().c_str());

        tableData.cell_padding = 1;
      }
    }
    else if (CStrUtil::casecmp(option_name, "cellspacing") == 0) {
      if (CStrUtil::isInteger(option->getValue()))
        tableData.cell_spacing = CStrUtil::toInteger(option->getValue());
      else {
        window->displayError("Illegal Value for cellspacing %s\n", option->getValue().c_str());

        tableData.cell_spacing = 2;
      }
    }
    else if (CStrUtil::casecmp(option_name, "hspace") == 0) {
      if (CStrUtil::isInteger(option->getValue()))
        tableData.hspace = CStrUtil::toInteger(option->getValue());
      else {
        window->displayError("Illegal Value for hspace %s\n", option->getValue().c_str());

        tableData.hspace = 0;
      }
    }
    else if (CStrUtil::casecmp(option_name, "valign") == 0) {
      if      (CStrUtil::casecmp(option->getValue(), "top") == 0)
        tableData.valign = CVALIGN_TYPE_TOP;
      else if (CStrUtil::casecmp(option->getValue(), "bottom") == 0)
        tableData.valign = CVALIGN_TYPE_BOTTOM;
      else
        window->displayError("Illegal Table vertical alignment %s\n",
                             option->getValue().c_str());
    }
    else if (CStrUtil::casecmp(option_name, "vspace") == 0) {
      if (CStrUtil::isInteger(option->getValue()))
        tableData.vspace = CStrUtil::toInteger(option->getValue());
      else {
        window->displayError("Illegal Value for vspace %s\n", option->getValue().c_str());

        tableData.vspace = 0;
      }
    }
    else if (CStrUtil::casecmp(option_name, "width") == 0) {
      int len = option->getValue().size();

      std::string value = option->getValue();

      if (value[len - 1] == '%') {
        std::string value;

        tableData.width_unit = UNIT_PERCENT;

        value = option->getValue().substr(0, len - 1);

        if (CStrUtil::isInteger(option->getValue()))
          tableData.width = CStrUtil::toInteger(value);
        else {
          window->displayError("Illegal Value for Width %s\n", option->getValue().c_str());
          tableData.width = -1;
        }
      }
      else {
        tableData.width_unit = UNIT_PIXEL;

        if (CStrUtil::isInteger(option->getValue()))
          tableData.width = CStrUtil::toInteger(option->getValue());
        else {
          window->displayError("Illegal Value for Width %s\n", option->getValue().c_str());
          tableData.width = -1;
        }
      }
    }
    else
      window->displayError("Illegal Table Option Name '%s'\n", option_name.c_str());
  }

  //---

  window->tableMgr()->startTable(tableData);
}

static void
CBrowserOutputTableEndTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  //---

  window->tableMgr()->endTable();
}

static void
CBrowserOutputTdStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserTableCellData cellData;

  cellData.type       = HEADER_CELL;
  cellData.width      = 0;
  cellData.width_unit = UNIT_NONE;
  cellData.colspan    = 1;
  cellData.rowspan    = 1;

  int num_options = tag->getNumOptions();

  for (int i = 0; i < num_options; i++) {
    const CHtmlTagOption *option = tag->getOption(i);

    std::string option_name = option->getName();

    if      (CStrUtil::casecmp(option_name, "align") == 0) {
      if      (CStrUtil::casecmp(option->getValue(), "left") == 0)
        cellData.halign = CHALIGN_TYPE_LEFT;
      else if (CStrUtil::casecmp(option->getValue(), "center") == 0)
        cellData.halign = CHALIGN_TYPE_CENTER;
      else if (CStrUtil::casecmp(option->getValue(), "right") == 0)
        cellData.halign = CHALIGN_TYPE_RIGHT;
      else
        window->displayError("Illegal Table Data Alignment %s\n", option->getValue().c_str());
    }
    else if (CStrUtil::casecmp(option_name, "bgcolor") == 0) {
      if (option->getValue() != "")
        cellData.background_color = CRGBName::toRGBA(option->getValue());
    }
    else if (CStrUtil::casecmp(option_name, "bordercolor") == 0) {
      if (option->getValue() != "")
        cellData.border_color = CRGBName::toRGBA(option->getValue());
    }
    else if (CStrUtil::casecmp(option_name, "bordercolorlight") == 0) {
      if (option->getValue() != "")
        cellData.border_color_light = CRGBName::toRGBA(option->getValue());
    }
    else if (CStrUtil::casecmp(option_name, "bordercolordark") == 0) {
      if (option->getValue() != "")
        cellData.border_color_dark = CRGBName::toRGBA(option->getValue());
    }
    else if (CStrUtil::casecmp(option_name, "colspan") == 0) {
      if (CStrUtil::isInteger(option->getValue()))
        cellData.colspan = CStrUtil::toInteger(option->getValue());
      else {
        window->displayError("Illegal Value for colspan %s\n", option->getValue().c_str());
        cellData.colspan = 1;
      }
    }
    else if (CStrUtil::casecmp(option_name, "id") == 0) {
      cellData.id = option->getValue();
    }
    else if (CStrUtil::casecmp(option_name, "nowrap") == 0) {
      cellData.wrap = false;

      std::string value = option->getValue();

      if (value[0] != '\0')
        window->displayError("No Value needed for nowrap\n");
    }
    else if (CStrUtil::casecmp(option_name, "rowspan") == 0) {
      if (CStrUtil::isInteger(option->getValue()))
        cellData.rowspan = CStrUtil::toInteger(option->getValue());
      else {
        window->displayError("Illegal Value for rowspan %s\n", option->getValue().c_str());
        cellData.rowspan = 1;
      }
    }
    else if (CStrUtil::casecmp(option_name, "valign") == 0) {
      if      (CStrUtil::casecmp(option->getValue(), "top") == 0)
        cellData.valign = CVALIGN_TYPE_TOP;
      else if (CStrUtil::casecmp(option->getValue(), "center") == 0)
        cellData.valign = CVALIGN_TYPE_CENTER;
      else if (CStrUtil::casecmp(option->getValue(), "bottom") == 0)
        cellData.valign = CVALIGN_TYPE_BOTTOM;
      else if (CStrUtil::casecmp(option->getValue(), "baseline") == 0)
        cellData.valign = CVALIGN_TYPE_BASELINE;
      else
        window->displayError("Illegal %s Vertical Alignment %s\n", "Table Data",
                             option->getValue().c_str());
    }
    else if (CStrUtil::casecmp(option_name, "width") == 0) {
      int len = option->getValue().size();

      std::string value = option->getValue();

      if (value[len - 1] == '%') {
        cellData.width_unit = UNIT_PERCENT;

        std::string value = option->getValue().substr(len - 1);

        if (CStrUtil::isInteger(option->getValue()))
          cellData.width = CStrUtil::toInteger(value);
        else {
          window->displayError("Illegal Value for Width %s\n", option->getValue().c_str());
          cellData.width = -1;
        }
      }
      else {
        cellData.width_unit = UNIT_PIXEL;

        if (CStrUtil::isInteger(option->getValue()))
          cellData.width = CStrUtil::toInteger(option->getValue());
        else {
          window->displayError("Illegal Value for Width %s\n", option->getValue().c_str());
          cellData.width = -1;
        }
      }
    }
    else
      window->displayError("Illegal 'td' Option Name '%s'\n", option_name.c_str());
  }

  //---

  window->tableMgr()->startTableCell(cellData);
}

static void
CBrowserOutputTdEndTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  //---

  window->tableMgr()->endTableCell();
}

static void
CBrowserOutputTextareaStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  std::string id;
  std::string name;
  std::string wrap;

  /*-------------*/

  int cols = 40;
  int rows = 5;

  int num_options = tag->getNumOptions();

  for (int i = 0; i < num_options; i++) {
    const CHtmlTagOption *option = tag->getOption(i);

    std::string option_name = option->getName();

    if      (CStrUtil::casecmp(option_name, "cols") == 0) {
      if (CStrUtil::isInteger(option->getValue()))
        cols = CStrUtil::toInteger(option->getValue());
      else {
        window->displayError("Illegal Value for cols %s\n", option->getValue().c_str());
        cols = 40;
      }
    }
    else if (CStrUtil::casecmp(option_name, "id") == 0) {
      id = option->getValue();
    }
    else if (CStrUtil::casecmp(option_name, "name") == 0) {
      name = option->getValue();
    }
    else if (CStrUtil::casecmp(option_name, "rows") == 0) {
      if (CStrUtil::isInteger(option->getValue()))
        rows = CStrUtil::toInteger(option->getValue());
      else {
        window->displayError("Illegal Value for rows %s\n", option->getValue().c_str());
        rows = 5;
      }
    }
    else if (CStrUtil::casecmp(option_name, "wrap") == 0)
      wrap = option->getValue();
    else
      window->displayError("Illegal 'textarea' Option Name '%s'\n", option_name.c_str());
  }

  /*-------------*/

  window->getDocument()->formMgr()->startTextarea(name, rows, cols, wrap);

  /*-------------*/

  in_textarea   = true;
  textarea_text = "";
}

static void
CBrowserOutputTextareaEndTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  /*-------------*/

  if (textarea_text != "" && textarea_text[0] == '\n')
    window->getDocument()->formMgr()->endTextarea(textarea_text.substr(1));
  else
    window->getDocument()->formMgr()->endTextarea(textarea_text);

  /*-------------*/

  in_textarea   = false;
  textarea_text = "";
}

static void
CBrowserOutputThStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserTableCellData cellData;

  cellData.type       = HEADER_CELL;
  cellData.width      = 0;
  cellData.width_unit = UNIT_NONE;
  cellData.colspan    = 1;
  cellData.rowspan    = 1;

  int num_options = tag->getNumOptions();

  for (int i = 0; i < num_options; i++) {
    const CHtmlTagOption *option = tag->getOption(i);

    std::string option_name = option->getName();

    if      (CStrUtil::casecmp(option_name, "align") == 0) {
      if      (CStrUtil::casecmp(option->getValue(), "left") == 0)
        cellData.halign = CHALIGN_TYPE_LEFT;
      else if (CStrUtil::casecmp(option->getValue(), "center") == 0)
        cellData.halign = CHALIGN_TYPE_CENTER;
      else if (CStrUtil::casecmp(option->getValue(), "right") == 0)
        cellData.halign = CHALIGN_TYPE_RIGHT;
      else
        window->displayError("Illegal Table Header Alignment %s\n",
                             option->getValue().c_str());
    }
    else if (CStrUtil::casecmp(option_name, "bgcolor") == 0) {
      if (option->getValue() != "")
        cellData.background_color = CRGBName::toRGBA(option->getValue());
    }
    else if (CStrUtil::casecmp(option_name, "bordercolor") == 0) {
      if (option->getValue() != "")
        cellData.border_color = CRGBName::toRGBA(option->getValue());
    }
    else if (CStrUtil::casecmp(option_name, "bordercolorlight") == 0) {
      if (option->getValue() != "")
        cellData.border_color_light = CRGBName::toRGBA(option->getValue());
    }
    else if (CStrUtil::casecmp(option_name, "bordercolordark") == 0) {
      if (option->getValue() != "")
        cellData.border_color_dark = CRGBName::toRGBA(option->getValue());
    }
    else if (CStrUtil::casecmp(option_name, "colspan") == 0) {
      if (CStrUtil::isInteger(option->getValue()))
        cellData.colspan = CStrUtil::toInteger(option->getValue());
      else {
        window->displayError("Illegal Value for colspan %s\n", option->getValue().c_str());
        cellData.colspan = 1;
      }
    }
    else if (CStrUtil::casecmp(option_name, "nowrap") == 0) {
      cellData.wrap = false;

      std::string value = option->getValue();

      if (value[0] != '\0')
        window->displayError("No Value needed for nowrap\n");
    }
    else if (CStrUtil::casecmp(option_name, "rowspan") == 0) {
      if (CStrUtil::isInteger(option->getValue()))
        cellData.rowspan = CStrUtil::toInteger(option->getValue());
      else {
        window->displayError("Illegal Value for rowspan %s\n", option->getValue().c_str());
        cellData.rowspan = 1;
      }
    }
    else if (CStrUtil::casecmp(option_name, "valign") == 0) {
      if      (CStrUtil::casecmp(option->getValue(), "top") == 0)
        cellData.valign = CVALIGN_TYPE_TOP;
      else if (CStrUtil::casecmp(option->getValue(), "center") == 0)
        cellData.valign = CVALIGN_TYPE_CENTER;
      else if (CStrUtil::casecmp(option->getValue(), "bottom") == 0)
        cellData.valign = CVALIGN_TYPE_BOTTOM;
      else if (CStrUtil::casecmp(option->getValue(), "baseline") == 0)
        cellData.valign = CVALIGN_TYPE_BASELINE;
      else
        window->displayError("Illegal %s Vertical Alignment %s\n",
                             "Table Header", option->getValue().c_str());
    }
    else if (CStrUtil::casecmp(option_name, "width") == 0) {
      int len = option->getValue().size();

      std::string value = option->getValue();

      if (value[len - 1] == '%') {
        std::string value;

        cellData.width_unit = UNIT_PERCENT;

        value = option->getValue().substr(0, len - 1);

        if (CStrUtil::isInteger(option->getValue()))
          cellData.width = CStrUtil::toInteger(value);
        else {
          window->displayError("Illegal Value for Width %s\n", option->getValue().c_str());
          cellData.width = -1;
        }
      }
      else {
        cellData.width_unit = UNIT_PIXEL;

        if (CStrUtil::isInteger(option->getValue()))
          cellData.width = CStrUtil::toInteger(option->getValue());
        else {
          window->displayError("Illegal Value for Width %s\n", option->getValue().c_str());
          cellData.width = -1;
        }
      }
    }
    else
      window->displayError("Illegal Th Option Name '%s'\n", option_name.c_str());
  }

  //---

  window->tableMgr()->startTableCell(cellData);
}

static void
CBrowserOutputThEndTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  //---

  window->tableMgr()->endTableCell();
}

static void
CBrowserOutputTitleStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  in_title = true;
}

static void
CBrowserOutputTitleEndTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  in_title = false;
}

static void
CBrowserOutputTrStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserTableRowData rowData;

  int num_options = tag->getNumOptions();

  for (int i = 0; i < num_options; i++) {
    const CHtmlTagOption *option = tag->getOption(i);

    std::string option_name = option->getName();

    if      (CStrUtil::casecmp(option_name, "align") == 0) {
      if      (CStrUtil::casecmp(option->getValue(), "left") == 0)
        rowData.halign = CHALIGN_TYPE_LEFT;
      else if (CStrUtil::casecmp(option->getValue(), "center") == 0)
        rowData.halign = CHALIGN_TYPE_CENTER;
      else if (CStrUtil::casecmp(option->getValue(), "right") == 0)
        rowData.halign = CHALIGN_TYPE_RIGHT;
      else
        window->displayError("Illegal Table Row Alignment %s\n", option->getValue().c_str());
    }
    else if (CStrUtil::casecmp(option_name, "bgcolor") == 0) {
      if (option->getValue() != "")
        rowData.background_color = CRGBName::toRGBA(option->getValue());
    }
    else if (CStrUtil::casecmp(option_name, "bordercolor") == 0) {
      if (option->getValue() != "")
        rowData.border_color = CRGBName::toRGBA(option->getValue());
    }
    else if (CStrUtil::casecmp(option_name, "bordercolorlight") == 0) {
      if (option->getValue() != "")
        rowData.border_color_light = CRGBName::toRGBA(option->getValue());
    }
    else if (CStrUtil::casecmp(option_name, "bordercolordark") == 0) {
      if (option->getValue() != "")
        rowData.border_color_dark = CRGBName::toRGBA(option->getValue());
    }
    else if (CStrUtil::casecmp(option_name, "valign") == 0) {
      if      (CStrUtil::casecmp(option->getValue(), "top") == 0)
        rowData.valign = CVALIGN_TYPE_TOP;
      else if (CStrUtil::casecmp(option->getValue(), "center") == 0)
        rowData.valign = CVALIGN_TYPE_CENTER;
      else if (CStrUtil::casecmp(option->getValue(), "bottom") == 0)
        rowData.valign = CVALIGN_TYPE_BOTTOM;
      else if (CStrUtil::casecmp(option->getValue(), "baseline") == 0)
        rowData.valign = CVALIGN_TYPE_BASELINE;
      else
        window->displayError("Illegal %s Vertical Alignment %s\n",
                             "Table Row", option->getValue().c_str());
    }
    else
      window->displayError("Illegal Tr Option Name '%s'\n", option_name.c_str());
  }

  //---

  window->tableMgr()->startTableRow(rowData);
}

static void
CBrowserOutputTrEndTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  //---

  window->tableMgr()->endTableRow();
}

static void
CBrowserOutputTtStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  window->startFontFace("courier");
}

static void
CBrowserOutputTtEndTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  window->endFontFace();
}

static void
CBrowserOutputUStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  textData.underline = true;
}

static void
CBrowserOutputUEndTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  textData.underline = false;
}

static void
CBrowserOutputUlStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  if (current_list)
    list_stack.push_back(current_list);

  current_list = new CBrowserOutputList(CBrowserOutputListType::UL);

  /*-------------*/

  int num_options = tag->getNumOptions();

  for (int i = 0; i < num_options; i++) {
    const CHtmlTagOption *option = tag->getOption(i);

    std::string option_name = option->getName();

    if      (CStrUtil::casecmp(option_name, "compact") == 0) {
      current_list->setCompact(true);

      std::string value = option->getValue();

      if (value[0] != '\0')
        window->displayError("No Value needed for Compact\n");
    }
    else if (CStrUtil::casecmp(option_name, "type") == 0)
      current_list->setSymbol(option->getValue());
    else
      window->displayError("Illegal Ul Option Name '%s'\n", option_name.c_str());
  }

  /*-------------*/

  window->indentLeft(2);

  if (list_stack.empty() || ! current_list->getCompact())
    CBrowserOutputSkipLine(window);
  else
    CBrowserOutputNewLine(window);
}

static void
CBrowserOutputUlEndTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  window->indentLeft(-2);

  if (list_stack.empty())
    CBrowserOutputSkipLine(window);
  else
    CBrowserOutputNewLine(window);

  /*-------------*/

  delete current_list;

  if (! list_stack.empty()) {
    current_list = list_stack[list_stack.size() - 1];

    list_stack.pop_back();
  }
  else
    current_list = nullptr;
}

static void
CBrowserOutputVarStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  window->startItalic();
}

static void
CBrowserOutputVarEndTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  window->endItalic();
}

static void
CBrowserOutputWbrTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());
}

static void
CBrowserOutputXmpStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  CBrowserOutputSkipLine(window);

  window->startFontFace("courier");

  textData.breakup = false;
  textData.format  = false;
}

static void
CBrowserOutputXmpEndTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  textData.breakup = true;
  textData.format  = true;

  window->endFontFace();

  CBrowserOutputSkipLine(window);
}

static void
CBrowserOutputSkipStartTag(CBrowserWindow *, CHtmlTag *)
{
}

static void
CBrowserOutputSkipEndTag(CBrowserWindow *, CHtmlTag *)
{
}

static void
CBrowserOutputNewLine(CBrowserWindow *window)
{
  window->newLine();
}

static void
CBrowserOutputSkipLine(CBrowserWindow *window)
{
  window->skipLine();
}
