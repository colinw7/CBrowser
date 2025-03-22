#include <CBrowserImage.h>
#include <CBrowserNamedImage.h>
#include <CBrowserMain.h>
#include <CBrowserText.h>
#include <CBrowserLink.h>
#include <CBrowserWindow.h>
#include <CBrowserDocument.h>
#include <CBrowserProperty.h>
#include <CBrowserGraphics.h>

#include <CQJImageObj.h>

#include <cstring>

CBrowserImage::
CBrowserImage(CBrowserWindowIFace *window) :
 CBrowserObject(window, CHtmlTagId::IMG), iface_(this)
{
  setDisplay(CBrowserObject::Display::INLINE);

  link_ = window->getCurrentLink();
}

CBrowserImage::
~CBrowserImage()
{
}

void
CBrowserImage::
init()
{
  if (data_.src == "") {
    window_->displayError("No Image Source Specified\n");
    return;
  }

  //---

  CImagePtr image;

  CBrowserImageData data = data_;

  CUrl url = window_->getDocument()->getUrl();

  if      (url.isHttp()) {
    CUrl url1 = url;

    CUrl url2(data.src);

    if (url2.isHttp())
      url1 = url2;
    else
      url1.setFile(data.src);

    std::string filename;

    if (! window_->downloadFile(url1, filename)) {
      std::cerr << "Failed to download '" << url1 << "'\n";
      return;
    }

    data.src = filename;

    int iwidth  = CBrowserObject::width ().pxValue();
    int iheight = CBrowserObject::height().pxValue();

    image = window_->lookupImage(data, iwidth, iheight);
  }
  else if (data_.src.substr(0, 6) == "_html_") {
    std::string name = data_.src.substr(6);

    image = CBrowserNamedImage::lookup(name);
  }
  else {
    int iwidth  = CBrowserObject::width ().pxValue();
    int iheight = CBrowserObject::height().pxValue();

    image = window_->lookupImage(data_, iwidth, iheight);
  }

  if (! image)
    image = CBrowserNamedImage::genNoImage();

  setImage(image);

  //---

  std::vector<std::string> strs = {{
    "src", "align", "border", "width", "height", "usemap", "hspace", "vspace", "alt" }};

  addProperties(strs);

  CBrowserObject::init();
}

void
CBrowserImage::
setNameValue(const std::string &name, const std::string &value)
{
  std::string lname  = CStrUtil::toLower(name);
  std::string lvalue = CStrUtil::toLower(value);

  if      (lname == "align") {
    // left/right is float
    // middle, top, bottom is vertical-align
    if      (lvalue == "left" )
      float_ = CBrowserFloat(CBrowserFloat::Type::LEFT );
    else if (lvalue == "right")
      float_ = CBrowserFloat(CBrowserFloat::Type::RIGHT);
    else if (lvalue == "top")
      textProp_.setVerticalAlign(CBrowserTextVAlign(CBrowserTextVAlign::TOP));
    else if (lvalue == "middle")
      textProp_.setVerticalAlign(CBrowserTextVAlign(CBrowserTextVAlign::MIDDLE));
    else if (lvalue == "bottom")
      textProp_.setVerticalAlign(CBrowserTextVAlign(CBrowserTextVAlign::BOTTOM));
    else if (lvalue == "texttop")
      textProp_.setVerticalAlign(CBrowserTextVAlign(CBrowserTextVAlign::TEXT_TOP));
    else if (lvalue == "absmiddle")
      textProp_.setVerticalAlign(CBrowserTextVAlign(CBrowserTextVAlign::ABS_MIDDLE));
    else if (lvalue == "absbottom")
      textProp_.setVerticalAlign(CBrowserTextVAlign(CBrowserTextVAlign::ABS_BOTTOM));
    else
      window_->displayError("Illegal 'img' Align '%s'\n", value.c_str());
  }
  else if (lname == "alt") {
    data_.alt = value;
  }
  else if (lname == "border") {
    if (CStrUtil::isInteger(value))
      data_.border = CStrUtil::toInteger(value);
    else {
      window_->displayError("Illegal 'img' Value for Border '%s'\n", value.c_str());
      data_.border = 2;
    }
  }
  else if (lname == "controls") {
  }
  else if (lname == "dynsrc") {
  }
  else if (lname == "height") {
    CBrowserObject::setHeight(CBrowserUnitValue(value));
  }
  else if (lname == "hspace") {
    if (CStrUtil::isInteger(value))
      data_.hspace = CStrUtil::toInteger(value);
    else {
      window_->displayError("Illegal 'img' Value for HSpace '%s'\n", value.c_str());
      data_.hspace = 2;
    }
  }
  else if (lname == "ismap") {
    //ismap = true;

    if (value != "")
      window_->displayError("No Value needed for IsMap\n");
  }
  else if (lname == "loop") {
  }
  else if (lname == "lowsrc") {
  }
  else if (lname == "src") {
    data_.src = value;
  }
  else if (lname == "start") {
  }
  else if (lname == "usemap") {
    data_.usemap = value;
  }
  else if (lname == "vspace") {
    if (CStrUtil::isInteger(value))
      data_.vspace = CStrUtil::toInteger(value);
    else {
      window_->displayError("Illegal 'img' Value for VSpace '%s'\n", value.c_str());
      data_.vspace = 2;
    }
  }
  else if (lname == "width") {
    CBrowserObject::setWidth(CBrowserUnitValue(value));
  }
  else {
    CBrowserObject::setNameValue(name, value);
  }
}

void
CBrowserImage::
getInlineWords(Words &words) const
{
  auto *th = const_cast<CBrowserImage *>(this);

  CBrowserWord word(th, image_, isHierSelected());

  if      (float_.type() == CBrowserFloat::Type::LEFT)
    word.setFloat(CBrowserWord::Float::LEFT);
  else if (float_.type() == CBrowserFloat::Type::RIGHT)
    word.setFloat(CBrowserWord::Float::RIGHT);

  words.push_back(word);
}

std::string
CBrowserImage::
propertyValue(int i) const
{
  const std::string &name = propertyName(i);

  if      (name == "src"   ) return CBrowserProperty::toString(data_.src);
  else if (name == "align" ) {
    if (float_.type() != CBrowserFloat::Type::INVALID)
      return CBrowserProperty::toString(float_);

    //return CBrowserProperty::toString(textProp_);
    return "";
  }
  else if (name == "border") return CBrowserProperty::toString(data_.border);
  else if (name == "width" ) return CBrowserProperty::toString(CBrowserObject::width());
  else if (name == "height") return CBrowserProperty::toString(CBrowserObject::height());
  else if (name == "usemap") return CBrowserProperty::toString(data_.usemap);
  else if (name == "hspace") return CBrowserProperty::toString(data_.hspace);
  else if (name == "vspace") return CBrowserProperty::toString(data_.vspace);
  else if (name == "alt"   ) return CBrowserProperty::toString(data_.alt);

  return CBrowserObject::propertyValue(i);
}

CBrowserRegion
CBrowserImage::
calcRegion() const
{
  int hspace = std::max(data_.hspace, data_.border);
  int vspace = std::max(data_.vspace, data_.border);

  //---

  int width  = image_->getWidth () + 2*hspace;
  int height = image_->getHeight() + 2*vspace;

  CBrowserTextVAlign::Type valign = textProp_.verticalAlign().type();

  if      (valign == CBrowserTextVAlign::Type::TOP ||
           valign == CBrowserTextVAlign::Type::TEXT_TOP)
    return CBrowserRegion(width, vspace, height - vspace);
  else if (valign == CBrowserTextVAlign::Type::MIDDLE ||
           valign == CBrowserTextVAlign::Type::ABS_MIDDLE)
    return CBrowserRegion(width, (height + 1)/2, height/2);
  else
    return CBrowserRegion(width, height - vspace, vspace);
}

void
CBrowserImage::
draw(const CTextBox &region)
{
  int hspace = std::max(data_.hspace, data_.border);
  int vspace = std::max(data_.vspace, data_.border);

  /*-------------*/

  int x1 = region.x() + hspace;
  int y1 = region.y() + vspace;

  CBrowserTextVAlign::Type valign = textProp_.verticalAlign().type();

  if      (valign == CBrowserTextVAlign::Type::TOP)
    y1 += region.ascent();
  else if (valign == CBrowserTextVAlign::Type::MIDDLE)
    y1 += region.ascent() - image_->getHeight()/2;
  else if (valign == CBrowserTextVAlign::Type::ABS_MIDDLE)
    y1 += (region.height() - image_->getHeight())/2;
  else if (valign == CBrowserTextVAlign::Type::ABS_BOTTOM)
    y1 += region.descent();
  else
    y1 += 0;

  auto *graphics = window_->graphics();

  graphics->drawImage(x1, y1, image_);

  if (link_) {
    if (link_->isSource()) {
      CRGBA color = window_->getDocument()->getLinkColor();

      CPen pen(color);

      for (int i = 0; i < data_.border; i++)
        graphics->drawRectangle(x1 - i - 1, y1 - i - 1,
                                image_->getWidth () + 2*i + 1,
                                image_->getHeight() + 2*i + 1,
                                pen);
    }

    int x2 = x1 + image_->getWidth();
    int y2 = y1 + image_->getHeight();

    link_->addRect(x1, y1, x2, y2);
  }

  //region.setX(region.x() + image_->getWidth() + 2*hspace);
}

CQJHtmlObj *
CBrowserImage::
createJObj(CJavaScript *js)
{
  return new CQJImageObj(js, iface(), CBrowserObject::iface());
}
