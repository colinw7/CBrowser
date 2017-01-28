#include <CBrowserObject.h>
#include <CBrowserWindow.h>
#include <CBrowserProperty.h>
#include <CQJHtmlObj.h>
#include <CHtmlTagDef.h>
#include <CStrUtil.h>

CBrowserObject::
CBrowserObject(CBrowserWindow *window, CHtmlTagId type, const CBrowserBaseData &data) :
 CBrowserBox(window), window_(window), type_(type), data_(data)
{
}

CBrowserObject::
~CBrowserObject()
{
  for (auto &c : children_)
    delete c;
}

void
CBrowserObject::
init()
{
  std::vector<std::string> strs = {{
    "id", "class", "name", "type", "display",
    "margin-left", "margin-bottom", "margin-right", "margin-top" }};

  addProperties(strs);
}

void
CBrowserObject::
setClass(const std::string &s)
{
  class_ = s;

  CStrUtil::toWords(class_, classes_);
}

CQJHtmlObj *
CBrowserObject::
getJObj() const
{
  return CJValue::cast<CQJHtmlObj>(objValue_).get();
}

void
CBrowserObject::
setJObj(CQJHtmlObj *obj)
{
  objValue_ = CJValueP(obj);
}

std::string
CBrowserObject::
typeName() const
{
  const CHtmlTagDef &tagDef = CHtmlTagDefLookupInst->lookup(type_);

  return tagDef.getName();
}

std::string
CBrowserObject::
hierTypeName() const
{
  const CBrowserObject *parent = this->parent();

  if (! parent)
    return typeName();

  if (parent->type() == CHtmlTagId::HTML || parent->type() == CHtmlTagId::BODY)
    return typeName();

  return parent->hierTypeName() + "/" + typeName();
}

void
CBrowserObject::
addChild(CBrowserObject *child)
{
  child->setParent(this);

  children_.push_back(child);
}

int
CBrowserObject::
childIndex(const CBrowserObject *child) const
{
  for (uint i = 0; i < children_.size(); ++i)
    if (children_[i] == child)
      return i;

  return -1;
}

void
CBrowserObject::
addProperties(const Properties &properties)
{
  for (const auto &p : properties)
    properties_.push_back(p);
}

std::string
CBrowserObject::
propertyValue(int i) const
{
  const std::string &name = propertyName(i);

  if      (name == "id"   ) return CBrowserProperty::toString(id());
  else if (name == "class") return CBrowserProperty::toString(getClass());
  else if (name == "name" ) return CBrowserProperty::toString(getName());
  else if (name == "type" ) return CBrowserProperty::toString(typeName());

  else if (name == "display") return CBrowserProperty::toString(display());

  else if (name == "margin-left"  ) return CBrowserProperty::toString(marginLeft  ());
  else if (name == "margin-bottom") return CBrowserProperty::toString(marginBottom());
  else if (name == "margin-right" ) return CBrowserProperty::toString(marginRight ());
  else if (name == "margin-top"   ) return CBrowserProperty::toString(marginTop   ());

  return "";
}

void
CBrowserObject::
setNameValue(const std::string &name, const std::string &value)
{
  std::string lname  = CStrUtil::toLower(name);
  std::string lvalue = CStrUtil::toLower(value);

  if      (lname == "accesskey") {
  }
  else if (lname == "background") {
    // bg-color bg-image position/bg-size bg-repeat bg-origin bg-clip bg-attachment initial|inherit

    CBrowserBackground bg;

    std::vector<std::string> words;

    CStrUtil::toWords(value, words);

    if (words.size() > 0)
      bg.setColor(CBrowserColor(words[0]));

    if (words.size() > 1)
      bg.setImage(CBrowserBackgroundImage(words[1]));

    if (words.size() > 2)
      bg.setPosition(words[2]);

    setBackground(bg);
  }
  else if (lname == "background-color" || lname == "bgcolor") {
    background_.setColor(CBrowserColor(value));
  }
  else if (lname == "background-image") {
    background_.setImage(CBrowserBackgroundImage(value));
  }
  else if (lname == "background-position") {
    background_.setPosition(value);
  }
  else if (lname == "background-size") {
    background_.setSize(value);
  }
  else if (lname == "background-repeat") {
    background_.setRepeat(value);
  }
  else if (lname == "background-origin") {
    background_.setOrigin(value);
  }
  else if (lname == "background-clip") {
    background_.setClip(value);
  }
  else if (lname == "background-attachment") {
    background_.setAttachment(value);
  }
  else if (lname == "border") {
    std::vector<std::string> words;

    CStrUtil::toWords(value, words);

    if (words.size() == 3) {
      CBrowserBorder border = this->border();

      border.setBottom   (CBrowserUnitValue(words[0]));
      border.setLeft     (CBrowserUnitValue(words[0]));
      border.setTop      (CBrowserUnitValue(words[0]));
      border.setRight    (CBrowserUnitValue(words[0]));
      border.setLineStyle(CBrowserBorder::stringToStyle(words[1]));
      border.setLineColor(CBrowserColor(words[2]));

      setBorder(border);
    }
  }
  else if (lname == "border-bottom") {
    std::vector<std::string> words;

    CStrUtil::toWords(value, words);

    if (words.size() == 3) {
      borderRef().setBottom   (CBrowserUnitValue(words[0]));
      borderRef().setLineStyle(CBrowserBorder::stringToStyle(words[1]));
      borderRef().setLineColor(CBrowserColor(words[2]));
    }
    else
      borderRef().setBottom(CBrowserUnitValue(value));
  }
  else if (lname == "border-left") {
    std::vector<std::string> words;

    CStrUtil::toWords(value, words);

    if (words.size() == 3) {
      borderRef().setLeft     (CBrowserUnitValue(words[0]));
      borderRef().setLineStyle(CBrowserBorder::stringToStyle(words[1]));
      borderRef().setLineColor(CBrowserColor(words[2]));
    }
    else
      borderRef().setLeft(CBrowserUnitValue(value));
  }
  else if (lname == "border-right") {
    std::vector<std::string> words;

    CStrUtil::toWords(value, words);

    if (words.size() == 3) {
      borderRef().setRight    (CBrowserUnitValue(words[0]));
      borderRef().setLineStyle(CBrowserBorder::stringToStyle(words[1]));
      borderRef().setLineColor(CBrowserColor(words[2]));
    }
    else
      borderRef().setRight(CBrowserUnitValue(value));
  }
  else if (lname == "border-top") {
    std::vector<std::string> words;

    CStrUtil::toWords(value, words);

    if (words.size() == 3) {
      borderRef().setTop      (CBrowserUnitValue(words[0]));
      borderRef().setLineStyle(CBrowserBorder::stringToStyle(words[1]));
      borderRef().setLineColor(CBrowserColor(words[2]));
    }
    else
      borderRef().setTop(CBrowserUnitValue(value));
  }
  else if (lname == "border-top-bottom-radius") {
    // TODO
  }
  else if (lname == "border-top-left-radius") {
    // TODO
  }
  else if (lname == "border-top-right-radius") {
    // TODO
  }
  else if (lname == "border-top-top-radius") {
    // TODO
  }
  else if (lname == "border-color") {
    borderRef().setLineColor(CBrowserColor(value));
  }
  else if (lname == "border-collapse") {
  }
  else if (lname == "border-spacing") {
  }
  else if (lname == "border-radius") {
    borderRef().setCornerRadius(CBrowserUnitValue(value));
  }
  else if (lname == "border-style") {
    borderRef().setLineStyle(CBrowserBorder::stringToStyle(value));
  }
  else if (lname == "border-width") {
    borderRef().setLineWidth(CBrowserUnitValue(value));
  }
  else if (lname == "box-shadow") {
    // TODO
  }
  else if (lname == "box-sizing") {
    // TODO
  }
  else if (lname == "class") {
    setClass(value);
  }
  else if (lname == "clear") {
    clear_ = CBrowserClear(value);
  }
  else if (lname == "color") {
    foreground_ = CBrowserColor(value);
  }
  else if (lname == "contenteditable") {
  }
  else if (lname == "contentmenu") {
  }
  else if (lname == "cursor") {
    // TODO
  }
  else if (lname == "dir") {
  }
  else if (lname == "display") {
    CBrowserObject::Display d;

    if (CBrowserProperty::fromString<CBrowserObject::Display>(lvalue, d))
      setDisplay(d);
  }
  else if (lname == "draggable") {
  }
  else if (lname == "dropzone") {
  }
  else if (lname == "float") {
    float_ = CBrowserFloat(value);
  }
  else if (lname == "font-family") {
    font_.setFamily(CBrowserFontFamily(value));
  }
  else if (lname == "font-size") {
    font_.setSize(CBrowserFontSize(value));
  }
  else if (lname == "font-size-adjust") {
    font_.setSizeAdjust(CBrowserFontSizeAdjust(value));
  }
  else if (lname == "font-stretch") {
    font_.setStretch(CBrowserFontStretch(value));
  }
  else if (lname == "font-weight") {
    font_.setWeight(CBrowserFontWeight(value));
  }
  else if (lname == "font-style") {
    font_.setStyle(CBrowserFontStyle(value));
  }
  else if (lname == "font") {
    // TODO
  }
  else if (lname == "height") {
    height_ = CBrowserUnitValue(value);

    if (! height_.isValid())
      window_->displayError("Illegal '%s' Height '%s' Value '%s'\n", typeName().c_str(),
                            name.c_str(), value.c_str());
  }
  else if (lname == "hidden") {
  }
  else if (lname == "id") {
    setId(value);
  }
  else if (lname == "lang") {
  }
  else if (lname == "left") {
    position_.setLeft(CBrowserUnitValue(value));
  }
  else if (lname == "margin") {
    std::vector<std::string> words;

    CStrUtil::toWords(value, words);

    std::vector<CBrowserUnitValue> uvalues;

    for (const auto &w : words)
      uvalues.push_back(CBrowserUnitValue(w));

    CBrowserMargin margin = this->margin();

    if      (words.size() == 1) {
      margin.setTop   (uvalues[0]);
      margin.setRight (uvalues[0]);
      margin.setBottom(uvalues[0]);
      margin.setLeft  (uvalues[0]);
    }
    else if (words.size() == 2) {
      margin.setTop   (uvalues[0]);
      margin.setBottom(uvalues[0]);
      margin.setRight (uvalues[1]);
      margin.setLeft  (uvalues[1]);
    }
    else if (words.size() == 3) {
      margin.setTop   (uvalues[0]);
      margin.setRight (uvalues[1]);
      margin.setLeft  (uvalues[1]);
      margin.setBottom(uvalues[2]);
    }
    else if (words.size() == 4) {
      margin.setTop   (uvalues[0]);
      margin.setRight (uvalues[1]);
      margin.setBottom(uvalues[2]);
      margin.setLeft  (uvalues[3]);
    }

    setMargin(margin);
  }
  else if (lname == "margin-bottom") {
    marginRef().setBottom(CBrowserUnitValue(value));
  }
  else if (lname == "margin-left") {
    marginRef().setLeft(CBrowserUnitValue(value));
  }
  else if (lname == "margin-right") {
    marginRef().setRight(CBrowserUnitValue(value));
  }
  else if (lname == "margin-top") {
    marginRef().setTop(CBrowserUnitValue(value));
  }
  else if (lname == "name") {
    setName(value);
  }
  else if (lname == "outline") {
    // TODO
  }
  else if (lname == "overflow") {
    // TODO
  }
  else if (lname == "padding") {
    std::vector<std::string> words;

    CStrUtil::toWords(value, words);

    std::vector<CBrowserUnitValue> uvalues;

    for (const auto &w : words)
      uvalues.push_back(CBrowserUnitValue(w));

    CBrowserPadding padding = this->padding();

    if      (words.size() == 1) {
      padding.setTop   (uvalues[0]);
      padding.setRight (uvalues[0]);
      padding.setBottom(uvalues[0]);
      padding.setLeft  (uvalues[0]);
    }
    else if (words.size() == 2) {
      padding.setTop   (uvalues[0]);
      padding.setBottom(uvalues[0]);
      padding.setRight (uvalues[1]);
      padding.setLeft  (uvalues[1]);
    }
    else if (words.size() == 3) {
      padding.setTop   (uvalues[0]);
      padding.setRight (uvalues[1]);
      padding.setLeft  (uvalues[1]);
      padding.setBottom(uvalues[2]);
    }
    else if (words.size() == 4) {
      padding.setTop   (uvalues[0]);
      padding.setRight (uvalues[1]);
      padding.setBottom(uvalues[2]);
      padding.setLeft  (uvalues[3]);
    }

    setPadding(padding);
  }
  else if (lname == "padding-bottom") {
    paddingRef().setBottom(CBrowserUnitValue(value));
  }
  else if (lname == "padding-left") {
    paddingRef().setLeft(CBrowserUnitValue(value));
  }
  else if (lname == "padding-right") {
    paddingRef().setRight(CBrowserUnitValue(value));
  }
  else if (lname == "padding-top") {
    paddingRef().setTop(CBrowserUnitValue(value));
  }
  else if (lname == "position") {
    position_.setType(value);
  }
  else if (lname == "spellcheck") {
  }
  else if (lname == "style") {
    processStyleAttribute(value);
  }
  else if (lname == "tabindex") {
  }
  else if (lname == "text-align") {
    textProp_.setAlign(CBrowserTextAlign(value));
  }
  else if (lname == "text-decoration") {
    textProp_.setDecoration(CBrowserTextDecoration(value));
  }
  else if (lname == "text-shadow") {
    textProp_.setShadow(CBrowserTextShadow(value));
  }
  else if (lname == "title") {
    title_ = value;
  }
  else if (lname == "top") {
    position_.setTop(CBrowserUnitValue(value));
  }
  else if (lname == "translate") {
  }
  else if (lname == "vertical-align") {
    textProp_.setVerticalAlign(CBrowserTextVAlign(value));
  }
  else if (lname == "white-space") {
    CBrowserObject::WhiteSpace w;

    if (CBrowserProperty::fromString<CBrowserObject::WhiteSpace>(lvalue, w))
      setWhiteSpace(w);
  }
  else if (lname == "width") {
    width_ = CBrowserUnitValue(value);

    if (! width_.isValid())
      window_->displayError("Illegal '%s' Width '%s' Value '%s'\n", typeName().c_str(),
                            name.c_str(), value.c_str());
  }
  else if (lname.substr(0, 5) == "data-") {
  }
  else {
    window_->displayError("Illegal '%s' Option Name '%s' Value '%s'\n", typeName().c_str(),
                          name.c_str(), value.c_str());
  }
}

void
CBrowserObject::
processStyleAttribute(const std::string &style)
{
  std::vector<std::string> words;

  CStrUtil::addFields(style, words, ";");

  for (const auto &word : words) {
    std::string sword = CStrUtil::stripSpaces(word);

    if (sword == "")
      continue;

    std::vector<std::string> words1;

    CStrUtil::addFields(sword, words1, ":");

    if (words1.size() != 2) {
      window_->displayError("Style name:value '%s'", word.c_str());
      continue;
    }

    std::string name  = CStrUtil::stripSpaces(words1[0]);
    std::string value = CStrUtil::stripSpaces(words1[1]);

    setNameValue(name, value);
  }
}

bool
CBrowserObject::
isHierSelected() const
{
  if (isSelected())
    return true;

  const CBrowserObject *parent = this->parent();

  while (parent) {
    if (parent->isSelected())
      return true;

    parent = parent->parent();
  }

  return false;
}

CIBBox2D
CBrowserObject::
calcBBox() const
{
  CBrowserRegion region = this->calcRegion();

  return CIBBox2D(region.x(), region.y(),
                  region.x() + region.width(), region.y() + region.height());
}

bool
CBrowserObject::
isVisible() const
{
  return display() != Display::NONE;
}

bool
CBrowserObject::
isInline() const
{
  return display() == Display::INLINE;
}

bool
CBrowserObject::
hasInlineChildren() const
{
  for (auto &c : children_) {
    if (c->isInline())
      return true;
  }

  return false;
}

void
CBrowserObject::
heightForWidth(CTextBox &box) const
{
  CBrowserRegion region = this->calcRegion();

  box.setAscent (region.ascent ());
  box.setDescent(region.descent());
}

void
CBrowserObject::
getInlineWords(Words &) const
{
}

void
CBrowserObject::
draw(const CTextBox &region)
{
  fillBackground(region);

  drawBorder(region);

  if (isSelected())
    window_->drawSelected(region.x(), region.y(), region.width(), region.height());
}

void
CBrowserObject::
fillBackground(const CTextBox &region)
{
  if      (background_.color().isValid()) {
    window_->fillRectangle(region.x(), region.y(), region.width(), region.height(),
                           CBrush(background().color().color()));
  }
  else if (background().image().isValid()) {
    window_->fillRectangle(region.x(), region.y(), region.width(), region.height(),
                           CBrush(background().image().image()));
  }
}

void
CBrowserObject::
drawBorder(const CTextBox &region)
{
  int x1 = region.xmin();
  int y1 = region.ymin();
  int x2 = region.xmax();
  int y2 = region.ymax();

  CPen pen(border().lineColor().color());

  if (border().top   ().isValid()) { window_->drawLine(x1, y1, x2, y1, pen); }
  if (border().left  ().isValid()) { window_->drawLine(x1, y1, x1, y2, pen); }
  if (border().right ().isValid()) { window_->drawLine(x2, y1, x2, y2, pen); }
  if (border().bottom().isValid()) { window_->drawLine(x1, y2, x2, y2, pen); }
}

CBrowserObject::Display
CBrowserObject::
display() const
{
  if (display_ == Display::INVALID) {
    const CHtmlTagDef &tagDef = CHtmlTagDefLookupInst->lookup(type_);

    if (tagDef.isInline())
      return Display::INLINE;

    return Display::BLOCK;
  }
  else
    return display_;
}

CBrowserObject::WhiteSpace
CBrowserObject::
hierWhiteSpace() const
{
  if (whiteSpace() == WhiteSpace::INHERIT) {
    if (parent())
      return parent()->hierWhiteSpace();

    return WhiteSpace::NORMAL;
  }

  return whiteSpace();
}

CFontPtr
CBrowserObject::
hierFont() const
{
  CBrowserObject *th = const_cast<CBrowserObject *>(this);

  th->font_.setUnderline(textProp_.decoration().type() ==
                          CBrowserTextDecoration::Type::UNDERLINE);

  th->font_.setStrike(textProp_.decoration().type() ==
                       CBrowserTextDecoration::Type::LINE_THROUGH);

  th->font_.setSuperscript(textProp_.verticalAlign().type() == CBrowserTextVAlign::Type::SUPER);
  th->font_.setSubscript(textProp_.verticalAlign().type() == CBrowserTextVAlign::Type::SUB);

  return font().font(this);
}

CRGBA
CBrowserObject::
hierFgColor() const
{
  if (foreground().isValid())
    return foreground().color();

  if (parent())
    return parent()->hierFgColor();

  return window_->getFgColor();
}
