#include <CBrowserOutput.h>
#include <CBrowserOutputTag.h>
#include <CBrowserAddress.h>
#include <CBrowserBaseFont.h>
#include <CBrowserBlockQuote.h>
#include <CBrowserBody.h>
#include <CBrowserBreak.h>
#include <CBrowserCanvas.h>
#include <CBrowserCenter.h>
#include <CBrowserCode.h>
#include <CBrowserDiv.h>
#include <CBrowserDocument.h>
#include <CBrowserForm.h>
#include <CBrowserHead.h>
#include <CBrowserHeader.h>
#include <CBrowserHtml.h>
#include <CBrowserImage.h>
#include <CBrowserKbd.h>
#include <CBrowserLabel.h>
#include <CBrowserLink.h>
#include <CBrowserList.h>
#include <CBrowserMisc.h>
#include <CBrowserMeta.h>
#include <CBrowserNoBr.h>
#include <CBrowserOutputList.h>
#include <CBrowserParagraph.h>
#include <CBrowserPre.h>
#include <CBrowserRule.h>
#include <CBrowserSamp.h>
#include <CBrowserScript.h>
#include <CBrowserStyle.h>
#include <CBrowserSymbol.h>
#include <CBrowserTable.h>
#include <CBrowserText.h>
#include <CBrowserTitle.h>
#include <CBrowserTT.h>
#include <CBrowserWindow.h>
#include <CBrowserXmp.h>
#include <CBrowserMain.h>
#include <CBrowserCeil.h>
#include <CBrowserJS.h>
#include <CHtmlLib.h>
#include <CRGBName.h>

CBrowserObject *
CBrowserFlushTags(CBrowserWindow *window, CHtmlTag *tag, CHtmlTagId id, bool flushFound=false)
{
  CBrowserObject *currentObj = window->currentObj();

  while (currentObj && currentObj->type() != id) {
    CBrowserOutputTagBase *output_data = CBrowserOutputTagMgrInst->getTag(currentObj->type());

    if (! output_data || output_data->isEmpty()) {
      if (currentObj->type() != CHtmlTagId::TEXT)
        window->displayError("End Tag '%s' not implemented\n", currentObj->typeName().c_str());

      window->endObject();

      currentObj = window->currentObj();

      continue;
    }

std::cerr << "Flush Tag '" << currentObj->typeName() << "' for '" <<
             tag->getName() << "'" << std::endl;

    //---

    // terminate object (used ?)
    output_data->term(window, nullptr);

    output_data->end(window, currentObj, nullptr);

    //---

    // update DOM
    window->endObject();

    //---

    currentObj = window->currentObj();
  }

  if (currentObj && currentObj->type() == id) {
    if (flushFound) {
      CBrowserOutputTagBase *output_data = CBrowserOutputTagMgrInst->getTag(currentObj->type());

std::cerr << "Flush Tag '" << currentObj->typeName() << "' for '" <<
             tag->getName() << "'" << std::endl;

      //---

      // terminate object (used ?)
      output_data->term(window, nullptr);

      output_data->end(window, currentObj, nullptr);

      //---

      // update DOM
      window->endObject();

      return nullptr;
    }
    else
      return currentObj;
  }

  return nullptr;
}

//------

CBrowserOutput::
CBrowserOutput(CBrowserWindow *window) :
 window_(window)
{
}

void
CBrowserOutput::
processTokens(const CHtmlParserTokens &tokens)
{
  typedef std::vector<CHtmlTag *> TagStack;

  TagStack tagStack;

  init();

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

      processTag(tag);
    }
    else if (t->isText()) {
      CHtmlText *text = t->getText();

      CHtmlTag *currentTag = (! tagStack.empty() ? tagStack.back() : nullptr);

      if (currentTag && currentTag->getTagDef().getId() == CHtmlTagId::CANVAS)
        continue;

      processText(text);
    }
  }

  term();
}

void
CBrowserOutput::
layoutObjects()
{
  CBrowserObject *root = window_->rootObject();

  if (root)
    layoutObj(root);
}

void
CBrowserOutput::
layoutObj(CBrowserObject *obj)
{
  obj->initLayout();

  for (const auto &c : obj->children())
    layoutObj(c);

  obj->termLayout();
}

void
CBrowserOutput::
init()
{
  window_->setTextColor(window_->currentFontColor());
}

void
CBrowserOutput::
term()
{
}

void
CBrowserOutput::
processTag(CHtmlTag *tag)
{
  CBrowserOutputTagBase *output_data = CBrowserOutputTagMgrInst->getTag(tag);

  if (! output_data) {
    window_->displayError("Tag '%s' not implemented\n", tag->getName().c_str());
    return;
  }

  if (tag->isStartTag())
    processStartTag(tag, output_data);
  else
    processEndTag(tag, output_data);
}

void
CBrowserOutput::
processText(CHtmlText *text)
{
  std::string str = text->getText();

  std::string sstr = CStrUtil::stripSpaces(str);

  //if (sstr == "") return;

  CBrowserObject *currentObj = window_->currentObj();

  if (currentObj) {
    if      (currentObj->type() == CHtmlTagId::TITLE)
      window_->getDocument()->setTitle(sstr);
    else if (currentObj->type() == CHtmlTagId::SCRIPT) {
      CBrowserScript *script = dynamic_cast<CBrowserScript *>(currentObj);

      script->setText(script->text() + str);
    }
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
    }
    else {
      std::string lstr = CStrUtil::stripSpaces(str, /*front*/false, /*back*/true);

      if (lstr != "") {
        CBrowserText *textObj = new CBrowserText(window_, lstr, window_->textData());

        window_->startObject(textObj, /*add*/true);
      }
    }
  }
  else {
    std::string lstr = CStrUtil::stripSpaces(str, /*front*/false, /*back*/true);

    if (lstr != "") {
      CBrowserText *textObj = new CBrowserText(window_, lstr, window_->textData());

      window_->startObject(textObj, /*add*/true);
    }
  }

  if (currentObj)
    currentObj->setText(sstr);
}

void
CBrowserOutput::
processStartTag(CHtmlTag *tag, CBrowserOutputTagBase *output_data)
{
//std::cerr << "start '" << tag->getName() << "'" << std::endl;

  // initialize object from tag options
  CBrowserObject *obj = output_data->start(window_, tag);

  if (! obj) {
    window_->displayError("No browser object for '%s'\n", tag->getName().c_str());
    return;
  }

  // add object to DOM
  window_->startObject(obj, /*add*/true);

  // post-initialize object
  output_data->init(window_, obj);

  // init process state
  obj->initProcess();

  //---

  // auto end empty tag (no end tag)
  if (output_data->isEmpty()) {
    // term process state
    obj->termProcess();

    // update DOM
    window_->endObject();
  }
}

void
CBrowserOutput::
processEndTag(CHtmlTag *tag, CBrowserOutputTagBase *output_data)
{
//std::cerr << "end '" << tag->getName() << "'" << std::endl;

  if (output_data->isEmpty()) {
    window_->displayError("Unexpected End Tag '%s' for empty tag.\n", tag->getName().c_str());
    return;
  }

  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window_->displayError("Invalid Number of Options for '%s'\n", tag->getName().c_str());

  //---

  // flush open tags
  CBrowserObject *obj = CBrowserFlushTags(window_, tag, output_data->id());

  if (! obj)
    window_->displayError("End Tag '%s' with no Start Tag.\n", tag->getName().c_str());

  //---

  // terminate object (used ?)
  output_data->term(window_, tag);

  output_data->end(window_, obj, tag);

  //---

  // term process state
  if (obj) {
    obj->termProcess();

    // update DOM
    window_->endObject();
  }
}

//------

CBrowserObject *
CBrowserOutputATag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserLinkData linkData;

  int num_options = tag->getNumOptions();

  for (int i = 0; i < num_options; i++) {
    const CHtmlTagOption *option = tag->getOption(i);

    std::string option_name = CStrUtil::toLower(option->getName());

    if      (option_name == "download")
      linkData.download = option->getValue();
    else if (option_name == "href")
      linkData.href = option->getValue();
    else if (option_name == "id")
      linkData.id = option->getValue();
    else if (option_name == "methods")
      linkData.methods = option->getValue();
    else if (option_name == "name")
      linkData.id = option->getValue();
    else if (option_name == "rel")
      linkData.rel = option->getValue();
    else if (option_name == "rev")
      linkData.rev = option->getValue();
    else if (option_name == "target")
      linkData.target = option->getValue();
    else if (option_name == "type")
      linkData.type = option->getValue();
    else if (option_name == "title")
      linkData.title = option->getValue();
    else if (option_name == "url")
      linkData.url = option->getValue();
    else
      window->displayError("Illegal 'a' Option Name '%s'\n", option_name.c_str());
  }

  //---

  if (linkData.href == "" && linkData.id == "")
    window->displayError("No 'href' or 'name' specified for 'a' Tag'\n");

  //---

  CBrowserLinkObj *link = new CBrowserLinkObj(window, linkData);

  return link;
}

CBrowserObject *
CBrowserOutputAddressTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for '%s'\n", tag->getName().c_str());

  //---

  CBrowserAddress *address = new CBrowserAddress(window);

  return address;
}

CBrowserObject *
CBrowserOutputBTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for '%s'\n", tag->getName().c_str());

  //---

  CBrowserBStyle *style = new CBrowserBStyle(window);

  return style;
}

CBrowserObject *
CBrowserOutputBasefontTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserBaseFontData data;

  int num_options = tag->getNumOptions();

  for (int i = 0; i < num_options; i++) {
    const CHtmlTagOption *option = tag->getOption(i);

    std::string option_name = CStrUtil::toLower(option->getName());

    if (option_name == "size") {
      std::string value = option->getValue();

      if      (value[0] == '+')
        data.sizeSign = 1;
      else if (value[0] == '-')
        data.sizeSign = -1;
      else
        data.sizeSign = 0;

      if (CStrUtil::isInteger(option->getValue())) {
        data.size    = CStrUtil::toInteger(option->getValue());
        data.sizeSet = true;
      }
      else
        window->displayError("Illegal 'basefont' Value for Size '%s'\n",
                             option->getValue().c_str());
    }
    else
      window->displayError("Illegal 'basefont' Option Name '%s'\n", option_name.c_str());
  }

  //---

  CBrowserBaseFont *baseFont = new CBrowserBaseFont(window, data);

  return baseFont;
}

CBrowserObject *
CBrowserOutputBigTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for '%s'\n", tag->getName().c_str());

  //---

  CBrowserBigStyle *style = new CBrowserBigStyle(window);

  return style;
}

CBrowserObject *
CBrowserOutputBlinkTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for '%s'\n", tag->getName().c_str());

  //---

  CBrowserBlinkStyle *style = new CBrowserBlinkStyle(window);

  return style;
}

CBrowserObject *
CBrowserOutputBlockquoteTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for '%s'\n", tag->getName().c_str());

  //---

  CBrowserBlockQuote *blockQuote = new CBrowserBlockQuote(window);

  return blockQuote;
}

CBrowserObject *
CBrowserOutputBodyTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserBodyData bodyData;

  for (const auto &option: tag->getOptions()) {
    std::string option_name = CStrUtil::toLower(option->getName());

    if      (option_name == "alink")
      bodyData.alink = option->getValue();
    else if (option_name == "background")
      bodyData.background = option->getValue();
    else if (option_name == "bgcolor")
      bodyData.bgcolor = option->getValue();
    else if (option_name == "bgproperties") {
      if      (CStrUtil::casecmp(option->getValue(), "fixed") == 0)
        bodyData.fixed = true;
      else
        window->displayError("Illegal 'body' bgproperties value '%s'\n",
                             option->getValue().c_str());
    }
    else if (option_name == "leftmargin") {
      if (CStrUtil::isInteger(option->getValue()))
        bodyData.leftmargin = CStrUtil::toInteger(option->getValue());
      else {
        window->displayError("Illegal 'body' Value for leftmargin '%s'\n",
                             option->getValue().c_str());

        bodyData.leftmargin = CBrowserDataConstants::LEFT_MARGIN;
      }
    }
    else if (option_name == "link")
      bodyData.link = option->getValue();
    else if (option_name == "text")
      bodyData.text = option->getValue();
    else if (option_name == "topmargin") {
      if (CStrUtil::isInteger(option->getValue()))
        bodyData.topmargin = CStrUtil::toInteger(option->getValue());
      else {
        window->displayError("Illegal 'body' Value for topmargin '%s'\n",
                             option->getValue().c_str());

        bodyData.topmargin = CBrowserDataConstants::TOP_MARGIN;
      }
    }
    else if (option_name == "vlink")
      bodyData.vlink = option->getValue();
    else
      window->displayError("Illegal 'body' Option Name '%s'\n", option_name.c_str());
  }

  //---

  CBrowserBody *body = new CBrowserBody(window, bodyData);

  return body;
}

CBrowserObject *
CBrowserOutputBrTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserBreakData breakData;

  int num_options = tag->getNumOptions();

  for (int i = 0; i < num_options; i++) {
    const CHtmlTagOption *option = tag->getOption(i);

    std::string option_name = option->getName();

    if (CStrUtil::casecmp(option_name, "clear") == 0) {
      if      (CStrUtil::casecmp(option->getValue(), "left" ) == 0)
        breakData.clear = CHtmlLayoutClearType::LEFT;
      else if (CStrUtil::casecmp(option->getValue(), "right") == 0)
        breakData.clear = CHtmlLayoutClearType::RIGHT;
      else if (CStrUtil::casecmp(option->getValue(), "all"  ) == 0)
        breakData.clear = CHtmlLayoutClearType::ALL;
      else
        window->displayError("Illegal 'br' Clear '%s'\n", option->getValue().c_str());
    }
    else
      window->displayError("Illegal 'br' Option Name '%s'\n", option_name.c_str());
  }

  //---

  CBrowserBreak *breaker = new CBrowserBreak(window, breakData);

  return breaker;
}

CBrowserObject *
CBrowserOutputCanvasTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserCanvasData data;

  int num_options = tag->getNumOptions();

  for (int i = 0; i < num_options; i++) {
    const CHtmlTagOption *option = tag->getOption(i);

    std::string option_name = CStrUtil::toLower(option->getName());

    if      (option_name == "id") {
      data.id = option->getValue();
    }
    else if (option_name == "width") {
      if (CStrUtil::isInteger(option->getValue()))
        data.width = CStrUtil::toInteger(option->getValue());
      else {
        window->displayError("Illegal 'canvas' Value for Width '%s'\n",
                             option->getValue().c_str());
        data.width = -1;
      }
    }
    else if (option_name == "height") {
      if (CStrUtil::isInteger(option->getValue()))
        data.height = CStrUtil::toInteger(option->getValue());
      else {
        window->displayError("Illegal 'canvas' Value for Height '%s'\n",
                             option->getValue().c_str());
        data.height = -1;
      }
    }
    else if (option_name == "style") {
      data.style = option->getValue();
    }
    else
      window->displayError("Illegal 'canvas' Option Name '%s'\n", option_name.c_str());
  }

  //---

  CBrowserCanvas *canvas = new CBrowserCanvas(window, data);

  return canvas;
}

CBrowserObject *
CBrowserOutputCaptionTag::start(CBrowserWindow *window, CHtmlTag *tag)
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
        window->displayError("Illegal '%s' Alignment '%s'\n", "Table Caption",
                             option->getValue().c_str());
    }
    else if (CStrUtil::casecmp(option_name, "valign") == 0) {
      if      (CStrUtil::casecmp(option->getValue(), "top") == 0)
        captionData.valign = CVALIGN_TYPE_TOP;
      else if (CStrUtil::casecmp(option->getValue(), "bottom") == 0)
        captionData.valign = CVALIGN_TYPE_BOTTOM;
      else
        window->displayError("Illegal '%s' Vertical Alignment '%s'\n", "Table Caption",
                             option->getValue().c_str());
    }
    else
      window->displayError("Illegal 'caption' Option Name '%s'\n", option_name.c_str());
  }

  //---

  CBrowserTableCaption *caption = new CBrowserTableCaption(window, captionData);

  return caption;
}

CBrowserObject *
CBrowserOutputCenterTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for '%s'\n", tag->getName().c_str());

  //---

  CBrowserCenter *center = new CBrowserCenter(window);

  return center;
}

CBrowserObject *
CBrowserOutputCiteTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for '%s'\n", tag->getName().c_str());

  //---

  CBrowserCiteStyle *style = new CBrowserCiteStyle(window);

  return style;
}

CBrowserObject *
CBrowserOutputCodeTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for '%s'\n", tag->getName().c_str());

  //---

  CBrowserCode *code = new CBrowserCode(window);

  return code;
}

CBrowserObject *
CBrowserOutputDdTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserObject *currentObj = window->currentObj();

  if (currentObj) {
    if      (currentObj->type() == CHtmlTagId::DT)
      CBrowserFlushTags(window, tag, CHtmlTagId::DT, true);
    else if (currentObj->type() == CHtmlTagId::DD)
      CBrowserFlushTags(window, tag, CHtmlTagId::DD, true);
  }

  //---

  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for '%s'\n", tag->getName().c_str());

  //---

  CBrowserDataListData *dd = new CBrowserDataListData(window);

  return dd;
}

CBrowserObject *
CBrowserOutputDfnTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for '%s'\n", tag->getName().c_str());

  //---

  CBrowserDfnStyle *dfn = new CBrowserDfnStyle(window);

  return dfn;
}

CBrowserObject *
CBrowserOutputDirTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserOutputListData listData;

  int num_options = tag->getNumOptions();

  for (int i = 0; i < num_options; i++) {
    const CHtmlTagOption *option = tag->getOption(i);

    std::string option_name = option->getName();

    if      (CStrUtil::casecmp(option_name, "compact") == 0) {
      listData.compact = true;

      std::string value = option->getValue();

      if (value[0] != '\0')
        window->displayError("No Value needed for Compact\n");
    }
    else if (CStrUtil::casecmp(option_name, "type") == 0)
      listData.symbol = option->getValue();
    else
      window->displayError("Illegal 'dir' Option Name '%s'\n", option_name.c_str());
  }

  //---

  CBrowserOutputList *list = new CBrowserOutputList(window, CHtmlTagId::DIR, listData);

  return list;
}

CBrowserObject *
CBrowserOutputDivTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserDivData divData;

  int num_options = tag->getNumOptions();

  for (int i = 0; i < num_options; i++) {
    const CHtmlTagOption *option = tag->getOption(i);

    std::string option_name = CStrUtil::toLower(option->getName());

    if      (option_name == "id") {
      divData.id = option->getValue();
    }
    else if (option_name == "style") {
      divData.style = option->getValue();
    }
    else
      window->displayError("Illegal 'div' Option Name '%s'\n", option_name.c_str());
  }

  //---

  CBrowserDiv *div = new CBrowserDiv(window, divData);

  return div;
}

CBrowserObject *
CBrowserOutputDlTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserOutputListData listData;

  int num_options = tag->getNumOptions();

  for (int i = 0; i < num_options; i++) {
    const CHtmlTagOption *option = tag->getOption(i);

    std::string option_name = option->getName();

    if      (CStrUtil::casecmp(option_name, "compact") == 0) {
      listData.compact = true;

      std::string value = option->getValue();

      if (value[0] != '\0')
        window->displayError("No Value needed for Compact\n");
    }
    else
      window->displayError("Illegal 'dl' Option Name '%s'\n", option_name.c_str());
  }

  //---

  CBrowserOutputList *list = new CBrowserOutputList(window, CHtmlTagId::DL, listData);

  return list;
}

void
CBrowserOutputDlTag::term(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserObject *currentObj = window->currentObj();

  if (currentObj) {
    if      (currentObj->type() == CHtmlTagId::DT)
      CBrowserFlushTags(window, tag, CHtmlTagId::DT, true);
    else if (currentObj->type() == CHtmlTagId::DD)
      CBrowserFlushTags(window, tag, CHtmlTagId::DD, true);
  }
}

CBrowserObject *
CBrowserOutputDtTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserObject *currentObj = window->currentObj();

  if (currentObj) {
    if      (currentObj->type() == CHtmlTagId::DT)
      CBrowserFlushTags(window, tag, CHtmlTagId::DT, true);
    else if (currentObj->type() == CHtmlTagId::DD)
      CBrowserFlushTags(window, tag, CHtmlTagId::DD, true);
  }

  //---

  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for '%s'\n", tag->getName().c_str());

  //---

  CBrowserDataListTerm *dt = new CBrowserDataListTerm(window);

  return dt;
}

CBrowserObject *
CBrowserOutputEmTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for '%s'\n", tag->getName().c_str());

  //---

  CBrowserEmStyle *style = new CBrowserEmStyle(window);

  return style;
}

CBrowserObject *
CBrowserOutputFontTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserFontData fontData;

  fontData.color = window->getFontColor();
  fontData.face  = window->getCurrentFontFace();
  fontData.size  = window->getFontSize();
  fontData.delta = 1;

  //---

  int num_options = tag->getNumOptions();

  for (int i = 0; i < num_options; i++) {
    const CHtmlTagOption *option = tag->getOption(i);

    std::string option_name = option->getName();

    if      (CStrUtil::casecmp(option_name, "color") == 0)
      fontData.color = option->getValue();
    else if (CStrUtil::casecmp(option_name, "face") == 0)
      fontData.face = option->getValue();
    else if (CStrUtil::casecmp(option_name, "size") == 0) {
      std::string value = option->getValue();

      if      (value[0] == '+')
        fontData.delta =  1;
      else if (value[0] == '-')
        fontData.delta = -1;
      else
        fontData.delta =  0;

      if (CStrUtil::isInteger(option->getValue()))
        fontData.size = CStrUtil::toInteger(option->getValue());
      else {
        window->displayError("Illegal 'font' Value for Size '%s'\n", option->getValue().c_str());

        fontData.size  = -1;
        fontData.delta = 1;
      }
    }
    else
      window->displayError("Illegal 'font' Option Name '%s'\n", option_name.c_str());
  }

  //---

  CBrowserFontStyle *font = new CBrowserFontStyle(window, fontData);

  return font;
}

CBrowserObject *
CBrowserOutputFormTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserFormData formData;

  int num_options = tag->getNumOptions();

  for (int i = 0; i < num_options; i++) {
    const CHtmlTagOption *option = tag->getOption(i);

    std::string option_name = option->getName();

    if      (CStrUtil::casecmp(option_name, "action") == 0)
      formData.action = option->getValue();
    else if (CStrUtil::casecmp(option_name, "enctype") == 0)
      ;
    else if (CStrUtil::casecmp(option_name, "method") == 0) {
      if      (CStrUtil::casecmp(option->getValue(), "get" ) == 0)
        formData.method = CBrowserFormMethodType::GET;
      else if (CStrUtil::casecmp(option->getValue(), "post") == 0)
        formData.method = CBrowserFormMethodType::POST;
      else
        window->displayError("Illegal 'form' method '%s'\n", option->getValue().c_str());
    }
    else if (CStrUtil::casecmp(option_name, "name") == 0)
      formData.name = option->getValue();
    else
      window->displayError("Illegal '%s' Option Name '%s'\n",
                           tag->getName().c_str(), option_name.c_str());
  }

  //----

  CBrowserForm *form = new CBrowserForm(window, formData);

  return form;
}

CBrowserObject *
CBrowserOutputH1Tag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserHeaderData headerData;

  int num_options = tag->getNumOptions();

  for (int i = 0; i < num_options; i++) {
    const CHtmlTagOption *option = tag->getOption(i);

    std::string option_name = option->getName();

    if (CStrUtil::casecmp(option_name, "align") == 0)
      window->parseHAlignOption(option->getValue(), headerData.align);
    else
      window->displayError("Illegal 'h1' Option Name '%s'\n", option_name.c_str());
  }

  //---

  CBrowserHeader *header = new CBrowserHeader(window, CHtmlTagId::H1, headerData);

  return header;
}

CBrowserObject *
CBrowserOutputH2Tag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserHeaderData headerData;

  int num_options = tag->getNumOptions();

  for (int i = 0; i < num_options; i++) {
    const CHtmlTagOption *option = tag->getOption(i);

    std::string option_name = option->getName();

    if (CStrUtil::casecmp(option_name, "align") == 0)
      window->parseHAlignOption(option->getValue(), headerData.align);
    else
      window->displayError("Illegal 'h2' Option Name '%s'\n", option_name.c_str());
  }

  //---

  CBrowserHeader *header = new CBrowserHeader(window, CHtmlTagId::H2, headerData);

  return header;
}

CBrowserObject *
CBrowserOutputH3Tag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserHeaderData headerData;

  int num_options = tag->getNumOptions();

  for (int i = 0; i < num_options; i++) {
    const CHtmlTagOption *option = tag->getOption(i);

    std::string option_name = option->getName();

    if (CStrUtil::casecmp(option_name, "align") == 0)
      window->parseHAlignOption(option->getValue(), headerData.align);
    else
      window->displayError("Illegal 'h3' Option Name '%s'\n", option_name.c_str());
  }

  //---

  CBrowserHeader *header = new CBrowserHeader(window, CHtmlTagId::H3, headerData);

  return header;
}

CBrowserObject *
CBrowserOutputH4Tag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserHeaderData headerData;

  int num_options = tag->getNumOptions();

  for (int i = 0; i < num_options; i++) {
    const CHtmlTagOption *option = tag->getOption(i);

    std::string option_name = option->getName();

    if (CStrUtil::casecmp(option_name, "align") == 0)
      window->parseHAlignOption(option->getValue(), headerData.align);
    else
      window->displayError("Illegal 'h4' Option Name '%s'\n", option_name.c_str());
  }

  //---

  CBrowserHeader *header = new CBrowserHeader(window, CHtmlTagId::H4, headerData);

  return header;
}

CBrowserObject *
CBrowserOutputH5Tag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserHeaderData headerData;

  int num_options = tag->getNumOptions();

  for (int i = 0; i < num_options; i++) {
    const CHtmlTagOption *option = tag->getOption(i);

    std::string option_name = option->getName();

    if (CStrUtil::casecmp(option_name, "align") == 0)
      window->parseHAlignOption(option->getValue(), headerData.align);
    else
      window->displayError("Illegal 'h5' Option Name '%s'\n", option_name.c_str());
  }

  //---

  CBrowserHeader *header = new CBrowserHeader(window, CHtmlTagId::H5, headerData);

  return header;
}

CBrowserObject *
CBrowserOutputH6Tag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserHeaderData headerData;

  int num_options = tag->getNumOptions();

  for (int i = 0; i < num_options; i++) {
    const CHtmlTagOption *option = tag->getOption(i);

    std::string option_name = option->getName();

    if (CStrUtil::casecmp(option_name, "align") == 0)
      window->parseHAlignOption(option->getValue(), headerData.align);
    else
      window->displayError("Illegal 'h6' Option Name '%s'\n", option_name.c_str());
  }

  //---

  CBrowserHeader *header = new CBrowserHeader(window, CHtmlTagId::H6, headerData);

  return header;
}

CBrowserObject *
CBrowserOutputHeadTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for '%s'\n", tag->getName().c_str());

  //---

  CBrowserHead *head = new CBrowserHead(window);

  return head;
}

CBrowserObject *
CBrowserOutputHrTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserRuleData ruleData;

  int num_options = tag->getNumOptions();

  for (int i = 0; i < num_options; i++) {
    const CHtmlTagOption *option = tag->getOption(i);

    std::string option_name = option->getName();

    if      (CStrUtil::casecmp(option_name, "align") == 0) {
      window->parseHAlignOption(option->getValue(), ruleData.align);
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
        window->displayError("Illegal 'hr' Value for Size '%s'\n", option->getValue().c_str());
        ruleData.size = -1;
      }
    }
    else if (CStrUtil::casecmp(option_name, "width") == 0) {
      uint len = option->getValue().size();

      std::string value = option->getValue();

      if (value[len - 1] == '%') {
        ruleData.unit = CBrowserUnitsType::PERCENT;

        std::string value1 = value.substr(0, len - 1);

        if (CStrUtil::isInteger(value1))
          ruleData.width = CStrUtil::toInteger(value1);
        else {
          window->displayError("Illegal 'hr' Value for Width '%s'%\n", value1.c_str());
          ruleData.width = -1;
        }
      }
      else {
        ruleData.unit = CBrowserUnitsType::PIXEL;

        if (CStrUtil::isInteger(option->getValue()))
          ruleData.width = CStrUtil::toInteger(option->getValue());
        else {
          window->displayError("Illegal 'hr' Value for Width '%s'\n", option->getValue().c_str());
          ruleData.width = -1;
        }
      }
    }
    else
      window->displayError("Illegal 'hr' Option Name '%s'\n", option_name.c_str());
  }

  //---

  CBrowserRule *rule = new CBrowserRule(window, ruleData);

  return rule;
}

CBrowserObject *
CBrowserOutputHtmlTag::start(CBrowserWindow *window, CHtmlTag *tag)
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

  CBrowserHtml *html = new CBrowserHtml(window);

  return html;
}

CBrowserObject *
CBrowserOutputITag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for '%s'\n", tag->getName().c_str());

  //---

  CBrowserIStyle *style = new CBrowserIStyle(window);

  return style;
}

CBrowserObject *
CBrowserOutputImgTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserImageData imageData;

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
        window->displayError("Illegal 'img' Align '%s'\n", option->getValue().c_str());
    }
    else if (CStrUtil::casecmp(option_name, "alt") == 0) {
      imageData.alt = option->getValue();
    }
    else if (CStrUtil::casecmp(option_name, "border") == 0) {
      if (CStrUtil::isInteger(option->getValue()))
        imageData.border = CStrUtil::toInteger(option->getValue());
      else {
        window->displayError("Illegal 'img' Value for Border '%s'\n", option->getValue().c_str());
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
        window->displayError("Illegal 'img' Value for Height '%s'\n", option->getValue().c_str());
        imageData.height = -1;
      }
    }
    else if (CStrUtil::casecmp(option_name, "hspace") == 0) {
      if (CStrUtil::isInteger(option->getValue()))
        imageData.hspace = CStrUtil::toInteger(option->getValue());
      else {
        window->displayError("Illegal 'img' Value for HSpace '%s'\n", option->getValue().c_str());
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
        window->displayError("Illegal 'img' Value for VSpace '%s'\n", option->getValue().c_str());
        imageData.vspace = 2;
      }
    }
    else if (CStrUtil::casecmp(option_name, "width") == 0) {
      if (CStrUtil::isInteger(option->getValue()))
        imageData.width = CStrUtil::toInteger(option->getValue());
      else {
        window->displayError("Illegal 'img' Value for Width '%s'\n", option->getValue().c_str());
        imageData.width = -1;
      }
    }
    else
      window->displayError("Illegal 'img' Option Name '%s'\n", option_name.c_str());
  }

  if (imageData.src == "") {
    window->displayError("No Image Source Specified\n");
    return 0;
  }

  //---

  CBrowserObject *imageOrText = window->addImage(imageData);

  return imageOrText;
}

CBrowserObject *
CBrowserOutputInputTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserFormInputData inputData;

  int num_options = tag->getNumOptions();

  for (int i = 0; i < num_options; i++) {
    const CHtmlTagOption *option = tag->getOption(i);

    std::string option_name = option->getName();

    if      (CStrUtil::casecmp(option_name, "align") == 0)
      inputData.align = option->getValue();
    else if (CStrUtil::casecmp(option_name, "checked") == 0)
      inputData.checked = true;
    else if (CStrUtil::casecmp(option_name, "maxlength") == 0) {
      if (CStrUtil::isInteger(option->getValue()))
        inputData.maxlength = CStrUtil::toInteger(option->getValue());
      else {
        window->displayError("Illegal 'input' Value '%s' for '%s'\n",
                             option->getValue().c_str(), option_name.c_str());
        inputData.maxlength = -1;
      }
    }
    else if (CStrUtil::casecmp(option_name, "id") == 0)
      inputData.id = option->getValue();
    else if (CStrUtil::casecmp(option_name, "class") == 0)
      inputData.classStr = option->getValue();
    else if (CStrUtil::casecmp(option_name, "name") == 0)
      inputData.name = option->getValue();
    else if (CStrUtil::casecmp(option_name, "placeholder") == 0)
      inputData.placeholder = option->getValue();
    else if (CStrUtil::casecmp(option_name, "onchange") == 0)
      inputData.onchange = option->getValue();
    else if (CStrUtil::casecmp(option_name, "onclick") == 0)
      inputData.onclick = option->getValue();
    else if (CStrUtil::casecmp(option_name, "size") == 0) {
      if (CStrUtil::isInteger(option->getValue()))
        inputData.size = CStrUtil::toInteger(option->getValue());
      else {
        window->displayError("Illegal 'input' Value '%s' for '%s'\n",
                             option->getValue().c_str(), option_name.c_str());
        inputData.size = -1;
      }
    }
    else if (CStrUtil::casecmp(option_name, "src") == 0)
      inputData.src = option->getValue();
    else if (CStrUtil::casecmp(option_name, "type") == 0)
      inputData.type = option->getValue();
    else if (CStrUtil::casecmp(option_name, "min") == 0)
      inputData.min = option->getValue();
    else if (CStrUtil::casecmp(option_name, "max") == 0)
      inputData.max = option->getValue();
    else if (CStrUtil::casecmp(option_name, "step") == 0)
      inputData.step = option->getValue();
    else if (CStrUtil::casecmp(option_name, "value") == 0)
      inputData.value = option->getValue();
    else
      window->displayError("Illegal '%s' Option Name '%s'\n",
                           tag->getName().c_str(), option_name.c_str());
  }

  if (inputData.type == "")
    inputData.type = "text";

  //---

  if      (CStrUtil::casecmp(inputData.type, "button"  ) == 0)
    return new CBrowserFormButton(window, inputData);
  else if (CStrUtil::casecmp(inputData.type, "checkbox") == 0)
    return new CBrowserFormCheckBox(window, inputData);
  else if (CStrUtil::casecmp(inputData.type, "file"    ) == 0)
    return new CBrowserFormFileUpload(window, inputData);
  else if (CStrUtil::casecmp(inputData.type, "hidden"  ) == 0)
    return new CBrowserFormHidden(window, inputData);
  else if (CStrUtil::casecmp(inputData.type, "image"   ) == 0)
    return new CBrowserFormImage(window, inputData);
  else if (CStrUtil::casecmp(inputData.type, "password") == 0)
    return new CBrowserFormPassword(window, inputData);
  else if (CStrUtil::casecmp(inputData.type, "radio"   ) == 0)
    return new CBrowserFormRadio(window, inputData);
  else if (CStrUtil::casecmp(inputData.type, "range"   ) == 0)
    return new CBrowserFormRange(window, inputData);
  else if (CStrUtil::casecmp(inputData.type, "reset"   ) == 0)
    return new CBrowserFormReset(window, inputData);
  else if (CStrUtil::casecmp(inputData.type, "submit"  ) == 0)
    return new CBrowserFormSubmit(window, inputData);
  else if (CStrUtil::casecmp(inputData.type, "text"    ) == 0)
    return new CBrowserFormText(window, inputData);
  else {
    window->displayError("Invalid Input Type '%s'\n", inputData.type.c_str());
    return nullptr;
  }
}

CBrowserObject *
CBrowserOutputKbdTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for '%s'\n", tag->getName().c_str());

  //---

  CBrowserKbd *kbd = new CBrowserKbd(window);

  return kbd;
}

CBrowserObject *
CBrowserOutputLiTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserObject *currentObj = window->currentObj();

  if (currentObj && currentObj->type() == CHtmlTagId::LI)
    CBrowserFlushTags(window, tag, CHtmlTagId::LI, true);

  //---

  CBrowserOutputListItemData listItemData;

  int num_options = tag->getNumOptions();

  for (int i = 0; i < num_options; i++) {
    const CHtmlTagOption *option = tag->getOption(i);

    std::string option_name = option->getName();

    if      (CStrUtil::casecmp(option_name, "type") == 0) {
      listItemData.symbol = option->getValue();
    }
    else if (CStrUtil::casecmp(option_name, "value") == 0) {
      if (CStrUtil::isInteger(option->getValue()))
        listItemData.item_num = CStrUtil::toInteger(option->getValue());
      else
        window->displayError("Illegal 'li' Value for Value '%s'\n", option->getValue().c_str());

    }
    else
      window->displayError("Illegal 'li' Option Name '%s'\n", option_name.c_str());
  }

  //---

  CBrowserListItem *listItem = new CBrowserListItem(window, listItemData);

  return listItem;
}

void
CBrowserOutputLiTag::init(CBrowserWindow *, CBrowserObject *obj)
{
  CBrowserListItem *listItem = dynamic_cast<CBrowserListItem *>(obj);

  if (listItem)
    listItem->addLabelSymbols();
}

CBrowserObject *
CBrowserOutputMenuTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserOutputListData listData;

  int num_options = tag->getNumOptions();

  for (int i = 0; i < num_options; i++) {
    const CHtmlTagOption *option = tag->getOption(i);

    std::string option_name = option->getName();

    if      (CStrUtil::casecmp(option_name, "compact") == 0) {
      listData.compact = true;

      std::string value = option->getValue();

      if (value[0] != '\0')
        window->displayError("No Value needed for Compact\n");
    }
    else if (CStrUtil::casecmp(option_name, "type") == 0)
      listData.symbol = option->getValue();
    else
      window->displayError("Illegal 'menu' Option Name '%s'\n", option_name.c_str());
  }

  //---

  CBrowserOutputList *list = new CBrowserOutputList(window, CHtmlTagId::MENU, listData);

  return list;
}

CBrowserObject *
CBrowserOutputMetaTag::start(CBrowserWindow *window, CHtmlTag *)
{
  CBrowserMeta *meta = new CBrowserMeta(window);

  return meta;
}

CBrowserObject *
CBrowserOutputNobrTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for '%s'\n", tag->getName().c_str());

  //---

  CBrowserNoBr *nobr = new CBrowserNoBr(window);

  return nobr;
}

CBrowserObject *
CBrowserOutputOlTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserObject *currentObj = window->currentObj();

  if (currentObj && currentObj->type() == CHtmlTagId::LI)
    CBrowserFlushTags(window, tag, CHtmlTagId::LI);

  //---

  CBrowserOutputListData listData;

  int num_options = tag->getNumOptions();

  for (int i = 0; i < num_options; i++) {
    const CHtmlTagOption *option = tag->getOption(i);

    std::string option_name = option->getName();

    if      (CStrUtil::casecmp(option_name, "compact") == 0) {
      listData.compact = true;

      std::string value = option->getValue();

      if (value[0] != '\0')
        window->displayError("No Value needed for Compact\n");
    }
    else if (CStrUtil::casecmp(option_name, "start") == 0) {
      if (CStrUtil::isInteger(option->getValue())) {
        int item_num = CStrUtil::toInteger(option->getValue());

        listData.item_num = item_num;
      }
      else {
        window->displayError("Illegal 'ol' for Value '%s'\n", option->getValue().c_str());

        listData.item_num = 1;
      }
    }
    else if (CStrUtil::casecmp(option_name, "type") == 0)
      listData.symbol = option->getValue();
    else
      window->displayError("Illegal 'ol' Option Name '%s'\n", option_name.c_str());
  }

  //---

  CBrowserOutputList *list = new CBrowserOutputList(window, CHtmlTagId::OL, listData);

  return list;
}

CBrowserObject *
CBrowserOutputOptionTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserObject *currentObj = window->currentObj();

  if (currentObj && currentObj->type() == CHtmlTagId::OPTION)
    CBrowserFlushTags(window, tag, CHtmlTagId::OPTION, true);

  //---

  CBrowserFormOptionData optionData;

  int num_options = tag->getNumOptions();

  for (int i = 0; i < num_options; i++) {
    const CHtmlTagOption *option = tag->getOption(i);

    std::string option_name = option->getName();

    if      (CStrUtil::casecmp(option_name, "selected") == 0)
      optionData.selected = true;
    else if (CStrUtil::casecmp(option_name, "value") == 0)
      optionData.value = option->getValue();
    else
      window->displayError("Illegal '%s' Option '%s'\n", tag->getName().c_str(),
                           option_name.c_str());
  }

  //---

  CBrowserFormOption *option = new CBrowserFormOption(window, optionData);

  return option;
}

CBrowserObject *
CBrowserOutputPTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserObject *currentObj = window->currentObj();

  if (currentObj && currentObj->type() == CHtmlTagId::P)
    CBrowserFlushTags(window, tag, CHtmlTagId::P, true);

  //---

  CBrowserParagraphData paragraphData;

  int num_options = tag->getNumOptions();

  for (int i = 0; i < num_options; i++) {
    const CHtmlTagOption *option = tag->getOption(i);

    std::string option_name = option->getName();

    if (CStrUtil::casecmp(option_name, "align") == 0)
      window->parseHAlignOption(option->getValue(), paragraphData.align);
    else
      window->displayError("Illegal 'p' Option Name '%s'\n", option_name.c_str());
  }

  //---

  CBrowserParagraph *paragraph = new CBrowserParagraph(window, paragraphData);

  return paragraph;
}

CBrowserObject *
CBrowserOutputPreTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserPreData preData;

  int num_options = tag->getNumOptions();

  for (int i = 0; i < num_options; i++) {
    const CHtmlTagOption *option = tag->getOption(i);

    std::string option_name = option->getName();

    if (CStrUtil::casecmp(option_name, "width") == 0) {
      if (CStrUtil::isInteger(option->getValue()))
        preData.width = CStrUtil::toInteger(option->getValue());
      else {
        window->displayError("Illegal 'pre' value for Width '%s'\n", option->getValue().c_str());
        preData.width = -1;
      }
    }
    else
      window->displayError("Illegal 'pre' Option Name '%s'\n", option_name.c_str());
  }

  //---

  CBrowserPre *pre = new CBrowserPre(window, preData);

  return pre;
}

CBrowserObject *
CBrowserOutputSampTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for '%s'\n", tag->getName().c_str());

  //---

  CBrowserSamp *samp = new CBrowserSamp(window);

  return samp;
}

CBrowserObject *
CBrowserOutputScriptTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserScriptData scriptData;

  scriptData.type = "text/javascript";

  int num_options = tag->getNumOptions();

  for (int i = 0; i < num_options; i++) {
    const CHtmlTagOption *option = tag->getOption(i);

    std::string option_name = option->getName();

    if      (CStrUtil::casecmp(option_name, "language") == 0)
      scriptData.language = option->getValue();
    else if (CStrUtil::casecmp(option_name, "src") == 0)
      scriptData.src = option->getValue();
    else if (CStrUtil::casecmp(option_name, "type") == 0)
      scriptData.type = option->getValue();
    else
      window->displayError("Illegal '%s' Option '%s'\n",
                           tag->getName().c_str(), option_name.c_str());
  }

  if (scriptData.src != "") {
    if      (CStrUtil::casecmp(scriptData.language, "ceil") == 0)
      CBrowserCeilInst->runScriptFile(window, scriptData.src);
    else if (CStrUtil::casecmp(scriptData.type, "text/javascript") == 0)
      window->addScriptFile(scriptData.src);
  }

  //---

  CBrowserScript *script = new CBrowserScript(window, scriptData);

  return script;
}

CBrowserObject *
CBrowserOutputSelectTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserFormSelectData selectData;

  int num_options = tag->getNumOptions();

  for (int i = 0; i < num_options; i++) {
    const CHtmlTagOption *option = tag->getOption(i);

    std::string option_name = option->getName();

    if      (CStrUtil::casecmp(option_name, "multiple") == 0)
      selectData.multiple = true;
    else if (CStrUtil::casecmp(option_name, "id") == 0)
      selectData.id = option->getValue();
    else if (CStrUtil::casecmp(option_name, "name") == 0)
      selectData.name = option->getValue();
    else if (CStrUtil::casecmp(option_name, "size") == 0) {
      if (CStrUtil::isInteger(option->getValue()))
        selectData.size = CStrUtil::toInteger(option->getValue());
      else {
        window->displayError("Illegal 'select' Value '%s' for '%s'\n",
                             option->getValue().c_str(), option_name.c_str());
        selectData.size = 1;
      }
    }
    else if (CStrUtil::casecmp(option_name, "onchange") == 0)
      selectData.onchange = option->getValue();
    else
      window->displayError("Illegal '%s' Option '%s'\n",
                           tag->getName().c_str(), option_name.c_str());
  }

  //---

  CBrowserFormSelect *select = new CBrowserFormSelect(window, selectData);

  return select;
}

CBrowserObject *
CBrowserOutputSmallTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for '%s'\n", tag->getName().c_str());

  //---

  CBrowserSmallStyle *style = new CBrowserSmallStyle(window);

  return style;
}

CBrowserObject *
CBrowserOutputStrikeTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for '%s'\n", tag->getName().c_str());

  //---

  CBrowserStrikeStyle *style = new CBrowserStrikeStyle(window);

  return style;
}

CBrowserObject *
CBrowserOutputStrongTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for '%s'\n", tag->getName().c_str());

  //---

  CBrowserStrongStyle *style = new CBrowserStrongStyle(window);

  return style;
}

CBrowserObject *
CBrowserOutputSubTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for '%s'\n", tag->getName().c_str());

  //---

  CBrowserSubStyle *style = new CBrowserSubStyle(window);

  return style;
}

CBrowserObject *
CBrowserOutputSupTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for '%s'\n", tag->getName().c_str());

  //---

  CBrowserSupStyle *style = new CBrowserSupStyle(window);

  return style;
}

CBrowserObject *
CBrowserOutputTableTag::start(CBrowserWindow *window, CHtmlTag *tag)
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
        window->displayError("Illegal 'table' Alignment '%s'\n", option->getValue().c_str());
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
          window->displayError("Illegal 'table' Value for border '%s'\n",
                               option->getValue().c_str());

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
        window->displayError("Illegal 'table' Value for cellpadding '%s'\n",
                             option->getValue().c_str());

        tableData.cell_padding = 1;
      }
    }
    else if (CStrUtil::casecmp(option_name, "cellspacing") == 0) {
      if (CStrUtil::isInteger(option->getValue()))
        tableData.cell_spacing = CStrUtil::toInteger(option->getValue());
      else {
        window->displayError("Illegal 'table' Value for cellspacing '%s'\n",
                             option->getValue().c_str());

        tableData.cell_spacing = 2;
      }
    }
    else if (CStrUtil::casecmp(option_name, "hspace") == 0) {
      if (CStrUtil::isInteger(option->getValue()))
        tableData.hspace = CStrUtil::toInteger(option->getValue());
      else {
        window->displayError("Illegal 'table' Value for hspace '%s'\n",
                             option->getValue().c_str());

        tableData.hspace = 0;
      }
    }
    else if (CStrUtil::casecmp(option_name, "valign") == 0) {
      if      (CStrUtil::casecmp(option->getValue(), "top") == 0)
        tableData.valign = CVALIGN_TYPE_TOP;
      else if (CStrUtil::casecmp(option->getValue(), "bottom") == 0)
        tableData.valign = CVALIGN_TYPE_BOTTOM;
      else
        window->displayError("Illegal 'table' vertical alignment '%s'\n",
                             option->getValue().c_str());
    }
    else if (CStrUtil::casecmp(option_name, "vspace") == 0) {
      if (CStrUtil::isInteger(option->getValue()))
        tableData.vspace = CStrUtil::toInteger(option->getValue());
      else {
        window->displayError("Illegal 'table' Value for vspace '%s'\n",
                             option->getValue().c_str());

        tableData.vspace = 0;
      }
    }
    else if (CStrUtil::casecmp(option_name, "width") == 0) {
      int len = option->getValue().size();

      std::string value = option->getValue();

      if (value[len - 1] == '%') {
        tableData.width_unit = CBrowserUnitsType::PERCENT;

        std::string value1 = value.substr(0, len - 1);

        if (CStrUtil::isInteger(value1))
          tableData.width = CStrUtil::toInteger(value1);
        else {
          window->displayError("Illegal 'table' Value for Width '%s'%\n", value1.c_str());
          tableData.width = -1;
        }
      }
      else {
        tableData.width_unit = CBrowserUnitsType::PIXEL;

        if (CStrUtil::isInteger(option->getValue()))
          tableData.width = CStrUtil::toInteger(option->getValue());
        else {
          window->displayError("Illegal 'table' Value for Width '%s'\n",
                               option->getValue().c_str());
          tableData.width = -1;
        }
      }
    }
    else
      window->displayError("Illegal 'table' Option Name '%s'\n", option_name.c_str());
  }

  //---

  CBrowserTable *table = new CBrowserTable(window, tableData);

  return table;
}

CBrowserObject *
CBrowserOutputTdTag::start(CBrowserWindow *window, CHtmlTag *tag)
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
      window->parseHAlignOption(option->getValue(), cellData.halign);
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
        window->displayError("Illegal 'td' Value for colspan '%s'\n", option->getValue().c_str());
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
        window->displayError("Illegal 'td' Value for rowspan '%s'\n", option->getValue().c_str());
        cellData.rowspan = 1;
      }
    }
    else if (CStrUtil::casecmp(option_name, "valign") == 0) {
      window->parseVAlignOption(option->getValue(), cellData.valign);
    }
    else if (CStrUtil::casecmp(option_name, "width") == 0) {
      int len = option->getValue().size();

      std::string value = option->getValue();

      if (value[len - 1] == '%') {
        cellData.width_unit = CBrowserUnitsType::PERCENT;

        std::string value1 = value.substr(0, len - 1);

        if (CStrUtil::isInteger(value1))
          cellData.width = CStrUtil::toInteger(value1);
        else {
          window->displayError("Illegal 'td' Value for Width '%s'%\n", value1.c_str());
          cellData.width = -1;
        }
      }
      else {
        cellData.width_unit = CBrowserUnitsType::PIXEL;

        if (CStrUtil::isInteger(option->getValue()))
          cellData.width = CStrUtil::toInteger(option->getValue());
        else {
          window->displayError("Illegal 'td' Value for Width '%s'\n", option->getValue().c_str());
          cellData.width = -1;
        }
      }
    }
    else
      window->displayError("Illegal 'td' Option Name '%s'\n", option_name.c_str());
  }

  //---

  CBrowserTableCell *cell = new CBrowserTableCell(window, CHtmlTagId::TD, cellData);

  return cell;
}

CBrowserObject *
CBrowserOutputTextareaTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserFormTextareaData textareaData;

  int num_options = tag->getNumOptions();

  for (int i = 0; i < num_options; i++) {
    const CHtmlTagOption *option = tag->getOption(i);

    std::string option_name = option->getName();

    if      (CStrUtil::casecmp(option_name, "cols") == 0) {
      if (CStrUtil::isInteger(option->getValue()))
        textareaData.cols = CStrUtil::toInteger(option->getValue());
      else {
        window->displayError("Illegal 'textarea' Value for cols '%s'\n",
                             option->getValue().c_str());
        textareaData.cols = 40;
      }
    }
    else if (CStrUtil::casecmp(option_name, "id") == 0) {
      textareaData.id = option->getValue();
    }
    else if (CStrUtil::casecmp(option_name, "name") == 0) {
      textareaData.name = option->getValue();
    }
    else if (CStrUtil::casecmp(option_name, "rows") == 0) {
      if (CStrUtil::isInteger(option->getValue()))
        textareaData.rows = CStrUtil::toInteger(option->getValue());
      else {
        window->displayError("Illegal 'textarea' Value for rows '%s'\n",
                             option->getValue().c_str());
        textareaData.rows = 5;
      }
    }
    else if (CStrUtil::casecmp(option_name, "wrap") == 0)
      textareaData.wrap = option->getValue();
    else
      window->displayError("Illegal 'textarea' Option Name '%s'\n", option_name.c_str());
  }

  //---

  CBrowserFormTextarea *textArea = new CBrowserFormTextarea(window, textareaData);

  return textArea;
}

CBrowserObject *
CBrowserOutputThTag::start(CBrowserWindow *window, CHtmlTag *tag)
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
      window->parseHAlignOption(option->getValue(), cellData.halign);
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
        window->displayError("Illegal 'th' Value for colspan '%s'\n", option->getValue().c_str());
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
        window->displayError("Illegal 'th' Value for rowspan '%s'\n", option->getValue().c_str());
        cellData.rowspan = 1;
      }
    }
    else if (CStrUtil::casecmp(option_name, "valign") == 0) {
      window->parseVAlignOption(option->getValue(), cellData.valign);
    }
    else if (CStrUtil::casecmp(option_name, "width") == 0) {
      int len = option->getValue().size();

      std::string value = option->getValue();

      if (value[len - 1] == '%') {
        cellData.width_unit = CBrowserUnitsType::PERCENT;

        std::string value1 = value.substr(0, len - 1);

        if (CStrUtil::isInteger(value1))
          cellData.width = CStrUtil::toInteger(value1);
        else {
          window->displayError("Illegal 'th' Value for Width '%s'%\n", value1.c_str());
          cellData.width = -1;
        }
      }
      else {
        cellData.width_unit = CBrowserUnitsType::PIXEL;

        if (CStrUtil::isInteger(option->getValue()))
          cellData.width = CStrUtil::toInteger(option->getValue());
        else {
          window->displayError("Illegal 'th' Value for Width '%s'\n", option->getValue().c_str());
          cellData.width = -1;
        }
      }
    }
    else
      window->displayError("Illegal 'th' Option Name '%s'\n", option_name.c_str());
  }

  //---

  CBrowserTableCell *cell = new CBrowserTableCell(window, CHtmlTagId::TH, cellData);

  return cell;
}

CBrowserObject *
CBrowserOutputTitleTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for '%s'\n", tag->getName().c_str());

  //---

  CBrowserTitle *title = new CBrowserTitle(window);

  return title;
}

CBrowserObject *
CBrowserOutputTrTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserTableRowData rowData;

  int num_options = tag->getNumOptions();

  for (int i = 0; i < num_options; i++) {
    const CHtmlTagOption *option = tag->getOption(i);

    std::string option_name = option->getName();

    if      (CStrUtil::casecmp(option_name, "align") == 0) {
      window->parseHAlignOption(option->getValue(), rowData.halign);
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
      window->parseVAlignOption(option->getValue(), rowData.valign);
    }
    else
      window->displayError("Illegal 'tr' Option Name '%s'\n", option_name.c_str());
  }

  //---

  CBrowserTableRow *row = new CBrowserTableRow(window, rowData);

  return row;
}

CBrowserObject *
CBrowserOutputTtTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for '%s'\n", tag->getName().c_str());

  //---

  CBrowserTT *tt = new CBrowserTT(window);

  return tt;
}

CBrowserObject *
CBrowserOutputUTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for '%s'\n", tag->getName().c_str());

  //---

  CBrowserUStyle *style = new CBrowserUStyle(window);

  return style;
}

CBrowserObject *
CBrowserOutputUlTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserObject *currentObj = window->currentObj();

  if (currentObj && currentObj->type() == CHtmlTagId::LI)
    CBrowserFlushTags(window, tag, CHtmlTagId::LI);

  //---

  CBrowserOutputListData listData;

  int num_options = tag->getNumOptions();

  for (int i = 0; i < num_options; i++) {
    const CHtmlTagOption *option = tag->getOption(i);

    std::string option_name = option->getName();

    if      (CStrUtil::casecmp(option_name, "compact") == 0) {
      listData.compact = true;

      std::string value = option->getValue();

      if (value[0] != '\0')
        window->displayError("No Value needed for Compact\n");
    }
    else if (CStrUtil::casecmp(option_name, "type") == 0)
      listData.symbol = option->getValue();
    else
      window->displayError("Illegal 'ul' Option Name '%s'\n", option_name.c_str());
  }

  //---

  CBrowserOutputList *list = new CBrowserOutputList(window, CHtmlTagId::UL, listData);

  return list;
}

CBrowserObject *
CBrowserOutputVarTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for '%s'\n", tag->getName().c_str());

  //---

  CBrowserVarStyle *style = new CBrowserVarStyle(window);

  return style;
}

CBrowserObject *
CBrowserOutputWbrTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for '%s'\n", tag->getName().c_str());

  //---

  CBrowserWbr *wbr = new CBrowserWbr(window);

  return wbr;
}

CBrowserObject *
CBrowserOutputXmpTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window->displayError("Invalid Number of Options for '%s'\n", tag->getName().c_str());

  //---

  CBrowserXmp *xmp = new CBrowserXmp(window);

  return xmp;
}
