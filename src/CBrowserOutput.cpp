#include <CBrowserOutput.h>
#include <CBrowserOutputList.h>
#include <CBrowserAddress.h>
#include <CBrowserBreak.h>
#include <CBrowserWindow.h>
#include <CBrowserText.h>
#include <CBrowserLabel.h>
#include <CBrowserDocument.h>
#include <CBrowserLink.h>
#include <CBrowserTable.h>
#include <CBrowserForm.h>
#include <CBrowserRule.h>
#include <CBrowserHeader.h>
#include <CBrowserImage.h>
#include <CBrowserCanvas.h>
#include <CBrowserMisc.h>
#include <CBrowserStyle.h>
#include <CBrowserSymbol.h>
#include <CBrowserMain.h>
#include <CBrowserCeil.h>
#include <CBrowserJS.h>
#include <CHtmlLib.h>
#include <CRomanNumber.h>
#include <CRGBName.h>

enum class CBrowserOutputFontFlag {
  NONE  = 0,
  FACE  = (1<<0),
  COLOR = (1<<1),
  SIZE  = (1<<2)
};

typedef CBrowserObject *(*CBrowserStartOutputProc)
                           (CBrowserWindow *window, CHtmlTag *tag);
typedef void            (*CBrowserEndOutputProc)
                           (CBrowserWindow *window, CHtmlTag *tag);

//------

class CBrowserOutputData {
 public:
  CHtmlTagId              id;
  CBrowserStartOutputProc startProc;
  CBrowserEndOutputProc   endProc;
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
  std::string language = "";
  std::string type     = "";
  std::string text     = "";

  void reset() {
    language = "";
    type     = "";
    text     = "";
  }
};

static CBrowserScriptData scriptData;
static CBrowserTextData   textData;

std::vector<CBrowserOutputFont *> CBrowserOutputFont::font_stack_;

std::vector<CBrowserBreak *> breakers;

//------

static CBrowserObject *CBrowserOutputAStartTag         (CBrowserWindow *, CHtmlTag *);
static void            CBrowserOutputAEndTag           (CBrowserWindow *, CHtmlTag *);
static CBrowserObject *CBrowserOutputAddressStartTag   (CBrowserWindow *, CHtmlTag *);
static void            CBrowserOutputAddressEndTag     (CBrowserWindow *, CHtmlTag *);
static CBrowserObject *CBrowserOutputBStartTag         (CBrowserWindow *, CHtmlTag *);
static void            CBrowserOutputBEndTag           (CBrowserWindow *, CHtmlTag *);
static CBrowserObject *CBrowserOutputBasefontStartTag  (CBrowserWindow *, CHtmlTag *);
static void            CBrowserOutputBasefontEndTag    (CBrowserWindow *, CHtmlTag *);
static CBrowserObject *CBrowserOutputBigStartTag       (CBrowserWindow *, CHtmlTag *);
static void            CBrowserOutputBigEndTag         (CBrowserWindow *, CHtmlTag *);
static CBrowserObject *CBrowserOutputBlinkStartTag     (CBrowserWindow *, CHtmlTag *);
static void            CBrowserOutputBlinkEndTag       (CBrowserWindow *, CHtmlTag *);
static CBrowserObject *CBrowserOutputBlockquoteStartTag(CBrowserWindow *, CHtmlTag *);
static void            CBrowserOutputBlockquoteEndTag  (CBrowserWindow *, CHtmlTag *);
static CBrowserObject *CBrowserOutputBodyStartTag      (CBrowserWindow *, CHtmlTag *);
static void            CBrowserOutputBodyEndTag        (CBrowserWindow *, CHtmlTag *);
static CBrowserObject *CBrowserOutputBrTag             (CBrowserWindow *, CHtmlTag *);
static CBrowserObject *CBrowserOutputCanvasStartTag    (CBrowserWindow *, CHtmlTag *);
static void            CBrowserOutputCanvasEndTag      (CBrowserWindow *, CHtmlTag *);
static CBrowserObject *CBrowserOutputCaptionStartTag   (CBrowserWindow *, CHtmlTag *);
static void            CBrowserOutputCaptionEndTag     (CBrowserWindow *, CHtmlTag *);
static CBrowserObject *CBrowserOutputCenterStartTag    (CBrowserWindow *, CHtmlTag *);
static void            CBrowserOutputCenterEndTag      (CBrowserWindow *, CHtmlTag *);
static CBrowserObject *CBrowserOutputCiteStartTag      (CBrowserWindow *, CHtmlTag *);
static void            CBrowserOutputCiteEndTag        (CBrowserWindow *, CHtmlTag *);
static CBrowserObject *CBrowserOutputCodeStartTag      (CBrowserWindow *, CHtmlTag *);
static void            CBrowserOutputCodeEndTag        (CBrowserWindow *, CHtmlTag *);
static CBrowserObject *CBrowserOutputDdStartTag        (CBrowserWindow *, CHtmlTag *);
static void            CBrowserOutputDdEndTag          (CBrowserWindow *, CHtmlTag *);
static CBrowserObject *CBrowserOutputDfnStartTag       (CBrowserWindow *, CHtmlTag *);
static void            CBrowserOutputDfnEndTag         (CBrowserWindow *, CHtmlTag *);
static CBrowserObject *CBrowserOutputDirStartTag       (CBrowserWindow *, CHtmlTag *);
static void            CBrowserOutputDirEndTag         (CBrowserWindow *, CHtmlTag *);
static CBrowserObject *CBrowserOutputDivStartTag       (CBrowserWindow *, CHtmlTag *);
static void            CBrowserOutputDivEndTag         (CBrowserWindow *, CHtmlTag *);
static CBrowserObject *CBrowserOutputDlStartTag        (CBrowserWindow *, CHtmlTag *);
static void            CBrowserOutputDlEndTag          (CBrowserWindow *, CHtmlTag *);
static CBrowserObject *CBrowserOutputDtStartTag        (CBrowserWindow *, CHtmlTag *);
static void            CBrowserOutputDtEndTag          (CBrowserWindow *, CHtmlTag *);
static CBrowserObject *CBrowserOutputEmStartTag        (CBrowserWindow *, CHtmlTag *);
static void            CBrowserOutputEmEndTag          (CBrowserWindow *, CHtmlTag *);
static CBrowserObject *CBrowserOutputFontStartTag      (CBrowserWindow *, CHtmlTag *);
static void            CBrowserOutputFontEndTag        (CBrowserWindow *, CHtmlTag *);
static CBrowserObject *CBrowserOutputFormStartTag      (CBrowserWindow *, CHtmlTag *);
static void            CBrowserOutputFormEndTag        (CBrowserWindow *, CHtmlTag *);
static CBrowserObject *CBrowserOutputH1StartTag        (CBrowserWindow *, CHtmlTag *);
static void            CBrowserOutputH1EndTag          (CBrowserWindow *, CHtmlTag *);
static CBrowserObject *CBrowserOutputH2StartTag        (CBrowserWindow *, CHtmlTag *);
static void            CBrowserOutputH2EndTag          (CBrowserWindow *, CHtmlTag *);
static CBrowserObject *CBrowserOutputH3StartTag        (CBrowserWindow *, CHtmlTag *);
static void            CBrowserOutputH3EndTag          (CBrowserWindow *, CHtmlTag *);
static CBrowserObject *CBrowserOutputH4StartTag        (CBrowserWindow *, CHtmlTag *);
static void            CBrowserOutputH4EndTag          (CBrowserWindow *, CHtmlTag *);
static CBrowserObject *CBrowserOutputH5StartTag        (CBrowserWindow *, CHtmlTag *);
static void            CBrowserOutputH5EndTag          (CBrowserWindow *, CHtmlTag *);
static CBrowserObject *CBrowserOutputH6StartTag        (CBrowserWindow *, CHtmlTag *);
static void            CBrowserOutputH6EndTag          (CBrowserWindow *, CHtmlTag *);
static CBrowserObject *CBrowserOutputHeadStartTag      (CBrowserWindow *, CHtmlTag *);
static void            CBrowserOutputHeadEndTag        (CBrowserWindow *, CHtmlTag *);
static CBrowserObject *CBrowserOutputHrTag             (CBrowserWindow *, CHtmlTag *);
static CBrowserObject *CBrowserOutputHtmlStartTag      (CBrowserWindow *, CHtmlTag *);
static void            CBrowserOutputHtmlEndTag        (CBrowserWindow *, CHtmlTag *);
static CBrowserObject *CBrowserOutputIStartTag         (CBrowserWindow *, CHtmlTag *);
static void            CBrowserOutputIEndTag           (CBrowserWindow *, CHtmlTag *);
static CBrowserObject *CBrowserOutputImgTag            (CBrowserWindow *, CHtmlTag *);
static CBrowserObject *CBrowserOutputInputTag          (CBrowserWindow *, CHtmlTag *);
static CBrowserObject *CBrowserOutputKbdStartTag       (CBrowserWindow *, CHtmlTag *);
static void            CBrowserOutputKbdEndTag         (CBrowserWindow *, CHtmlTag *);
static CBrowserObject *CBrowserOutputLiStartTag        (CBrowserWindow *, CHtmlTag *);
static void            CBrowserOutputLiEndTag          (CBrowserWindow *, CHtmlTag *);
static CBrowserObject *CBrowserOutputMenuStartTag      (CBrowserWindow *, CHtmlTag *);
static void            CBrowserOutputMenuEndTag        (CBrowserWindow *, CHtmlTag *);
static CBrowserObject *CBrowserOutputNobrStartTag      (CBrowserWindow *, CHtmlTag *);
static void            CBrowserOutputNobrEndTag        (CBrowserWindow *, CHtmlTag *);
static CBrowserObject *CBrowserOutputOlStartTag        (CBrowserWindow *, CHtmlTag *);
static void            CBrowserOutputOlEndTag          (CBrowserWindow *, CHtmlTag *);
static CBrowserObject *CBrowserOutputOptionStartTag    (CBrowserWindow *, CHtmlTag *);
static void            CBrowserOutputOptionEndTag      (CBrowserWindow *, CHtmlTag *);
static CBrowserObject *CBrowserOutputPStartTag         (CBrowserWindow *, CHtmlTag *);
static void            CBrowserOutputPEndTag           (CBrowserWindow *, CHtmlTag *);
static CBrowserObject *CBrowserOutputPreStartTag       (CBrowserWindow *, CHtmlTag *);
static void            CBrowserOutputPreEndTag         (CBrowserWindow *, CHtmlTag *);
static CBrowserObject *CBrowserOutputSampStartTag      (CBrowserWindow *, CHtmlTag *);
static void            CBrowserOutputSampEndTag        (CBrowserWindow *, CHtmlTag *);
static CBrowserObject *CBrowserOutputScriptStartTag    (CBrowserWindow *, CHtmlTag *);
static void            CBrowserOutputScriptEndTag      (CBrowserWindow *, CHtmlTag *);
static CBrowserObject *CBrowserOutputSelectStartTag    (CBrowserWindow *, CHtmlTag *);
static void            CBrowserOutputSelectEndTag      (CBrowserWindow *, CHtmlTag *);
static CBrowserObject *CBrowserOutputSmallStartTag     (CBrowserWindow *, CHtmlTag *);
static void            CBrowserOutputSmallEndTag       (CBrowserWindow *, CHtmlTag *);
static CBrowserObject *CBrowserOutputStrikeStartTag    (CBrowserWindow *, CHtmlTag *);
static void            CBrowserOutputStrikeEndTag      (CBrowserWindow *, CHtmlTag *);
static CBrowserObject *CBrowserOutputStrongStartTag    (CBrowserWindow *, CHtmlTag *);
static void            CBrowserOutputStrongEndTag      (CBrowserWindow *, CHtmlTag *);
static CBrowserObject *CBrowserOutputSubStartTag       (CBrowserWindow *, CHtmlTag *);
static void            CBrowserOutputSubEndTag         (CBrowserWindow *, CHtmlTag *);
static CBrowserObject *CBrowserOutputSupStartTag       (CBrowserWindow *, CHtmlTag *);
static void            CBrowserOutputSupEndTag         (CBrowserWindow *, CHtmlTag *);
static CBrowserObject *CBrowserOutputTableStartTag     (CBrowserWindow *, CHtmlTag *);
static void            CBrowserOutputTableEndTag       (CBrowserWindow *, CHtmlTag *);
static CBrowserObject *CBrowserOutputTdStartTag        (CBrowserWindow *, CHtmlTag *);
static void            CBrowserOutputTdEndTag          (CBrowserWindow *, CHtmlTag *);
static CBrowserObject *CBrowserOutputTextareaStartTag  (CBrowserWindow *, CHtmlTag *);
static void            CBrowserOutputTextareaEndTag    (CBrowserWindow *, CHtmlTag *);
static CBrowserObject *CBrowserOutputThStartTag        (CBrowserWindow *, CHtmlTag *);
static void            CBrowserOutputThEndTag          (CBrowserWindow *, CHtmlTag *);
static CBrowserObject *CBrowserOutputTitleStartTag     (CBrowserWindow *, CHtmlTag *);
static void            CBrowserOutputTitleEndTag       (CBrowserWindow *, CHtmlTag *);
static CBrowserObject *CBrowserOutputTrStartTag        (CBrowserWindow *, CHtmlTag *);
static void            CBrowserOutputTrEndTag          (CBrowserWindow *, CHtmlTag *);
static CBrowserObject *CBrowserOutputTtStartTag        (CBrowserWindow *, CHtmlTag *);
static void            CBrowserOutputTtEndTag          (CBrowserWindow *, CHtmlTag *);
static CBrowserObject *CBrowserOutputUStartTag         (CBrowserWindow *, CHtmlTag *);
static void            CBrowserOutputUEndTag           (CBrowserWindow *, CHtmlTag *);
static CBrowserObject *CBrowserOutputUlStartTag        (CBrowserWindow *, CHtmlTag *);
static void            CBrowserOutputUlEndTag          (CBrowserWindow *, CHtmlTag *);
static CBrowserObject *CBrowserOutputVarStartTag       (CBrowserWindow *, CHtmlTag *);
static void            CBrowserOutputVarEndTag         (CBrowserWindow *, CHtmlTag *);
static CBrowserObject *CBrowserOutputWbrTag            (CBrowserWindow *, CHtmlTag *);
static CBrowserObject *CBrowserOutputXmpStartTag       (CBrowserWindow *, CHtmlTag *);
static void            CBrowserOutputXmpEndTag         (CBrowserWindow *, CHtmlTag *);
static CBrowserObject *CBrowserOutputSkipStartTag      (CBrowserWindow *, CHtmlTag *);
static void            CBrowserOutputSkipEndTag        (CBrowserWindow *, CHtmlTag *);

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

//------

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
CBrowserParseHAlignOption(CBrowserWindow *window, const std::string &name, CHAlignType &align)
{
  std::string lname = CStrUtil::toLower(name);

  if      (lname == "left")
    align = CHALIGN_TYPE_LEFT;
  else if (lname == "center")
    align = CHALIGN_TYPE_CENTER;
  else if (lname == "right")
    align = CHALIGN_TYPE_RIGHT;
  else
    window->displayError("Illegal Horizontal Alignment '%s'\n", name.c_str());
}

void
CBrowserParseVAlignOption(CBrowserWindow *window, const std::string &name, CVAlignType &align)
{
  std::string lname = CStrUtil::toLower(name);

  if      (lname == "top")
    align = CVALIGN_TYPE_TOP;
  else if (lname == "center")
    align = CVALIGN_TYPE_CENTER;
  else if (lname == "bottom")
    align = CVALIGN_TYPE_BOTTOM;
  else if (lname == "baseline")
    align = CVALIGN_TYPE_BASELINE;
  else
    window->displayError("Illegal Vertical Alignment %s\n", name.c_str());
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
    if (! output_data->startProc) {
      window->displayError("Start Tag %s not implemented\n", tag->getName().c_str());
      return;
    }

    CBrowserObject *obj = (*output_data->startProc)(window, tag);

    if (! obj) {
      std::cerr << "No browser object for " << tag->getName().c_str() << std::endl;
      return;
    }

    window->startObject(obj);

    if (! output_data->endProc)
      window->endObject();
  }
  else  {
    if (! output_data->endProc) {
      window->displayError("End Tag %s not implemented\n", tag->getName().c_str());
      return;
    }

    int num_options = tag->getNumOptions();

    if (num_options != 0)
      window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

    (*output_data->endProc)(window, tag);

    window->endObject();
  }
}

void
CBrowserFlushTags(CBrowserWindow *window, CHtmlTagId id)
{
  CBrowserObject *currentObj = window->currentObj();

  while (currentObj && currentObj->type() != id) {
    CBrowserOutputData *output_data = CBrowserGetOutputData(currentObj->type());

    if (! output_data || ! output_data->endProc) {
      window->displayError("End Tag %s not implemented\n", currentObj->typeName().c_str());
      continue;
    }

    (*output_data->endProc)(window, 0);

    window->endObject();
  }
}

//------

void
CBrowserOutputText(CBrowserWindow *window, CHtmlText *text)
{
  std::string str = text->getText();

  std::string sstr = CStrUtil::stripSpaces(str);

  //if (sstr == "") return;

  CBrowserObject *currentObj = window->currentObj();

  if (currentObj) {
    if      (currentObj->type() == CHtmlTagId::TITLE)
      window->getDocument()->setTitle(sstr);
    else if (currentObj->type() == CHtmlTagId::SCRIPT)
      scriptData.text += str;
    else if (currentObj->type() == CHtmlTagId::OPTION) {
      CBrowserFormOption *option = dynamic_cast<CBrowserFormOption *>(currentObj);

      if (sstr != "")
        option->setText(option->text() + sstr);
    }
    else if (currentObj->type() == CHtmlTagId::TEXTAREA) {
      CBrowserFormTextarea *textArea = dynamic_cast<CBrowserFormTextarea *>(currentObj);

      textArea->setText(textArea->text() + str);
    }
    else if (currentObj->type() == CHtmlTagId::H1 || currentObj->type() == CHtmlTagId::H2 ||
             currentObj->type() == CHtmlTagId::H3 || currentObj->type() == CHtmlTagId::H4 ||
             currentObj->type() == CHtmlTagId::H5 || currentObj->type() == CHtmlTagId::H6) {
      CBrowserHeader *header = dynamic_cast<CBrowserHeader *>(currentObj);

      header->setText(header->text() + str);

      window->addCellRedrawData(header);
    }
    else {
      CBrowserObject *textObj = window->addText(str, textData);

      if (sstr != "")
        currentObj->addChild(textObj);
    }
  }
  else {
    window->addText(str, textData);
  }

  if (currentObj)
    currentObj->setText(sstr);
}

static CBrowserObject *
CBrowserOutputAStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserLinkData linkData;

  int num_options = tag->getNumOptions();

  for (int i = 0; i < num_options; i++) {
    const CHtmlTagOption *option = tag->getOption(i);

    std::string option_name = CStrUtil::toLower(option->getName());

    if      (option_name == "download")
      ;
    else if (option_name == "href")
      linkData.href = option->getValue();
    else if (option_name == "id")
      linkData.id = option->getValue();
    else if (option_name == "methods")
      ;
    else if (option_name == "name")
      linkData.id = option->getValue();
    else if (option_name == "rel")
      ;
    else if (option_name == "rev")
      ;
    else if (option_name == "target")
      ;
    else if (option_name == "type")
      ;
    else if (option_name == "title")
      linkData.title = option->getValue();
    else if (option_name == "url")
      ;
    else
      window->displayError("Illegal 'a' Option Name '%s'\n", option_name.c_str());
  }

  //---

  if (linkData.href == "" && linkData.id == "")
    window->displayError("No 'href' or 'name' specified for 'a' Tag'\n");

  if (linkData.href != "") {
    window->linkMgr()->startSourceLink(linkData.href, linkData.title);

    textData.color = window->getDocument()->getLinkColor();

    textData.underline = true;
  }
  else
    window->linkMgr()->startDestLink(linkData.id, linkData.title);

  //---

  CBrowserObject *obj = new CBrowserLinkObj(window, linkData);

  return obj;
}

static void
CBrowserOutputAEndTag(CBrowserWindow *window, CHtmlTag *)
{
  textData.underline = false;

  textData.color = window->getDocument()->getFgColor();

  window->linkMgr()->endLink();
}

static CBrowserObject *
CBrowserOutputAddressStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  window->startItalic();

  //---

  CBrowserAddress *obj = new CBrowserAddress(window);

  return obj;
}

static void
CBrowserOutputAddressEndTag(CBrowserWindow *window, CHtmlTag *)
{
  window->endItalic();
}

static CBrowserObject *
CBrowserOutputBStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  window->startBold();

  //---

  CBrowserBStyle *obj = new CBrowserBStyle(window);

  return obj;
}

static void
CBrowserOutputBEndTag(CBrowserWindow *window, CHtmlTag *)
{
  window->endBold();
}

static CBrowserObject *
CBrowserOutputBasefontStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int size;
  int size_flag;

  //---

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

      error_code = 0;
    }
    else
      window->displayError("Illegal 'basefont' Option Name '%s'\n", option_name.c_str());
  }

  //---

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

  //---

  CBrowserObject *obj = new CBrowserObject(CHtmlTagId::BASEFONT);

  return obj;
}

static void
CBrowserOutputBasefontEndTag(CBrowserWindow *window, CHtmlTag *)
{
  window->resetFontSize();
}

static CBrowserObject *
CBrowserOutputBigStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  window->increaseFontSize();

  //---

  CBrowserBigStyle *obj = new CBrowserBigStyle(window);

  return obj;
}

static void
CBrowserOutputBigEndTag(CBrowserWindow *window, CHtmlTag *)
{
  window->decreaseFontSize();
}

static CBrowserObject *
CBrowserOutputBlinkStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  window->startBold();

  //---

  CBrowserObject *obj = new CBrowserBlinkStyle(window);

  return obj;
}

static void
CBrowserOutputBlinkEndTag(CBrowserWindow *window, CHtmlTag *)
{
  window->endBold();
}

static CBrowserObject *
CBrowserOutputBlockquoteStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  window->indentLeft (2);
  window->indentRight(2);

  window->skipLine();

  window->startItalic();

  //---

  CBrowserObject *obj = new CBrowserObject(CHtmlTagId::BLOCKQUOTE);

  return obj;
}

static void
CBrowserOutputBlockquoteEndTag(CBrowserWindow *window, CHtmlTag *)
{
  window->endItalic();

  window->indentLeft (-2);
  window->indentRight(-2);

  window->skipLine();
}

static CBrowserObject *
CBrowserOutputBodyStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  std::string text;
  std::string bgcolor;
  std::string background;

  //---

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

  //---

  window->getDocument()->setBgColor(bgcolor);
  window->getDocument()->setFgColor(text   );

  window->setBackgroundImage(background, fixed);

  window->getDocument()->setLinkColor (link);
  window->getDocument()->setALinkColor(alink);
  window->getDocument()->setVLinkColor(vlink);

  window->setMargins(leftmargin, topmargin);

  //---

  CBrowserObject *obj = new CBrowserObject(CHtmlTagId::BODY);

  return obj;
}

static void
CBrowserOutputBodyEndTag(CBrowserWindow *, CHtmlTag *)
{
}

static CBrowserObject *
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

  //---

  CBrowserBreak *breaker = new CBrowserBreak(window, clear);

  window->addCellRedrawData(breaker);

  //---

  CBrowserObject *obj = new CBrowserObject(CHtmlTagId::BR);

  return obj;
}

static CBrowserObject *
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

  CBrowserObject *canvas = window->addCanvas(data);

  window->skipLine();

  //---

  return canvas;
}

static void
CBrowserOutputCanvasEndTag(CBrowserWindow *, CHtmlTag *)
{
}

static CBrowserObject *
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

  //---

  CBrowserObject *obj = new CBrowserObject(CHtmlTagId::CAPTION);

  return obj;
}

static void
CBrowserOutputCaptionEndTag(CBrowserWindow *window, CHtmlTag *)
{
  window->tableMgr()->endTableCaption();
}

static CBrowserObject *
CBrowserOutputCenterStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  window->skipLine();

  window->setAlign(CHALIGN_TYPE_CENTER, CVALIGN_TYPE_TOP);

  //---

  CBrowserObject *obj = new CBrowserObject(CHtmlTagId::CENTER);

  return obj;
}

static void
CBrowserOutputCenterEndTag(CBrowserWindow *window, CHtmlTag *)
{
  window->skipLine();

  window->setAlign(CHALIGN_TYPE_LEFT, CVALIGN_TYPE_TOP);
}

static CBrowserObject *
CBrowserOutputCiteStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  window->startItalic();

  //---

  CBrowserCiteStyle *obj = new CBrowserCiteStyle(window);

  return obj;
}

static void
CBrowserOutputCiteEndTag(CBrowserWindow *window, CHtmlTag *)
{
  window->endItalic();
}

static CBrowserObject *
CBrowserOutputCodeStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  window->startFontFace("courier");

  //---

  CBrowserObject *obj = new CBrowserObject(CHtmlTagId::CODE);

  return obj;
}

static void
CBrowserOutputCodeEndTag(CBrowserWindow *window, CHtmlTag *)
{
  window->endFontFace();
}

static CBrowserObject *
CBrowserOutputDdStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserObject *currentObj = window->currentObj();

  if (currentObj && currentObj->type() == CHtmlTagId::DT)
    CBrowserOutputDtEndTag(window, nullptr);

  if (currentObj && currentObj->type() == CHtmlTagId::DD)
    CBrowserOutputDdEndTag(window, nullptr);

  //---

  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  window->indentLeft(4);

  window->newLine();

  //---

  CBrowserObject *obj = new CBrowserObject(CHtmlTagId::DD);

  return obj;
}

static void
CBrowserOutputDdEndTag(CBrowserWindow *window, CHtmlTag *)
{
  window->indentLeft(-4);

  window->newLine();
}

static CBrowserObject *
CBrowserOutputDfnStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  window->startItalic();

  //---

  CBrowserObject *obj = new CBrowserObject(CHtmlTagId::DFN);

  return obj;
}

static void
CBrowserOutputDfnEndTag(CBrowserWindow *window, CHtmlTag *)
{
  window->endItalic();
}

static CBrowserObject *
CBrowserOutputDirStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserOutputList *list = new CBrowserOutputList(CHtmlTagId::DIR);

  CBrowserOutputListMgrInst->startList(list);

  //---

  int num_options = tag->getNumOptions();

  for (int i = 0; i < num_options; i++) {
    const CHtmlTagOption *option = tag->getOption(i);

    std::string option_name = option->getName();

    if      (CStrUtil::casecmp(option_name, "compact") == 0) {
      list->setCompact(true);

      std::string value = option->getValue();

      if (value[0] != '\0')
        window->displayError("No Value needed for Compact\n");
    }
    else if (CStrUtil::casecmp(option_name, "type") == 0)
      list->setSymbol(option->getValue());
    else
      window->displayError("Illegal Dir Option Name '%s'\n", option_name.c_str());
  }

  //---

  window->indentLeft(2);

  if (CBrowserOutputListMgrInst->listDepth() == 1 || ! list->getCompact())
    window->skipLine();
  else
    window->newLine();

  //---

  return list;
}

static void
CBrowserOutputDirEndTag(CBrowserWindow *window, CHtmlTag *)
{
  window->indentLeft(-2);

  if (CBrowserOutputListMgrInst->listDepth() == 1)
    window->skipLine();
  else
    window->newLine();

  //---

  CBrowserOutputListMgrInst->endList();
}

static CBrowserObject *
CBrowserOutputDivStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  std::string id;

  //---

  int num_options = tag->getNumOptions();

  for (int i = 0; i < num_options; i++) {
    const CHtmlTagOption *option = tag->getOption(i);

    std::string option_name = option->getName();

    if (CStrUtil::casecmp(option_name, "id") == 0) {
      id = option->getValue();
    }
    else
      window->displayError("Illegal 'div' Option Name '%s'\n", option_name.c_str());
  }

  //---

  CBrowserObject *obj = new CBrowserObject(CHtmlTagId::DIV);

  return obj;
}

static void
CBrowserOutputDivEndTag(CBrowserWindow *, CHtmlTag *)
{
}

static CBrowserObject *
CBrowserOutputDlStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserOutputList *list = new CBrowserOutputList(CHtmlTagId::DL);

  CBrowserOutputListMgrInst->startList(list);

  //---

  int num_options = tag->getNumOptions();

  for (int i = 0; i < num_options; i++) {
    const CHtmlTagOption *option = tag->getOption(i);

    std::string option_name = option->getName();

    if      (CStrUtil::casecmp(option_name, "compact") == 0) {
      list->setCompact(true);

      std::string value = option->getValue();

      if (value[0] != '\0')
        window->displayError("No Value needed for Compact\n");
    }
    else
      window->displayError("Illegal 'dl' Option Name '%s'\n", option_name.c_str());
  }

  //---

  window->indentLeft(2);

  if (CBrowserOutputListMgrInst->listDepth() == 1 || ! list->getCompact())
    window->skipLine();
  else
    window->newLine();

  //---

  return list;
}

static void
CBrowserOutputDlEndTag(CBrowserWindow *window, CHtmlTag *)
{
  CBrowserObject *currentObj = window->currentObj();

  if (currentObj && currentObj->type() == CHtmlTagId::DT)
    CBrowserOutputDtEndTag(window, nullptr);

  if (currentObj && currentObj->type() == CHtmlTagId::DD)
    CBrowserOutputDdEndTag(window, nullptr);

  //---

  window->indentLeft(-2);

  if (CBrowserOutputListMgrInst->listDepth() == 1)
    window->skipLine();
  else
    window->newLine();

  //---

  CBrowserOutputListMgrInst->endList();
}

static CBrowserObject *
CBrowserOutputDtStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserObject *currentObj = window->currentObj();

  if (currentObj && currentObj->type() == CHtmlTagId::DT)
    CBrowserOutputDtEndTag(window, nullptr);

  if (currentObj && currentObj->type() == CHtmlTagId::DD)
    CBrowserOutputDdEndTag(window, nullptr);

  //---

  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  window->startBold();

  //---

  CBrowserObject *obj = new CBrowserObject(CHtmlTagId::DT);

  return obj;
}

static void
CBrowserOutputDtEndTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (tag && num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  window->endBold();
}

static CBrowserObject *
CBrowserOutputEmStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  //---

  window->startItalic();

  //---

  CBrowserObject *obj = new CBrowserEmStyle(window);

  return obj;
}

static void
CBrowserOutputEmEndTag(CBrowserWindow *window, CHtmlTag *)
{
  window->endItalic();
}

static CBrowserObject *
CBrowserOutputFontStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  std::string color = window->getFontColor();
  std::string face  = window->getCurrentFontFace();
  int         size  = window->getFontSize();

  CBrowserOutputFont *font = new CBrowserOutputFont(face, color, size);

  CBrowserOutputFont::startFont(font);

  //---

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

  //---

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

  //---

  CBrowserObject *obj = new CBrowserObject(CHtmlTagId::FONT);

  return obj;
}

static void
CBrowserOutputFontEndTag(CBrowserWindow *window, CHtmlTag *)
{
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

  //---

  delete font;
}

static CBrowserObject *
CBrowserOutputFormStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  std::string            name   = "";
  CBrowserFormMethodType method = CBrowserFormMethodType::GET;
  std::string            action = "";

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
        method = CBrowserFormMethodType::GET;
      else if (CStrUtil::casecmp(option->getValue(), "post") == 0)
        method = CBrowserFormMethodType::POST;
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

  //---

  CBrowserObject *obj = new CBrowserObject(CHtmlTagId::FORM);

  return obj;
}

static void
CBrowserOutputFormEndTag(CBrowserWindow *window, CHtmlTag *)
{
  window->getDocument()->formMgr()->endForm();
}

static CBrowserObject *
CBrowserOutputH1StartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  CHAlignType align = CHALIGN_TYPE_LEFT;

  int num_options = tag->getNumOptions();

  for (int i = 0; i < num_options; i++) {
    const CHtmlTagOption *option = tag->getOption(i);

    std::string option_name = option->getName();

    if (CStrUtil::casecmp(option_name, "align") == 0)
      CBrowserParseHAlignOption(window, option->getValue(), align);
    else
      window->displayError("Illegal H1 Option Name '%s'\n", option_name.c_str());
  }

  //---

  CBrowserHeader *header = new CBrowserHeader(window, CHtmlTagId::H1, align);

  header->initFormat();

  return header;
}

static void
CBrowserOutputH1EndTag(CBrowserWindow *window, CHtmlTag *)
{
  CBrowserFlushTags(window, CHtmlTagId::H1);

  //---

  CBrowserObject *currentObj = window->currentObj();

  if (currentObj && currentObj->type() == CHtmlTagId::H1) {
    CBrowserHeader *header = dynamic_cast<CBrowserHeader *>(currentObj);

    header->termFormat();
  }
}

static CBrowserObject *
CBrowserOutputH2StartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  CHAlignType align = CHALIGN_TYPE_LEFT;

  int num_options = tag->getNumOptions();

  for (int i = 0; i < num_options; i++) {
    const CHtmlTagOption *option = tag->getOption(i);

    std::string option_name = option->getName();

    if (CStrUtil::casecmp(option_name, "align") == 0)
      CBrowserParseHAlignOption(window, option->getValue(), align);
    else
      window->displayError("Illegal H2 Option Name '%s'\n", option_name.c_str());
  }

  //---

  CBrowserHeader *header = new CBrowserHeader(window, CHtmlTagId::H2, align);

  header->initFormat();

  return header;
}

static void
CBrowserOutputH2EndTag(CBrowserWindow *window, CHtmlTag *)
{
  CBrowserFlushTags(window, CHtmlTagId::H2);

  //---

  CBrowserObject *currentObj = window->currentObj();

  if (currentObj && currentObj->type() == CHtmlTagId::H2) {
    CBrowserHeader *header = dynamic_cast<CBrowserHeader *>(currentObj);

    header->termFormat();
  }
}

static CBrowserObject *
CBrowserOutputH3StartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  CHAlignType align = CHALIGN_TYPE_LEFT;

  int num_options = tag->getNumOptions();

  for (int i = 0; i < num_options; i++) {
    const CHtmlTagOption *option = tag->getOption(i);

    std::string option_name = option->getName();

    if (CStrUtil::casecmp(option_name, "align") == 0)
      CBrowserParseHAlignOption(window, option->getValue(), align);
    else
      window->displayError("Illegal H3 Option Name '%s'\n", option_name.c_str());
  }

  //---

  CBrowserHeader *header = new CBrowserHeader(window, CHtmlTagId::H3, align);

  header->initFormat();

  return header;
}

static void
CBrowserOutputH3EndTag(CBrowserWindow *window, CHtmlTag *)
{
  CBrowserFlushTags(window, CHtmlTagId::H3);

  //---

  CBrowserObject *currentObj = window->currentObj();

  if (currentObj && currentObj->type() == CHtmlTagId::H3) {
    CBrowserHeader *header = dynamic_cast<CBrowserHeader *>(currentObj);

    header->termFormat();
  }
}

static CBrowserObject *
CBrowserOutputH4StartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  CHAlignType align = CHALIGN_TYPE_LEFT;

  int num_options = tag->getNumOptions();

  for (int i = 0; i < num_options; i++) {
    const CHtmlTagOption *option = tag->getOption(i);

    std::string option_name = option->getName();

    if (CStrUtil::casecmp(option_name, "align") == 0)
      CBrowserParseHAlignOption(window, option->getValue(), align);
    else
      window->displayError("Illegal H4 Option Name '%s'\n", option_name.c_str());
  }

  //---

  CBrowserHeader *header = new CBrowserHeader(window, CHtmlTagId::H4, align);

  header->initFormat();

  return header;
}

static void
CBrowserOutputH4EndTag(CBrowserWindow *window, CHtmlTag *)
{
  CBrowserFlushTags(window, CHtmlTagId::H4);

  //---

  CBrowserObject *currentObj = window->currentObj();

  if (currentObj && currentObj->type() == CHtmlTagId::H4) {
    CBrowserHeader *header = dynamic_cast<CBrowserHeader *>(currentObj);

    header->termFormat();
  }
}

static CBrowserObject *
CBrowserOutputH5StartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  CHAlignType align = CHALIGN_TYPE_LEFT;

  int num_options = tag->getNumOptions();

  for (int i = 0; i < num_options; i++) {
    const CHtmlTagOption *option = tag->getOption(i);

    std::string option_name = option->getName();

    if (CStrUtil::casecmp(option_name, "align") == 0)
      CBrowserParseHAlignOption(window, option->getValue(), align);
    else
      window->displayError("Illegal H5 Option Name '%s'\n", option_name.c_str());
  }

  //---

  CBrowserHeader *header = new CBrowserHeader(window, CHtmlTagId::H5, align);

  header->initFormat();

  return header;
}

static void
CBrowserOutputH5EndTag(CBrowserWindow *window, CHtmlTag *)
{
  CBrowserFlushTags(window, CHtmlTagId::H5);

  //---

  CBrowserObject *currentObj = window->currentObj();

  if (currentObj && currentObj->type() == CHtmlTagId::H5) {
    CBrowserHeader *header = dynamic_cast<CBrowserHeader *>(currentObj);

    header->termFormat();
  }
}

static CBrowserObject *
CBrowserOutputH6StartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  CHAlignType align = CHALIGN_TYPE_LEFT;

  int num_options = tag->getNumOptions();

  for (int i = 0; i < num_options; i++) {
    const CHtmlTagOption *option = tag->getOption(i);

    std::string option_name = option->getName();

    if (CStrUtil::casecmp(option_name, "align") == 0)
      CBrowserParseHAlignOption(window, option->getValue(), align);
    else
      window->displayError("Illegal H6 Option Name '%s'\n", option_name.c_str());
  }

  //---

  CBrowserHeader *header = new CBrowserHeader(window, CHtmlTagId::H6, align);

  header->initFormat();

  return header;
}

static void
CBrowserOutputH6EndTag(CBrowserWindow *window, CHtmlTag *)
{
  CBrowserFlushTags(window, CHtmlTagId::H6);

  //---

  CBrowserObject *currentObj = window->currentObj();

  if (currentObj && currentObj->type() == CHtmlTagId::H6) {
    CBrowserHeader *header = dynamic_cast<CBrowserHeader *>(currentObj);

    header->termFormat();
  }
}

static CBrowserObject *
CBrowserOutputHeadStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  //---

  CBrowserObject *obj = new CBrowserObject(CHtmlTagId::HEAD);

  return obj;
}

static void
CBrowserOutputHeadEndTag(CBrowserWindow *, CHtmlTag *)
{
}

static CBrowserObject *
CBrowserOutputHrTag(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserRuleData ruleData;

  int num_options = tag->getNumOptions();

  for (int i = 0; i < num_options; i++) {
    const CHtmlTagOption *option = tag->getOption(i);

    std::string option_name = option->getName();

    if      (CStrUtil::casecmp(option_name, "align") == 0) {
      CBrowserParseHAlignOption(window, option->getValue(), ruleData.align);
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
        ruleData.unit = CBrowserUnitsType::PERCENT;

        std::string value = value.substr(0, len - 1);

        if (CStrUtil::isInteger(option->getValue()))
          ruleData.width = CStrUtil::toInteger(value);
        else {
          window->displayError("Illegal Value for Width %s\n", option->getValue().c_str());
          ruleData.width = -1;
        }
      }
      else {
        ruleData.unit = CBrowserUnitsType::PIXEL;

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

  //---

  CBrowserRule *rule = new CBrowserRule(window, ruleData);

  rule->initFormat();

  //---

  return rule;
}

static CBrowserObject *
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

  //---

  CBrowserObject *obj = new CBrowserObject(CHtmlTagId::HTML);

  window->setRootObject(obj);

  return obj;
}

static void
CBrowserOutputHtmlEndTag(CBrowserWindow *, CHtmlTag *)
{
}

static CBrowserObject *
CBrowserOutputIStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  window->startItalic();

  //---

  CBrowserObject *obj = new CBrowserIStyle(window);

  return obj;
}

static void
CBrowserOutputIEndTag(CBrowserWindow *window, CHtmlTag *)
{
  window->endItalic();
}

static CBrowserObject *
CBrowserOutputImgTag(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserImageData imageData;

  //---

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
    return 0;
  }

  //---

  CBrowserObject *obj = window->addImage(imageData);

  return obj;
}

static CBrowserObject *
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

  //---

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

  //---

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
    return 0;
  }

  input->setId(id);

  //---

  return input;
}

static CBrowserObject *
CBrowserOutputKbdStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  window->startFontFace("courier");

  //---

  CBrowserObject *obj = new CBrowserObject(CHtmlTagId::KBD);

  return obj;
}

static void
CBrowserOutputKbdEndTag(CBrowserWindow *window, CHtmlTag *)
{
  window->endFontFace();
}

static CBrowserObject *
CBrowserOutputLiStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserOutputList *currentList = CBrowserOutputListMgrInst->currentList();

  int num_options = tag->getNumOptions();

  for (int i = 0; i < num_options; i++) {
    const CHtmlTagOption *option = tag->getOption(i);

    std::string option_name = option->getName();

    if      (CStrUtil::casecmp(option_name, "type") == 0) {
      if (currentList)
        currentList->setSymbol(option->getValue());
    }
    else if (CStrUtil::casecmp(option_name, "value") == 0) {
      if (CStrUtil::isInteger(option->getValue())) {
        int item_num = CStrUtil::toInteger(option->getValue());

        if (currentList)
          currentList->setItemNum(item_num);
      }
      else
        window->displayError("Illegal Value for Value %s\n", option->getValue().c_str());

    }
    else
      window->displayError("Illegal Li Option Name '%s'\n", option_name.c_str());
  }

  CHtmlTagId listId = (currentList ? currentList->type() : CHtmlTagId::NONE);

  //---

  if (! currentList || ! currentList->getCompact())
    window->skipLine();
  else
    window->newLine();

  if      (listId == CHtmlTagId::OL) {
    std::string text;

    if      (! currentList)
      text = "";
    else if (currentList->getSymbol() == "")
      text = CStrUtil::toString(currentList->getItemNum());
    else if (currentList->getSymbol() == "A") {
      text = CBrowserMisc::integerToAlphabetic(currentList->getItemNum());

      CStrUtil::toUpper(text);
    }
    else if (currentList->getSymbol() == "a") {
      text = CBrowserMisc::integerToAlphabetic(currentList->getItemNum());

      CStrUtil::toLower(text);
    }
    else if (currentList->getSymbol() == "I") {
      CRomanNumber roman(currentList->getItemNum());

      text = roman.getString();

      CStrUtil::toUpper(roman.getString());
    }
    else if (currentList->getSymbol() == "i") {
      CRomanNumber roman(currentList->getItemNum());

      text = roman.getString();

      CStrUtil::toLower(text);
    }
    else if (currentList->getSymbol() == "1")
      text = CStrUtil::toString(currentList->getItemNum());
    else
      text = CStrUtil::toString(currentList->getItemNum());

    window->addLabel(text, 4, CHALIGN_TYPE_RIGHT, textData.color);

    window->addLabel(". ", 2, CHALIGN_TYPE_LEFT, textData.color);
  }
  else if (listId == CHtmlTagId::UL || listId == CHtmlTagId::DIR || listId == CHtmlTagId::MENU) {
    if      (currentList->getSymbol() == "") {
      int num = CBrowserOutputListMgrInst->listDepth();

      num %= 4;

      CBrowserSymbolType type = CBrowserSymbolType::CIRCLE;

      if      (num == 0) type = CBrowserSymbolType::DISC;
      else if (num == 1) type = CBrowserSymbolType::CIRCLE;
      else if (num == 2) type = CBrowserSymbolType::BLOCK;
      else               type = CBrowserSymbolType::SQUARE;

      window->addSymbol(type);
    }
    else if (CStrUtil::casecmp(currentList->getSymbol(), "disc"  ) == 0)
      window->addSymbol(CBrowserSymbolType::DISC);
    else if (CStrUtil::casecmp(currentList->getSymbol(), "circle") == 0)
      window->addSymbol(CBrowserSymbolType::CIRCLE);
    else if (CStrUtil::casecmp(currentList->getSymbol(), "square") == 0)
      window->addSymbol(CBrowserSymbolType::SQUARE);
    else
      window->addSymbol(CBrowserSymbolType::DISC);

    window->addLabel(" ", 1, CHALIGN_TYPE_LEFT, textData.color);
  }

  if (currentList)
    currentList->setItemNum(currentList->getItemNum() + 1);

  //---

  CBrowserObject *obj = new CBrowserObject(CHtmlTagId::LI);

  return obj;
}

static void
CBrowserOutputLiEndTag(CBrowserWindow *, CHtmlTag *)
{
}

static CBrowserObject *
CBrowserOutputMenuStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserOutputList *list = new CBrowserOutputList(CHtmlTagId::MENU);

  CBrowserOutputListMgrInst->startList(list);

  //---

  int num_options = tag->getNumOptions();

  for (int i = 0; i < num_options; i++) {
    const CHtmlTagOption *option = tag->getOption(i);

    std::string option_name = option->getName();

    if      (CStrUtil::casecmp(option_name, "compact") == 0) {
      list->setCompact(true);

      std::string value = option->getValue();

      if (value[0] != '\0')
        window->displayError("No Value needed for Compact\n");
    }
    else if (CStrUtil::casecmp(option_name, "type") == 0)
      list->setSymbol(option->getValue());
    else
      window->displayError("Illegal Menu Option Name '%s'\n", option_name.c_str());
  }

  //---

  window->indentLeft(2);

  if (CBrowserOutputListMgrInst->listDepth() == 1 || ! list->getCompact())
    window->skipLine();
  else
    window->newLine();

  //---

  return list;
}

static void
CBrowserOutputMenuEndTag(CBrowserWindow *window, CHtmlTag *)
{
  window->indentLeft(-2);

  if (CBrowserOutputListMgrInst->listDepth() == 1)
    window->skipLine();
  else
    window->newLine();

  //---

  CBrowserOutputListMgrInst->endList();
}

static CBrowserObject *
CBrowserOutputNobrStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  textData.breakup = false;

  //---

  CBrowserObject *obj = new CBrowserObject(CHtmlTagId::NOBR);

  return obj;
}

static void
CBrowserOutputNobrEndTag(CBrowserWindow *, CHtmlTag *)
{
  textData.breakup = true;
}

static CBrowserObject *
CBrowserOutputOlStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserOutputList *list = new CBrowserOutputList(CHtmlTagId::OL);

  CBrowserOutputListMgrInst->startList(list);

  //---

  int num_options = tag->getNumOptions();

  for (int i = 0; i < num_options; i++) {
    const CHtmlTagOption *option = tag->getOption(i);

    std::string option_name = option->getName();

    if      (CStrUtil::casecmp(option_name, "compact") == 0) {
      list->setCompact(true);

      std::string value = option->getValue();

      if (value[0] != '\0')
        window->displayError("No Value needed for Compact\n");
    }
    else if (CStrUtil::casecmp(option_name, "start") == 0) {
      if (CStrUtil::isInteger(option->getValue())) {
        int item_num = CStrUtil::toInteger(option->getValue());

        list->setItemNum(item_num);
      }
      else {
        window->displayError("Illegal Value for Value %s\n", option->getValue().c_str());

        list->setItemNum(1);
      }
    }
    else if (CStrUtil::casecmp(option_name, "type") == 0)
      list->setSymbol(option->getValue());
    else
      window->displayError("Illegal Ol Option Name '%s'\n", option_name.c_str());
  }

  //---

  window->indentLeft(2);

  if (CBrowserOutputListMgrInst->listDepth() == 1 || ! list->getCompact())
    window->skipLine();
  else
    window->newLine();

  //---

  return list;
}

static void
CBrowserOutputOlEndTag(CBrowserWindow *window, CHtmlTag *)
{
  window->indentLeft(-2);

  if (CBrowserOutputListMgrInst->listDepth() == 1)
    window->skipLine();
  else
    window->newLine();

  //---

  CBrowserOutputListMgrInst->endList();
}

static CBrowserObject *
CBrowserOutputOptionStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  std::string value;

  CBrowserObject *currentObj = window->currentObj();

  if (currentObj && currentObj->type() == CHtmlTagId::OPTION)
    CBrowserOutputOptionEndTag(window, nullptr);

  //---

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

  //---

  CBrowserFormOption *option =
    window->getDocument()->formMgr()->startOption(value, selected);

  return option;
}

static void
CBrowserOutputOptionEndTag(CBrowserWindow *window, CHtmlTag *)
{
  CBrowserFlushTags(window, CHtmlTagId::OPTION);

  //---

  CBrowserObject *currentObj = window->currentObj();

  if (currentObj && currentObj->type() == CHtmlTagId::OPTION) {
    CBrowserFormOption *option = dynamic_cast<CBrowserFormOption *>(currentObj);

    window->getDocument()->formMgr()->endOption(option->getText());
  }
}

static CBrowserObject *
CBrowserOutputPStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  CHAlignType align = CHALIGN_TYPE_LEFT;

  int num_options = tag->getNumOptions();

  for (int i = 0; i < num_options; i++) {
    const CHtmlTagOption *option = tag->getOption(i);

    std::string option_name = option->getName();

    if (CStrUtil::casecmp(option_name, "align") == 0)
      CBrowserParseHAlignOption(window, option->getValue(), align);
    else
      window->displayError("Illegal P Option Name '%s'\n", option_name.c_str());
  }

  //---

  window->skipLine();

  window->setAlign(align, CVALIGN_TYPE_TOP);

  //---

  CBrowserObject *obj = new CBrowserObject(CHtmlTagId::P);

  return obj;
}

static void
CBrowserOutputPEndTag(CBrowserWindow *window, CHtmlTag *)
{
  window->newLine();

  window->setAlign(CHALIGN_TYPE_LEFT, CVALIGN_TYPE_TOP);
}

static CBrowserObject *
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

  //---

  window->skipLine();

  window->startFontFace("courier");

  textData.breakup = false;
  textData.format  = false;

  //---

  CBrowserObject *obj = new CBrowserObject(CHtmlTagId::PRE);

  return obj;
}

static void
CBrowserOutputPreEndTag(CBrowserWindow *window, CHtmlTag *)
{
  textData.breakup = true;
  textData.format  = true;

  window->endFontFace();

  window->skipLine();
}

static CBrowserObject *
CBrowserOutputSampStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  window->startFontFace("courier");

  //---

  CBrowserObject *obj = new CBrowserObject(CHtmlTagId::SAMP);

  return obj;
}

static void
CBrowserOutputSampEndTag(CBrowserWindow *window, CHtmlTag *)
{
  window->endFontFace();
}

static CBrowserObject *
CBrowserOutputScriptStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  std::string language;
  std::string src;
  std::string type = "text/javascript";

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
    if      (CStrUtil::casecmp(language, "ceil") == 0)
      CBrowserCeilInst->runScriptFile(window, src);
    else if (CStrUtil::casecmp(type, "text/javascript") == 0)
      window->addScriptFile(src);
  }
  else {
    scriptData.language = language;
    scriptData.type     = type;
    scriptData.text     = "";
  }

  //---

  CBrowserObject *obj = new CBrowserObject(CHtmlTagId::SCRIPT);

  return obj;
}

static void
CBrowserOutputScriptEndTag(CBrowserWindow *window, CHtmlTag *)
{
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

static CBrowserObject *
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

  //---

  CBrowserFormSelect *select =
    window->getDocument()->formMgr()->startSelect(name, size, multiple);

  select->setId(id);

  return select;
}

static void
CBrowserOutputSelectEndTag(CBrowserWindow *window, CHtmlTag *)
{
  CBrowserObject *currentObj = window->currentObj();

  if (currentObj && currentObj->type() == CHtmlTagId::OPTION)
    CBrowserOutputOptionEndTag(window, nullptr);

  //---

  window->getDocument()->formMgr()->endSelect();
}

static CBrowserObject *
CBrowserOutputSmallStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  window->decreaseFontSize();

  //---

  CBrowserSmallStyle *obj = new CBrowserSmallStyle(window);

  return obj;
}

static void
CBrowserOutputSmallEndTag(CBrowserWindow *window, CHtmlTag *)
{
  window->increaseFontSize();
}

static CBrowserObject *
CBrowserOutputStrikeStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  textData.strike = true;

  //---

  CBrowserStrikeStyle *obj = new CBrowserStrikeStyle(window);

  return obj;
}

static void
CBrowserOutputStrikeEndTag(CBrowserWindow *, CHtmlTag *)
{
  textData.strike = false;
}

static CBrowserObject *
CBrowserOutputStrongStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  window->startBold();

  //---

  CBrowserStrongStyle *obj = new CBrowserStrongStyle(window);

  return obj;
}

static void
CBrowserOutputStrongEndTag(CBrowserWindow *window, CHtmlTag *)
{
  window->endBold();
}

static CBrowserObject *
CBrowserOutputSubStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  textData.place = CBrowserTextPlaceType::SUBSCRIPT;

  //---

  CBrowserSubStyle *obj = new CBrowserSubStyle(window);

  return obj;
}

static void
CBrowserOutputSubEndTag(CBrowserWindow *, CHtmlTag *)
{
  textData.place = CBrowserTextPlaceType::NORMAL;
}

static CBrowserObject *
CBrowserOutputSupStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  textData.place = CBrowserTextPlaceType::SUPERSCRIPT;

  //---

  CBrowserSupStyle *obj = new CBrowserSupStyle(window);

  return obj;
}

static void
CBrowserOutputSupEndTag(CBrowserWindow *, CHtmlTag *)
{
  textData.place = CBrowserTextPlaceType::NORMAL;
}

static CBrowserObject *
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

        tableData.width_unit = CBrowserUnitsType::PERCENT;

        value = option->getValue().substr(0, len - 1);

        if (CStrUtil::isInteger(option->getValue()))
          tableData.width = CStrUtil::toInteger(value);
        else {
          window->displayError("Illegal Value for Width %s\n", option->getValue().c_str());
          tableData.width = -1;
        }
      }
      else {
        tableData.width_unit = CBrowserUnitsType::PIXEL;

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

  CBrowserTable *table = window->tableMgr()->startTable(tableData);

  return table;
}

static void
CBrowserOutputTableEndTag(CBrowserWindow *window, CHtmlTag *)
{
  window->tableMgr()->endTable();
}

static CBrowserObject *
CBrowserOutputTdStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserTableCellData cellData;

  cellData.type       = CBrowserTableCellData::Type::HEADER;
  cellData.width      = 0;
  cellData.width_unit = CBrowserUnitsType::NONE;
  cellData.colspan    = 1;
  cellData.rowspan    = 1;

  int num_options = tag->getNumOptions();

  for (int i = 0; i < num_options; i++) {
    const CHtmlTagOption *option = tag->getOption(i);

    std::string option_name = option->getName();

    if      (CStrUtil::casecmp(option_name, "align") == 0) {
      CBrowserParseHAlignOption(window, option->getValue(), cellData.halign);
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
      CBrowserParseVAlignOption(window, option->getValue(), cellData.valign);
    }
    else if (CStrUtil::casecmp(option_name, "width") == 0) {
      int len = option->getValue().size();

      std::string value = option->getValue();

      if (value[len - 1] == '%') {
        cellData.width_unit = CBrowserUnitsType::PERCENT;

        std::string value = option->getValue().substr(len - 1);

        if (CStrUtil::isInteger(option->getValue()))
          cellData.width = CStrUtil::toInteger(value);
        else {
          window->displayError("Illegal Value for Width %s\n", option->getValue().c_str());
          cellData.width = -1;
        }
      }
      else {
        cellData.width_unit = CBrowserUnitsType::PIXEL;

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

  //---

  CBrowserObject *obj = new CBrowserObject(CHtmlTagId::TD);

  return obj;
}

static void
CBrowserOutputTdEndTag(CBrowserWindow *window, CHtmlTag *)
{
  window->tableMgr()->endTableCell();
}

static CBrowserObject *
CBrowserOutputTextareaStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  std::string id;
  std::string name;
  std::string wrap;

  //---

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

  //---

  CBrowserFormTextarea *textArea =
    window->getDocument()->formMgr()->startTextarea(name, rows, cols, wrap);

  return textArea;
}

static void
CBrowserOutputTextareaEndTag(CBrowserWindow *window, CHtmlTag *)
{
  CBrowserFlushTags(window, CHtmlTagId::TEXTAREA);

  //---

  CBrowserObject *currentObj = window->currentObj();

  if (currentObj && currentObj->type() == CHtmlTagId::TEXTAREA) {
    CBrowserFormTextarea *textArea = dynamic_cast<CBrowserFormTextarea *>(currentObj);

    std::string text = textArea->text();

    if (text != "" && text[0] == '\n')
      window->getDocument()->formMgr()->endTextarea(text.substr(1));
    else
      window->getDocument()->formMgr()->endTextarea(text);
  }
}

static CBrowserObject *
CBrowserOutputThStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserTableCellData cellData;

  cellData.type       = CBrowserTableCellData::Type::HEADER;
  cellData.width      = 0;
  cellData.width_unit = CBrowserUnitsType::NONE;
  cellData.colspan    = 1;
  cellData.rowspan    = 1;

  int num_options = tag->getNumOptions();

  for (int i = 0; i < num_options; i++) {
    const CHtmlTagOption *option = tag->getOption(i);

    std::string option_name = option->getName();

    if      (CStrUtil::casecmp(option_name, "align") == 0) {
      CBrowserParseHAlignOption(window, option->getValue(), cellData.halign);
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
      CBrowserParseVAlignOption(window, option->getValue(), cellData.valign);
    }
    else if (CStrUtil::casecmp(option_name, "width") == 0) {
      int len = option->getValue().size();

      std::string value = option->getValue();

      if (value[len - 1] == '%') {
        std::string value;

        cellData.width_unit = CBrowserUnitsType::PERCENT;

        value = option->getValue().substr(0, len - 1);

        if (CStrUtil::isInteger(option->getValue()))
          cellData.width = CStrUtil::toInteger(value);
        else {
          window->displayError("Illegal Value for Width %s\n", option->getValue().c_str());
          cellData.width = -1;
        }
      }
      else {
        cellData.width_unit = CBrowserUnitsType::PIXEL;

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

  //---

  CBrowserObject *obj = new CBrowserObject(CHtmlTagId::TH);

  return obj;
}

static void
CBrowserOutputThEndTag(CBrowserWindow *window, CHtmlTag *)
{
  window->tableMgr()->endTableCell();
}

static CBrowserObject *
CBrowserOutputTitleStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  //---

  CBrowserObject *obj = new CBrowserObject(CHtmlTagId::TITLE);

  return obj;
}

static void
CBrowserOutputTitleEndTag(CBrowserWindow *, CHtmlTag *)
{
}

static CBrowserObject *
CBrowserOutputTrStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserTableRowData rowData;

  int num_options = tag->getNumOptions();

  for (int i = 0; i < num_options; i++) {
    const CHtmlTagOption *option = tag->getOption(i);

    std::string option_name = option->getName();

    if      (CStrUtil::casecmp(option_name, "align") == 0) {
      CBrowserParseHAlignOption(window, option->getValue(), rowData.halign);
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
      CBrowserParseVAlignOption(window, option->getValue(), rowData.valign);
    }
    else
      window->displayError("Illegal Tr Option Name '%s'\n", option_name.c_str());
  }

  //---

  window->tableMgr()->startTableRow(rowData);

  //---

  CBrowserObject *obj = new CBrowserObject(CHtmlTagId::TR);

  return obj;
}

static void
CBrowserOutputTrEndTag(CBrowserWindow *window, CHtmlTag *)
{
  window->tableMgr()->endTableRow();
}

static CBrowserObject *
CBrowserOutputTtStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  window->startFontFace("courier");

  //---

  CBrowserObject *obj = new CBrowserObject(CHtmlTagId::TT);

  return obj;
}

static void
CBrowserOutputTtEndTag(CBrowserWindow *window, CHtmlTag *)
{
  window->endFontFace();
}

static CBrowserObject *
CBrowserOutputUStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  textData.underline = true;

  //---

  CBrowserUStyle *obj = new CBrowserUStyle(window);

  return obj;
}

static void
CBrowserOutputUEndTag(CBrowserWindow *, CHtmlTag *)
{
  textData.underline = false;
}

static CBrowserObject *
CBrowserOutputUlStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserOutputList *list = new CBrowserOutputList(CHtmlTagId::UL);

  CBrowserOutputListMgrInst->startList(list);

  //---

  int num_options = tag->getNumOptions();

  for (int i = 0; i < num_options; i++) {
    const CHtmlTagOption *option = tag->getOption(i);

    std::string option_name = option->getName();

    if      (CStrUtil::casecmp(option_name, "compact") == 0) {
      list->setCompact(true);

      std::string value = option->getValue();

      if (value[0] != '\0')
        window->displayError("No Value needed for Compact\n");
    }
    else if (CStrUtil::casecmp(option_name, "type") == 0)
      list->setSymbol(option->getValue());
    else
      window->displayError("Illegal Ul Option Name '%s'\n", option_name.c_str());
  }

  //---

  window->indentLeft(2);

  if (CBrowserOutputListMgrInst->listDepth() == 1 || ! list->getCompact())
    window->skipLine();
  else
    window->newLine();

  //---

  return list;
}

static void
CBrowserOutputUlEndTag(CBrowserWindow *window, CHtmlTag *)
{
  window->indentLeft(-2);

  if (CBrowserOutputListMgrInst->listDepth() == 1)
    window->skipLine();
  else
    window->newLine();

  //---

  CBrowserOutputListMgrInst->endList();
}

static CBrowserObject *
CBrowserOutputVarStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  window->startItalic();

  //---

  CBrowserVarStyle *obj = new CBrowserVarStyle(window);

  return obj;
}

static void
CBrowserOutputVarEndTag(CBrowserWindow *window, CHtmlTag *)
{
  window->endItalic();
}

static CBrowserObject *
CBrowserOutputWbrTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  //---

  CBrowserObject *obj = new CBrowserObject(CHtmlTagId::WBR);

  return obj;
}

static CBrowserObject *
CBrowserOutputXmpStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  window->skipLine();

  window->startFontFace("courier");

  textData.breakup = false;
  textData.format  = false;

  //---

  CBrowserObject *obj = new CBrowserObject(CHtmlTagId::XMP);

  return obj;
}

static void
CBrowserOutputXmpEndTag(CBrowserWindow *window, CHtmlTag *)
{
  textData.breakup = true;
  textData.format  = true;

  window->endFontFace();

  window->skipLine();
}

static CBrowserObject *
CBrowserOutputSkipStartTag(CBrowserWindow *, CHtmlTag *)
{
  return 0;
}

static void
CBrowserOutputSkipEndTag(CBrowserWindow *, CHtmlTag *)
{
}
