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

CBrowserParagraph *createParagraph(CBrowserWindowIFace *window) {
  auto *paragraph = new CBrowserParagraph(window);

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
CBrowserFlushObj(CBrowserWindowIFace *window, CHtmlTag *tag, bool handleEmpty)
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

  //std::cerr << "Flush Tag '" << currentObj->typeName() <<
  //             "' for '" << tag->getName() << "'" << "\n";

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
CBrowserIsTagOpen(CBrowserWindowIFace *window, CHtmlTag *tag)
{
  CBrowserObject *obj = window->currentObj();

  while (obj && obj->type() != tag->getTagId()) {
    obj = obj->parent();
  }

  return (obj && obj->type() == tag->getTagId());
}

CBrowserObject *
CBrowserFlushTags(CBrowserWindowIFace *window, CHtmlTag *tag, CHtmlTagId id, bool flushFound=false)
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
CBrowserFlushBlock(CBrowserWindowIFace *window, CHtmlTag *tag)
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
CBrowserFlushText(CBrowserWindowIFace *window, CHtmlTag *tag)
{
  CBrowserObject *currentObj = window->currentObj();

  if (currentObj && currentObj->type() == CHtmlTagId::TEXT)
    CBrowserFlushTags(window, tag, CHtmlTagId::TEXT, true);
}

void
CBrowserFlushParagraph(CBrowserWindowIFace *window, CHtmlTag *tag)
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
CBrowserAutoStartParagraph(CBrowserWindowIFace *window)
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
CBrowserOutput(CBrowserWindowIFace *window) :
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
      auto *html = new CBrowserHtml(window_);

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

    CBrowserWindowIFace::NameValues nameValues;

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
    window_->getDocument()->setTitle(QString::fromStdString(sstr));
  }
  else if (currentObj->type() == CHtmlTagId::SCRIPT) {
    auto *script = dynamic_cast<CBrowserScript *>(currentObj);

    script->setText(script->text() + str);
  }
  else if (currentObj->type() == CHtmlTagId::STYLE) {
    auto *style = dynamic_cast<CBrowserCSSStyle *>(currentObj);

    style->setText(style->text() + str);
  }
  else if (currentObj->type() == CHtmlTagId::OPTION) {
    auto *option = dynamic_cast<CBrowserFormOption *>(currentObj);

    if (sstr != "")
      option->setText(option->text() + sstr);
  }
  else if (currentObj->type() == CHtmlTagId::TEXTAREA) {
    auto *textArea = dynamic_cast<CBrowserFormTextarea *>(currentObj);

    textArea->setText(textArea->text() + str);
  }
  else if (currentObj->type() == CHtmlTagId::BUTTON) {
    auto *button = dynamic_cast<CBrowserFormButton *>(currentObj);

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
//std::cerr << "start '" << tag->getName() << "'\n";

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
//std::cerr << "end '" << tag->getName() << "'\n";

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
CBrowserOutputATag::start(CBrowserWindowIFace *window, CHtmlTag *tag)
{
  CBrowserFlushText(window, tag);

  // auto start paragraph
  if (! isParagraphObj(window->currentObj()))
    createParagraph(window);

  //---

  auto *link = new CBrowserAnchor(window);

  CBrowserInitTag(link, tag);

  link->init();

  return link;
}

CBrowserObject *
CBrowserOutputAbbrTag::start(CBrowserWindowIFace *window, CHtmlTag *tag)
{
  auto *obj = new CBrowserObject(window, CHtmlTagId::ABBR);

  CBrowserInitTag(obj, tag);

  //---

  obj->init();

  return obj;
}

CBrowserObject *
CBrowserOutputAcronymTag::start(CBrowserWindowIFace *window, CHtmlTag *tag)
{
  auto *obj = new CBrowserObject(window, CHtmlTagId::ACRONYM);

  CBrowserInitTag(obj, tag);

  obj->init();

  return obj;
}

CBrowserObject *
CBrowserOutputAddressTag::start(CBrowserWindowIFace *window, CHtmlTag *tag)
{
  auto *address = new CBrowserAddress(window);

  CBrowserInitTag(address, tag);

  address->init();

  return address;
}

CBrowserObject *
CBrowserOutputAppletTag::start(CBrowserWindowIFace *window, CHtmlTag *tag)
{
  auto *obj = new CBrowserObject(window, CHtmlTagId::APPLET);

  obj->setDisplay(CBrowserObject::Display::INLINE);

  CBrowserInitTag(obj, tag);

  obj->init();

  return obj;
}

CBrowserObject *
CBrowserOutputAreaTag::start(CBrowserWindowIFace *window, CHtmlTag *tag)
{
  auto *area = new CBrowserArea(window);

  CBrowserInitTag(area, tag);

  area->init();

  return area;
}

CBrowserObject *
CBrowserOutputArticleTag::start(CBrowserWindowIFace *window, CHtmlTag *tag)
{
  auto *obj = new CBrowserObject(window, CHtmlTagId::ARTICLE);

  CBrowserInitTag(obj, tag);

  obj->init();

  return obj;
}

CBrowserObject *
CBrowserOutputASideTag::start(CBrowserWindowIFace *window, CHtmlTag *tag)
{
  auto *obj = new CBrowserObject(window, CHtmlTagId::ASIDE);

  CBrowserInitTag(obj, tag);

  obj->init();

  return obj;
}

CBrowserObject *
CBrowserOutputAudioTag::start(CBrowserWindowIFace *window, CHtmlTag *tag)
{
  auto *obj = new CBrowserObject(window, CHtmlTagId::AUDIO);

  CBrowserInitTag(obj, tag);

  obj->init();

  return obj;
}

CBrowserObject *
CBrowserOutputBTag::start(CBrowserWindowIFace *window, CHtmlTag *tag)
{
  CBrowserFlushText(window, tag);

  CBrowserAutoStartParagraph(window);

  //---

  auto *style = new CBrowserBStyle(window);

  CBrowserInitTag(style, tag);

  style->init();

  return style;
}

CBrowserObject *
CBrowserOutputBaseTag::start(CBrowserWindowIFace *window, CHtmlTag *tag)
{
  auto *obj = new CBrowserObject(window, CHtmlTagId::BASE);

  CBrowserInitTag(obj, tag);

  obj->init();

  return obj;
}

CBrowserObject *
CBrowserOutputBasefontTag::start(CBrowserWindowIFace *window, CHtmlTag *tag)
{
  CBrowserFlushText(window, tag);

  //---

  auto *baseFont = new CBrowserBaseFont(window);

  //---

  CBrowserInitTag(baseFont, tag);

  baseFont->init();

  return baseFont;
}

CBrowserObject *
CBrowserOutputBdiTag::start(CBrowserWindowIFace *window, CHtmlTag *tag)
{
  auto *obj = new CBrowserObject(window, CHtmlTagId::BDI);

  CBrowserInitTag(obj, tag);

  obj->init();

  return obj;
}

CBrowserObject *
CBrowserOutputBdoTag::start(CBrowserWindowIFace *window, CHtmlTag *tag)
{
  auto *obj = new CBrowserObject(window, CHtmlTagId::BDO);

  CBrowserInitTag(obj, tag);

  obj->init();

  return obj;
}

CBrowserObject *
CBrowserOutputBigTag::start(CBrowserWindowIFace *window, CHtmlTag *tag)
{
  CBrowserFlushText(window, tag);

  //---

  auto *style = new CBrowserBigStyle(window);

  CBrowserInitTag(style, tag);

  style->init();

  return style;
}

CBrowserObject *
CBrowserOutputBlinkTag::start(CBrowserWindowIFace *window, CHtmlTag *tag)
{
  CBrowserFlushText(window, tag);

  //---

  auto *style = new CBrowserBlinkStyle(window);

  CBrowserInitTag(style, tag);

  style->init();

  return style;
}

CBrowserObject *
CBrowserOutputBlockquoteTag::start(CBrowserWindowIFace *window, CHtmlTag *tag)
{
  auto *blockQuote = new CBrowserBlockQuote(window);

  //---

  CBrowserInitTag(blockQuote, tag);

  blockQuote->init();

  return blockQuote;
}

CBrowserObject *
CBrowserOutputBodyTag::start(CBrowserWindowIFace *window, CHtmlTag *tag)
{
  auto *body = new CBrowserBody(window);

  //---

  CBrowserInitTag(body, tag);

  body->init();

  return body;
}

CBrowserObject *
CBrowserOutputBrTag::start(CBrowserWindowIFace *window, CHtmlTag *tag)
{
  CBrowserFlushText(window, tag);

  //---

  auto *breaker = new CBrowserBreak(window);

  //---

  CBrowserInitTag(breaker, tag);

  breaker->init();

  return breaker;
}

CBrowserObject *
CBrowserOutputButtonTag::start(CBrowserWindowIFace *window, CHtmlTag *tag)
{
  auto *obj = new CBrowserFormButton(window);

  CBrowserInitTag(obj, tag);

  obj->init();

  return obj;
}

CBrowserObject *
CBrowserOutputCanvasTag::start(CBrowserWindowIFace *window, CHtmlTag *tag)
{
  auto *canvas = new CBrowserCanvas(window);

  //---

  CBrowserInitTag(canvas, tag);

  canvas->init();

  return canvas;
}

CBrowserObject *
CBrowserOutputCaptionTag::start(CBrowserWindowIFace *window, CHtmlTag *tag)
{
  auto *caption = new CBrowserTableCaption(window);

  //---

  CBrowserInitTag(caption, tag);

  caption->init();

  return caption;
}

CBrowserObject *
CBrowserOutputCenterTag::start(CBrowserWindowIFace *window, CHtmlTag *tag)
{
  // TODO
  //CBrowserFlushParagraph(window, tag);

  auto *center = new CBrowserCenter(window);

  //---

  CBrowserInitTag(center, tag);

  center->init();

  return center;
}

CBrowserObject *
CBrowserOutputCiteTag::start(CBrowserWindowIFace *window, CHtmlTag *tag)
{
  CBrowserFlushText(window, tag);

  //---

  auto *style = new CBrowserCiteStyle(window);

  CBrowserInitTag(style, tag);

  style->init();

  return style;
}

CBrowserObject *
CBrowserOutputCodeTag::start(CBrowserWindowIFace *window, CHtmlTag *tag)
{
  CBrowserFlushText(window, tag);

  //---

  auto *code = new CBrowserCode(window);

  //---

  CBrowserInitTag(code, tag);

  code->init();

  return code;
}

CBrowserObject *
CBrowserOutputDdTag::start(CBrowserWindowIFace *window, CHtmlTag *tag)
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

  auto *dd = new CBrowserDataListData(window);

  CBrowserInitTag(dd, tag);

  dd->init();

  return dd;
}

CBrowserObject *
CBrowserOutputDelTag::start(CBrowserWindowIFace *window, CHtmlTag *tag)
{
  auto *obj = new CBrowserObject(window, CHtmlTagId::DEL);

  CBrowserInitTag(obj, tag);

  obj->init();

  return obj;
}

CBrowserObject *
CBrowserOutputDetailsTag::start(CBrowserWindowIFace *window, CHtmlTag *tag)
{
  auto *obj = new CBrowserObject(window, CHtmlTagId::DETAILS);

  CBrowserInitTag(obj, tag);

  obj->init();

  return obj;
}

CBrowserObject *
CBrowserOutputDfnTag::start(CBrowserWindowIFace *window, CHtmlTag *tag)
{
  CBrowserFlushText(window, tag);

  //---

  auto *dfn = new CBrowserDfnStyle(window);

  //---

  CBrowserInitTag(dfn, tag);

  dfn->init();

  return dfn;
}

CBrowserObject *
CBrowserOutputDirTag::start(CBrowserWindowIFace *window, CHtmlTag *tag)
{
  CBrowserObject *currentObj = window->currentObj();

  if (currentObj && currentObj->type() == CHtmlTagId::LI)
    CBrowserFlushTags(window, tag, CHtmlTagId::LI);

  //---

  auto *list = new CBrowserList(window, CHtmlTagId::DIR);

  //---

  CBrowserInitTag(list, tag);

  list->init();

  return list;
}

CBrowserObject *
CBrowserOutputDivTag::start(CBrowserWindowIFace *window, CHtmlTag *tag)
{
  auto *div = new CBrowserDiv(window);

  //---

  CBrowserInitTag(div, tag);

  div->init();

  return div;
}

CBrowserObject *
CBrowserOutputDlTag::start(CBrowserWindowIFace *window, CHtmlTag *tag)
{
  CBrowserObject *currentObj = window->currentObj();

  if (currentObj && currentObj->type() == CHtmlTagId::LI)
    CBrowserFlushTags(window, tag, CHtmlTagId::LI);

  //---

  auto *list = new CBrowserList(window, CHtmlTagId::DL);

  //---

  CBrowserInitTag(list, tag);

  list->init();

  return list;
}

void
CBrowserOutputDlTag::term(CBrowserWindowIFace *window, CHtmlTag *tag)
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
CBrowserOutputDtTag::start(CBrowserWindowIFace *window, CHtmlTag *tag)
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

  auto *dt = new CBrowserDataListTerm(window);

  //---

  CBrowserInitTag(dt, tag);

  dt->init();

  return dt;
}

CBrowserObject *
CBrowserOutputEmTag::start(CBrowserWindowIFace *window, CHtmlTag *tag)
{
  CBrowserFlushText(window, tag);

  //---

  auto *style = new CBrowserEmStyle(window);

  CBrowserInitTag(style, tag);

  style->init();

  return style;
}

CBrowserObject *
CBrowserOutputFieldSetTag::start(CBrowserWindowIFace *window, CHtmlTag *tag)
{
  auto *obj = new CBrowserObject(window, CHtmlTagId::FIELDSET);

  CBrowserInitTag(obj, tag);

  obj->init();

  return obj;
}

CBrowserObject *
CBrowserOutputFontTag::start(CBrowserWindowIFace *window, CHtmlTag *tag)
{
  CBrowserFlushText(window, tag);

  //---

  auto *font = new CBrowserFontStyleObj(window);

  //---

  CBrowserInitTag(font, tag);

  font->init();

  return font;
}

CBrowserObject *
CBrowserOutputFooterTag::start(CBrowserWindowIFace *window, CHtmlTag *tag)
{
  auto *obj = new CBrowserObject(window, CHtmlTagId::FOOTER);

  CBrowserInitTag(obj, tag);

  obj->init();

  return obj;
}

CBrowserObject *
CBrowserOutputFormTag::start(CBrowserWindowIFace *window, CHtmlTag *tag)
{
  auto *form = new CBrowserForm(window);

  //---

  CBrowserInitTag(form, tag);

  form->init();

  return form;
}

CBrowserObject *
CBrowserOutputH1Tag::start(CBrowserWindowIFace *window, CHtmlTag *tag)
{
  auto *header = new CBrowserHeader(window, CHtmlTagId::H1);

  //---

  CBrowserInitTag(header, tag);

  header->init();

  return header;
}

CBrowserObject *
CBrowserOutputH2Tag::start(CBrowserWindowIFace *window, CHtmlTag *tag)
{
  auto *header = new CBrowserHeader(window, CHtmlTagId::H2);

  //---

  CBrowserInitTag(header, tag);

  header->init();

  return header;
}

CBrowserObject *
CBrowserOutputH3Tag::start(CBrowserWindowIFace *window, CHtmlTag *tag)
{
  auto *header = new CBrowserHeader(window, CHtmlTagId::H3);

  //---

  CBrowserInitTag(header, tag);

  header->init();

  return header;
}

CBrowserObject *
CBrowserOutputH4Tag::start(CBrowserWindowIFace *window, CHtmlTag *tag)
{
  auto *header = new CBrowserHeader(window, CHtmlTagId::H4);

  //---

  CBrowserInitTag(header, tag);

  header->init();

  return header;
}

CBrowserObject *
CBrowserOutputH5Tag::start(CBrowserWindowIFace *window, CHtmlTag *tag)
{
  auto *header = new CBrowserHeader(window, CHtmlTagId::H5);

  //---

  CBrowserInitTag(header, tag);

  header->init();

  return header;
}

CBrowserObject *
CBrowserOutputH6Tag::start(CBrowserWindowIFace *window, CHtmlTag *tag)
{
  auto *header = new CBrowserHeader(window, CHtmlTagId::H6);

  //---

  CBrowserInitTag(header, tag);

  header->init();

  return header;
}

CBrowserObject *
CBrowserOutputHeadTag::start(CBrowserWindowIFace *window, CHtmlTag *tag)
{
  auto *head = new CBrowserHead(window);

  //---

  CBrowserInitTag(head, tag);

  head->init();

  return head;
}

CBrowserObject *
CBrowserOutputHeaderTag::start(CBrowserWindowIFace *window, CHtmlTag *tag)
{
  auto *obj = new CBrowserObject(window, CHtmlTagId::HEADER);

  CBrowserInitTag(obj, tag);

  obj->init();

  return obj;
}

CBrowserObject *
CBrowserOutputHGroupTag::start(CBrowserWindowIFace *window, CHtmlTag *tag)
{
  auto *obj = new CBrowserObject(window, CHtmlTagId::HGROUP);

  CBrowserInitTag(obj, tag);

  obj->init();

  return obj;
}

CBrowserObject *
CBrowserOutputHrTag::start(CBrowserWindowIFace *window, CHtmlTag *tag)
{
  auto *rule = new CBrowserRule(window);

  //---

  CBrowserInitTag(rule, tag);

  rule->init();

  return rule;
}

CBrowserObject *
CBrowserOutputHtmlTag::start(CBrowserWindowIFace *window, CHtmlTag *tag)
{
  auto *html = new CBrowserHtml(window);

  //---

  CBrowserInitTag(html, tag);

  html->init();

  return html;
}

CBrowserObject *
CBrowserOutputITag::start(CBrowserWindowIFace *window, CHtmlTag *tag)
{
  CBrowserFlushText(window, tag);

  // auto start paragraph
  if (! isParagraphObj(window->currentObj()))
    createParagraph(window);

  //---

  auto *style = new CBrowserIStyle(window);

  CBrowserInitTag(style, tag);

  style->init();

  return style;
}

CBrowserObject *
CBrowserOutputIFrameTag::start(CBrowserWindowIFace *window, CHtmlTag *tag)
{
  auto *iframe = new CBrowserIFrame(window);

  CBrowserInitTag(iframe, tag);

  iframe->init();

  return iframe;
}

CBrowserObject *
CBrowserOutputInsTag::start(CBrowserWindowIFace *window, CHtmlTag *tag)
{
  auto *obj = new CBrowserObject(window, CHtmlTagId::INS);

  CBrowserInitTag(obj, tag);

  obj->init();

  return obj;
}

CBrowserObject *
CBrowserOutputImgTag::start(CBrowserWindowIFace *window, CHtmlTag *tag)
{
  CBrowserFlushText(window, tag);

  // auto start paragraph
  if (! isParagraphObj(window->currentObj()))
    createParagraph(window);

  //---

  auto *image = new CBrowserImage(window);

  //---

  CBrowserInitTag(image, tag);

  image->init();

  return image;
}

CBrowserObject *
CBrowserOutputInputTag::start(CBrowserWindowIFace *window, CHtmlTag *tag)
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
CBrowserOutputKbdTag::start(CBrowserWindowIFace *window, CHtmlTag *tag)
{
  CBrowserFlushText(window, tag);

  //---

  auto *kbd = new CBrowserKbd(window);

  //---

  CBrowserInitTag(kbd, tag);

  kbd->init();

  return kbd;
}

CBrowserObject *
CBrowserOutputLabelTag::start(CBrowserWindowIFace *window, CHtmlTag *tag)
{
  auto *obj = new CBrowserObject(window, CHtmlTagId::LABEL);

  obj->setDisplay(CBrowserObject::Display::INLINE);

  //---

  CBrowserInitTag(obj, tag);

  obj->init();

  return obj;
}

CBrowserObject *
CBrowserOutputLegendTag::start(CBrowserWindowIFace *window, CHtmlTag *tag)
{
  auto *obj = new CBrowserObject(window, CHtmlTagId::LEGEND);

  //---

  CBrowserInitTag(obj, tag);

  obj->init();

  return obj;
}

CBrowserObject *
CBrowserOutputLiTag::start(CBrowserWindowIFace *window, CHtmlTag *tag)
{
  CBrowserFlushText(window, tag);

  //---

  CBrowserObject *currentObj = window->currentObj();

  if (currentObj && currentObj->type() == CHtmlTagId::LI)
    CBrowserFlushTags(window, tag, CHtmlTagId::LI, true);

  //---

  auto *listItem = new CBrowserListItem(window);

  //---

  CBrowserInitTag(listItem, tag);

  listItem->init();

  return listItem;
}

CBrowserObject *
CBrowserOutputLinkTag::start(CBrowserWindowIFace *window, CHtmlTag *tag)
{
  auto *link = new CBrowserLink(window);

  //---

  CBrowserInitTag(link, tag);

  link->init();

  return link;
}

CBrowserObject *
CBrowserOutputMapTag::start(CBrowserWindowIFace *window, CHtmlTag *tag)
{
  auto *obj = new CBrowserObject(window, CHtmlTagId::MAP);

  obj->setDisplay(CBrowserObject::Display::INLINE);

  //---

  CBrowserInitTag(obj, tag);

  obj->init();

  return obj;
}

CBrowserObject *
CBrowserOutputMenuTag::start(CBrowserWindowIFace *window, CHtmlTag *tag)
{
  CBrowserObject *currentObj = window->currentObj();

  if (currentObj && currentObj->type() == CHtmlTagId::LI)
    CBrowserFlushTags(window, tag, CHtmlTagId::LI);

  //---

  auto *list = new CBrowserList(window, CHtmlTagId::MENU);

  //---

  CBrowserInitTag(list, tag);

  list->init();

  return list;
}

CBrowserObject *
CBrowserOutputMetaTag::start(CBrowserWindowIFace *window, CHtmlTag *tag)
{
  auto *meta = new CBrowserMeta(window);

  //---

  CBrowserInitTag(meta, tag);

  meta->init();

  return meta;
}

CBrowserObject *
CBrowserOutputNavTag::start(CBrowserWindowIFace *window, CHtmlTag *tag)
{
  auto *obj = new CBrowserObject(window, CHtmlTagId::NAV);

  //---

  CBrowserInitTag(obj, tag);

  obj->init();

  return obj;
}

CBrowserObject *
CBrowserOutputNobrTag::start(CBrowserWindowIFace *window, CHtmlTag *tag)
{
  CBrowserFlushText(window, tag);

  //---

  auto *nobr = new CBrowserNoBr(window);

  //---

  CBrowserInitTag(nobr, tag);

  nobr->init();

  return nobr;
}

CBrowserObject *
CBrowserOutputNoScriptTag::start(CBrowserWindowIFace *window, CHtmlTag *tag)
{
  auto *obj = new CBrowserObject(window, CHtmlTagId::NOSCRIPT);

  //---

  CBrowserInitTag(obj, tag);

  obj->init();

  return obj;
}

CBrowserObject *
CBrowserOutputOlTag::start(CBrowserWindowIFace *window, CHtmlTag *tag)
{
  CBrowserObject *currentObj = window->currentObj();

  if (currentObj && currentObj->type() == CHtmlTagId::LI)
    CBrowserFlushTags(window, tag, CHtmlTagId::LI);

  //---

  auto *list = new CBrowserList(window, CHtmlTagId::OL);

  //---

  CBrowserInitTag(list, tag);

  list->init();

  return list;
}

CBrowserObject *
CBrowserOutputOptionTag::start(CBrowserWindowIFace *window, CHtmlTag *tag)
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

  auto *option = new CBrowserFormOption(window, optionData);

  option->init();

  return option;
}

CBrowserObject *
CBrowserOutputOutputTag::start(CBrowserWindowIFace *window, CHtmlTag *tag)
{
  auto *obj = new CBrowserObject(window, CHtmlTagId::OUTPUT);

  //---

  CBrowserInitTag(obj, tag);

  obj->init();

  return obj;
}

CBrowserObject *
CBrowserOutputPTag::start(CBrowserWindowIFace *window, CHtmlTag *tag)
{
  CBrowserObject *currentObj = window->currentObj();

  if (currentObj && currentObj->type() == CHtmlTagId::P)
    CBrowserFlushTags(window, tag, currentObj->type(), true);

  //---

  auto *paragraph = new CBrowserParagraph(window);

  //---

  CBrowserInitTag(paragraph, tag);

  paragraph->init();

  return paragraph;
}

CBrowserObject *
CBrowserOutputPreTag::start(CBrowserWindowIFace *window, CHtmlTag *tag)
{
  auto *pre = new CBrowserPre(window);

  //---

  CBrowserInitTag(pre, tag);

  pre->init();

  return pre;
}

CBrowserObject *
CBrowserOutputSampTag::start(CBrowserWindowIFace *window, CHtmlTag *tag)
{
  auto *samp = new CBrowserSamp(window);

  //---

  CBrowserInitTag(samp, tag);

  samp->init();

  return samp;
}

CBrowserObject *
CBrowserOutputScriptTag::start(CBrowserWindowIFace *window, CHtmlTag *tag)
{
  CBrowserScriptData scriptData;

  scriptData.type = "text/javascript";

  auto *script = new CBrowserScript(window, scriptData);

  //---

  CBrowserInitTag(script, tag);

  script->init();

  return script;
}

CBrowserObject *
CBrowserOutputSectionTag::start(CBrowserWindowIFace *window, CHtmlTag *tag)
{
  auto *obj = new CBrowserObject(window, CHtmlTagId::SECTION);

  //---

  CBrowserInitTag(obj, tag);

  obj->init();

  return obj;
}

CBrowserObject *
CBrowserOutputSelectTag::start(CBrowserWindowIFace *window, CHtmlTag *tag)
{
  CBrowserFlushText(window, tag);

  //---

  auto *select = new CBrowserFormSelect(window);

  //---

  CBrowserInitTag(select, tag);

  select->init();

  return select;
}

CBrowserObject *
CBrowserOutputSmallTag::start(CBrowserWindowIFace *window, CHtmlTag *tag)
{
  CBrowserFlushText(window, tag);

  //---

  auto *style = new CBrowserSmallStyle(window);

  CBrowserInitTag(style, tag);

  style->init();

  return style;
}

CBrowserObject *
CBrowserOutputSpanTag::start(CBrowserWindowIFace *window, CHtmlTag *tag)
{
  auto *obj = new CBrowserObject(window, CHtmlTagId::SPAN);

  obj->setDisplay(CBrowserObject::Display::INLINE);

  //---

  CBrowserInitTag(obj, tag);

  obj->init();

  return obj;
}

CBrowserObject *
CBrowserOutputStrikeTag::start(CBrowserWindowIFace *window, CHtmlTag *tag)
{
  CBrowserFlushText(window, tag);

  //---

  auto *style = new CBrowserStrikeStyle(window);

  CBrowserInitTag(style, tag);

  style->init();

  return style;
}

CBrowserObject *
CBrowserOutputStrongTag::start(CBrowserWindowIFace *window, CHtmlTag *tag)
{
  CBrowserFlushText(window, tag);

  //---

  auto *style = new CBrowserStrongStyle(window);

  CBrowserInitTag(style, tag);

  style->init();

  return style;
}

CBrowserObject *
CBrowserOutputStyleTag::start(CBrowserWindowIFace *window, CHtmlTag *tag)
{
  auto *style = new CBrowserCSSStyle(window);

  CBrowserInitTag(style, tag);

  style->init();

  return style;
}

CBrowserObject *
CBrowserOutputSubTag::start(CBrowserWindowIFace *window, CHtmlTag *tag)
{
  CBrowserFlushText(window, tag);

  //---

  auto *style = new CBrowserSubStyle(window);

  CBrowserInitTag(style, tag);

  style->init();

  return style;
}

CBrowserObject *
CBrowserOutputSummaryTag::start(CBrowserWindowIFace *window, CHtmlTag *tag)
{
  auto *obj = new CBrowserObject(window, CHtmlTagId::SUMMARY);

  //---

  CBrowserInitTag(obj, tag);

  obj->init();

  return obj;
}

CBrowserObject *
CBrowserOutputSupTag::start(CBrowserWindowIFace *window, CHtmlTag *tag)
{
  CBrowserFlushText(window, tag);

  //---

  auto *style = new CBrowserSupStyle(window);

  CBrowserInitTag(style, tag);

  style->init();

  return style;
}

CBrowserObject *
CBrowserOutputSVGTag::start(CBrowserWindowIFace *window, CHtmlTag *tag)
{
  auto *svg = new CBrowserSVG(window);

  //---

  CBrowserInitTag(svg, tag);

  svg->init();

  return svg;
}

CBrowserObject *
CBrowserOutputTableTag::start(CBrowserWindowIFace *window, CHtmlTag *tag)
{
  CBrowserTableData tableData;

  tableData.hspace       = 0;
  tableData.vspace       = 0;
  tableData.cell_padding = 1;
  tableData.cell_spacing = 2;

  auto *table = new CBrowserTable(window, tableData);

  //---

  CBrowserInitTag(table, tag);

  table->init();

  return table;
}

CBrowserObject *
CBrowserOutputTdTag::start(CBrowserWindowIFace *window, CHtmlTag *tag)
{
  CBrowserTableCellData cellData;

  cellData.type    = CBrowserTableCellData::Type::HEADER;
  cellData.colspan = 1;
  cellData.rowspan = 1;

  auto *cell = new CBrowserTableCell(window, CHtmlTagId::TD, cellData);

  //---

  CBrowserInitTag(cell, tag);

  cell->init();

  return cell;
}

CBrowserObject *
CBrowserOutputTextareaTag::start(CBrowserWindowIFace *window, CHtmlTag *tag)
{
  auto *textArea = new CBrowserFormTextarea(window);

  //---

  CBrowserInitTag(textArea, tag);

  textArea->init();

  return textArea;
}

CBrowserObject *
CBrowserOutputTBodyTag::start(CBrowserWindowIFace *window, CHtmlTag *tag)
{
  auto *obj = new CBrowserObject(window, CHtmlTagId::TBODY);

  //---

  CBrowserInitTag(obj, tag);

  obj->init();

  return obj;
}

CBrowserObject *
CBrowserOutputTFootTag::start(CBrowserWindowIFace *window, CHtmlTag *tag)
{
  auto *obj = new CBrowserObject(window, CHtmlTagId::TFOOT);

  //---

  CBrowserInitTag(obj, tag);

  obj->init();

  return obj;
}

CBrowserObject *
CBrowserOutputTHeadTag::start(CBrowserWindowIFace *window, CHtmlTag *tag)
{
  auto *obj = new CBrowserObject(window, CHtmlTagId::THEAD);

  //---

  CBrowserInitTag(obj, tag);

  obj->init();

  return obj;
}

CBrowserObject *
CBrowserOutputThTag::start(CBrowserWindowIFace *window, CHtmlTag *tag)
{
  CBrowserTableCellData cellData;

  cellData.type    = CBrowserTableCellData::Type::HEADER;
  cellData.colspan = 1;
  cellData.rowspan = 1;

  auto *cell = new CBrowserTableCell(window, CHtmlTagId::TH, cellData);

  //---

  CBrowserInitTag(cell, tag);

  cell->init();

  return cell;
}

CBrowserObject *
CBrowserOutputTimeTag::start(CBrowserWindowIFace *window, CHtmlTag *tag)
{
  auto *obj = new CBrowserObject(window, CHtmlTagId::TIME);

  //---

  CBrowserInitTag(obj, tag);

  obj->init();

  return obj;
}

CBrowserObject *
CBrowserOutputTitleTag::start(CBrowserWindowIFace *window, CHtmlTag *tag)
{
  auto *title = new CBrowserTitle(window);

  //---

  CBrowserInitTag(title, tag);

  title->init();

  return title;
}

CBrowserObject *
CBrowserOutputTrTag::start(CBrowserWindowIFace *window, CHtmlTag *tag)
{
  auto *row = new CBrowserTableRow(window);

  //---

  CBrowserInitTag(row, tag);

  row->init();

  return row;
}

CBrowserObject *
CBrowserOutputTtTag::start(CBrowserWindowIFace *window, CHtmlTag *tag)
{
  CBrowserFlushText(window, tag);

  //---

  auto *tt = new CBrowserTT(window);

  //---

  CBrowserInitTag(tt, tag);

  tt->init();

  return tt;
}

CBrowserObject *
CBrowserOutputUTag::start(CBrowserWindowIFace *window, CHtmlTag *tag)
{
  CBrowserFlushText(window, tag);

  //---

  auto *style = new CBrowserUStyle(window);

  CBrowserInitTag(style, tag);

  style->init();

  return style;
}

CBrowserObject *
CBrowserOutputUlTag::start(CBrowserWindowIFace *window, CHtmlTag *tag)
{
  CBrowserObject *currentObj = window->currentObj();

  if (currentObj && currentObj->type() == CHtmlTagId::LI)
    CBrowserFlushTags(window, tag, CHtmlTagId::LI);

  //---

  auto *list = new CBrowserList(window, CHtmlTagId::UL);

  //---

  CBrowserInitTag(list, tag);

  list->init();

  return list;
}

CBrowserObject *
CBrowserOutputVarTag::start(CBrowserWindowIFace *window, CHtmlTag *tag)
{
  CBrowserFlushText(window, tag);

  //---

  auto *style = new CBrowserVarStyle(window);

  CBrowserInitTag(style, tag);

  style->init();

  return style;
}

CBrowserObject *
CBrowserOutputVideoTag::start(CBrowserWindowIFace *window, CHtmlTag *tag)
{
  auto *video = new CBrowserVideo(window);

  //---

  CBrowserInitTag(video, tag);

  video->init();

  return video;
}

CBrowserObject *
CBrowserOutputWbrTag::start(CBrowserWindowIFace *window, CHtmlTag *tag)
{
  auto *wbr = new CBrowserWbr(window);

  //---

  CBrowserInitTag(wbr, tag);

  wbr->init();

  return wbr;
}

CBrowserObject *
CBrowserOutputXmpTag::start(CBrowserWindowIFace *window, CHtmlTag *tag)
{
  auto *xmp = new CBrowserXmp(window);

  //---

  CBrowserInitTag(xmp, tag);

  xmp->init();

  return xmp;
}
