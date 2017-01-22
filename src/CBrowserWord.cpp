#include <CBrowserWord.h>
#include <CBrowserText.h>
#include <CBrowserBreak.h>
#include <CBrowserImage.h>
#include <CBrowserForm.h>

CBrowserWord::
CBrowserWord(CBrowserText *text, const std::string &str, const CPen &pen,
             const CFontPtr &font, bool breakup, bool selected) :
 type_(Type::TEXT), text_(str), obj_(text), pen_(pen), font_(font), breakup_(breakup),
 selected_(selected)
{
}

CBrowserWord::
CBrowserWord(CBrowserBreak *br, bool selected) :
 type_(Type::BREAK), obj_(br), breakup_(true),
 selected_(selected)
{
}

CBrowserWord::
CBrowserWord(CBrowserImage *img, const CImagePtr &image, bool selected) :
 type_(Type::IMAGE), image_(image), obj_(img), breakup_(false), selected_(selected)
{
}

CBrowserWord::
CBrowserWord(CBrowserFormInput *input, bool selected) :
 type_(Type::INPUT), obj_(input), breakup_(false), selected_(selected)
{
}

CBrowserText *
CBrowserWord::
textObj() const
{
  assert(type_ == Type::TEXT);

  return dynamic_cast<CBrowserText *>(obj_);
}

CBrowserBreak *
CBrowserWord::
breakObj() const
{
  assert(type_ == Type::BREAK);

  return dynamic_cast<CBrowserBreak *>(obj_);
}

CBrowserImage *
CBrowserWord::
imageObj() const
{
  assert(type_ == Type::IMAGE);

  return dynamic_cast<CBrowserImage *>(obj_);
}

CBrowserFormInput *
CBrowserWord::
inputObj() const
{
  assert(type_ == Type::INPUT);

  return dynamic_cast<CBrowserFormInput *>(obj_);
}

int
CBrowserWord::
width() const
{
  if      (type_ == Type::TEXT)
    return font_->getStringWidth(text_);
  else if (type_ == Type::BREAK)
    return 0;
  else if (type_ == Type::IMAGE)
    return image_->getWidth();
  else if (type_ == Type::INPUT)
    return inputObj()->calcRegion().width();
  else
    return 0;
}

int
CBrowserWord::
ascent() const
{
  if      (type_ == Type::TEXT)
    return font_->getCharAscent();
  else if (type_ == Type::BREAK)
    return 0;
  else if (type_ == Type::IMAGE)
    return image_->getHeight();
  else if (type_ == Type::INPUT)
    return inputObj()->calcRegion().height();
  else
    return 0;
}

int
CBrowserWord::
descent() const
{
  if      (type_ == Type::TEXT)
    return font_->getCharDescent();
  else if (type_ == Type::BREAK)
    return 0;
  else if (type_ == Type::IMAGE)
    return 0;
  else if (type_ == Type::INPUT)
    return 0;
  else
    return 0;
}
