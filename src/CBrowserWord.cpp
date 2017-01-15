#include <CBrowserWord.h>
#include <CBrowserForm.h>

int
CBrowserWord::
width() const
{
  if      (type_ == Type::TEXT)
    return font_->getStringWidth(text_);
  else if (type_ == Type::IMAGE)
    return image_->getWidth();
  else if (type_ == Type::INPUT)
    return input_->calcRegion().width();
  else
    return 0;
}

int
CBrowserWord::
ascent() const
{
  if      (type_ == Type::TEXT)
    return font_->getCharAscent();
  else if (type_ == Type::IMAGE)
    return image_->getHeight();
  else if (type_ == Type::INPUT)
    return input_->calcRegion().height();
  else
    return 0;
}

int
CBrowserWord::
descent() const
{
  if      (type_ == Type::TEXT)
    return font_->getCharDescent();
  else if (type_ == Type::IMAGE)
    return 0;
  else if (type_ == Type::INPUT)
    return 0;
  else
    return 0;
}
