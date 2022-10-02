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
#include <CBrowserIFrame.h>
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
#include <CBrowserSVG.h>
#include <CBrowserTable.h>
#include <CBrowserText.h>
#include <CBrowserTitle.h>
#include <CBrowserTT.h>
#include <CBrowserVideo.h>
#include <CBrowserWindow.h>
#include <CBrowserXmp.h>

#include <CBrowserMain.h>
#include <CBrowserCeil.h>
#include <CQJavaScript.h>

namespace {

bool isParagraphObj(CBrowserObject *obj) {
  if (! obj) return false;

  if (obj->type() == CHtmlTagId::ADDRESS || obj->type() == CHtmlTagId::P  ||
      obj->type() == CHtmlTagId::TD      ||
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

std::string tagName(const CHtmlTag *tag) {
  return tag->getName() + "@" + std::to_string(tag->lineNum()) +
                          ":" + std::to_string(tag->charNum());
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

bool
CBrowserIsTagOpen(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserObject *obj = window->currentObj();

  while (obj && obj->type() != tag->getTagId()) {
    obj = obj->parent();
  }

  return (obj && obj->type() == tag->getTagId());
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

  // can't flush out of paragraph or table cell
  if (currentObj->type() == CHtmlTagId::BODY ||
      currentObj->type() == CHtmlTagId::TD ||
      currentObj->type() == CHtmlTagId::LI ||
      currentObj->type() == CHtmlTagId::LABEL)
    return;

  if (isParagraphObj(currentObj))
    CBrowserFlushTags(window, tag, currentObj->type(), true);
}

CBrowserObject *
CBrowserAutoStartParagraph(CBrowserWindow *window)
{
  CBrowserObject *currentObj = window->currentObj();

  if (currentObj->type() == CHtmlTagId::BODY ||
      currentObj->type() == CHtmlTagId::FORM ||
      currentObj->type() == CHtmlTagId::DIV  ||
      currentObj->type() == CHtmlTagId::SPAN) {
    // auto start paragraph
    currentObj = createParagraph(window);
  }

  return currentObj;
}

void
CBrowserInitTag(CBrowserObject *obj, CHtmlTag *tag)
{
  if (! tag) return;

  obj->setTag(tag);

  for (const auto &option : tag->getOptions()) {
    obj->setNameValue(option->getName(), option->getValue());
  }
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

  for (int i = 0; i < int(tokens.size()); ++i) {
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

      if (currentTag &&
          (currentTag->getTagDef().getId() == CHtmlTagId::CANVAS ||
           currentTag->getTagDef().getId() == CHtmlTagId::SVG))
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
    CBrowserObject *currentObj = window_->currentObj();

    if (! currentObj->processTag(tag))
      window_->displayError("Tag '%s' not implemented\n", tagName(tag).c_str());

    return;
  }

  if (tag->isStartTag()) {
    CBrowserObject::Display d = CBrowserObject::Display::INVALID;

    CBrowserWindow::NameValues nameValues;

    window_->getTagNameValues(tag, nameValues);

    auto p = nameValues.find("display");

    if (p != nameValues.end()) {
      std::string lstr = CStrUtil::toLower((*p).second);

      if (! CBrowserProperty::fromString<CBrowserObject::Display>(lstr, d))
        d = CBrowserObject::Display::INVALID;
    }

    if (d == CBrowserObject::Display::BLOCK)
      CBrowserFlushParagraph(window_, tag);

    processStartTag(tag, output_data);
  }
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
      currentObj->type() == CHtmlTagId::DIV  ||
      currentObj->type() == CHtmlTagId::SPAN) {
    if (sstr == "")
      return;

    // auto start paragraph
    currentObj = createParagraph(window_);
  }

  //---

  if      (currentObj->type() == CHtmlTagId::HTML) {
    if (sstr != "")
      window_->displayError("Ignore text '%s' for html tag\n", str.c_str());
  }
  else if (currentObj->type() == CHtmlTagId::HEAD) {
    if (sstr != "")
      window_->displayError("Ignore text '%s' for head tag\n", str.c_str());
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
  else if (currentObj->type() == CHtmlTagId::BUTTON) {
    CBrowserFormButton *button = dynamic_cast<CBrowserFormButton *>(currentObj);

    button->setLabel(str);
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
    window_->displayError("No browser object for '%s'\n", tagName(tag).c_str());
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
    window_->displayError("Unexpected End Tag '%s' for empty tag.\n", tagName(tag).c_str());
    return;
  }

  int num_options = tag->getNumOptions();

  if (num_options != 0)
    window_->displayError("Invalid Number of Options for '%s'\n", tagName(tag).c_str());

  //---

  // flush open tags
  CBrowserObject *obj = nullptr;

  if (CBrowserIsTagOpen(window_, tag)) {
    obj = CBrowserFlushTags(window_, tag, output_data->id());

    if (! obj)
      window_->displayError("End Tag '%s' with no Start Tag.\n", tagName(tag).c_str());
  }
  else {
    window_->displayError("No open '%s' Tag.\n", tagName(tag).c_str());
  }

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

  CBrowserAnchor *link = new CBrowserAnchor(window);

  CBrowserInitTag(link, tag);

  link->init();

  return link;
}

CBrowserObject *
CBrowserOutputAbbrTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserObject *obj = new CBrowserObject(window, CHtmlTagId::ABBR);

  CBrowserInitTag(obj, tag);

  //---

  obj->init();

  return obj;
}

CBrowserObject *
CBrowserOutputAcronymTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserObject *obj = new CBrowserObject(window, CHtmlTagId::ACRONYM);

  CBrowserInitTag(obj, tag);

  obj->init();

  return obj;
}

CBrowserObject *
CBrowserOutputAddressTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserAddress *address = new CBrowserAddress(window);

  CBrowserInitTag(address, tag);

  address->init();

  return address;
}

CBrowserObject *
CBrowserOutputAppletTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserObject *obj = new CBrowserObject(window, CHtmlTagId::APPLET);

  obj->setDisplay(CBrowserObject::Display::INLINE);

  CBrowserInitTag(obj, tag);

  obj->init();

  return obj;
}

CBrowserObject *
CBrowserOutputAreaTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserArea *area = new CBrowserArea(window);

  CBrowserInitTag(area, tag);

  area->init();

  return area;
}

CBrowserObject *
CBrowserOutputArticleTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserObject *obj = new CBrowserObject(window, CHtmlTagId::ARTICLE);

  CBrowserInitTag(obj, tag);

  obj->init();

  return obj;
}

CBrowserObject *
CBrowserOutputASideTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserObject *obj = new CBrowserObject(window, CHtmlTagId::ASIDE);

  CBrowserInitTag(obj, tag);

  obj->init();

  return obj;
}

CBrowserObject *
CBrowserOutputAudioTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserObject *obj = new CBrowserObject(window, CHtmlTagId::AUDIO);

  CBrowserInitTag(obj, tag);

  obj->init();

  return obj;
}

CBrowserObject *
CBrowserOutputBTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserFlushText(window, tag);

  CBrowserAutoStartParagraph(window);

  //---

  CBrowserBStyle *style = new CBrowserBStyle(window);

  CBrowserInitTag(style, tag);

  style->init();

  return style;
}

CBrowserObject *
CBrowserOutputBaseTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserObject *obj = new CBrowserObject(window, CHtmlTagId::BASE);

  CBrowserInitTag(obj, tag);

  obj->init();

  return obj;
}

CBrowserObject *
CBrowserOutputBasefontTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserFlushText(window, tag);

  //---

  CBrowserBaseFont *baseFont = new CBrowserBaseFont(window);

  //---

  CBrowserInitTag(baseFont, tag);

  baseFont->init();

  return baseFont;
}

CBrowserObject *
CBrowserOutputBdiTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserObject *obj = new CBrowserObject(window, CHtmlTagId::BDI);

  CBrowserInitTag(obj, tag);

  obj->init();

  return obj;
}

CBrowserObject *
CBrowserOutputBdoTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserObject *obj = new CBrowserObject(window, CHtmlTagId::BDO);

  CBrowserInitTag(obj, tag);

  obj->init();

  return obj;
}

CBrowserObject *
CBrowserOutputBigTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserFlushText(window, tag);

  //---

  CBrowserBigStyle *style = new CBrowserBigStyle(window);

  CBrowserInitTag(style, tag);

  style->init();

  return style;
}

CBrowserObject *
CBrowserOutputBlinkTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserFlushText(window, tag);

  //---

  CBrowserBlinkStyle *style = new CBrowserBlinkStyle(window);

  CBrowserInitTag(style, tag);

  style->init();

  return style;
}

CBrowserObject *
CBrowserOutputBlockquoteTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserBlockQuote *blockQuote = new CBrowserBlockQuote(window);

  //---

  CBrowserInitTag(blockQuote, tag);

  blockQuote->init();

  return blockQuote;
}

CBrowserObject *
CBrowserOutputBodyTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserBody *body = new CBrowserBody(window);

  //---

  CBrowserInitTag(body, tag);

  body->init();

  return body;
}

CBrowserObject *
CBrowserOutputBrTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserFlushText(window, tag);

  //---

  CBrowserBreak *breaker = new CBrowserBreak(window);

  //---

  CBrowserInitTag(breaker, tag);

  breaker->init();

  return breaker;
}

CBrowserObject *
CBrowserOutputButtonTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserObject *obj = new CBrowserFormButton(window);

  CBrowserInitTag(obj, tag);

  obj->init();

  return obj;
}

CBrowserObject *
CBrowserOutputCanvasTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserCanvas *canvas = new CBrowserCanvas(window);

  //---

  CBrowserInitTag(canvas, tag);

  canvas->init();

  return canvas;
}

CBrowserObject *
CBrowserOutputCaptionTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserTableCaption *caption = new CBrowserTableCaption(window);

  //---

  CBrowserInitTag(caption, tag);

  caption->init();

  return caption;
}

CBrowserObject *
CBrowserOutputCenterTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  // TODO
  //CBrowserFlushParagraph(window, tag);

  CBrowserCenter *center = new CBrowserCenter(window);

  //---

  CBrowserInitTag(center, tag);

  center->init();

  return center;
}

CBrowserObject *
CBrowserOutputCiteTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserFlushText(window, tag);

  //---

  CBrowserCiteStyle *style = new CBrowserCiteStyle(window);

  CBrowserInitTag(style, tag);

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

  CBrowserInitTag(code, tag);

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

  CBrowserInitTag(dd, tag);

  dd->init();

  return dd;
}

CBrowserObject *
CBrowserOutputDelTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserObject *obj = new CBrowserObject(window, CHtmlTagId::DEL);

  CBrowserInitTag(obj, tag);

  obj->init();

  return obj;
}

CBrowserObject *
CBrowserOutputDetailsTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserObject *obj = new CBrowserObject(window, CHtmlTagId::DETAILS);

  CBrowserInitTag(obj, tag);

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

  CBrowserInitTag(dfn, tag);

  dfn->init();

  return dfn;
}

CBrowserObject *
CBrowserOutputDirTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserObject *currentObj = window->currentObj();

  if (currentObj && currentObj->type() == CHtmlTagId::LI)
    CBrowserFlushTags(window, tag, CHtmlTagId::LI);

  //---

  CBrowserList *list = new CBrowserList(window, CHtmlTagId::DIR);

  //---

  CBrowserInitTag(list, tag);

  list->init();

  return list;
}

CBrowserObject *
CBrowserOutputDivTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserDiv *div = new CBrowserDiv(window);

  //---

  CBrowserInitTag(div, tag);

  div->init();

  return div;
}

CBrowserObject *
CBrowserOutputDlTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserObject *currentObj = window->currentObj();

  if (currentObj && currentObj->type() == CHtmlTagId::LI)
    CBrowserFlushTags(window, tag, CHtmlTagId::LI);

  //---

  CBrowserList *list = new CBrowserList(window, CHtmlTagId::DL);

  //---

  CBrowserInitTag(list, tag);

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

  CBrowserInitTag(dt, tag);

  dt->init();

  return dt;
}

CBrowserObject *
CBrowserOutputEmTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserFlushText(window, tag);

  //---

  CBrowserEmStyle *style = new CBrowserEmStyle(window);

  CBrowserInitTag(style, tag);

  style->init();

  return style;
}

CBrowserObject *
CBrowserOutputFieldSetTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserObject *obj = new CBrowserObject(window, CHtmlTagId::FIELDSET);

  CBrowserInitTag(obj, tag);

  obj->init();

  return obj;
}

CBrowserObject *
CBrowserOutputFontTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserFlushText(window, tag);

  //---

  CBrowserFontStyleObj *font = new CBrowserFontStyleObj(window);

  //---

  CBrowserInitTag(font, tag);

  font->init();

  return font;
}

CBrowserObject *
CBrowserOutputFooterTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserObject *obj = new CBrowserObject(window, CHtmlTagId::FOOTER);

  CBrowserInitTag(obj, tag);

  obj->init();

  return obj;
}

CBrowserObject *
CBrowserOutputFormTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserForm *form = new CBrowserForm(window);

  //---

  CBrowserInitTag(form, tag);

  form->init();

  return form;
}

CBrowserObject *
CBrowserOutputH1Tag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserHeader *header = new CBrowserHeader(window, CHtmlTagId::H1);

  //---

  CBrowserInitTag(header, tag);

  header->init();

  return header;
}

CBrowserObject *
CBrowserOutputH2Tag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserHeader *header = new CBrowserHeader(window, CHtmlTagId::H2);

  //---

  CBrowserInitTag(header, tag);

  header->init();

  return header;
}

CBrowserObject *
CBrowserOutputH3Tag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserHeader *header = new CBrowserHeader(window, CHtmlTagId::H3);

  //---

  CBrowserInitTag(header, tag);

  header->init();

  return header;
}

CBrowserObject *
CBrowserOutputH4Tag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserHeader *header = new CBrowserHeader(window, CHtmlTagId::H4);

  //---

  CBrowserInitTag(header, tag);

  header->init();

  return header;
}

CBrowserObject *
CBrowserOutputH5Tag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserHeader *header = new CBrowserHeader(window, CHtmlTagId::H5);

  //---

  CBrowserInitTag(header, tag);

  header->init();

  return header;
}

CBrowserObject *
CBrowserOutputH6Tag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserHeader *header = new CBrowserHeader(window, CHtmlTagId::H6);

  //---

  CBrowserInitTag(header, tag);

  header->init();

  return header;
}

CBrowserObject *
CBrowserOutputHeadTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserHead *head = new CBrowserHead(window);

  //---

  CBrowserInitTag(head, tag);

  head->init();

  return head;
}

CBrowserObject *
CBrowserOutputHeaderTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserObject *obj = new CBrowserObject(window, CHtmlTagId::HEADER);

  CBrowserInitTag(obj, tag);

  obj->init();

  return obj;
}

CBrowserObject *
CBrowserOutputHGroupTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserObject *obj = new CBrowserObject(window, CHtmlTagId::HGROUP);

  CBrowserInitTag(obj, tag);

  obj->init();

  return obj;
}

CBrowserObject *
CBrowserOutputHrTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserRule *rule = new CBrowserRule(window);

  //---

  CBrowserInitTag(rule, tag);

  rule->init();

  return rule;
}

CBrowserObject *
CBrowserOutputHtmlTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserHtml *html = new CBrowserHtml(window);

  //---

  CBrowserInitTag(html, tag);

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

  CBrowserInitTag(style, tag);

  style->init();

  return style;
}

CBrowserObject *
CBrowserOutputIFrameTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserIFrame *iframe = new CBrowserIFrame(window);

  CBrowserInitTag(iframe, tag);

  iframe->init();

  return iframe;
}

CBrowserObject *
CBrowserOutputInsTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserObject *obj = new CBrowserObject(window, CHtmlTagId::INS);

  CBrowserInitTag(obj, tag);

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

  CBrowserImage *image = new CBrowserImage(window);

  //---

  CBrowserInitTag(image, tag);

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

  // get type
  CBrowserFormInputData inputData;

  for (const auto &option : tag->getOptions()) {
    std::string option_name = CStrUtil::toLower(option->getName());

    if (option_name == "type") {
      inputData.type = CStrUtil::toLower(option->getValue());
    }
  }

  if (inputData.type == "")
    inputData.type = "text";

  //---

  CBrowserObject *obj = nullptr;

  if      (inputData.type == "button")
    obj = new CBrowserFormButton(window, inputData);
  else if (inputData.type == "checkbox")
    obj = new CBrowserFormCheckBox(window, inputData);
  else if (inputData.type == "date")
    obj = new CBrowserFormDate(window, inputData);
  else if (inputData.type == "email")
    obj = new CBrowserFormEmail(window, inputData);
  else if (inputData.type == "file")
    obj = new CBrowserFormFileUpload(window, inputData);
  else if (inputData.type == "hidden")
    obj = new CBrowserFormHidden(window, inputData);
  else if (inputData.type == "image")
    obj = new CBrowserFormImage(window, inputData);
  else if (inputData.type == "month")
    obj = new CBrowserFormMonth(window, inputData);
  else if (inputData.type == "number")
    obj = new CBrowserFormNumber(window, inputData);
  else if (inputData.type == "password")
    obj = new CBrowserFormPassword(window, inputData);
  else if (inputData.type == "radio")
    obj = new CBrowserFormRadio(window, inputData);
  else if (inputData.type == "range")
    obj = new CBrowserFormRange(window, inputData);
  else if (inputData.type == "reset")
    obj = new CBrowserFormReset(window, inputData);
  else if (inputData.type == "search")
    obj = new CBrowserFormSearch(window, inputData);
  else if (inputData.type == "submit")
    obj = new CBrowserFormSubmit(window, inputData);
  else if (inputData.type == "tel")
    obj = new CBrowserFormTel(window, inputData);
  else if (inputData.type == "text")
    obj = new CBrowserFormText(window, inputData);
  else {
    window->displayError("Invalid input type '%s'\n", inputData.type.c_str());
    return nullptr;
  }

  //---

  CBrowserInitTag(obj, tag);

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

  CBrowserInitTag(kbd, tag);

  kbd->init();

  return kbd;
}

CBrowserObject *
CBrowserOutputLabelTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserObject *obj = new CBrowserObject(window, CHtmlTagId::LABEL);

  obj->setDisplay(CBrowserObject::Display::INLINE);

  //---

  CBrowserInitTag(obj, tag);

  obj->init();

  return obj;
}

CBrowserObject *
CBrowserOutputLegendTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserObject *obj = new CBrowserObject(window, CHtmlTagId::LEGEND);

  //---

  CBrowserInitTag(obj, tag);

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

  CBrowserListItem *listItem = new CBrowserListItem(window);

  //---

  CBrowserInitTag(listItem, tag);

  listItem->init();

  return listItem;
}

CBrowserObject *
CBrowserOutputLinkTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserLink *link = new CBrowserLink(window);

  //---

  CBrowserInitTag(link, tag);

  link->init();

  return link;
}

CBrowserObject *
CBrowserOutputMapTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserObject *obj = new CBrowserObject(window, CHtmlTagId::MAP);

  obj->setDisplay(CBrowserObject::Display::INLINE);

  //---

  CBrowserInitTag(obj, tag);

  obj->init();

  return obj;
}

CBrowserObject *
CBrowserOutputMenuTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserObject *currentObj = window->currentObj();

  if (currentObj && currentObj->type() == CHtmlTagId::LI)
    CBrowserFlushTags(window, tag, CHtmlTagId::LI);

  //---

  CBrowserList *list = new CBrowserList(window, CHtmlTagId::MENU);

  //---

  CBrowserInitTag(list, tag);

  list->init();

  return list;
}

CBrowserObject *
CBrowserOutputMetaTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserMeta *meta = new CBrowserMeta(window);

  //---

  CBrowserInitTag(meta, tag);

  meta->init();

  return meta;
}

CBrowserObject *
CBrowserOutputNavTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserObject *obj = new CBrowserObject(window, CHtmlTagId::NAV);

  //---

  CBrowserInitTag(obj, tag);

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

  CBrowserInitTag(nobr, tag);

  nobr->init();

  return nobr;
}

CBrowserObject *
CBrowserOutputNoScriptTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserObject *obj = new CBrowserObject(window, CHtmlTagId::NOSCRIPT);

  //---

  CBrowserInitTag(obj, tag);

  obj->init();

  return obj;
}

CBrowserObject *
CBrowserOutputOlTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserObject *currentObj = window->currentObj();

  if (currentObj && currentObj->type() == CHtmlTagId::LI)
    CBrowserFlushTags(window, tag, CHtmlTagId::LI);

  //---

  CBrowserList *list = new CBrowserList(window, CHtmlTagId::OL);

  //---

  CBrowserInitTag(list, tag);

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

  for (const auto &option : tag->getOptions()) {
    std::string option_name = CStrUtil::toLower(option->getName());

    if      (option_name == "selected") {
      optionData.selected = true;
    }
    else if (option_name == "value") {
      optionData.value = option->getValue();
    }
    else
      window->displayError("Illegal '%s' Option '%s'\n", tagName(tag).c_str(),
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
  CBrowserObject *obj = new CBrowserObject(window, CHtmlTagId::OUTPUT);

  //---

  CBrowserInitTag(obj, tag);

  obj->init();

  return obj;
}

CBrowserObject *
CBrowserOutputPTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserObject *currentObj = window->currentObj();

  if (currentObj && currentObj->type() == CHtmlTagId::P)
    CBrowserFlushTags(window, tag, currentObj->type(), true);

  //---

  CBrowserParagraph *paragraph = new CBrowserParagraph(window);

  //---

  CBrowserInitTag(paragraph, tag);

  paragraph->init();

  return paragraph;
}

CBrowserObject *
CBrowserOutputPreTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserPre *pre = new CBrowserPre(window);

  //---

  CBrowserInitTag(pre, tag);

  pre->init();

  return pre;
}

CBrowserObject *
CBrowserOutputSampTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserSamp *samp = new CBrowserSamp(window);

  //---

  CBrowserInitTag(samp, tag);

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

  CBrowserInitTag(script, tag);

  script->init();

  return script;
}

CBrowserObject *
CBrowserOutputSectionTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserObject *obj = new CBrowserObject(window, CHtmlTagId::SECTION);

  //---

  CBrowserInitTag(obj, tag);

  obj->init();

  return obj;
}

CBrowserObject *
CBrowserOutputSelectTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserFlushText(window, tag);

  //---

  CBrowserFormSelect *select = new CBrowserFormSelect(window);

  //---

  CBrowserInitTag(select, tag);

  select->init();

  return select;
}

CBrowserObject *
CBrowserOutputSmallTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserFlushText(window, tag);

  //---

  CBrowserSmallStyle *style = new CBrowserSmallStyle(window);

  CBrowserInitTag(style, tag);

  style->init();

  return style;
}

CBrowserObject *
CBrowserOutputSpanTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserObject *obj = new CBrowserObject(window, CHtmlTagId::SPAN);

  obj->setDisplay(CBrowserObject::Display::INLINE);

  //---

  CBrowserInitTag(obj, tag);

  obj->init();

  return obj;
}

CBrowserObject *
CBrowserOutputStrikeTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserFlushText(window, tag);

  //---

  CBrowserStrikeStyle *style = new CBrowserStrikeStyle(window);

  CBrowserInitTag(style, tag);

  style->init();

  return style;
}

CBrowserObject *
CBrowserOutputStrongTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserFlushText(window, tag);

  //---

  CBrowserStrongStyle *style = new CBrowserStrongStyle(window);

  CBrowserInitTag(style, tag);

  style->init();

  return style;
}

CBrowserObject *
CBrowserOutputStyleTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserCSSStyle *style = new CBrowserCSSStyle(window);

  CBrowserInitTag(style, tag);

  style->init();

  return style;
}

CBrowserObject *
CBrowserOutputSubTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserFlushText(window, tag);

  //---

  CBrowserSubStyle *style = new CBrowserSubStyle(window);

  CBrowserInitTag(style, tag);

  style->init();

  return style;
}

CBrowserObject *
CBrowserOutputSummaryTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserObject *obj = new CBrowserObject(window, CHtmlTagId::SUMMARY);

  //---

  CBrowserInitTag(obj, tag);

  obj->init();

  return obj;
}

CBrowserObject *
CBrowserOutputSupTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserFlushText(window, tag);

  //---

  CBrowserSupStyle *style = new CBrowserSupStyle(window);

  CBrowserInitTag(style, tag);

  style->init();

  return style;
}

CBrowserObject *
CBrowserOutputSVGTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserSVG *svg = new CBrowserSVG(window);

  //---

  CBrowserInitTag(svg, tag);

  svg->init();

  return svg;
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

  //---

  CBrowserInitTag(table, tag);

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

  CBrowserInitTag(cell, tag);

  cell->init();

  return cell;
}

CBrowserObject *
CBrowserOutputTextareaTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserFormTextarea *textArea = new CBrowserFormTextarea(window);

  //---

  CBrowserInitTag(textArea, tag);

  textArea->init();

  return textArea;
}

CBrowserObject *
CBrowserOutputTBodyTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserObject *obj = new CBrowserObject(window, CHtmlTagId::TBODY);

  //---

  CBrowserInitTag(obj, tag);

  obj->init();

  return obj;
}

CBrowserObject *
CBrowserOutputTFootTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserObject *obj = new CBrowserObject(window, CHtmlTagId::TFOOT);

  //---

  CBrowserInitTag(obj, tag);

  obj->init();

  return obj;
}

CBrowserObject *
CBrowserOutputTHeadTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserObject *obj = new CBrowserObject(window, CHtmlTagId::THEAD);

  //---

  CBrowserInitTag(obj, tag);

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

  CBrowserInitTag(cell, tag);

  cell->init();

  return cell;
}

CBrowserObject *
CBrowserOutputTimeTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserObject *obj = new CBrowserObject(window, CHtmlTagId::TIME);

  //---

  CBrowserInitTag(obj, tag);

  obj->init();

  return obj;
}

CBrowserObject *
CBrowserOutputTitleTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserTitle *title = new CBrowserTitle(window);

  //---

  CBrowserInitTag(title, tag);

  title->init();

  return title;
}

CBrowserObject *
CBrowserOutputTrTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserTableRow *row = new CBrowserTableRow(window);

  //---

  CBrowserInitTag(row, tag);

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

  CBrowserInitTag(tt, tag);

  tt->init();

  return tt;
}

CBrowserObject *
CBrowserOutputUTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserFlushText(window, tag);

  //---

  CBrowserUStyle *style = new CBrowserUStyle(window);

  CBrowserInitTag(style, tag);

  style->init();

  return style;
}

CBrowserObject *
CBrowserOutputUlTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserObject *currentObj = window->currentObj();

  if (currentObj && currentObj->type() == CHtmlTagId::LI)
    CBrowserFlushTags(window, tag, CHtmlTagId::LI);

  //---

  CBrowserList *list = new CBrowserList(window, CHtmlTagId::UL);

  //---

  CBrowserInitTag(list, tag);

  list->init();

  return list;
}

CBrowserObject *
CBrowserOutputVarTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserFlushText(window, tag);

  //---

  CBrowserVarStyle *style = new CBrowserVarStyle(window);

  CBrowserInitTag(style, tag);

  style->init();

  return style;
}

CBrowserObject *
CBrowserOutputVideoTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserVideo *video = new CBrowserVideo(window);

  //---

  CBrowserInitTag(video, tag);

  video->init();

  return video;
}

CBrowserObject *
CBrowserOutputWbrTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserWbr *wbr = new CBrowserWbr(window);

  //---

  CBrowserInitTag(wbr, tag);

  wbr->init();

  return wbr;
}

CBrowserObject *
CBrowserOutputXmpTag::start(CBrowserWindow *window, CHtmlTag *tag)
{
  CBrowserXmp *xmp = new CBrowserXmp(window);

  //---

  CBrowserInitTag(xmp, tag);

  xmp->init();

  return xmp;
}
