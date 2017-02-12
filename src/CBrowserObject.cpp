#include <CBrowserObject.h>
#include <CBrowserWindow.h>
#include <CBrowserList.h>
#include <CBrowserListItem.h>
#include <CBrowserProperty.h>
#include <CQJHtmlObj.h>
#include <CHtmlTagDef.h>
#include <CLinearGradient.h>
#include <CStrParse.h>
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
    "margin-left", "margin-bottom", "margin-right", "margin-top",
    "background-color", "color" }};

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

  else if (name == "color") return CBrowserProperty::toString(foreground());

  else if (name == "background-color") return CBrowserProperty::toString(background());

  return "";
}

void
CBrowserObject::
setNameValue(const std::string &name, const std::string &value)
{
  std::string lname  = CStrUtil::toLower(name);
//std::string lvalue = CStrUtil::toLower(value);

  if      (lname == "accesskey") {
  }
  else if (lname == "bgcolor") {
    background_.setColor(CBrowserColor(value));
  }
  else if (lname == "class") {
    setClass(value);
  }
  else if (lname == "contenteditable") {
  }
  else if (lname == "contentmenu") {
  }
  else if (lname == "dir") {
  }
  else if (lname == "draggable") {
  }
  else if (lname == "dropzone") {
  }
  else if (lname == "hidden") {
    //visible_ = false;
  }
  else if (lname == "id") {
    setId(value);
  }
  else if (lname == "lang") {
  }
  else if (lname == "name") {
    setName(value);
  }
  else if (lname == "spellcheck") {
  }
  else if (lname == "style") {
    processStyleAttribute(value);
  }
  else if (lname == "tabindex") {
  }
  else if (lname == "title") {
    title_ = value;
  }
  else if (lname == "translate") {
  }
  else if (lname.substr(0, 5) == "data-") {
  }
}

void
CBrowserObject::
setStyleValue(const std::string &name, const std::string &value)
{
  std::string lname  = CStrUtil::toLower(name);
  std::string lvalue = CStrUtil::toLower(value);

  //--- A ---
  if      (lname == "align-items") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }
  else if (lname == "align-self") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }
  else if (lname == "all") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }
  else if (lname == "animation") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }
  else if (lname == "animation-delay") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }
  else if (lname == "animation-direction") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }
  else if (lname == "animation-duration") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }
  else if (lname == "animation-fill-mode") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }
  else if (lname == "animation-iteration-count") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }
  else if (lname == "animation-name") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }
  else if (lname == "animation-play-state") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }
  else if (lname == "animation-timing-function") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }

  //--- B ---
  else if (lname == "backface-visibility") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }
  else if (lname == "background") {
    // bg-color bg-image position/bg-size bg-repeat bg-origin bg-clip bg-attachment initial|inherit

    CBrowserBackground bg;

    std::vector<std::string> words;

    styleValueToWords(value, words);

    if (words.size() > 0)
      bg.setColor(CBrowserColor(words[0]));

    if (words.size() > 1)
      bg.setImage(CBrowserBackgroundImage(words[1]));

    if (words.size() > 2)
      bg.setPosition(CBrowserBackgroundPosition(words[2]));

    setBackground(bg);
  }
  else if (lname == "background-attachment") {
    background_.setAttachment(CBrowserBackgroundAttachment(value));
  }
  else if (lname == "background-blend-mode") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }
  else if (lname == "background-clip") {
    background_.setClip(CBrowserBackgroundClip(value));
  }
  else if (lname == "background-color") {
    background_.setColor(CBrowserColor(value));
  }
  else if (lname == "background-image") {
    background_.setImage(CBrowserBackgroundImage(value));
  }
  else if (lname == "background-origin") {
    background_.setOrigin(CBrowserBackgroundOrigin(value));
  }
  else if (lname == "background-position") {
    background_.setPosition(CBrowserBackgroundPosition(value));
  }
  else if (lname == "background-repeat") {
    background_.setRepeat(CBrowserBackgroundRepeat(value));
  }
  else if (lname == "background-size") {
    background_.setSize(CBrowserBackgroundSize(value));
  }
  else if (lname == "border") {
    std::vector<std::string> words;

    styleValueToWords(value, words);

    CBrowserBorder border = this->border();

    if (words.size() >= 1) {
      CBrowserBorderWidth width(words[0]);

      border.setWidth(width);
    }

    if (words.size() >= 2) {
      CBrowserBorderStyle style = CBrowserBorder::stringToStyle(words[1]);

      border.setStyle(style);
    }

    if (words.size() >= 3) {
      CBrowserColor color(words[2]);

      border.setColor(color);
    }

    setBorder(border);
  }
  else if (lname == "border-bottom") {
    std::vector<std::string> words;

    styleValueToWords(value, words);

    if (words.size() >= 1) {
      CBrowserBorderWidth width(words[0]);

      borderRef().setBottomWidth(width);
    }

    if (words.size() >= 2) {
      CBrowserBorderStyle style = CBrowserBorder::stringToStyle(words[1]);

      borderRef().setBottomStyle(style);
    }

    if (words.size() >= 3) {
      CBrowserColor color(words[2]);

      borderRef().setBottomColor(color);
    }
  }
  else if (lname == "border-bottom-color") {
    CBrowserColor color(value);

    borderRef().setBottomColor(color);
  }
  else if (lname == "border-bottom-left-radius") {
    CBrowserUnitValue r(value);

    borderRef().setBottomLeftRadius(r);
  }
  else if (lname == "border-bottom-right-radius") {
    CBrowserUnitValue r(value);

    borderRef().setBottomRightRadius(r);
  }
  else if (lname == "border-bottom-style") {
    CBrowserBorderStyle style = CBrowserBorder::stringToStyle(value);

    borderRef().setBottomStyle(style);
  }
  else if (lname == "border-bottom-width") {
    CBrowserBorderWidth width(value);

    borderRef().setBottomWidth(width);
  }
  else if (lname == "border-collapse") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }
  else if (lname == "border-color") {
    CBrowserColor color(value);

    borderRef().setColor(color);
  }
  else if (lname == "border-image") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }
  else if (lname == "border-image-outset") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }
  else if (lname == "border-image-repeat") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }
  else if (lname == "border-image-slice") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }
  else if (lname == "border-image-source") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }
  else if (lname == "border-image-width") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }
  else if (lname == "border-left") {
    std::vector<std::string> words;

    styleValueToWords(value, words);

    if (words.size() >= 1) {
      CBrowserBorderWidth width(words[0]);

      borderRef().setLeftWidth(width);
    }

    if (words.size() >= 2) {
      CBrowserBorderStyle style = CBrowserBorder::stringToStyle(words[1]);

      borderRef().setLeftStyle(style);
    }

    if (words.size() >= 3) {
      CBrowserColor color(words[2]);

      borderRef().setLeftColor(color);
    }
  }
  else if (lname == "border-left-color") {
    CBrowserColor color(value);

    borderRef().setLeftColor(color);
  }
  else if (lname == "border-left-style") {
    CBrowserBorderStyle style = CBrowserBorder::stringToStyle(value);

    borderRef().setLeftStyle(style);
  }
  else if (lname == "border-left-width") {
    CBrowserBorderWidth width(value);

    borderRef().setLeftWidth(width);
  }
  else if (lname == "border-radius") {
    CBrowserUnitValue r(value);

    borderRef().setTopLeftRadius    (r);
    borderRef().setTopRightRadius   (r);
    borderRef().setBottomLeftRadius (r);
    borderRef().setBottomRightRadius(r);
  }
  else if (lname == "border-right") {
    std::vector<std::string> words;

    styleValueToWords(value, words);

    if (words.size() >= 1) {
      CBrowserBorderWidth width(words[0]);

      borderRef().setRightWidth(width);
    }

    if (words.size() >= 2) {
      CBrowserBorderStyle style = CBrowserBorder::stringToStyle(words[1]);

      borderRef().setRightStyle(style);
    }

    if (words.size() >= 3) {
      CBrowserColor color(words[2]);

      borderRef().setRightColor(color);
    }
  }
  else if (lname == "border-right-color") {
    CBrowserColor color(value);

    borderRef().setRightColor(color);
  }
  else if (lname == "border-right-style") {
    CBrowserBorderStyle style = CBrowserBorder::stringToStyle(value);

    borderRef().setRightStyle(style);
  }
  else if (lname == "border-right-width") {
    CBrowserBorderWidth width(value);

    borderRef().setRightWidth(width);
  }
  else if (lname == "border-spacing") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }
  else if (lname == "border-style") {
     std::vector<std::string> words;

    styleValueToWords(value, words);

    if      (words.size() == 1) {
      borderRef().setStyle(CBrowserBorder::stringToStyle(words[0]));
    }
    else if (words.size() == 2) {
      CBrowserBorderStyle hstyle = CBrowserBorder::stringToStyle(words[0]);
      CBrowserBorderStyle vstyle = CBrowserBorder::stringToStyle(words[1]);

      borderRef().setTopStyle   (hstyle);
      borderRef().setBottomStyle(hstyle);
      borderRef().setLeftStyle  (vstyle);
      borderRef().setRightStyle (vstyle);
    }
    else if (words.size() == 3) {
      CBrowserBorderStyle vstyle = CBrowserBorder::stringToStyle(words[1]);

      borderRef().setTopStyle   (CBrowserBorder::stringToStyle(words[0]));
      borderRef().setBottomStyle(CBrowserBorder::stringToStyle(words[2]));
      borderRef().setLeftStyle  (vstyle);
      borderRef().setRightStyle (vstyle);
    }
    else if (words.size() >= 4) {
      borderRef().setTopStyle   (CBrowserBorder::stringToStyle(words[0]));
      borderRef().setRightStyle (CBrowserBorder::stringToStyle(words[1]));
      borderRef().setBottomStyle(CBrowserBorder::stringToStyle(words[2]));
      borderRef().setLeftStyle  (CBrowserBorder::stringToStyle(words[3]));
    }
  }
  else if (lname == "border-top") {
    std::vector<std::string> words;

    styleValueToWords(value, words);

    if (words.size() >= 1) {
      CBrowserBorderWidth width(words[0]);

      borderRef().setTopWidth(width);
    }

    if (words.size() >= 2) {
      CBrowserBorderStyle style = CBrowserBorder::stringToStyle(words[1]);

      borderRef().setTopStyle(style);
    }

    if (words.size() >= 3) {
      CBrowserColor color(words[2]);

      borderRef().setTopColor(color);
    }
  }
  else if (lname == "border-top-color") {
    CBrowserColor color(value);

    borderRef().setTopColor(color);
  }
  else if (lname == "border-top-left-radius") {
    CBrowserUnitValue r(value);

    borderRef().setTopLeftRadius(r);
  }
  else if (lname == "border-top-right-radius") {
    CBrowserUnitValue r(value);

    borderRef().setTopRightRadius(r);
  }
  else if (lname == "border-top-style") {
    CBrowserBorderStyle style = CBrowserBorder::stringToStyle(value);

    borderRef().setTopStyle(style);
  }
  else if (lname == "border-top-width") {
    CBrowserBorderWidth width(value);

    borderRef().setTopWidth(width);
  }
  else if (lname == "border-width") {
    CBrowserBorderWidth width(value);

    borderRef().setWidth(width);
  }
  else if (lname == "bottom") {
    position_.setBottom(CBrowserUnitValue(value));

    if (! position_.bottom().isValid())
      window_->displayError("Illegal '%s' name '%s' value '%s'\n", typeName().c_str(),
                            name.c_str(), value.c_str());
  }
  else if (lname == "box-shadow") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }
  else if (lname == "box-sizing") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }

  //--- C ---
  else if (lname == "caption-side") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }
  else if (lname == "clear") {
    clear_ = CBrowserClear(value);
  }
  else if (lname == "clip") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }
  else if (lname == "color") {
    foreground_ = CBrowserColor(value);
  }
  else if (lname == "column-count") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }
  else if (lname == "column-fill") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }
  else if (lname == "column-gap") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }
  else if (lname == "column-rule") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }
  else if (lname == "column-rule-color") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }
  else if (lname == "column-rule-style") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }
  else if (lname == "column-rule-width") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }
  else if (lname == "column-span") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }
  else if (lname == "column-width") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }
  else if (lname == "columns") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }
  else if (lname == "content") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }
  else if (lname == "counter-increment") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }
  else if (lname == "counter-reset") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }
  else if (lname == "cursor") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }

  //--- D ---
  else if (lname == "direction") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }
  else if (lname == "display") {
    CBrowserObject::Display d;

    if (CBrowserProperty::fromString<CBrowserObject::Display>(lvalue, d))
      setDisplay(d);
  }

  //--- E ---
  else if (lname == "empty-cells") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }

  //--- F ---
  else if (lname == "filter") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }
  else if (lname == "flex") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }
  else if (lname == "flex-basis") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }
  else if (lname == "flex-direction") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }
  else if (lname == "flex-flow") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }
  else if (lname == "flex-grow") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }
  else if (lname == "flex-shrink") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }
  else if (lname == "flex-wrap") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }
  else if (lname == "float") {
    float_ = CBrowserFloat(value);
  }
  else if (lname == "font") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
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
  else if (lname == "font-style") {
    font_.setStyle(CBrowserFontStyle(value));
  }
  else if (lname == "font-variant") {
  }
  else if (lname == "font-weight") {
    font_.setWeight(CBrowserFontWeight(value));
  }
  else if (lname == "@font-face") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }

  //--- H ---
  else if (lname == "hanging-punctuation") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }
  else if (lname == "height") {
    setHeight(CBrowserUnitValue(value));

    if (! height().isValid())
      window_->displayError("Illegal '%s' name '%s' value '%s'\n", typeName().c_str(),
                            name.c_str(), value.c_str());
  }

  //--- J ---
  else if (lname == "justify-content") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }

  //--- K ---
  else if (lname == "@keyframes") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }

  //--- L ---
  else if (lname == "left") {
    position_.setLeft(CBrowserUnitValue(value));
  }
  else if (lname == "letter-spacing") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }
  else if (lname == "line-height") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }
  else if (lname == "list-style") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }
  else if (lname == "list-style-image") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }
  else if (lname == "list-style-position") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }
  else if (lname == "list-style-type") {
    CBrowserListStyleType styleType(value);

    CBrowserList     *ol = dynamic_cast<CBrowserList *>(this);
    CBrowserListItem *li = dynamic_cast<CBrowserListItem *>(this);

    if      (ol)
      ol->setStyleType(styleType);
    else if (li)
      li->setStyleType(styleType);
    else
      window_->displayError("Invalid object for '%s' : value '%s'\n",
                            name.c_str(), value.c_str());
  }

  //--- M ---
  else if (lname == "margin") {
    std::vector<std::string> words;

    styleValueToWords(value, words);

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
  else if (lname == "max-height") {
    maxHeight_ = CBrowserUnitValue(value);

    if (! maxHeight_.isValid())
      window_->displayError("Illegal '%s' name '%s' value '%s'\n", typeName().c_str(),
                            name.c_str(), value.c_str());
  }
  else if (lname == "max-width") {
    maxWidth_ = CBrowserUnitValue(value);

    if (! maxWidth_.isValid())
      window_->displayError("Illegal '%s' name '%s' value '%s'\n", typeName().c_str(),
                            name.c_str(), value.c_str());
  }
  else if (lname == "@media") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }
  else if (lname == "min-height") {
    minHeight_ = CBrowserUnitValue(value);

    if (! minHeight_.isValid())
      window_->displayError("Illegal '%s' name '%s' value '%s'\n", typeName().c_str(),
                            name.c_str(), value.c_str());
  }
  else if (lname == "min-width") {
    minWidth_ = CBrowserUnitValue(value);

    if (! minWidth_.isValid())
      window_->displayError("Illegal '%s' name '%s' value '%s'\n", typeName().c_str(),
                            name.c_str(), value.c_str());
  }

  //--- N ---
  else if (lname == "nav-down") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }
  else if (lname == "nav-index") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }
  else if (lname == "nav-left") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }
  else if (lname == "nav-right") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }
  else if (lname == "nav-up") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }

  //--- O ---
  else if (lname == "opacity") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }
  else if (lname == "order") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }
  else if (lname == "outline") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }
  else if (lname == "outline-color") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }
  else if (lname == "outline-offset") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }
  else if (lname == "outline-style") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }
  else if (lname == "outline-width") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }
  else if (lname == "overflow") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }
  else if (lname == "overflow-x") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }
  else if (lname == "overflow-y") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }

  //--- P ---
  else if (lname == "padding") {
    std::vector<std::string> words;

    styleValueToWords(value, words);

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
  else if (lname == "page-break-after") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }
  else if (lname == "page-break-before") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }
  else if (lname == "page-break-inside") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }
  else if (lname == "perspective") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }
  else if (lname == "perspective-origin") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }
  else if (lname == "position") {
    position_.setType(value);
  }

  //--- Q ---
  else if (lname == "quotes") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }

  //--- R ---
  else if (lname == "resize") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }
  else if (lname == "right") {
    position_.setRight(CBrowserUnitValue(value));

    if (! position_.right().isValid())
      window_->displayError("Illegal '%s' name '%s' value '%s'\n", typeName().c_str(),
                            name.c_str(), value.c_str());
  }

  //--- T ---
  else if (lname == "tab-size") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }
  else if (lname == "table-layout") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }
  else if (lname == "text-align") {
    textProp_.setAlign(CBrowserTextAlign(value));
  }
  else if (lname == "text-align-last") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }
  else if (lname == "text-decoration") {
    textProp_.setDecoration(CBrowserTextDecoration(value));
  }
  else if (lname == "text-decoration-color") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }
  else if (lname == "text-decoration-line") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }
  else if (lname == "text-decoration-style") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }
  else if (lname == "text-indent") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }
  else if (lname == "text-justify") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }
  else if (lname == "text-overflow") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }
  else if (lname == "text-shadow") {
    textProp_.setShadow(CBrowserTextShadow(value));
  }
  else if (lname == "text-transform") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }
  else if (lname == "top") {
    position_.setTop(CBrowserUnitValue(value));
  }
  else if (lname == "transform") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }
  else if (lname == "transform-origin") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }
  else if (lname == "transform-style") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }
  else if (lname == "transition") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }
  else if (lname == "transition-delay") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }
  else if (lname == "transition-duration") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }
  else if (lname == "transition-property") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }
  else if (lname == "transition-timing-function") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }

  //--- U ---
  else if (lname == "unicode-bidi") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }
  else if (lname == "user-select") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }

  //--- V ---
  else if (lname == "vertical-align") {
    textProp_.setVerticalAlign(CBrowserTextVAlign(value));
  }
  else if (lname == "visibility") {
    //visible_ = true;
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }

  //--- W ---
  else if (lname == "white-space") {
    CBrowserObject::WhiteSpace w;

    if (CBrowserProperty::fromString<CBrowserObject::WhiteSpace>(lvalue, w))
      setWhiteSpace(w);
  }
  else if (lname == "width") {
    setWidth(CBrowserUnitValue(value));

    if (! width().isValid())
      window_->displayError("Illegal '%s' name '%s' value '%s'\n", typeName().c_str(),
                            name.c_str(), value.c_str());
  }
  else if (lname == "word-break") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }
  else if (lname == "word-spacing") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }
  else if (lname == "word-wrap") {
    window_->displayError("Unsupported style name '%s' value '%s'\n",
                          name.c_str(), value.c_str());
  }

  //--- Z ---
  else if (lname == "z-index") {
    if (CStrUtil::isInteger(value))
      zIndex_ = CStrUtil::toInteger(value);
    else
      window_->displayError("Illegal '%s' name '%s' value '%s'\n", typeName().c_str(),
                            name.c_str(), value.c_str());
  }

  //---
  else {
    window_->displayError("Illegal '%s' name '%s' value '%s'\n", typeName().c_str(),
                          name.c_str(), value.c_str());
  }
}

void
CBrowserObject::
styleValueToWords(const std::string &value, std::vector<std::string> &words)
{
  //CStrUtil::toWords(value, words);
  CStrParse parse(value);

  while (! parse.eof()) {
    int i1 = parse.getPos();

    while (! parse.eof()) {
      if      (parse.isChar('(')) {
        parse.skipChar();

        int brackets = 1;

        while (! parse.eof()) {
          if      (parse.isChar('('))
            ++brackets;
          else if (parse.isChar(')')) {
            --brackets;

            if (brackets == 0) {
              parse.skipChar();
              break;
            }
          }

          parse.skipChar();
        }
      }
      else if (parse.isSpace())
        break;

      parse.skipChar();
    }

    std::string word = parse.getBefore(i1);

    words.push_back(word);

    parse.skipSpace();
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
fillBackground(const CTextBox &region)
{
  int x = region.x();
  int y = region.y();
  int w = region.width();
  int h = region.height();

  if      (background_.color().isValid()) {
    CBrush brush;

    if (background().color().type() == CBrowserColor::Type::COLOR) {
      const CRGBA &c = background().color().color();

      brush = CBrush(c);
    }
    else {
      const CBrowserColorGradient &g = background().color().gradient();

      CLinearGradient *gradient = new CLinearGradient;

      gradient->setX1(0);
      gradient->setY1(0);

      // top left -> right
      if (g.direction() & uint(CBrowserColorGradient::Direction::RIGHT))
        gradient->setX2(1);
      else
        gradient->setX2(0);

      if (g.direction() & uint(CBrowserColorGradient::Direction::BOTTOM))
        gradient->setY2(1);
      else
        gradient->setY2(0);

      double d = 1.0/std::max(int(g.colors().size()) - 1, 1);

      double offset = 0.0;

      for (const auto &c : g.colors()) {
        gradient->addStop(offset, c);

        offset += d;
      }

      brush.setGradient(CBrush::GradientPtr(gradient));
    }

    window_->fillRectangle(x, y, w, h, brush);
  }
  else if (background().image().isValid()) {
    CBrush brush = CBrush(background().image().image());

    window_->fillRectangle(x, y, w, h, brush);
  }
}

void
CBrowserObject::
draw(const CTextBox &region)
{
  if (isSelected())
    window_->drawSelected(region.x(), region.y(), region.width(), region.height());
}

void
CBrowserObject::
drawBorder(const CTextBox &region)
{
  int x1 = region.xmin();
  int y1 = region.ymin();
  int x2 = region.xmax();
  int y2 = region.ymax();

  if (border().isTopVisible()) {
    drawBorderLine(x1, y1, x2, y1, border().topStyle(), border().topPen(),
                   CBrowserBorderSide::TOP);
  }

  if (border().isLeftVisible()) {
    drawBorderLine(x1, y1, x1, y2, border().leftStyle(), border().leftPen(),
                   CBrowserBorderSide::LEFT);
  }

  if (border().isRightVisible()) {
    drawBorderLine(x2, y1, x2, y2, border().rightStyle(), border().rightPen(),
                   CBrowserBorderSide::RIGHT);
  }

  if (border().isBottomVisible()) {
    drawBorderLine(x1, y2, x2, y2, border().bottomStyle(), border().bottomPen(),
                   CBrowserBorderSide::BOTTOM);
  }
}

void
CBrowserObject::
drawBorderLine(double x1, double y1, double x2, double y2,
               CBrowserBorderStyle style, const CPen &pen, CBrowserBorderSide side)
{
  double w = pen.getWidth();

  if      (style == CBrowserBorderStyle::DOUBLE) {
    CPen pen1 = pen;

    pen1.setWidth(1);

    double dw = w - 1;

    if      (side == CBrowserBorderSide::TOP || side == CBrowserBorderSide::BOTTOM) {
      if (side == CBrowserBorderSide::TOP) {
        double y2 = y1 + dw;

        window_->drawLine(x1     , y1, x2     , y1, pen1);
        window_->drawLine(x1 + dw, y2, x2 - dw, y2, pen1);
      }
      else {
        double y2 = y1 - dw;

        window_->drawLine(x1     , y1, x2     , y1, pen1);
        window_->drawLine(x1 + dw, y2, x2 - dw, y2, pen1);
      }
    }
    else {
      if (side == CBrowserBorderSide::LEFT) {
        double x2 = x1 + dw;

        window_->drawLine(x1, y1     , x1, y2     , pen1);
        window_->drawLine(x2, y1 + dw, x2, y2 - dw, pen1);
      }
      else {
        double x2 = x1 - dw;

        window_->drawLine(x1, y1     , x1, y2     , pen1);
        window_->drawLine(x2, y1 + dw, x2, y2 - dw, pen1);
      }
    }
  }
  else if (style == CBrowserBorderStyle::SOLID ||
           style == CBrowserBorderStyle::INSET || style == CBrowserBorderStyle::OUTSET) {
    CRGBA bg = window_->getBgColor();

    CRGBA c     = pen.getColor();
    CRGBA light = c.blended(bg, 0.5);
    CRGBA dark  = c.blended(bg, 0.8);

    if      (side == CBrowserBorderSide::TOP) {
      double x11 = x1 + w;
      double x21 = x2 - w;
      double y11 = y1 + w;

      std::vector<CIPoint2D> points = {{
        CIPoint2D(x1, y1), CIPoint2D(x2, y1), CIPoint2D(x21, y11), CIPoint2D(x11, y11)
      }};

      if      (style == CBrowserBorderStyle::SOLID)
        window_->fillPolygon(points, CBrush(c));
      else if (style == CBrowserBorderStyle::INSET)
        window_->fillPolygon(points, CBrush(dark));
      else
        window_->fillPolygon(points, CBrush(light));
    }
    else if (side == CBrowserBorderSide::LEFT) {
      double x11 = x1 + w;
      double y11 = y1 + w;
      double y21 = y2 - w;

      std::vector<CIPoint2D> points = {{
        CIPoint2D(x1, y1), CIPoint2D(x11, y11), CIPoint2D(x11, y21), CIPoint2D(x1, y2)
      }};

      if      (style == CBrowserBorderStyle::SOLID)
        window_->fillPolygon(points, CBrush(c));
      else if (style == CBrowserBorderStyle::INSET)
        window_->fillPolygon(points, CBrush(dark));
      else
        window_->fillPolygon(points, CBrush(light));
    }
    else if (side == CBrowserBorderSide::RIGHT) {
      double x11 = x1 - w;
      double y11 = y1 + w;
      double y21 = y2 - w;

      if (y21 < y11) {
        y11 = (y1 + y2)/2;
        y21 = y11;
      }

      std::vector<CIPoint2D> points = {{
        CIPoint2D(x11, y11), CIPoint2D(x1, y1), CIPoint2D(x1, y2), CIPoint2D(x11, y21)
      }};

      if      (style == CBrowserBorderStyle::SOLID)
        window_->fillPolygon(points, CBrush(c));
      else if (style == CBrowserBorderStyle::INSET)
        window_->fillPolygon(points, CBrush(light));
      else
        window_->fillPolygon(points, CBrush(dark));
    }
    else if (side == CBrowserBorderSide::BOTTOM) {
      double x11 = x1 + w;
      double x21 = x2 - w;
      double y11 = y1 - w;

      std::vector<CIPoint2D> points = {{
        CIPoint2D(x11, y11), CIPoint2D(x21, y11), CIPoint2D(x2, y1), CIPoint2D(x1, y1)
      }};

      if      (style == CBrowserBorderStyle::SOLID)
        window_->fillPolygon(points, CBrush(c));
      else if (style == CBrowserBorderStyle::INSET)
        window_->fillPolygon(points, CBrush(light));
      else
        window_->fillPolygon(points, CBrush(dark));
    }
  }
  else if (style == CBrowserBorderStyle::OUTSET) {
    CRGBA bg = window_->getBgColor();

    CRGBA c;

    if (side == CBrowserBorderSide::TOP || side == CBrowserBorderSide::LEFT)
      c = pen.getColor().blended(bg, 0.5);
    else
      c = pen.getColor().blended(bg, 0.8);

    CPen pen1 = pen;

    pen1.setColor(c);

    window_->drawLine(x1, y1, x2, y2, pen1);
  }
  else if (style == CBrowserBorderStyle::GROOVE || style == CBrowserBorderStyle::RIDGE) {
    double w1 = w/2;

    CRGBA bg = window_->getBgColor();

    CRGBA light = pen.getColor().blended(bg, 0.5);
    CRGBA dark  = pen.getColor().blended(bg, 0.8);

    if      (side == CBrowserBorderSide::TOP) {
      double x11 = x1 + w1;
      double x21 = x2 - w1;
      double y11 = y1 + w1;

      std::vector<CIPoint2D> points1 = {{
        CIPoint2D(x1, y1), CIPoint2D(x2, y1), CIPoint2D(x21, y11), CIPoint2D(x11, y11)
      }};

      if (style == CBrowserBorderStyle::GROOVE)
        window_->fillPolygon(points1, CBrush(dark));
      else
        window_->fillPolygon(points1, CBrush(light));

      double x12 = x11 + w1;
      double x22 = x21 - w1;
      double y22 = y11 + w1;

      std::vector<CIPoint2D> points2 = {{
        CIPoint2D(x11, y11), CIPoint2D(x21, y11), CIPoint2D(x22, y22), CIPoint2D(x12, y22)
      }};

      if (style == CBrowserBorderStyle::GROOVE)
        window_->fillPolygon(points2, CBrush(light));
      else
        window_->fillPolygon(points2, CBrush(dark));
    }
    else if (side == CBrowserBorderSide::LEFT) {
      double x11 = x1 + w1;
      double y11 = y1 + w1;
      double y21 = y2 - w1;

      std::vector<CIPoint2D> points1 = {{
        CIPoint2D(x1, y1), CIPoint2D(x11, y11), CIPoint2D(x11, y21), CIPoint2D(x1, y2)
      }};

      if (style == CBrowserBorderStyle::GROOVE)
        window_->fillPolygon(points1, CBrush(dark));
      else
        window_->fillPolygon(points1, CBrush(light));

      double x12 = x11 + w1;
      double y12 = y11 + w1;
      double y22 = y21 - w1;

      std::vector<CIPoint2D> points2 = {{
        CIPoint2D(x11, y11), CIPoint2D(x12, y12), CIPoint2D(x12, y22), CIPoint2D(x11, y21)
      }};

      if (style == CBrowserBorderStyle::GROOVE)
        window_->fillPolygon(points2, CBrush(light));
      else
        window_->fillPolygon(points2, CBrush(dark));
    }
    else if (side == CBrowserBorderSide::RIGHT) {
      double x11 = x1 - w1;
      double y11 = y1 + w1;
      double y21 = y2 - w1;

      std::vector<CIPoint2D> points1 = {{
        CIPoint2D(x11, y11), CIPoint2D(x1, y1), CIPoint2D(x1, y2), CIPoint2D(x11, y21)
      }};

      if (style == CBrowserBorderStyle::GROOVE)
        window_->fillPolygon(points1, CBrush(light));
      else
        window_->fillPolygon(points1, CBrush(dark));

      double x12 = x11 - w1;
      double y12 = y11 + w1;
      double y22 = y21 - w1;

      std::vector<CIPoint2D> points2 = {{
        CIPoint2D(x12, y12), CIPoint2D(x11, y11), CIPoint2D(x11, y21), CIPoint2D(x12, y22)
      }};

      if (style == CBrowserBorderStyle::GROOVE)
        window_->fillPolygon(points2, CBrush(dark));
      else
        window_->fillPolygon(points2, CBrush(light));
    }
    else if (side == CBrowserBorderSide::BOTTOM) {
      double x11 = x1 + w1;
      double x21 = x2 - w1;
      double y11 = y1 - w1;

      std::vector<CIPoint2D> points1 = {{
        CIPoint2D(x11, y11), CIPoint2D(x21, y11), CIPoint2D(x2, y1), CIPoint2D(x1, y1)
      }};

      if (style == CBrowserBorderStyle::GROOVE)
        window_->fillPolygon(points1, CBrush(light));
      else
        window_->fillPolygon(points1, CBrush(dark));

      double x12 = x11 + w1;
      double x22 = x21 - w1;
      double y22 = y11 - w1;

      std::vector<CIPoint2D> points2 = {{
        CIPoint2D(x12, y22), CIPoint2D(x22, y22), CIPoint2D(x21, y11), CIPoint2D(x11, y11)
      }};

      if (style == CBrowserBorderStyle::GROOVE)
        window_->fillPolygon(points2, CBrush(dark));
      else
        window_->fillPolygon(points2, CBrush(light));
    }
  }
  else {
    window_->drawLine(x1, y1, x2, y2, pen);
  }
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
