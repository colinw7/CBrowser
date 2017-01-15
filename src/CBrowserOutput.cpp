#include <CBrowserOutput.h>
#include <CBrowserOutputTag.h>

#include <CBrowserAddress.h>
#include <CBrowserArea.h>
#include <CBrowserBaseFont.h>
#include <CBrowserBlockQuote.h>
#include <CBrowserBody.h>
#include <CBrowserBreak.h>
#include <CBrowserCanvas.h>
#include <CBrowserCenter.h>
#include <CBrowserCode.h>
#include <CBrowserCSSStyle.h>
#include <CBrowserDiv.h>
#include <CBrowserDocument.h>
#include <CBrowserForm.h>
#include <CBrowserHead.h>
#include <CBrowserHeader.h>
#include <CBrowserHtml.h>
#include <CBrowserImage.h>
#include <CBrowserKbd.h>
#include <CBrowserLayout.h>
#include <CBrowserLink.h>
#include <CBrowserList.h>
#include <CBrowserListItem.h>
#include <CBrowserMisc.h>
#include <CBrowserMeta.h>
#include <CBrowserNoBr.h>
#include <CBrowserList.h>
#include <CBrowserParagraph.h>
#include <CBrowserPre.h>
#include <CBrowserRule.h>
#include <CBrowserSamp.h>
#include <CBrowserScript.h>
#include <CBrowserStyle.h>
#include <CBrowserTable.h>
#include <CBrowserText.h>
#include <CBrowserTitle.h>
#include <CBrowserTT.h>
#include <CBrowserWindow.h>
#include <CBrowserXmp.h>

#include <CBrowserMain.h>
#include <CBrowserCeil.h>
#include <CBrowserJS.h>

namespace {

bool isParagraphObj(CBrowserObject *obj) {
  if (! obj) return false;

  if (obj->type() == CHtmlTagId::ADDRESS || obj->type() == CHtmlTagId::P  ||
      obj->type() == CHtmlTagId::CENTER  || obj->type() == CHtmlTagId::LI ||
      obj->type() == CHtmlTagId::PRE     || obj->type() == CHtmlTagId::DT ||
      obj->type() == CHtmlTagId::H1      || obj->type() == CHtmlTagId::H2 ||
      obj->type() == CHtmlTagId::H3      || obj->type() == CHtmlTagId::H4 ||
      obj->type() == CHtmlTagId::H5      || obj->type() == CHtmlTagId::H6)
    return true;

  if (obj->isInline())
    return isParagraphObj(obj->parent());

  return false;
}

CBrowserParagraph *createParagraph(CBrowserWindow *window) {
  CBrowserParagraph *paragraph = new CBrowserParagraph(window);

  paragraph->init();

  window->startObject(paragraph, /*add*/true);

  return paragraph;
}

}

//------

void
CBrowserFlushObj(CBrowserWindow *window, CHtmlTag *tag, bool handleEmpty)
{
  CBrowserObject *currentObj = window->currentObj();

  CBrowserOutputTagBase *output_data = CBrowserOutputTagMgrInst->getTag(currentObj->type());

  if (handleEmpty) {
    if (! output_data || tag->isEmpty()) {
      if (currentObj->type() != CHtmlTagId::TEXT)
        window->displayError("End Tag '%s' not implemented\n", currentObj->typeName().c_str());

      window->endObject();

      return;
    }
  }

std::cerr << "Flush Tag '" << currentObj->typeName() << "' for '" <<
             tag->getName() << "'" << std::endl;

  //---

  if (output_data) {
    // terminate object (used ?)
    output_data->term(window, nullptr);

    output_data->end(window, currentObj, nullptr);
  }

  //---

  // update DOM
  window->endObject();
}

CBrowserObject *
CBrowserFlushTags(CBrowserWindow *window, CHtmlTag *tag, CHtmlTagId id, bool flushFound=false)
{
  CBrowserObject *currentObj = window->currentObj();

  while (currentObj && currentObj->type() != id) {
    CBrowserFlushObj(window, tag, /*handleEmpty*/true);

    currentObj = window->currentObj();
  }

  if (currentObj && currentObj->type() == id) {
    if (flushFound) {
      CBrowserFlushObj(window, tag, /*handleEmpty*/false);

      return nullptr;
    }
    else
      return currentObj;
  }

  return nullptr;
}

#if 0
void
CBrowserFlushBlock(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserObject *currentObj = window->currentObj();

  while (currentObj && currentObj->display() != CBrowserObject::Display::BLOCK) {
    CBrowserFlushObj(window, tag, /*handleEmpty*/true);

    currentObj = window->currentObj();
  }

  if (currentObj && currentObj->display() == CBrowserObject::Display::BLOCK &&
      currentObj->type() != CHtmlTagId::BODY) {
    CBrowserFlushObj(window, tag, /*handleEmpty*/false);
  }
}
#endif

void
CBrowserFlushText(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserObject *currentObj = window->currentObj();

  if (currentObj && currentObj->type() == CHtmlTagId::TEXT)
    CBrowserFlushTags(window, tag, CHtmlTagId::TEXT, true);
}

void
CBrowserFlushParagraph(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserFlushText(window, tag);

  CBrowserObject *currentObj = window->currentObj();

  if (isParagraphObj(currentObj))
    CBrowserFlushTags(window, tag, currentObj->type(), true);
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
  window_->getLayout()->startBox(obj);

  obj->initLayout();

  for (const auto &c : obj->children())
    layoutObj(c);

  obj->termLayout();

  window_->getLayout()->endBox(obj);
}

void
CBrowserOutput::
init()
{
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
  // auto add html (root) tag if missing
  if (tag->getTagId() != CHtmlTagId::HTML) {
    CBrowserObject *root = window_->rootObject();

    if (! root) {
      CBrowserHtml *html = new CBrowserHtml(window_);

      html->init();

      window_->startObject(html, /*add*/true);
    }
  }

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

  CBrowserObject *currentObj = window_->currentObj();

  if (! currentObj) {
    std::string sstr = CStrUtil::stripSpaces(str);

    if (sstr == "")
      return;

    currentObj = createParagraph(window_);
  }

  //---

  bool removeSpace = (currentObj->hierWhiteSpace() == CBrowserObject::WhiteSpace::NORMAL);

  std::string sstr, rstr;

  if (! removeSpace) {
    sstr = str;
    rstr = str;
  }
  else {
    sstr = CStrUtil::stripSpaces(str);
    rstr = CStrUtil::stripSpaces(str, /*front*/true, /*back*/false);
  }

  //---

  if (currentObj->type() == CHtmlTagId::BODY ||
      currentObj->type() == CHtmlTagId::FORM ||
      currentObj->type() == CHtmlTagId::DIV) {
    if (sstr == "")
      return;

    // auto start paragraph
    currentObj = createParagraph(window_);
  }

  //---

  if      (currentObj->type() == CHtmlTagId::HTML) {
    if (sstr != "")
      window_->displayError("Ignore text '%s' for html tag\n", str);
  }
  else if (currentObj->type() == CHtmlTagId::HEAD) {
    if (sstr != "")
      window_->displayError("Ignore text '%s' for head tag\n", str);
  }
  else if (currentObj->type() == CHtmlTagId::TITLE) {
    window_->getDocument()->setTitle(sstr);
  }
  else if (currentObj->type() == CHtmlTagId::SCRIPT) {
    CBrowserScript *script = dynamic_cast<CBrowserScript *>(currentObj);

    script->setText(script->text() + str);
  }
  else if (currentObj->type() == CHtmlTagId::STYLE) {
    CBrowserCSSStyle *style = dynamic_cast<CBrowserCSSStyle *>(currentObj);

    style->setText(style->text() + str);
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
  else {
    if (sstr != "") {
      CBrowserText *textObj;

      if (removeSpace) {
        bool lspace = (str.size() > 0 && isspace(str[0]));

        if (currentObj->hasInlineChildren() && lspace)
          textObj = new CBrowserText(window_, " " + rstr);
        else
          textObj = new CBrowserText(window_, rstr);
      }
      else
        textObj = new CBrowserText(window_, str);

      textObj->init();

      window_->startObject(textObj, /*add*/true);
    }
  }
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

  // init process state
  obj->initProcess();

  //---

  // auto end empty tag (no end tag)
  if (tag->isEmpty()) {
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

  if (tag->isEmpty()) {
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
  CBrowserFlushText(window, tag);

  // auto start paragraph
  if (! isParagraphObj(window->currentObj()))
    createParagraph(window);

  //---

  CBrowserLinkData linkData;

  CBrowserAnchor *link = new CBrowserAnchor(window, linkData);

  //---

  for (const auto &option: tag->getOptions()) {
    link->setNameValue(option->getName(), option->getValue());
  }

  //---

  link->init();

  return link;
}

CBrowserObject *
CBrowserOutputAbbrTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserObject *obj = new CBrowserObject(window, CHtmlTagId::ABBR);

  //---

  for (const auto &option: tag->getOptions()) {
    obj->setNameValue(option->getName(), option->getValue());
  }

  //---

  obj->init();

  return obj;
}

CBrowserObject *
CBrowserOutputAcronymTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserObject *obj = new CBrowserObject(window, CHtmlTagId::ACRONYM);

  //---

  for (const auto &option: tag->getOptions()) {
    obj->setNameValue(option->getName(), option->getValue());
  }

  //---

  obj->init();

  return obj;
}

CBrowserObject *
CBrowserOutputAddressTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserFlushParagraph(window, tag);

  //---

  CBrowserAddress *address = new CBrowserAddress(window);

  //---

  for (const auto &option: tag->getOptions()) {
    address->setNameValue(option->getName(), option->getValue());
  }

  //---

  address->init();

  return address;
}

CBrowserObject *
CBrowserOutputAppletTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserObject *obj = new CBrowserObject(window, CHtmlTagId::APPLET);

  obj->setDisplay(CBrowserObject::Display::INLINE);

  //---

  for (const auto &option: tag->getOptions()) {
    obj->setNameValue(option->getName(), option->getValue());
  }

  //---

  obj->init();

  return obj;
}

CBrowserObject *
CBrowserOutputAreaTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserArea *area = new CBrowserArea(window);

  //---

  for (const auto &option: tag->getOptions()) {
    area->setNameValue(option->getName(), option->getValue());
  }

  //---

  area->init();

  return area;
}

CBrowserObject *
CBrowserOutputArticleTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserObject *obj = new CBrowserObject(window, CHtmlTagId::ARTICLE);

  //---

  for (const auto &option: tag->getOptions()) {
    obj->setNameValue(option->getName(), option->getValue());
  }

  //---

  obj->init();

  return obj;
}

CBrowserObject *
CBrowserOutputASideTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserObject *obj = new CBrowserObject(window, CHtmlTagId::ASIDE);

  //---

  for (const auto &option: tag->getOptions()) {
    obj->setNameValue(option->getName(), option->getValue());
  }

  //---

  obj->init();

  return obj;
}

CBrowserObject *
CBrowserOutputAudioTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserObject *obj = new CBrowserObject(window, CHtmlTagId::AUDIO);

  //---

  for (const auto &option: tag->getOptions()) {
    obj->setNameValue(option->getName(), option->getValue());
  }

  //---

  obj->init();

  return obj;
}

CBrowserObject *
CBrowserOutputBTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserFlushText(window, tag);

  //---

  CBrowserBStyle *style = new CBrowserBStyle(window);

  //---

  for (const auto &option: tag->getOptions()) {
    style->setNameValue(option->getName(), option->getValue());
  }

  //---

  style->init();

  return style;
}

CBrowserObject *
CBrowserOutputBaseTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserObject *obj = new CBrowserObject(window, CHtmlTagId::BASE);

  //---

  for (const auto &option: tag->getOptions()) {
    obj->setNameValue(option->getName(), option->getValue());
  }

  //---

  obj->init();

  return obj;
}

CBrowserObject *
CBrowserOutputBasefontTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserFlushText(window, tag);

  //---

  CBrowserBaseFontData data;

  CBrowserBaseFont *baseFont = new CBrowserBaseFont(window, data);

  //---

  for (const auto &option: tag->getOptions()) {
    baseFont->setNameValue(option->getName(), option->getValue());
  }

  //---

  baseFont->init();

  return baseFont;
}

CBrowserObject *
CBrowserOutputBdiTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserObject *obj = new CBrowserObject(window, CHtmlTagId::BDI);

  //---

  for (const auto &option: tag->getOptions()) {
    obj->setNameValue(option->getName(), option->getValue());
  }

  //---

  obj->init();

  return obj;
}

CBrowserObject *
CBrowserOutputBdoTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserObject *obj = new CBrowserObject(window, CHtmlTagId::BDO);

  //---

  for (const auto &option: tag->getOptions()) {
    obj->setNameValue(option->getName(), option->getValue());
  }

  //---

  obj->init();

  return obj;
}

CBrowserObject *
CBrowserOutputBigTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserFlushText(window, tag);

  //---

  CBrowserBigStyle *style = new CBrowserBigStyle(window);

  //---

  for (const auto &option: tag->getOptions()) {
    style->setNameValue(option->getName(), option->getValue());
  }

  //---

  style->init();

  return style;
}

CBrowserObject *
CBrowserOutputBlinkTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserFlushText(window, tag);

  //---

  CBrowserBlinkStyle *style = new CBrowserBlinkStyle(window);

  //---

  for (const auto &option: tag->getOptions()) {
    style->setNameValue(option->getName(), option->getValue());
  }

  //---

  style->init();

  return style;
}

CBrowserObject *
CBrowserOutputBlockquoteTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserFlushParagraph(window, tag);

  //---

  CBrowserBlockQuote *blockQuote = new CBrowserBlockQuote(window);

  //---

  for (const auto &option: tag->getOptions()) {
    blockQuote->setNameValue(option->getName(), option->getValue());
  }

  //---

  blockQuote->init();

  return blockQuote;
}

CBrowserObject *
CBrowserOutputBodyTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserBodyData bodyData;

  CBrowserBody *body = new CBrowserBody(window, bodyData);

  //---

  for (const auto &option: tag->getOptions()) {
    body->setNameValue(option->getName(), option->getValue());
  }

  //---

  body->init();

  return body;
}

CBrowserObject *
CBrowserOutputBrTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserFlushText(window, tag);

  //---

  CBrowserBreakData breakData;

  CBrowserBreak *breaker = new CBrowserBreak(window, breakData);

  //---

  for (const auto &option: tag->getOptions()) {
    breaker->setNameValue(option->getName(), option->getValue());
  }

  //---

  breaker->init();

  return breaker;
}

CBrowserObject *
CBrowserOutputButtonTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserFormButtonData data;

  CBrowserObject *obj = new CBrowserFormButton(window, data);

  //---

  for (const auto &option: tag->getOptions()) {
    obj->setNameValue(option->getName(), option->getValue());
  }

  //---

  obj->init();

  return obj;
}

CBrowserObject *
CBrowserOutputCanvasTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserCanvasData data;

  CBrowserCanvas *canvas = new CBrowserCanvas(window, data);

  //---

  for (const auto &option: tag->getOptions()) {
    canvas->setNameValue(option->getName(), option->getValue());
  }

  //---

  canvas->init();

  return canvas;
}

CBrowserObject *
CBrowserOutputCaptionTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserTableCaptionData captionData;

  CBrowserTableCaption *caption = new CBrowserTableCaption(window, captionData);

  //---

  for (const auto &option: tag->getOptions()) {
    caption->setNameValue(option->getName(), option->getValue());
  }

  //---

  caption->init();

  return caption;
}

CBrowserObject *
CBrowserOutputCenterTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserFlushParagraph(window, tag);

  //---

  CBrowserCenter *center = new CBrowserCenter(window);

  //---

  for (const auto &option: tag->getOptions()) {
    center->setNameValue(option->getName(), option->getValue());
  }

  //---

  center->init();

  return center;
}

CBrowserObject *
CBrowserOutputCiteTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserFlushText(window, tag);

  //---

  CBrowserCiteStyle *style = new CBrowserCiteStyle(window);

  //---

  for (const auto &option: tag->getOptions()) {
    style->setNameValue(option->getName(), option->getValue());
  }

  //---

  style->init();

  return style;
}

CBrowserObject *
CBrowserOutputCodeTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserFlushText(window, tag);

  //---

  CBrowserCode *code = new CBrowserCode(window);

  //---

  for (const auto &option: tag->getOptions()) {
    code->setNameValue(option->getName(), option->getValue());
  }

  //---

  code->init();

  return code;
}

CBrowserObject *
CBrowserOutputDdTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserFlushText(window, tag);

  //---

  CBrowserObject *currentObj = window->currentObj();

  if (currentObj) {
    if      (currentObj->type() == CHtmlTagId::DT)
      CBrowserFlushTags(window, tag, CHtmlTagId::DT, true);
    else if (currentObj->type() == CHtmlTagId::DD)
      CBrowserFlushTags(window, tag, CHtmlTagId::DD, true);
  }

  //---

  CBrowserDataListData *dd = new CBrowserDataListData(window);

  //---

  for (const auto &option: tag->getOptions()) {
    dd->setNameValue(option->getName(), option->getValue());
  }

  //---

  dd->init();

  return dd;
}

CBrowserObject *
CBrowserOutputDelTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserObject *obj = new CBrowserObject(window, CHtmlTagId::DEL);

  //---

  for (const auto &option: tag->getOptions()) {
    obj->setNameValue(option->getName(), option->getValue());
  }

  //---

  obj->init();

  return obj;
}

CBrowserObject *
CBrowserOutputDetailsTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserObject *obj = new CBrowserObject(window, CHtmlTagId::DETAILS);

  //---

  for (const auto &option: tag->getOptions()) {
    obj->setNameValue(option->getName(), option->getValue());
  }

  //---

  obj->init();

  return obj;
}

CBrowserObject *
CBrowserOutputDfnTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserFlushText(window, tag);

  //---

  CBrowserDfnStyle *dfn = new CBrowserDfnStyle(window);

  //---

  for (const auto &option: tag->getOptions()) {
    dfn->setNameValue(option->getName(), option->getValue());
  }

  //---

  dfn->init();

  return dfn;
}

CBrowserObject *
CBrowserOutputDirTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserFlushParagraph(window, tag);

  //---

  CBrowserObject *currentObj = window->currentObj();

  if (currentObj && currentObj->type() == CHtmlTagId::LI)
    CBrowserFlushTags(window, tag, CHtmlTagId::LI);

  //---

  CBrowserOutputListData listData;

  CBrowserList *list = new CBrowserList(window, CHtmlTagId::DIR, listData);

  //---

  for (const auto &option: tag->getOptions()) {
    list->setNameValue(option->getName(), option->getValue());
  }

  //---

  list->init();

  return list;
}

CBrowserObject *
CBrowserOutputDivTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserDivData divData;

  CBrowserDiv *div = new CBrowserDiv(window, divData);

  //---

  for (const auto &option: tag->getOptions()) {
    div->setNameValue(option->getName(), option->getValue());
  }

  //---

  div->init();

  return div;
}

CBrowserObject *
CBrowserOutputDlTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserFlushParagraph(window, tag);

  //---

  CBrowserObject *currentObj = window->currentObj();

  if (currentObj && currentObj->type() == CHtmlTagId::LI)
    CBrowserFlushTags(window, tag, CHtmlTagId::LI);

  //---

  CBrowserOutputListData listData;

  CBrowserList *list = new CBrowserList(window, CHtmlTagId::DL, listData);

  //---

  for (const auto &option: tag->getOptions()) {
    list->setNameValue(option->getName(), option->getValue());
  }

  //---

  list->init();

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
  CBrowserFlushText(window, tag);

  //---

  CBrowserObject *currentObj = window->currentObj();

  if (currentObj) {
    if      (currentObj->type() == CHtmlTagId::DT)
      CBrowserFlushTags(window, tag, CHtmlTagId::DT, true);
    else if (currentObj->type() == CHtmlTagId::DD)
      CBrowserFlushTags(window, tag, CHtmlTagId::DD, true);
  }

  //---

  CBrowserDataListTerm *dt = new CBrowserDataListTerm(window);

  //---

  for (const auto &option: tag->getOptions()) {
    dt->setNameValue(option->getName(), option->getValue());
  }

  //---

  dt->init();

  return dt;
}

CBrowserObject *
CBrowserOutputEmTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserFlushText(window, tag);

  //---

  CBrowserEmStyle *style = new CBrowserEmStyle(window);

  //---

  for (const auto &option: tag->getOptions()) {
    style->setNameValue(option->getName(), option->getValue());
  }

  //---

  style->init();

  return style;
}

CBrowserObject *
CBrowserOutputFieldSetTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserObject *obj = new CBrowserObject(window, CHtmlTagId::FIELDSET);

  //---

  for (const auto &option: tag->getOptions()) {
    obj->setNameValue(option->getName(), option->getValue());
  }

  //---

  obj->init();

  return obj;
}

CBrowserObject *
CBrowserOutputFontTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserFlushText(window, tag);

  //---

  CBrowserFontData fontData;

  CBrowserFontStyleObj *font = new CBrowserFontStyleObj(window, fontData);

  //---

  for (const auto &option: tag->getOptions()) {
    font->setNameValue(option->getName(), option->getValue());
  }

  //---

  font->init();

  return font;
}

CBrowserObject *
CBrowserOutputFooterTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserObject *obj = new CBrowserObject(window, CHtmlTagId::FOOTER);

  //---

  for (const auto &option: tag->getOptions()) {
    obj->setNameValue(option->getName(), option->getValue());
  }

  //---

  obj->init();

  return obj;
}

CBrowserObject *
CBrowserOutputFormTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserFormData formData;

  CBrowserForm *form = new CBrowserForm(window, formData);

  //---

  for (const auto &option: tag->getOptions()) {
    form->setNameValue(option->getName(), option->getValue());
  }

  //----

  form->init();

  return form;
}

CBrowserObject *
CBrowserOutputH1Tag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserHeaderData headerData;

  CBrowserHeader *header = new CBrowserHeader(window, CHtmlTagId::H1, headerData);

  //---

  for (const auto &option: tag->getOptions()) {
    header->setNameValue(option->getName(), option->getValue());
  }

  //---

  header->init();

  return header;
}

CBrowserObject *
CBrowserOutputH2Tag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserHeaderData headerData;

  CBrowserHeader *header = new CBrowserHeader(window, CHtmlTagId::H2, headerData);

  //---

  for (const auto &option: tag->getOptions()) {
    header->setNameValue(option->getName(), option->getValue());
  }

  //---

  header->init();

  return header;
}

CBrowserObject *
CBrowserOutputH3Tag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserHeaderData headerData;

  CBrowserHeader *header = new CBrowserHeader(window, CHtmlTagId::H3, headerData);

  //---

  for (const auto &option: tag->getOptions()) {
    header->setNameValue(option->getName(), option->getValue());
  }

  //---

  header->init();

  return header;
}

CBrowserObject *
CBrowserOutputH4Tag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserHeaderData headerData;

  CBrowserHeader *header = new CBrowserHeader(window, CHtmlTagId::H4, headerData);

  for (const auto &option: tag->getOptions()) {
    header->setNameValue(option->getName(), option->getValue());
  }

  //---

  header->init();

  return header;
}

CBrowserObject *
CBrowserOutputH5Tag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserHeaderData headerData;

  CBrowserHeader *header = new CBrowserHeader(window, CHtmlTagId::H5, headerData);

  //---

  for (const auto &option: tag->getOptions()) {
    header->setNameValue(option->getName(), option->getValue());
  }

  //---

  return header;
}

CBrowserObject *
CBrowserOutputH6Tag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserHeaderData headerData;

  CBrowserHeader *header = new CBrowserHeader(window, CHtmlTagId::H6, headerData);

  //---

  for (const auto &option: tag->getOptions()) {
    header->setNameValue(option->getName(), option->getValue());
  }

  //---

  header->init();

  return header;
}

CBrowserObject *
CBrowserOutputHeadTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserHead *head = new CBrowserHead(window);

  //---

  for (const auto &option: tag->getOptions()) {
    head->setNameValue(option->getName(), option->getValue());
  }

  //---

  head->init();

  return head;
}

CBrowserObject *
CBrowserOutputHeaderTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserObject *obj = new CBrowserObject(window, CHtmlTagId::HEADER);

  //---

  for (const auto &option: tag->getOptions()) {
    obj->setNameValue(option->getName(), option->getValue());
  }

  //---

  obj->init();

  return obj;
}

CBrowserObject *
CBrowserOutputHGroupTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserObject *obj = new CBrowserObject(window, CHtmlTagId::HGROUP);

  //---

  for (const auto &option: tag->getOptions()) {
    obj->setNameValue(option->getName(), option->getValue());
  }

  //---

  obj->init();

  return obj;
}

CBrowserObject *
CBrowserOutputHrTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserFlushParagraph(window, tag);

  //---

  CBrowserRuleData ruleData;

  CBrowserRule *rule = new CBrowserRule(window, ruleData);

  //---

  for (const auto &option: tag->getOptions()) {
    rule->setNameValue(option->getName(), option->getValue());
  }

  //---

  rule->init();

  return rule;
}

CBrowserObject *
CBrowserOutputHtmlTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserHtml *html = new CBrowserHtml(window);

  //---

  for (const auto &option: tag->getOptions()) {
    html->setNameValue(option->getName(), option->getValue());
  }

  //---

  html->init();

  return html;
}

CBrowserObject *
CBrowserOutputITag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserFlushText(window, tag);

  // auto start paragraph
  if (! isParagraphObj(window->currentObj()))
    createParagraph(window);

  //---

  CBrowserIStyle *style = new CBrowserIStyle(window);

  //---

  for (const auto &option: tag->getOptions()) {
    style->setNameValue(option->getName(), option->getValue());
  }

  //---

  style->init();

  return style;
}

CBrowserObject *
CBrowserOutputIFrameTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserObject *obj = new CBrowserObject(window, CHtmlTagId::IFRAME);

  //---

  for (const auto &option: tag->getOptions()) {
    obj->setNameValue(option->getName(), option->getValue());
  }

  //---

  obj->init();

  return obj;
}

CBrowserObject *
CBrowserOutputInsTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserObject *obj = new CBrowserObject(window, CHtmlTagId::INS);

  //---

  for (const auto &option: tag->getOptions()) {
    obj->setNameValue(option->getName(), option->getValue());
  }

  //---

  obj->init();

  return obj;
}

CBrowserObject *
CBrowserOutputImgTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserFlushText(window, tag);

  // auto start paragraph
  if (! isParagraphObj(window->currentObj()))
    createParagraph(window);

  //---

  CBrowserImageData imageData;

  CBrowserImage *image = new CBrowserImage(window, imageData);

  //---

  for (const auto &option: tag->getOptions()) {
    image->setNameValue(option->getName(), option->getValue());
  }

  //---

  image->init();

  return image;
}

CBrowserObject *
CBrowserOutputInputTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserFlushText(window, tag);

  // auto start paragraph
  if (! isParagraphObj(window->currentObj()))
    createParagraph(window);

  //---

  CBrowserFormInputData inputData;

  for (const auto &option: tag->getOptions()) {
    inputData.nameValues[option->getName()] = option->getValue();

    //---

    std::string option_name = CStrUtil::toLower(option->getName());

    if      (option_name == "accept") {
    }
    else if (option_name == "align") {
      inputData.align = option->getValue();
    }
    else if (option_name == "autocomplete") {
    }
    else if (option_name == "autofocus") {
    }
    else if (option_name == "capture") {
    }
    else if (option_name == "checked") {
      inputData.checked = true;
    }
    else if (option_name == "class") {
      inputData.cclass = option->getValue();
    }
    else if (option_name == "disabled") {
    }
    else if (option_name == "form") {
    }
    else if (option_name == "formaction") {
    }
    else if (option_name == "formenctype") {
    }
    else if (option_name == "formmethod") {
    }
    else if (option_name == "formnovalidate") {
    }
    else if (option_name == "formtarget") {
    }
    else if (option_name == "height") {
    }
    else if (option_name == "id") {
      inputData.id = option->getValue();
    }
    else if (option_name == "inputmode") {
    }
    else if (option_name == "list") {
    }
    else if (option_name == "max") {
      inputData.max = option->getValue();
    }
    else if (option_name == "maxlength") {
      if (CStrUtil::isInteger(option->getValue()))
        inputData.maxlength = CStrUtil::toInteger(option->getValue());
      else {
        window->displayError("Illegal 'input' Value '%s' for '%s'\n",
                             option->getValue().c_str(), option_name.c_str());
        inputData.maxlength = -1;
      }
    }
    else if (option_name == "min") {
      inputData.min = option->getValue();
    }
    else if (option_name == "minlength") {
    }
    else if (option_name == "multiple") {
    }
    else if (option_name == "name") {
      inputData.name = option->getValue();
    }
    else if (option_name == "pattern") {
    }
    else if (option_name == "placeholder") {
      inputData.placeholder = option->getValue();
    }
    else if (option_name == "readonly") {
    }
    else if (option_name == "required") {
      inputData.required = option->getValue();
    }
    else if (option_name == "selectionDirection") {
    }
    else if (option_name == "selectionStart") {
    }
    else if (option_name == "size") {
      if (CStrUtil::isInteger(option->getValue()))
        inputData.size = CStrUtil::toInteger(option->getValue());
      else {
        window->displayError("Illegal 'input' Value '%s' for '%s'\n",
                             option->getValue().c_str(), option_name.c_str());
        inputData.size = -1;
      }
    }
    else if (option_name == "spellcheck") {
    }
    else if (option_name == "src") {
      inputData.src = option->getValue();
    }
    else if (option_name == "step") {
      inputData.step = option->getValue();
    }
    else if (option_name == "type") {
      inputData.type = CStrUtil::toLower(option->getValue());
    }
    else if (option_name == "value") {
      inputData.value = option->getValue();
    }
    else if (option_name == "width") {
    }
    else if (option_name == "onchange") {
      inputData.onchange = option->getValue();
    }
    else if (option_name == "onclick") {
      inputData.onclick = option->getValue();
    }
  }

  if (inputData.type == "")
    inputData.type = "text";

  //---

  CBrowserObject *obj = nullptr;

  if      (inputData.type == "button"  )
    obj = new CBrowserFormButton(window, inputData);
  else if (inputData.type == "checkbox")
    obj = new CBrowserFormCheckBox(window, inputData);
  else if (inputData.type == "date")
    obj = new CBrowserFormDate(window, inputData);
  else if (inputData.type == "email"   )
    obj = new CBrowserFormEmail(window, inputData);
  else if (inputData.type == "file"    )
    obj = new CBrowserFormFileUpload(window, inputData);
  else if (inputData.type == "hidden"  )
    obj = new CBrowserFormHidden(window, inputData);
  else if (inputData.type == "image"   )
    obj = new CBrowserFormImage(window, inputData);
  else if (inputData.type == "month")
    obj = new CBrowserFormMonth(window, inputData);
  else if (inputData.type == "number")
    obj = new CBrowserFormNumber(window, inputData);
  else if (inputData.type == "password")
    obj = new CBrowserFormPassword(window, inputData);
  else if (inputData.type == "radio"   )
    obj = new CBrowserFormRadio(window, inputData);
  else if (inputData.type == "range"   )
    obj = new CBrowserFormRange(window, inputData);
  else if (inputData.type == "reset"   )
    obj = new CBrowserFormReset(window, inputData);
  else if (inputData.type == "search")
    obj = new CBrowserFormSearch(window, inputData);
  else if (inputData.type == "submit"  )
    obj = new CBrowserFormSubmit(window, inputData);
  else if (inputData.type == "tel"  )
    obj = new CBrowserFormTel(window, inputData);
  else if (inputData.type == "text"    )
    obj = new CBrowserFormText(window, inputData);
  else {
    window->displayError("Invalid input type '%s'\n", inputData.type.c_str());
    return nullptr;
  }

  obj->init();

  return obj;
}

CBrowserObject *
CBrowserOutputKbdTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserFlushText(window, tag);

  //---

  CBrowserKbd *kbd = new CBrowserKbd(window);

  //---

  for (const auto &option: tag->getOptions()) {
    kbd->setNameValue(option->getName(), option->getValue());
  }

  //---

  kbd->init();

  return kbd;
}

CBrowserObject *
CBrowserOutputLabelTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserObject *obj = new CBrowserObject(window, CHtmlTagId::LABEL);

  obj->setDisplay(CBrowserObject::Display::INLINE);

  //---

  for (const auto &option: tag->getOptions()) {
    obj->setNameValue(option->getName(), option->getValue());
  }

  //---

  obj->init();

  return obj;
}

CBrowserObject *
CBrowserOutputLegendTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserObject *obj = new CBrowserObject(window, CHtmlTagId::LEGEND);

  //---

  for (const auto &option: tag->getOptions()) {
    obj->setNameValue(option->getName(), option->getValue());
  }

  //---

  obj->init();

  return obj;
}

CBrowserObject *
CBrowserOutputLiTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserFlushText(window, tag);

  //---

  CBrowserObject *currentObj = window->currentObj();

  if (currentObj && currentObj->type() == CHtmlTagId::LI)
    CBrowserFlushTags(window, tag, CHtmlTagId::LI, true);

  //---

  CBrowserOutputListItemData listItemData;

  CBrowserListItem *listItem = new CBrowserListItem(window, listItemData);

  //---

  for (const auto &option: tag->getOptions()) {
    listItem->setNameValue(option->getName(), option->getValue());
  }

  //---

  listItem->init();

  return listItem;
}

CBrowserObject *
CBrowserOutputLinkTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserLinkData linkData;

  CBrowserLink *link = new CBrowserLink(window, linkData);

  //---

  for (const auto &option: tag->getOptions()) {
    link->setNameValue(option->getName(), option->getValue());
  }

  //---

  link->init();

  return link;
}

CBrowserObject *
CBrowserOutputMapTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserObject *obj = new CBrowserObject(window, CHtmlTagId::MAP);

  obj->setDisplay(CBrowserObject::Display::INLINE);

  //---

  for (const auto &option: tag->getOptions()) {
    obj->setNameValue(option->getName(), option->getValue());
  }

  //---

  obj->init();

  return obj;
}

CBrowserObject *
CBrowserOutputMenuTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserFlushParagraph(window, tag);

  //---

  CBrowserObject *currentObj = window->currentObj();

  if (currentObj && currentObj->type() == CHtmlTagId::LI)
    CBrowserFlushTags(window, tag, CHtmlTagId::LI);

  //---

  CBrowserOutputListData listData;

  CBrowserList *list = new CBrowserList(window, CHtmlTagId::MENU, listData);

  //---

  for (const auto &option: tag->getOptions()) {
    list->setNameValue(option->getName(), option->getValue());
  }

  //---

  list->init();

  return list;
}

CBrowserObject *
CBrowserOutputMetaTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserMeta *meta = new CBrowserMeta(window);

  //---

  for (const auto &option: tag->getOptions()) {
    meta->setNameValue(option->getName(), option->getValue());
  }

  //---

  meta->init();

  return meta;
}

CBrowserObject *
CBrowserOutputNavTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserObject *obj = new CBrowserObject(window, CHtmlTagId::NAV);

  //---

  for (const auto &option: tag->getOptions()) {
    obj->setNameValue(option->getName(), option->getValue());
  }

  //---

  obj->init();

  return obj;
}

CBrowserObject *
CBrowserOutputNobrTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserFlushText(window, tag);

  //---

  CBrowserNoBr *nobr = new CBrowserNoBr(window);

  //---

  for (const auto &option: tag->getOptions()) {
    nobr->setNameValue(option->getName(), option->getValue());
  }

  //---

  nobr->init();

  return nobr;
}

CBrowserObject *
CBrowserOutputOlTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserFlushParagraph(window, tag);

  //---

  CBrowserObject *currentObj = window->currentObj();

  if (currentObj && currentObj->type() == CHtmlTagId::LI)
    CBrowserFlushTags(window, tag, CHtmlTagId::LI);

  //---

  CBrowserOutputListData listData;

  CBrowserList *list = new CBrowserList(window, CHtmlTagId::OL, listData);

  //---

  for (const auto &option: tag->getOptions()) {
    list->setNameValue(option->getName(), option->getValue());
  }

  //---

  list->init();

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

  for (const auto &option: tag->getOptions()) {
    std::string option_name = CStrUtil::toLower(option->getName());

    if      (option_name == "selected") {
      optionData.selected = true;
    }
    else if (option_name == "value") {
      optionData.value = option->getValue();
    }
    else
      window->displayError("Illegal '%s' Option '%s'\n", tag->getName().c_str(),
                           option_name.c_str());
  }

  //---

  CBrowserFormOption *option = new CBrowserFormOption(window, optionData);

  option->init();

  return option;
}

CBrowserObject *
CBrowserOutputOutputTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserBaseData data;

  CBrowserObject *obj = new CBrowserObject(window, CHtmlTagId::OUTPUT, data);

  //---

  for (const auto &option: tag->getOptions()) {
    obj->setNameValue(option->getName(), option->getValue());
  }

  //---

  obj->init();

  return obj;
}

CBrowserObject *
CBrowserOutputPTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserFlushParagraph(window, tag);

  //---

  CBrowserParagraph *paragraph = new CBrowserParagraph(window);

  //---

  for (const auto &option: tag->getOptions()) {
    paragraph->setNameValue(option->getName(), option->getValue());
  }

  //---

  paragraph->init();

  return paragraph;
}

CBrowserObject *
CBrowserOutputPreTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserFlushParagraph(window, tag);

  //---

  CBrowserPreData preData;

  CBrowserPre *pre = new CBrowserPre(window, preData);

  //---

  for (const auto &option: tag->getOptions()) {
    pre->setNameValue(option->getName(), option->getValue());
  }

  //---

  pre->init();

  return pre;
}

CBrowserObject *
CBrowserOutputSampTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserSamp *samp = new CBrowserSamp(window);

  //---

  for (const auto &option: tag->getOptions()) {
    samp->setNameValue(option->getName(), option->getValue());
  }

  //---

  samp->init();

  return samp;
}

CBrowserObject *
CBrowserOutputScriptTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserScriptData scriptData;

  scriptData.type = "text/javascript";

  CBrowserScript *script = new CBrowserScript(window, scriptData);

  //---

  for (const auto &option: tag->getOptions()) {
    script->setNameValue(option->getName(), option->getValue());
  }

  //---

  script->init();

  return script;
}

CBrowserObject *
CBrowserOutputSectionTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserObject *obj = new CBrowserObject(window, CHtmlTagId::SECTION);

  //---

  for (const auto &option: tag->getOptions()) {
    obj->setNameValue(option->getName(), option->getValue());
  }

  //---

  obj->init();

  return obj;
}

CBrowserObject *
CBrowserOutputSelectTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserFlushText(window, tag);

  //---

  CBrowserFormSelectData selectData;

  CBrowserFormSelect *select = new CBrowserFormSelect(window, selectData);

  //---

  for (const auto &option: tag->getOptions()) {
    select->setNameValue(option->getName(), option->getValue());
  }

  //---

  select->init();

  return select;
}

CBrowserObject *
CBrowserOutputSmallTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserFlushText(window, tag);

  //---

  CBrowserSmallStyle *style = new CBrowserSmallStyle(window);

  //---

  for (const auto &option: tag->getOptions()) {
    style->setNameValue(option->getName(), option->getValue());
  }

  //---

  style->init();

  return style;
}

CBrowserObject *
CBrowserOutputSpanTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserObject *obj = new CBrowserObject(window, CHtmlTagId::SPAN);

  obj->setDisplay(CBrowserObject::Display::INLINE);

  //---

  for (const auto &option: tag->getOptions()) {
    obj->setNameValue(option->getName(), option->getValue());
  }

  //---

  obj->init();

  return obj;
}

CBrowserObject *
CBrowserOutputStrikeTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserFlushText(window, tag);

  //---

  CBrowserStrikeStyle *style = new CBrowserStrikeStyle(window);

  //---

  for (const auto &option: tag->getOptions()) {
    style->setNameValue(option->getName(), option->getValue());
  }

  //---

  style->init();

  return style;
}

CBrowserObject *
CBrowserOutputStrongTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserFlushText(window, tag);

  //---

  CBrowserStrongStyle *style = new CBrowserStrongStyle(window);

  //---

  for (const auto &option: tag->getOptions()) {
    style->setNameValue(option->getName(), option->getValue());
  }

  //---

  style->init();

  return style;
}

CBrowserObject *
CBrowserOutputStyleTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserCSSStyle *style = new CBrowserCSSStyle(window);

  //---

  for (const auto &option: tag->getOptions()) {
    style->setNameValue(option->getName(), option->getValue());
  }

  //---

  style->init();

  return style;
}

CBrowserObject *
CBrowserOutputSubTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserFlushText(window, tag);

  //---

  CBrowserSubStyle *style = new CBrowserSubStyle(window);

  //---

  for (const auto &option: tag->getOptions()) {
    style->setNameValue(option->getName(), option->getValue());
  }

  //---

  style->init();

  return style;
}

CBrowserObject *
CBrowserOutputSummaryTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserObject *obj = new CBrowserObject(window, CHtmlTagId::SUMMARY);

  //---

  for (const auto &option: tag->getOptions()) {
    obj->setNameValue(option->getName(), option->getValue());
  }

  //---

  obj->init();

  return obj;
}

CBrowserObject *
CBrowserOutputSupTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserFlushText(window, tag);

  //---

  CBrowserSupStyle *style = new CBrowserSupStyle(window);

  //---

  for (const auto &option: tag->getOptions()) {
    style->setNameValue(option->getName(), option->getValue());
  }

  //---

  style->init();

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

  CBrowserTable *table = new CBrowserTable(window, tableData);

  for (const auto &option: tag->getOptions()) {
    table->setNameValue(option->getName(), option->getValue());
  }

  //---

  table->init();

  return table;
}

CBrowserObject *
CBrowserOutputTdTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserTableCellData cellData;

  cellData.type    = CBrowserTableCellData::Type::HEADER;
  cellData.colspan = 1;
  cellData.rowspan = 1;

  CBrowserTableCell *cell = new CBrowserTableCell(window, CHtmlTagId::TD, cellData);

  //---

  for (const auto &option: tag->getOptions()) {
    cell->setNameValue(option->getName(), option->getValue());
  }

  //---

  cell->init();

  return cell;
}

CBrowserObject *
CBrowserOutputTextareaTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserFormTextareaData textareaData;

  CBrowserFormTextarea *textArea = new CBrowserFormTextarea(window, textareaData);

  //---

  for (const auto &option: tag->getOptions()) {
    textArea->setNameValue(option->getName(), option->getValue());
  }

  //---

  textArea->init();

  return textArea;
}

CBrowserObject *
CBrowserOutputTBodyTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserObject *obj = new CBrowserObject(window, CHtmlTagId::TBODY);

  //---

  for (const auto &option: tag->getOptions()) {
    obj->setNameValue(option->getName(), option->getValue());
  }

  //---

  obj->init();

  return obj;
}

CBrowserObject *
CBrowserOutputTFootTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserObject *obj = new CBrowserObject(window, CHtmlTagId::TFOOT);

  //---

  for (const auto &option: tag->getOptions()) {
    obj->setNameValue(option->getName(), option->getValue());
  }

  //---

  obj->init();

  return obj;
}

CBrowserObject *
CBrowserOutputTHeadTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserObject *obj = new CBrowserObject(window, CHtmlTagId::THEAD);

  //---

  for (const auto &option: tag->getOptions()) {
    obj->setNameValue(option->getName(), option->getValue());
  }

  //---

  obj->init();

  return obj;
}

CBrowserObject *
CBrowserOutputThTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserTableCellData cellData;

  cellData.type    = CBrowserTableCellData::Type::HEADER;
  cellData.colspan = 1;
  cellData.rowspan = 1;

  CBrowserTableCell *cell = new CBrowserTableCell(window, CHtmlTagId::TH, cellData);

  //---

  for (const auto &option: tag->getOptions()) {
    cell->setNameValue(option->getName(), option->getValue());
  }

  //---

  cell->init();

  return cell;
}

CBrowserObject *
CBrowserOutputTimeTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserObject *obj = new CBrowserObject(window, CHtmlTagId::TIME);

  //---

  for (const auto &option: tag->getOptions()) {
    obj->setNameValue(option->getName(), option->getValue());
  }

  //---

  obj->init();

  return obj;
}

CBrowserObject *
CBrowserOutputTitleTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserTitle *title = new CBrowserTitle(window);

  //---

  for (const auto &option: tag->getOptions()) {
    title->setNameValue(option->getName(), option->getValue());
  }

  //---

  title->init();

  return title;
}

CBrowserObject *
CBrowserOutputTrTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserTableRowData rowData;

  CBrowserTableRow *row = new CBrowserTableRow(window, rowData);

  //---

  for (const auto &option: tag->getOptions()) {
    row->setNameValue(option->getName(), option->getValue());
  }

  //---

  row->init();

  return row;
}

CBrowserObject *
CBrowserOutputTtTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserFlushText(window, tag);

  //---

  CBrowserTT *tt = new CBrowserTT(window);

  //---

  for (const auto &option: tag->getOptions()) {
    tt->setNameValue(option->getName(), option->getValue());
  }

  //---

  tt->init();

  return tt;
}

CBrowserObject *
CBrowserOutputUTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserFlushText(window, tag);

  //---

  CBrowserUStyle *style = new CBrowserUStyle(window);

  //---

  for (const auto &option: tag->getOptions()) {
    style->setNameValue(option->getName(), option->getValue());
  }

  //---

  style->init();

  return style;
}

CBrowserObject *
CBrowserOutputUlTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserFlushParagraph(window, tag);

  //---

  CBrowserObject *currentObj = window->currentObj();

  if (currentObj && currentObj->type() == CHtmlTagId::LI)
    CBrowserFlushTags(window, tag, CHtmlTagId::LI);

  //---

  CBrowserOutputListData listData;

  CBrowserList *list = new CBrowserList(window, CHtmlTagId::UL, listData);

  //---

  for (const auto &option: tag->getOptions()) {
    list->setNameValue(option->getName(), option->getValue());
  }

  //---

  list->init();

  return list;
}

CBrowserObject *
CBrowserOutputVarTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserFlushText(window, tag);

  //---

  CBrowserVarStyle *style = new CBrowserVarStyle(window);

  //---

  for (const auto &option: tag->getOptions()) {
    style->setNameValue(option->getName(), option->getValue());
  }

  //---

  style->init();

  return style;
}

CBrowserObject *
CBrowserOutputWbrTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserWbr *wbr = new CBrowserWbr(window);

  //---

  for (const auto &option: tag->getOptions()) {
    wbr->setNameValue(option->getName(), option->getValue());
  }

  //---

  wbr->init();

  return wbr;
}

CBrowserObject *
CBrowserOutputXmpTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserXmp *xmp = new CBrowserXmp(window);

  //---

  for (const auto &option: tag->getOptions()) {
    xmp->setNameValue(option->getName(), option->getValue());
  }

  //---

  xmp->init();

  return xmp;
}
