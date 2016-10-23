#include <CBrowserHtmlI.h>

enum CBrowserOutputListType {
  CBROWSER_OUTPUT_LIST_TYPE_NONE,
  CBROWSER_OUTPUT_LIST_TYPE_UL,
  CBROWSER_OUTPUT_LIST_TYPE_OL,
  CBROWSER_OUTPUT_LIST_TYPE_DIR,
  CBROWSER_OUTPUT_LIST_TYPE_MENU,
  CBROWSER_OUTPUT_LIST_TYPE_DL
};

enum CBrowserOutputFontFlag {
  CBROWSER_OUTPUT_FONT_FLAG_NONE  = 0,
  CBROWSER_OUTPUT_FONT_FLAG_FACE  = (1<<0),
  CBROWSER_OUTPUT_FONT_FLAG_COLOR = (1<<1),
  CBROWSER_OUTPUT_FONT_FLAG_SIZE  = (1<<2)
};

typedef void (*CBrowserOutputProc)
                (CBrowserWindow *window, CHtmlTag *tag);

class CBrowserOutputData {
 public:
  CHtmlTagId         id;
  CBrowserOutputProc start_proc;
  CBrowserOutputProc end_proc;
};

class CBrowserOutputList {
 private:
  CBrowserOutputListType type_;
  std::string            symbol_;
  int                    item_num_;
  bool                   compact_;

 public:
  CBrowserOutputList(CBrowserOutputListType type) :
   type_(type), symbol_(""), item_num_(1), compact_(false) {
  }

  CBrowserOutputListType getType() const { return type_; }

  const std::string &getSymbol() const { return symbol_; }

  void setSymbol(const std::string &symbol) { symbol_ = symbol; }

  int getItemNum() const { return item_num_; }

  void setItemNum(int item_num) { item_num_ = item_num; }

  bool getCompact() const { return compact_; }

  void setCompact(bool compact) { compact_ = compact; }
};

class CBrowserOutputFont {
 private:
  int         flags_;
  std::string face_;
  std::string color_;
  int         size_;

  static std::vector<CBrowserOutputFont *> font_stack_;

 public:
  CBrowserOutputFont(const std::string &face, const std::string &color, int size) :
   flags_(CBROWSER_OUTPUT_FONT_FLAG_NONE), face_(face), color_(color), size_(size) {
    font_stack_.push_back(this);
  }

  void setFlag(CBrowserOutputFontFlag flag) { flags_ = flags_ | flag; }
  bool getFlag(CBrowserOutputFontFlag flag) { return (flags_ & flag); }

  const std::string &getFace () const { return face_ ; }
  const std::string &getColor() const { return color_; }
  int                getSize () const { return size_ ; }

  static void startFont(CBrowserOutputFont *font) {
    font_stack_.push_back(font);
  }

  static CBrowserOutputFont *endFont() {
    if (font_stack_.empty())
      return NULL;

    CBrowserOutputFont *font = font_stack_[font_stack_.size() - 1];

    font_stack_.pop_back();

    return font;
  }
};

static CBrowserOutputList                *current_list    = NULL;
static std::vector<CBrowserOutputList *>  list_stack;
static bool                               in_title        = false;
static bool                               in_script       = false;
static std::string                        script_language = "";
static std::string                        script_text     = "";
static bool                               in_option       = false;
static std::string                        option_text     = "";
static bool                               in_textarea     = false;
static std::string                        textarea_text   = "";
static bool                               in_dd           = false;
static bool                               in_dt           = false;

std::vector<CBrowserOutputFont *> CBrowserOutputFont::font_stack_;

std::vector<CBrowserBreak *> breakers;

static void  CBrowserOutputAStartTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputAEndTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputAddressStartTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputAddressEndTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputBStartTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputBEndTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputBasefontStartTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputBasefontEndTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputBigStartTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputBigEndTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputBlinkStartTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputBlinkEndTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputBlockquoteStartTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputBlockquoteEndTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputBodyStartTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputBodyEndTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputBrTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputCaptionStartTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputCaptionEndTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputCenterStartTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputCenterEndTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputCiteStartTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputCiteEndTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputCodeStartTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputCodeEndTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputDdStartTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputDdEndTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputDfnStartTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputDfnEndTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputDirStartTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputDirEndTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputDlStartTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputDlEndTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputDtStartTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputDtEndTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputEmStartTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputEmEndTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputFontStartTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputFontEndTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputFormStartTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputFormEndTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputH1StartTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputH1EndTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputH2StartTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputH2EndTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputH3StartTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputH3EndTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputH4StartTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputH4EndTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputH5StartTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputH5EndTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputH6StartTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputH6EndTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputHeadStartTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputHeadEndTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputHrTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputHtmlStartTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputHtmlEndTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputIStartTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputIEndTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputImgTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputInputTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputKbdStartTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputKbdEndTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputLiStartTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputLiEndTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputMenuStartTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputMenuEndTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputNobrStartTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputNobrEndTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputOlStartTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputOlEndTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputOptionStartTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputOptionEndTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputPStartTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputPEndTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputPreStartTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputPreEndTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputSampStartTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputSampEndTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputScriptStartTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputScriptEndTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputSelectStartTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputSelectEndTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputSmallStartTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputSmallEndTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputStrikeStartTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputStrikeEndTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputStrongStartTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputStrongEndTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputSubStartTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputSubEndTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputSupStartTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputSupEndTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputTableStartTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputTableEndTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputTdStartTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputTdEndTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputTextareaStartTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputTextareaEndTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputThStartTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputThEndTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputTitleStartTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputTitleEndTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputTrStartTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputTrEndTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputTtStartTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputTtEndTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputUStartTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputUEndTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputUlStartTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputUlEndTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputVarStartTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputVarEndTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputWbrTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputXmpStartTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputXmpEndTag
              (CBrowserWindow *, CHtmlTag *);
static void  CBrowserOutputNewLine
              (CBrowserWindow *);
static void  CBrowserOutputSkipLine
              (CBrowserWindow *);

static CBrowserOutputData
output_datas[] = {
  {
    CHTML_TAG_ID_A,
    CBrowserOutputAStartTag,
    CBrowserOutputAEndTag,
  },
  {
    CHTML_TAG_ID_ADDRESS,
    CBrowserOutputAddressStartTag,
    CBrowserOutputAddressEndTag,
  },
  {
    CHTML_TAG_ID_APPLET,
    NULL,
    NULL,
  },
  {
    CHTML_TAG_ID_AREA,
    NULL,
    NULL,
  },
  {
    CHTML_TAG_ID_B,
    CBrowserOutputBStartTag,
    CBrowserOutputBEndTag,
  },
  {
    CHTML_TAG_ID_BASE,
    NULL,
    NULL,
  },
  {
    CHTML_TAG_ID_BASEFONT,
    CBrowserOutputBasefontStartTag,
    CBrowserOutputBasefontEndTag,
  },
  {
    CHTML_TAG_ID_BGSOUND,
    NULL,
    NULL,
  },
  {
    CHTML_TAG_ID_BIG,
    CBrowserOutputBigStartTag,
    CBrowserOutputBigEndTag,
  },
  {
    CHTML_TAG_ID_BLINK,
    CBrowserOutputBlinkStartTag,
    CBrowserOutputBlinkEndTag,
  },
  {
    CHTML_TAG_ID_BLOCKQUOTE,
    CBrowserOutputBlockquoteStartTag,
    CBrowserOutputBlockquoteEndTag,
  },
  {
    CHTML_TAG_ID_BODY,
    CBrowserOutputBodyStartTag,
    CBrowserOutputBodyEndTag,
  },
  {
    CHTML_TAG_ID_BR,
    CBrowserOutputBrTag,
    NULL,
  },
  {
    CHTML_TAG_ID_CAPTION,
    CBrowserOutputCaptionStartTag,
    CBrowserOutputCaptionEndTag,
  },
  {
    CHTML_TAG_ID_CENTER,
    CBrowserOutputCenterStartTag,
    CBrowserOutputCenterEndTag,
  },
  {
    CHTML_TAG_ID_CITE,
    CBrowserOutputCiteStartTag,
    CBrowserOutputCiteEndTag,
  },
  {
    CHTML_TAG_ID_CODE,
    CBrowserOutputCodeStartTag,
    CBrowserOutputCodeEndTag,
  },
  {
    CHTML_TAG_ID_DD,
    CBrowserOutputDdStartTag,
    CBrowserOutputDdEndTag,
  },
  {
    CHTML_TAG_ID_DFN,
    CBrowserOutputDfnStartTag,
    CBrowserOutputDfnEndTag,
  },
  {
    CHTML_TAG_ID_DIR,
    CBrowserOutputDirStartTag,
    CBrowserOutputDirEndTag,
  },
  {
    CHTML_TAG_ID_DIV,
    NULL,
    NULL,
  },
  {
    CHTML_TAG_ID_DL,
    CBrowserOutputDlStartTag,
    CBrowserOutputDlEndTag,
  },
  {
    CHTML_TAG_ID_DT,
    CBrowserOutputDtStartTag,
    CBrowserOutputDtEndTag,
  },
  {
    CHTML_TAG_ID_EM,
    CBrowserOutputEmStartTag,
    CBrowserOutputEmEndTag,
  },
  {
    CHTML_TAG_ID_FONT,
    CBrowserOutputFontStartTag,
    CBrowserOutputFontEndTag,
  },
  {
    CHTML_TAG_ID_FORM,
    CBrowserOutputFormStartTag,
    CBrowserOutputFormEndTag,
  },
  {
    CHTML_TAG_ID_FRAME,
    NULL,
    NULL,
  },
  {
    CHTML_TAG_ID_FRAMESET,
    NULL,
    NULL,
  },
  {
    CHTML_TAG_ID_H1,
    CBrowserOutputH1StartTag,
    CBrowserOutputH1EndTag,
  },
  {
    CHTML_TAG_ID_H2,
    CBrowserOutputH2StartTag,
    CBrowserOutputH2EndTag,
  },
  {
    CHTML_TAG_ID_H3,
    CBrowserOutputH3StartTag,
    CBrowserOutputH3EndTag,
  },
  {
    CHTML_TAG_ID_H4,
    CBrowserOutputH4StartTag,
    CBrowserOutputH4EndTag,
  },
  {
    CHTML_TAG_ID_H5,
    CBrowserOutputH5StartTag,
    CBrowserOutputH5EndTag,
  },
  {
    CHTML_TAG_ID_H6,
    CBrowserOutputH6StartTag,
    CBrowserOutputH6EndTag,
  },
  {
    CHTML_TAG_ID_HEAD,
    CBrowserOutputHeadStartTag,
    CBrowserOutputHeadEndTag,
  },
  {
    CHTML_TAG_ID_HR,
    CBrowserOutputHrTag,
    NULL,
  },
  {
    CHTML_TAG_ID_HTML,
    CBrowserOutputHtmlStartTag,
    CBrowserOutputHtmlEndTag,
  },
  {
    CHTML_TAG_ID_I,
    CBrowserOutputIStartTag,
    CBrowserOutputIEndTag,
  },
  {
    CHTML_TAG_ID_IMG,
    CBrowserOutputImgTag,
    NULL,
  },
  {
    CHTML_TAG_ID_INPUT,
    CBrowserOutputInputTag,
    NULL,
  },
  {
    CHTML_TAG_ID_ISINDEX,
    NULL,
    NULL,
  },
  {
    CHTML_TAG_ID_KBD,
    CBrowserOutputKbdStartTag,
    CBrowserOutputKbdEndTag,
  },
  {
    CHTML_TAG_ID_LI,
    CBrowserOutputLiStartTag,
    CBrowserOutputLiEndTag,
  },
  {
    CHTML_TAG_ID_LINK,
    NULL,
    NULL,
  },
  {
    CHTML_TAG_ID_LISTING,
    NULL,
    NULL,
  },
  {
    CHTML_TAG_ID_MAP,
    NULL,
    NULL,
  },
  {
    CHTML_TAG_ID_MARQUEE,
    NULL,
    NULL,
  },
  {
    CHTML_TAG_ID_MENU,
    CBrowserOutputMenuStartTag,
    CBrowserOutputMenuEndTag,
  },
  {
    CHTML_TAG_ID_META,
    NULL,
    NULL,
  },
  {
    CHTML_TAG_ID_NEXTID,
    NULL,
    NULL,
  },
  {
    CHTML_TAG_ID_NOBR,
    CBrowserOutputNobrStartTag,
    CBrowserOutputNobrEndTag,
  },
  {
    CHTML_TAG_ID_NOFRAMES,
    NULL,
    NULL,
  },
  {
    CHTML_TAG_ID_OL,
    CBrowserOutputOlStartTag,
    CBrowserOutputOlEndTag,
  },
  {
    CHTML_TAG_ID_OPTION,
    CBrowserOutputOptionStartTag,
    CBrowserOutputOptionEndTag,
  },
  {
    CHTML_TAG_ID_P,
    CBrowserOutputPStartTag,
    CBrowserOutputPEndTag,
  },
  {
    CHTML_TAG_ID_PARAM,
    NULL,
    NULL,
  },
  {
    CHTML_TAG_ID_PLAINTEXT,
    NULL,
    NULL,
  },
  {
    CHTML_TAG_ID_PRE,
    CBrowserOutputPreStartTag,
    CBrowserOutputPreEndTag,
  },
  {
    CHTML_TAG_ID_S,
    CBrowserOutputStrikeStartTag,
    CBrowserOutputStrikeEndTag,
  },
  {
    CHTML_TAG_ID_SAMP,
    CBrowserOutputSampStartTag,
    CBrowserOutputSampEndTag,
  },
  {
    CHTML_TAG_ID_SCRIPT,
    CBrowserOutputScriptStartTag,
    CBrowserOutputScriptEndTag,
  },
  {
    CHTML_TAG_ID_SELECT,
    CBrowserOutputSelectStartTag,
    CBrowserOutputSelectEndTag,
  },
  {
    CHTML_TAG_ID_SMALL,
    CBrowserOutputSmallStartTag,
    CBrowserOutputSmallEndTag,
  },
  {
    CHTML_TAG_ID_SPAN,
    NULL,
    NULL,
  },
  {
    CHTML_TAG_ID_STRIKE,
    CBrowserOutputStrikeStartTag,
    CBrowserOutputStrikeEndTag,
  },
  {
    CHTML_TAG_ID_STRONG,
    CBrowserOutputStrongStartTag,
    CBrowserOutputStrongEndTag,
  },
  {
    CHTML_TAG_ID_STYLE,
    NULL,
    NULL,
  },
  {
    CHTML_TAG_ID_SUB,
    CBrowserOutputSubStartTag,
    CBrowserOutputSubEndTag,
  },
  {
    CHTML_TAG_ID_SUP,
    CBrowserOutputSupStartTag,
    CBrowserOutputSupEndTag,
  },
  {
    CHTML_TAG_ID_TABLE,
    CBrowserOutputTableStartTag,
    CBrowserOutputTableEndTag,
  },
  {
    CHTML_TAG_ID_TD,
    CBrowserOutputTdStartTag,
    CBrowserOutputTdEndTag,
  },
  {
    CHTML_TAG_ID_TEXTAREA,
    CBrowserOutputTextareaStartTag,
    CBrowserOutputTextareaEndTag,
  },
  {
    CHTML_TAG_ID_TH,
    CBrowserOutputThStartTag,
    CBrowserOutputThEndTag,
  },
  {
    CHTML_TAG_ID_TITLE,
    CBrowserOutputTitleStartTag,
    CBrowserOutputTitleEndTag,
  },
  {
    CHTML_TAG_ID_TR,
    CBrowserOutputTrStartTag,
    CBrowserOutputTrEndTag,
  },
  {
    CHTML_TAG_ID_TT,
    CBrowserOutputTtStartTag,
    CBrowserOutputTtEndTag,
  },
  {
    CHTML_TAG_ID_U,
    CBrowserOutputUStartTag,
    CBrowserOutputUEndTag,
  },
  {
    CHTML_TAG_ID_UL,
    CBrowserOutputUlStartTag,
    CBrowserOutputUlEndTag,
  },
  {
    CHTML_TAG_ID_VAR,
    CBrowserOutputVarStartTag,
    CBrowserOutputVarEndTag,
  },
  {
    CHTML_TAG_ID_WBR,
    CBrowserOutputWbrTag,
    NULL,
  },
  {
    CHTML_TAG_ID_XMP,
    CBrowserOutputXmpStartTag,
    CBrowserOutputXmpEndTag,
  },
  {
    CHTML_TAG_ID_COMMENT,
    NULL,
    NULL,
  },
};

static uint num_output_datas = sizeof(output_datas)/sizeof(CBrowserOutputData);

void
CBrowserOutputTokens(CBrowserWindow *window, CHtmlParserTokens &tokens)
{
  CBrowserOutputInit();

  uint num = tokens.size();

  for (uint i = 0; i < num; ++i) {
    if      (tokens[i]->isTag())
      CBrowserOutputTag (window, tokens[i]->getTag ());
    else if (tokens[i]->isText())
      CBrowserOutputText(window, tokens[i]->getText());
  }

  CBrowserOutputTerm();
}

void
CBrowserOutputInit(void)
{
}

void
CBrowserOutputTerm(void)
{
}

void
CBrowserOutputTag(CBrowserWindow *window, CHtmlTag *tag)
{
  const CHtmlTagDef &data = tag->getTagDef();

  CHtmlTagId id = data.getId();

  CBrowserOutputData *output_data = NULL;

  for (uint i = 0; i < num_output_datas; ++i)
    if (output_datas[i].id == id) {
      output_data = &output_datas[i];
      break;
    }

  if (output_data == NULL) {
    window->displayError("Tag %s not found\n", tag->getName().c_str());
    return;
  }

  if (tag->isStartTag()) {
    if (output_data->start_proc != NULL)
      (*output_data->start_proc)(window, tag);
    else
      window->displayError("Start Tag %s not implemented\n", tag->getName().c_str());
  }
  else  {
    if (output_data->end_proc != NULL)
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
  else if (in_script) {
    if (CStrUtil::casecmp(script_language, "ceil") != 0)
      return;

    script_text += str;
  }
  else if (in_option)
    option_text += str;
  else if (in_textarea)
    textarea_text += str;
  else
    HtmlDrawText(window, str);
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
      window->displayError("Illegal A Option Name '%s'\n", option_name.c_str());
  }

  /*-------------*/

  if (href == "" && name == "") {
    window->displayError("No 'href' or 'name' specified for A Tag'\n");
    return;
  }

  if (href != "") {
    HtmlStartSourceLink(window, href, title);

    HtmlSetTextColor(window->getDocument()->getLinkColor());

    HtmlSetTextUnderline(true);
  }
  else
    HtmlStartDestLink(window, name, title);
}

static void
CBrowserOutputAEndTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  HtmlSetTextUnderline(false);

  HtmlSetTextColor(window->getDocument()->getFgColor());

  HtmlEndLink(window);
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
        window->displayError("Illegal Value for Size %s\n", option->getValue().c_str());
    }
    else
      window->displayError("Illegal Basefont Option Name '%s'\n", option_name.c_str());
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
    else if (CStrUtil::casecmp(option_name, "bgproperties") == 0)
      {
        if      (CStrUtil::casecmp(option->getValue(), "fixed") == 0)
          fixed = true;
        else
          window->displayError("Illegal bgproperties value %s\n", option->getValue().c_str());
      }
    else if (CStrUtil::casecmp(option_name, "leftmargin") == 0) {
      if (CStrUtil::isInteger(option->getValue()))
        leftmargin = CStrUtil::toInteger(option->getValue());
      else {
        window->displayError("Illegal Value for leftmargin %s\n", option->getValue().c_str());

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
        window->displayError("Illegal Value for leftmargin %s\n", option->getValue().c_str());

        topmargin = TOP_MARGIN;
      }
    }
    else if (CStrUtil::casecmp(option_name, "vlink") == 0)
      vlink = option->getValue();
    else
      window->displayError("Illegal Body Option Name '%s'\n", option_name.c_str());
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
  CHtmlLayoutClearType clear = CHTML_LAYOUT_CLEAR_NONE;

  int num_options = tag->getNumOptions();

  for (int i = 0; i < num_options; i++)
    {
      const CHtmlTagOption *option = tag->getOption(i);

      std::string option_name = option->getName();

      if (CStrUtil::casecmp(option_name, "clear") == 0)
        {
          if      (CStrUtil::casecmp(option->getValue(), "left" ) == 0)
            clear = CHTML_LAYOUT_CLEAR_LEFT;
          else if (CStrUtil::casecmp(option->getValue(), "right") == 0)
            clear = CHTML_LAYOUT_CLEAR_RIGHT;
          else if (CStrUtil::casecmp(option->getValue(), "all"  ) == 0)
            clear = CHTML_LAYOUT_CLEAR_ALL;
          else
            window->displayError("Illegal Break Clear %s\n", option->getValue().c_str());
        }
      else
        window->displayError("Illegal Br Option Name '%s'\n", option_name.c_str());
    }

  /*-------------*/

  CBrowserBreak *breaker = new CBrowserBreak(window, clear);

  breakers.push_back(breaker);
}

static void
CBrowserOutputCaptionStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  CHAlignType halign = CHALIGN_TYPE_NONE;
  CVAlignType valign = CVALIGN_TYPE_NONE;

  int num_options = tag->getNumOptions();

  for (int i = 0; i < num_options; i++)
    {
      const CHtmlTagOption *option = tag->getOption(i);

      std::string option_name = option->getName();

      if      (CStrUtil::casecmp(option_name, "align") == 0) {
        if      (CStrUtil::casecmp(option->getValue(), "left") == 0)
          halign = CHALIGN_TYPE_LEFT;
        else if (CStrUtil::casecmp(option->getValue(), "center") == 0)
          halign = CHALIGN_TYPE_CENTER;
        else if (CStrUtil::casecmp(option->getValue(), "right") == 0)
          halign = CHALIGN_TYPE_RIGHT;
        else if (CStrUtil::casecmp(option->getValue(), "top") == 0)
          valign = CVALIGN_TYPE_TOP;
        else if (CStrUtil::casecmp(option->getValue(), "bottom") == 0)
          valign = CVALIGN_TYPE_BOTTOM;
        else
          window->displayError("Illegal %s Alignment %s\n", "Table Caption",
                               option->getValue().c_str());
      }
      else if (CStrUtil::casecmp(option_name, "valign") == 0) {
        if      (CStrUtil::casecmp(option->getValue(), "top") == 0)
          valign = CVALIGN_TYPE_TOP;
        else if (CStrUtil::casecmp(option->getValue(), "bottom") == 0)
          valign = CVALIGN_TYPE_BOTTOM;
        else
          window->displayError("Illegal %s Vertical Alignment %s\n", "Table Caption",
                               option->getValue().c_str());
      }
      else
        window->displayError("Illegal Caption Option Name '%s'\n", option_name.c_str());
    }

  /*-------------*/

  HtmlSetTableCaptionAlign(halign, valign);

  HtmlStartTableCaption(window);
}

static void
CBrowserOutputCaptionEndTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  HtmlEndTableCaption(window);
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
    CBrowserOutputDtEndTag(window, NULL);

  if (in_dd)
    CBrowserOutputDdEndTag(window, NULL);

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

  if (tag != NULL && num_options != 0)
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
  if (current_list != NULL)
    list_stack.push_back(current_list);

  current_list = new CBrowserOutputList(CBROWSER_OUTPUT_LIST_TYPE_DIR);

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
    current_list = NULL;
}

static void
CBrowserOutputDlStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  if (current_list != NULL)
    list_stack.push_back(current_list);

  current_list = new CBrowserOutputList(CBROWSER_OUTPUT_LIST_TYPE_DL);

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
    CBrowserOutputDtEndTag(window, NULL);

  if (in_dd)
    CBrowserOutputDdEndTag(window, NULL);

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
    current_list = NULL;
}

static void
CBrowserOutputDtStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  if (in_dt)
    CBrowserOutputDtEndTag(window, NULL);

  if (in_dd)
    CBrowserOutputDdEndTag(window, NULL);

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

  if (tag != NULL && num_options != 0)
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

    font->setFlag(CBROWSER_OUTPUT_FONT_FLAG_FACE);
  }

  if (color != "") {
    window->setFontColor(color);

    font->setFlag(CBROWSER_OUTPUT_FONT_FLAG_COLOR);
  }

  int base_size = window->getBaseFontSize();

  if (delta != 0)
    size += base_size;

  if (size > 0) {
    window->setFontSize(size);

    font->setFlag(CBROWSER_OUTPUT_FONT_FLAG_SIZE);
  }
}

static void
CBrowserOutputFontEndTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  /*-------------*/

  CBrowserOutputFont *font = CBrowserOutputFont::endFont();;

  if (font == NULL) {
    window->displayError("<font> ... </font> mismatch\n");
    return;
  }

  if (font->getFlag(CBROWSER_OUTPUT_FONT_FLAG_FACE))
    window->setCurrentFontFace(font->getFace());

  if (font->getFlag(CBROWSER_OUTPUT_FONT_FLAG_COLOR))
    window->setFontColor(font->getColor());

  if (font->getFlag(CBROWSER_OUTPUT_FONT_FLAG_SIZE))
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

  /*-------------*/

  HtmlStartForm(window->getDocument(), name, method, action);
}

static void
CBrowserOutputFormEndTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  /*-------------*/

  HtmlEndForm(window->getDocument());
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

  for (int i = 0; i < num_options; i++)
    {
      const CHtmlTagOption *option = tag->getOption(i);

      std::string option_name = option->getName();

      if (CStrUtil::casecmp(option_name, "align") == 0)
        {
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

  for (int i = 0; i < num_options; i++)
    {
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

  for (int i = 0; i < num_options; i++)
    {
      const CHtmlTagOption *option = tag->getOption(i);

      std::string option_name = option->getName();

      if (CStrUtil::casecmp(option_name, "align") == 0)
        {
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

  for (int i = 0; i < num_options; i++)
    {
      const CHtmlTagOption *option = tag->getOption(i);

      std::string option_name = option->getName();

      if (CStrUtil::casecmp(option_name, "align") == 0)
        {
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

  for (int i = 0; i < num_options; i++)
    {
      const CHtmlTagOption *option = tag->getOption(i);

      std::string option_name = option->getName();

      if (CStrUtil::casecmp(option_name, "align") == 0)
        {
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
  int len;

  CHAlignType align = CHALIGN_TYPE_CENTER;
  bool        shade = true;
  int         width = -1;
  int         unit  = UNIT_NONE;
  int         size  = -1;

  int num_options = tag->getNumOptions();

  for (int i = 0; i < num_options; i++) {
    const CHtmlTagOption *option = tag->getOption(i);

    std::string option_name = option->getName();

    if      (CStrUtil::casecmp(option_name, "align") == 0)
      {
        if      (CStrUtil::casecmp(option->getValue(), "left"  ) == 0)
          align = CHALIGN_TYPE_LEFT;
        else if (CStrUtil::casecmp(option->getValue(), "center" ) == 0)
          align = CHALIGN_TYPE_CENTER;
        else if (CStrUtil::casecmp(option->getValue(), "right" ) == 0)
          align = CHALIGN_TYPE_RIGHT;
        else
          window->displayError("Illegal Align %s\n", option->getValue().c_str());
      }
    else if (CStrUtil::casecmp(option_name, "noshade") == 0) {
      shade = false;

      std::string value = option->getValue();

      if (value[0] != '\0')
        window->displayError("No Value needed for Noshade\n");
    }
    else if (CStrUtil::casecmp(option_name, "size") == 0) {
      if (CStrUtil::isInteger(option->getValue()))
        size = CStrUtil::toInteger(option->getValue());
      else {
        window->displayError("Illegal Value for Size %s\n", option->getValue().c_str());
        size = -1;
      }
    }
    else if (CStrUtil::casecmp(option_name, "width") == 0) {
      len = option->getValue().size();

      std::string value = option->getValue();

      if (value[len - 1] == '%') {
        unit = UNIT_PERCENT;

        std::string value = value.substr(0, len - 1);

        if (CStrUtil::isInteger(option->getValue()))
          width = CStrUtil::toInteger(value);
        else {
          window->displayError("Illegal Value for Width %s\n", option->getValue().c_str());
          width = -1;
        }
      }
      else {
        unit = UNIT_PIXEL;

        if (CStrUtil::isInteger(option->getValue()))
          width = CStrUtil::toInteger(option->getValue());
        else {
          window->displayError("Illegal Value for Width %s\n", option->getValue().c_str());
          width = -1;
        }
      }
    }
    else
      window->displayError("Illegal Hr Option Name '%s'\n", option_name.c_str());
  }

  HtmlSetRuleAlign(align);
  HtmlSetRuleShade(shade);
  HtmlSetRuleSize (size );
  HtmlSetRuleWidth(width, unit);

  HtmlDrawRule(window);
}

static void
CBrowserOutputHtmlStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());
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
  std::string src;
  std::string alt;
  std::string usemap;

  CBrowserImageAlignType align  = CBROWSER_IMAGE_ALIGN_BOTTOM;
  int                    border = 2;
  int                    height = -1;
  int                    hspace = 2;
//bool                   ismap  = false;
  int                    width  = -1;
  int                    vspace = 2;

  /*-----------*/

  int num_options = tag->getNumOptions();

  for (int i = 0; i < num_options; i++) {
    const CHtmlTagOption *option = tag->getOption(i);

    std::string option_name = option->getName();

    if      (CStrUtil::casecmp(option_name, "align") == 0)
      {
        if      (CStrUtil::casecmp(option->getValue(), "top"      ) == 0)
          align = CBROWSER_IMAGE_ALIGN_TOP;
        else if (CStrUtil::casecmp(option->getValue(), "middle"   ) == 0)
          align = CBROWSER_IMAGE_ALIGN_MIDDLE;
        else if (CStrUtil::casecmp(option->getValue(), "bottom"   ) == 0)
          align = CBROWSER_IMAGE_ALIGN_BOTTOM;
        else if (CStrUtil::casecmp(option->getValue(), "left"     ) == 0)
          align = CBROWSER_IMAGE_ALIGN_LEFT;
        else if (CStrUtil::casecmp(option->getValue(), "right"    ) == 0)
          align = CBROWSER_IMAGE_ALIGN_RIGHT;
        else if (CStrUtil::casecmp(option->getValue(), "texttop"  ) == 0)
          align = CBROWSER_IMAGE_ALIGN_TEXTTOP;
        else if (CStrUtil::casecmp(option->getValue(), "absmiddle") == 0)
          align = CBROWSER_IMAGE_ALIGN_ABSMIDDLE;
        else if (CStrUtil::casecmp(option->getValue(), "absbottom") == 0)
          align = CBROWSER_IMAGE_ALIGN_ABSBOTTOM;
        else
          window->displayError("Illegal Image Align %s\n", option->getValue().c_str());
      }
    else if (CStrUtil::casecmp(option_name, "alt") == 0)
      alt = option->getValue();
    else if (CStrUtil::casecmp(option_name, "border") == 0) {
      if (CStrUtil::isInteger(option->getValue()))
        border = CStrUtil::toInteger(option->getValue());
      else {
        window->displayError("Illegal Value for Border %s\n", option->getValue().c_str());
        border = 2;
      }
    }
    else if (CStrUtil::casecmp(option_name, "controls") == 0)
      ;
    else if (CStrUtil::casecmp(option_name, "dynsrc") == 0)
      ;
    else if (CStrUtil::casecmp(option_name, "height") == 0) {
      if (CStrUtil::isInteger(option->getValue()))
        height = CStrUtil::toInteger(option->getValue());
      else {
        window->displayError("Illegal Value for Height %s\n", option->getValue().c_str());
        height = -1;
      }
    }
    else if (CStrUtil::casecmp(option_name, "hspace") == 0) {
      if (CStrUtil::isInteger(option->getValue()))
        hspace = CStrUtil::toInteger(option->getValue());
      else {
        window->displayError("Illegal Value for HSpace %s\n", option->getValue().c_str());
        hspace = 2;
      }
    }
    else if (CStrUtil::casecmp(option_name, "ismap") == 0) {
      //ismap = true;

      std::string value = option->getValue();

      if (value[0] != '\0')
        window->displayError("No Value needed for IsMap\n");
    }
    else if (CStrUtil::casecmp(option_name, "loop") == 0)
      ;
    else if (CStrUtil::casecmp(option_name, "lowsrc") == 0)
      ;
    else if (CStrUtil::casecmp(option_name, "src") == 0)
      src  = option->getValue();
    else if (CStrUtil::casecmp(option_name, "start") == 0)
      ;
    else if (CStrUtil::casecmp(option_name, "usemap") == 0)
      usemap  = option->getValue();
    else if (CStrUtil::casecmp(option_name, "vspace") == 0) {
      if (CStrUtil::isInteger(option->getValue()))
        vspace = CStrUtil::toInteger(option->getValue());
      else {
        window->displayError("Illegal Value for VSpace %s\n", option->getValue().c_str());
        vspace = 2;
      }
    }
    else if (CStrUtil::casecmp(option_name, "width") == 0) {
      if (CStrUtil::isInteger(option->getValue()))
        width = CStrUtil::toInteger(option->getValue());
      else {
        window->displayError("Illegal Value for Width %s\n", option->getValue().c_str());
        width = -1;
      }
    }
    else
      window->displayError("Illegal Img Option Name '%s'\n", option_name.c_str());
  }

  if (src == "") {
    window->displayError("No Image Source Specified\n");
    return;
  }

  /*-----------*/

  HtmlSetImageAlign  (align);
  HtmlSetImageBorder (border);
  HtmlSetImageSize   (width, height);
  HtmlSetImageSpacing(hspace, vspace);
  HtmlSetImageAlt    (alt);

  /*-----------*/

  HtmlDrawImage(window, src);
}

static void
CBrowserOutputInputTag(CBrowserWindow *window, CHtmlTag *tag)
{
  std::string src;
  std::string name;
  std::string type;
  std::string align;
  std::string value;
  std::string onclick;
  std::string onchange;

  bool checked   = false;
  int  maxlength = -1;
  int  size      = -1;

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
    else if (CStrUtil::casecmp(option_name, "name") == 0)
      name = option->getValue();
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
    else if (CStrUtil::casecmp(option_name, "value") == 0)
      value = option->getValue();
    else
      window->displayError("Illegal %s Option Name '%s'\n",
                           tag->getName().c_str(), option_name.c_str());
  }

  if (type == "") {
    window->displayError("No Input Type Specified\n");
    return;
  }

  /*-----------*/

  if (onclick != "")
    CBrowserFormSetInputOnClick(onclick);

  if (onchange != "")
    CBrowserFormSetInputOnChange(onchange);

  if      (CStrUtil::casecmp(type, "button"  ) == 0)
    CBrowserFormAddButtonInput(window->getDocument(), name, value);
  else if (CStrUtil::casecmp(type, "checkbox") == 0)
    CBrowserFormAddCheckboxInput(window->getDocument(), name, value, checked);
  else if (CStrUtil::casecmp(type, "file"    ) == 0)
    CBrowserFormAddFileInput(window->getDocument(), name, value, size, maxlength);
  else if (CStrUtil::casecmp(type, "hidden"  ) == 0)
    CBrowserFormAddHiddenInput(window->getDocument(), name, value);
  else if (CStrUtil::casecmp(type, "image"   ) == 0)
    CBrowserFormAddImageInput(window->getDocument(), name, src, align);
  else if (CStrUtil::casecmp(type, "password") == 0)
    CBrowserFormAddPasswordInput(window->getDocument(), name, size, maxlength);
  else if (CStrUtil::casecmp(type, "radio"   ) == 0)
    CBrowserFormAddRadioInput(window->getDocument(), name, value, checked);
  else if (CStrUtil::casecmp(type, "reset"   ) == 0)
    CBrowserFormAddResetInput(window->getDocument(), name, value);
  else if (CStrUtil::casecmp(type, "submit"  ) == 0)
    CBrowserFormAddSubmitInput(window->getDocument(), name, value);
  else if (CStrUtil::casecmp(type, "text"    ) == 0)
    CBrowserFormAddTextInput(window->getDocument(), name, value, size, maxlength);
  else
    window->displayError("Invalid Input Type '%s'\n", type.c_str());
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

  if      (current_list != NULL &&
           current_list->getType() == CBROWSER_OUTPUT_LIST_TYPE_OL) {
    std::string text;

    if      (current_list->getSymbol() == "")
      text = CStrUtil::toString(current_list->getItemNum());
    else if (current_list->getSymbol() == "A") {
      text = HtmlIntegerToAlphabetic(current_list->getItemNum());

      CStrUtil::toUpper(text);
    }
    else if (current_list->getSymbol() == "a") {
      text = HtmlIntegerToAlphabetic(current_list->getItemNum());

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

    HtmlDrawLabel(window, text, 4, CHALIGN_TYPE_RIGHT);

    HtmlDrawLabel(window, ". ", 2, CHALIGN_TYPE_LEFT);
  }
  else if (current_list != NULL &&
           (current_list->getType() == CBROWSER_OUTPUT_LIST_TYPE_UL  ||
            current_list->getType() == CBROWSER_OUTPUT_LIST_TYPE_DIR ||
            current_list->getType() == CBROWSER_OUTPUT_LIST_TYPE_MENU)) {
    if      (current_list->getSymbol() == "") {
      int num = list_stack.size();

      num %= 4;

      if      (num == 0)
        HtmlDrawSymbol(window, CBROWSER_SYMBOL_TYPE_DISC);
      else if (num == 1)
        HtmlDrawSymbol(window, CBROWSER_SYMBOL_TYPE_CIRCLE);
      else if (num == 2)
        HtmlDrawSymbol(window, CBROWSER_SYMBOL_TYPE_BLOCK);
      else
        HtmlDrawSymbol(window, CBROWSER_SYMBOL_TYPE_SQUARE);
    }
    else if (CStrUtil::casecmp(current_list->getSymbol(), "disc"  ) == 0)
      HtmlDrawSymbol(window, CBROWSER_SYMBOL_TYPE_DISC);
    else if (CStrUtil::casecmp(current_list->getSymbol(), "circle") == 0)
      HtmlDrawSymbol(window, CBROWSER_SYMBOL_TYPE_CIRCLE);
    else if (CStrUtil::casecmp(current_list->getSymbol(), "square") == 0)
      HtmlDrawSymbol(window, CBROWSER_SYMBOL_TYPE_SQUARE);
    else
      HtmlDrawSymbol(window, CBROWSER_SYMBOL_TYPE_DISC);

    HtmlDrawLabel(window, " ", 1, CHALIGN_TYPE_LEFT);
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
  if (current_list != NULL)
    list_stack.push_back(current_list);

  current_list = new CBrowserOutputList(CBROWSER_OUTPUT_LIST_TYPE_MENU);

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
    current_list = NULL;
}

static void
CBrowserOutputNobrStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  HtmlSetTextBreakup(false);
}

static void
CBrowserOutputNobrEndTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  HtmlSetTextBreakup(true);
}

static void
CBrowserOutputOlStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  if (current_list != NULL)
    list_stack.push_back(current_list);

  current_list = new CBrowserOutputList(CBROWSER_OUTPUT_LIST_TYPE_OL);

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
    current_list = NULL;
}

static void
CBrowserOutputOptionStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  std::string value;

  if (in_option)
    CBrowserOutputOptionEndTag(window, NULL);

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

  CBrowserFormStartOption(value, selected);
}

static void
CBrowserOutputOptionEndTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (tag != NULL && num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  /*-------------*/

  CBrowserFormEndOption(option_text);

  in_option   = false;
  option_text = "";
}

static void
CBrowserOutputPStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  CHAlignType align = CHALIGN_TYPE_LEFT;

  int num_options = tag->getNumOptions();

  for (int i = 0; i < num_options; i++)
    {
      const CHtmlTagOption *option = tag->getOption(i);

      std::string option_name = option->getName();

      if (CStrUtil::casecmp(option_name, "align") == 0)
        {
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

  HtmlSetTextBreakup(false);
  HtmlSetTextFormat (false);
}

static void
CBrowserOutputPreEndTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  /*-----------*/

  HtmlSetTextBreakup(true);
  HtmlSetTextFormat (true);

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
  std::string src;
  std::string language;

  int num_options = tag->getNumOptions();

  for (int i = 0; i < num_options; i++) {
    const CHtmlTagOption *option = tag->getOption(i);

    std::string option_name = option->getName();

    if      (CStrUtil::casecmp(option_name, "language") == 0)
      language = option->getValue();
    else if (CStrUtil::casecmp(option_name, "src") == 0)
      src = option->getValue();
    else
      window->displayError("Illegal %s Option '%s'\n", tag->getName().c_str(), option_name.c_str());
  }

  if (src != "") {
    if (CStrUtil::casecmp(language, "ceil") == 0)
      HtmlRunScriptFile(window, src);
  }
  else {
    in_script       = true;
    script_language = language;
    script_text     = "";
  }
}

static void
CBrowserOutputScriptEndTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  if (! in_script)
    return;

  in_script = false;

  if (script_text != "")
    HtmlRunScript(window, script_text);

  script_language = "";
  script_text     = "";
}

static void
CBrowserOutputSelectStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  std::string name;
  std::string onchange;

  bool multiple = false;
  int  size     = 1;

  int num_options = tag->getNumOptions();

  for (int i = 0; i < num_options; i++) {
    const CHtmlTagOption *option = tag->getOption(i);

    std::string option_name = option->getName();

    if      (CStrUtil::casecmp(option_name, "multiple") == 0)
      multiple = true;
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
      window->displayError("Illegal %s Option '%s'\n",
                           tag->getName().c_str(), option_name.c_str());
  }

  if (onchange != "")
    CBrowserFormSetInputOnChange(onchange);

  CBrowserFormStartSelect(window->getDocument(), name, size, multiple);
}

static void
CBrowserOutputSelectEndTag(CBrowserWindow *window, CHtmlTag *tag)
{
  if (in_option)
    CBrowserOutputOptionEndTag(window, NULL);

  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  CBrowserFormEndSelect(window->getDocument());
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

  HtmlSetTextStrike(true);
}

static void
CBrowserOutputStrikeEndTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  HtmlSetTextStrike(false);
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

  HtmlSetTextSubscript(true);
}

static void
CBrowserOutputSubEndTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  HtmlSetTextSubscript(false);
}

static void
CBrowserOutputSupStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  HtmlSetTextSuperscript(true);
}

static void
CBrowserOutputSupEndTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  HtmlSetTextSuperscript(false);
}

static void
CBrowserOutputTableStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  CHAlignType  halign           = CHALIGN_TYPE_NONE;
  std::string  bgcolor          = "";
  bool         border           = false;
  std::string  bordercolor      = "";
  std::string  bordercolorlight = "";
  std::string  bordercolordark  = "";
  int          cellpadding      = 1;
  int          cellspacing      = 2;
  int          hspace           = 0;
  CVAlignType  valign           = CVALIGN_TYPE_NONE;
  int          vspace           = 0;
  int          width            = 0;
  int          width_unit       = UNIT_NONE;

  int num_options = tag->getNumOptions();

  for (int i = 0; i < num_options; i++)
    {
      const CHtmlTagOption *option = tag->getOption(i);

      std::string option_name = option->getName();

      if      (CStrUtil::casecmp(option_name, "align") == 0)
        {
          if      (CStrUtil::casecmp(option->getValue(), "left") == 0)
            halign = CHALIGN_TYPE_LEFT;
          else if (CStrUtil::casecmp(option->getValue(), "right") == 0)
            halign = CHALIGN_TYPE_RIGHT;
          else
            window->displayError("Illegal Table Alignment %s\n", option->getValue().c_str());
        }
      else if (CStrUtil::casecmp(option_name, "bgcolor") == 0)
        bgcolor = option->getValue();
      else if (CStrUtil::casecmp(option_name, "border") == 0) {
        std::string value = option->getValue();

        if (value != "") {
          if (CStrUtil::isInteger(option->getValue()))
            border = (CStrUtil::toInteger(option->getValue()) != 0);
          else {
            window->displayError("Illegal Value for border %s\n", option->getValue().c_str());

            border = false;
          }
        }
        else
          border = true;
      }
      else if (CStrUtil::casecmp(option_name, "bordercolor") == 0)
        bordercolor = option->getValue();
      else if (CStrUtil::casecmp(option_name, "bordercolorlight") == 0)
        bordercolorlight = option->getValue();
      else if (CStrUtil::casecmp(option_name, "bordercolordark") == 0)
        bordercolordark = option->getValue();
      else if (CStrUtil::casecmp(option_name, "cellpadding") == 0) {
        if (CStrUtil::isInteger(option->getValue()))
          cellpadding = CStrUtil::toInteger(option->getValue());
        else {
          window->displayError("Illegal Value for cellpadding %s\n", option->getValue().c_str());

          cellpadding = 1;
        }
      }
      else if (CStrUtil::casecmp(option_name, "cellspacing") == 0) {
        if (CStrUtil::isInteger(option->getValue()))
          cellspacing = CStrUtil::toInteger(option->getValue());
        else {
          window->displayError("Illegal Value for cellspacing %s\n", option->getValue().c_str());

          cellspacing = 2;
        }
      }
      else if (CStrUtil::casecmp(option_name, "hspace") == 0) {
        if (CStrUtil::isInteger(option->getValue()))
          hspace = CStrUtil::toInteger(option->getValue());
        else {
          window->displayError("Illegal Value for hspace %s\n", option->getValue().c_str());

          hspace = 0;
        }
      }
      else if (CStrUtil::casecmp(option_name, "valign") == 0) {
          if      (CStrUtil::casecmp(option->getValue(), "top") == 0)
            valign = CVALIGN_TYPE_TOP;
          else if (CStrUtil::casecmp(option->getValue(), "bottom") == 0)
            valign = CVALIGN_TYPE_BOTTOM;
          else
            window->displayError("Illegal Table vertical alignment %s\n",
                                 option->getValue().c_str());
        }
      else if (CStrUtil::casecmp(option_name, "vspace") == 0) {
        if (CStrUtil::isInteger(option->getValue()))
          vspace = CStrUtil::toInteger(option->getValue());
        else {
          window->displayError("Illegal Value for vspace %s\n", option->getValue().c_str());

          vspace = 0;
        }
      }
      else if (CStrUtil::casecmp(option_name, "width") == 0) {
        int  len;

        len = option->getValue().size();

        std::string value = option->getValue();

        if (value[len - 1] == '%') {
          std::string value;

          width_unit = UNIT_PERCENT;

          value = option->getValue().substr(0, len - 1);

          if (CStrUtil::isInteger(option->getValue()))
            width = CStrUtil::toInteger(value);
          else {
            window->displayError("Illegal Value for Width %s\n", option->getValue().c_str());
            width = -1;
          }
        }
        else {
          width_unit = UNIT_PIXEL;

          if (CStrUtil::isInteger(option->getValue()))
            width = CStrUtil::toInteger(option->getValue());
          else {
            window->displayError("Illegal Value for Width %s\n", option->getValue().c_str());
            width = -1;
          }
        }
      }
      else
        window->displayError("Illegal Table Option Name '%s'\n", option_name.c_str());
    }

  /*-------------*/

  HtmlSetTableBorder(border);
  HtmlSetTableSpace (hspace, vspace);
  HtmlSetTableAlign (halign, valign);
  HtmlSetTableWidth (width, width_unit);

  HtmlSetTableCellPadding(cellpadding);
  HtmlSetTableCellSpacing(cellspacing);

  HtmlSetTableBackgroundColor(window, bgcolor);

  HtmlSetTableBorderColors(window, bordercolor, bordercolorlight, bordercolordark);

  HtmlStartTable(window);
}

static void
CBrowserOutputTableEndTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  /*-------------*/

  HtmlEndTable(window);
}

static void
CBrowserOutputTdStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  CHAlignType  halign           = CHALIGN_TYPE_NONE;
  std::string  bgcolor          = "";
  std::string  bordercolor      = "";
  std::string  bordercolorlight = "";
  std::string  bordercolordark  = "";
  int          colspan          = 1;
  int          rowspan          = 1;
  CVAlignType  valign           = CVALIGN_TYPE_NONE;
  int          width            = 0;
  int          width_unit       = UNIT_NONE;
  bool         wrap             = true;

  int num_options = tag->getNumOptions();

  for (int i = 0; i < num_options; i++)
    {
      const CHtmlTagOption *option = tag->getOption(i);

      std::string option_name = option->getName();

      if      (CStrUtil::casecmp(option_name, "align") == 0)
        {
          if      (CStrUtil::casecmp(option->getValue(), "left") == 0)
            halign = CHALIGN_TYPE_LEFT;
          else if (CStrUtil::casecmp(option->getValue(), "center") == 0)
            halign = CHALIGN_TYPE_CENTER;
          else if (CStrUtil::casecmp(option->getValue(), "right") == 0)
            halign = CHALIGN_TYPE_RIGHT;
          else
            window->displayError("Illegal Table Data Alignment %s\n", option->getValue().c_str());
        }
      else if (CStrUtil::casecmp(option_name, "bgcolor") == 0)
        bgcolor = option->getValue();
      else if (CStrUtil::casecmp(option_name, "bordercolor") == 0)
        bordercolor = option->getValue();
      else if (CStrUtil::casecmp(option_name, "bordercolorlight") == 0)
        bordercolorlight = option->getValue();
      else if (CStrUtil::casecmp(option_name, "bordercolordark") == 0)
        bordercolordark = option->getValue();
      else if (CStrUtil::casecmp(option_name, "colspan") == 0) {
        if (CStrUtil::isInteger(option->getValue()))
          colspan = CStrUtil::toInteger(option->getValue());
        else {
          window->displayError("Illegal Value for colspan %s\n", option->getValue().c_str());
          colspan = 1;
        }
      }
      else if (CStrUtil::casecmp(option_name, "nowrap") == 0) {
        wrap = false;

        std::string value = option->getValue();

        if (value[0] != '\0')
          window->displayError("No Value needed for nowrap\n");
      }
      else if (CStrUtil::casecmp(option_name, "rowspan") == 0) {
        if (CStrUtil::isInteger(option->getValue()))
          rowspan = CStrUtil::toInteger(option->getValue());
        else {
          window->displayError("Illegal Value for rowspan %s\n", option->getValue().c_str());
          rowspan = 1;
        }
      }
      else if (CStrUtil::casecmp(option_name, "valign") == 0) {
        if      (CStrUtil::casecmp(option->getValue(), "top") == 0)
          valign = CVALIGN_TYPE_TOP;
        else if (CStrUtil::casecmp(option->getValue(), "center") == 0)
          valign = CVALIGN_TYPE_CENTER;
        else if (CStrUtil::casecmp(option->getValue(), "bottom") == 0)
          valign = CVALIGN_TYPE_BOTTOM;
        else if (CStrUtil::casecmp(option->getValue(), "baseline") == 0)
          valign = CVALIGN_TYPE_BASELINE;
        else
          window->displayError("Illegal %s Vertical Alignment %s\n", "Table Data",
                               option->getValue().c_str());
      }
      else if (CStrUtil::casecmp(option_name, "width") == 0) {
        int  len;

        len = option->getValue().size();

        std::string value = option->getValue();

        if (value[len - 1] == '%') {
          width_unit = UNIT_PERCENT;

          std::string value = option->getValue().substr(len - 1);

          if (CStrUtil::isInteger(option->getValue()))
            width = CStrUtil::toInteger(value);
          else {
            window->displayError("Illegal Value for Width %s\n", option->getValue().c_str());
            width = -1;
          }
        }
        else {
          width_unit = UNIT_PIXEL;

          if (CStrUtil::isInteger(option->getValue()))
            width = CStrUtil::toInteger(option->getValue());
          else {
            window->displayError("Illegal Value for Width %s\n", option->getValue().c_str());
            width = -1;
          }
        }
      }
      else
        window->displayError("Illegal Td Option Name '%s'\n", option_name.c_str());
    }

  /*-------------*/

  HtmlSetTableCellType(DATA_CELL);

  HtmlSetTableCellAlign(halign, valign);
  HtmlSetTableCellWidth(width, width_unit);
  HtmlSetTableCellSpan (colspan, rowspan);
  HtmlSetTableCellWrap (wrap);

  HtmlSetTableCellBackgroundColor(window, bgcolor);

  HtmlSetTableCellBorderColors(window, bordercolor, bordercolorlight, bordercolordark);

  HtmlStartTableCell(window);
}

static void
CBrowserOutputTdEndTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  /*-------------*/

  HtmlEndTableCell(window);
}

static void
CBrowserOutputTextareaStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
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
    else if (CStrUtil::casecmp(option_name, "name") == 0)
      name = option->getValue();
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
      window->displayError("Illegal Td Option Name '%s'\n", option_name.c_str());
  }

  /*-------------*/

  CBrowserFormStartTextarea(window->getDocument(), name, rows, cols, wrap);

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
    CBrowserFormEndTextarea(window->getDocument(), &textarea_text[1]);
  else
    CBrowserFormEndTextarea(window->getDocument(), textarea_text);

  /*-------------*/

  in_textarea   = false;
  textarea_text = "";
}

static void
CBrowserOutputThStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  CHAlignType  halign           = CHALIGN_TYPE_NONE;
  std::string  bgcolor          = "";
  std::string  bordercolor      = "";
  std::string  bordercolorlight = "";
  std::string  bordercolordark  = "";
  int          colspan          = 1;
  int          rowspan          = 1;
  CVAlignType  valign           = CVALIGN_TYPE_NONE;
  int          width            = 0;
  int          width_unit       = UNIT_NONE;
  bool         wrap             = true;

  int num_options = tag->getNumOptions();

  for (int i = 0; i < num_options; i++)
    {
      const CHtmlTagOption *option = tag->getOption(i);

      std::string option_name = option->getName();

      if      (CStrUtil::casecmp(option_name, "align") == 0)
        {
          if      (CStrUtil::casecmp(option->getValue(), "left") == 0)
            halign = CHALIGN_TYPE_LEFT;
          else if (CStrUtil::casecmp(option->getValue(), "center") == 0)
            halign = CHALIGN_TYPE_CENTER;
          else if (CStrUtil::casecmp(option->getValue(), "right") == 0)
            halign = CHALIGN_TYPE_RIGHT;
          else
            window->displayError("Illegal Table Header Alignment %s\n",
                                 option->getValue().c_str());
        }
      else if (CStrUtil::casecmp(option_name, "bgcolor") == 0)
        bgcolor = option->getValue();
      else if (CStrUtil::casecmp(option_name, "bordercolor") == 0)
        bordercolor = option->getValue();
      else if (CStrUtil::casecmp(option_name, "bordercolorlight") == 0)
        bordercolorlight = option->getValue();
      else if (CStrUtil::casecmp(option_name, "bordercolordark") == 0)
        bordercolordark = option->getValue();
      else if (CStrUtil::casecmp(option_name, "colspan") == 0) {
        if (CStrUtil::isInteger(option->getValue()))
          colspan = CStrUtil::toInteger(option->getValue());
        else {
          window->displayError("Illegal Value for colspan %s\n", option->getValue().c_str());
          colspan = 1;
        }
      }
      else if (CStrUtil::casecmp(option_name, "nowrap") == 0) {
        wrap = false;

        std::string value = option->getValue();

        if (value[0] != '\0')
          window->displayError("No Value needed for nowrap\n");
      }
      else if (CStrUtil::casecmp(option_name, "rowspan") == 0) {
        if (CStrUtil::isInteger(option->getValue()))
          rowspan = CStrUtil::toInteger(option->getValue());
        else {
          window->displayError("Illegal Value for rowspan %s\n", option->getValue().c_str());
          rowspan = 1;
        }
      }
      else if (CStrUtil::casecmp(option_name, "valign") == 0)
        {
          if      (CStrUtil::casecmp(option->getValue(), "top") == 0)
            valign = CVALIGN_TYPE_TOP;
          else if (CStrUtil::casecmp(option->getValue(), "center") == 0)
            valign = CVALIGN_TYPE_CENTER;
          else if (CStrUtil::casecmp(option->getValue(), "bottom") == 0)
            valign = CVALIGN_TYPE_BOTTOM;
          else if (CStrUtil::casecmp(option->getValue(), "baseline") == 0)
            valign = CVALIGN_TYPE_BASELINE;
          else
            window->displayError("Illegal %s Vertical Alignment %s\n",
                                 "Table Header", option->getValue().c_str());
        }
      else if (CStrUtil::casecmp(option_name, "width") == 0) {
        int  len;

        len = option->getValue().size();

        std::string value = option->getValue();

        if (value[len - 1] == '%') {
          std::string value;

          width_unit = UNIT_PERCENT;

          value = option->getValue().substr(0, len - 1);

          if (CStrUtil::isInteger(option->getValue()))
            width = CStrUtil::toInteger(value);
          else {
            window->displayError("Illegal Value for Width %s\n", option->getValue().c_str());
            width = -1;
          }
        }
        else {
          width_unit = UNIT_PIXEL;

          if (CStrUtil::isInteger(option->getValue()))
            width = CStrUtil::toInteger(option->getValue());
          else {
            window->displayError("Illegal Value for Width %s\n", option->getValue().c_str());
            width = -1;
          }
        }
      }
      else
        window->displayError("Illegal Th Option Name '%s'\n", option_name.c_str());
    }

  /*-------------*/

  HtmlSetTableCellType(HEADER_CELL);

  HtmlSetTableCellAlign(halign, valign);
  HtmlSetTableCellWidth(width, width_unit);
  HtmlSetTableCellSpan (colspan, rowspan);
  HtmlSetTableCellWrap (wrap);

  HtmlSetTableCellBackgroundColor(window, bgcolor);

  HtmlSetTableCellBorderColors(window, bordercolor, bordercolorlight, bordercolordark);

  HtmlStartTableCell(window);
}

static void
CBrowserOutputThEndTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  HtmlEndTableCell(window);
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
  CHAlignType  halign           = CHALIGN_TYPE_NONE;
  std::string  bgcolor          = "";
  std::string  bordercolor      = "";
  std::string  bordercolorlight = "";
  std::string  bordercolordark  = "";
  CVAlignType  valign           = CVALIGN_TYPE_NONE;

  int num_options = tag->getNumOptions();

  for (int i = 0; i < num_options; i++)
    {
      const CHtmlTagOption *option = tag->getOption(i);

      std::string option_name = option->getName();

      if      (CStrUtil::casecmp(option_name, "align") == 0)
        {
          if      (CStrUtil::casecmp(option->getValue(), "left") == 0)
            halign = CHALIGN_TYPE_LEFT;
          else if (CStrUtil::casecmp(option->getValue(), "center") == 0)
            halign = CHALIGN_TYPE_CENTER;
          else if (CStrUtil::casecmp(option->getValue(), "right") == 0)
            halign = CHALIGN_TYPE_RIGHT;
          else
            window->displayError("Illegal Table Row Alignment %s\n", option->getValue().c_str());
        }
      else if (CStrUtil::casecmp(option_name, "bgcolor") == 0)
        bgcolor = option->getValue();
      else if (CStrUtil::casecmp(option_name, "bordercolor") == 0)
        bordercolor = option->getValue();
      else if (CStrUtil::casecmp(option_name, "bordercolorlight") == 0)
        bordercolorlight = option->getValue();
      else if (CStrUtil::casecmp(option_name, "bordercolordark") == 0)
        bordercolordark = option->getValue();
      else if (CStrUtil::casecmp(option_name, "valign") == 0)
        {
          if      (CStrUtil::casecmp(option->getValue(), "top") == 0)
            valign = CVALIGN_TYPE_TOP;
          else if (CStrUtil::casecmp(option->getValue(), "center") == 0)
            valign = CVALIGN_TYPE_CENTER;
          else if (CStrUtil::casecmp(option->getValue(), "bottom") == 0)
            valign = CVALIGN_TYPE_BOTTOM;
          else if (CStrUtil::casecmp(option->getValue(), "baseline") == 0)
            valign = CVALIGN_TYPE_BASELINE;
          else
            window->displayError("Illegal %s Vertical Alignment %s\n",
                                 "Table Row", option->getValue().c_str());
        }
      else
        window->displayError("Illegal Tr Option Name '%s'\n", option_name.c_str());
    }

  /*-------------*/

  HtmlSetTableRowAlign(halign, valign);

  HtmlSetTableRowBackgroundColor(window, bgcolor);

  HtmlSetTableRowBorderColors(window, bordercolor, bordercolorlight, bordercolordark);

  HtmlStartTableRow(window);
}

static void
CBrowserOutputTrEndTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  /*-------------*/

  HtmlEndTableRow(window);
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

  HtmlSetTextUnderline(true);
}

static void
CBrowserOutputUEndTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  HtmlSetTextUnderline(false);
}

static void
CBrowserOutputUlStartTag(CBrowserWindow *window, CHtmlTag *tag)
{
  if (current_list != NULL)
    list_stack.push_back(current_list);

  current_list = new CBrowserOutputList(CBROWSER_OUTPUT_LIST_TYPE_UL);

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
    current_list = NULL;
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

  HtmlSetTextBreakup(false);
  HtmlSetTextFormat (false);
}

static void
CBrowserOutputXmpEndTag(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for %s\n", tag->getName().c_str());

  HtmlSetTextBreakup(true);
  HtmlSetTextFormat (true);

  window->endFontFace();

  CBrowserOutputSkipLine(window);
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
