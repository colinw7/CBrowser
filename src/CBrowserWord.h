#ifndef CBrowserWord_H
#define CBrowserWord_H

#include <CImagePtr.h>
#include <CFont.h>
#include <CPen.h>

class CBrowserFormInput;

class CBrowserWord {
 public:
  enum class Type {
    NONE,
    TEXT,
    IMAGE,
    INPUT
  };

 public:
  CBrowserWord(const std::string &text, const CPen &pen, const CFontPtr &font,
               bool breakup=false, bool selected=false) :
   type_(Type::TEXT), text_(text), pen_(pen), font_(font), breakup_(breakup), selected_(selected) {
  }

  CBrowserWord(const CImagePtr &image, bool breakup=false, bool selected=false) :
   type_(Type::IMAGE), image_(image), breakup_(breakup), selected_(selected) {
  }

  CBrowserWord(CBrowserFormInput *input, bool breakup=false, bool selected=false) :
   type_(Type::INPUT), input_(input), breakup_(breakup), selected_(selected) {
  }

  const Type &type() const { return type_; }

  const std::string &text() const { assert(type_ == Type::TEXT); return text_; }

  const CImagePtr &image() const { assert(type_ == Type::IMAGE); return image_; }

  CBrowserFormInput *input() const { assert(type_ == Type::INPUT); return input_; }

  const CPen &pen() const { assert(type_ == Type::TEXT); return pen_; }

  const CFontPtr &font() const { assert(type_ == Type::TEXT); return font_; }

  bool isBreakup() const { return breakup_; }

  bool isSelected() const { return selected_; }

  int width() const;

  int ascent() const;
  int descent() const;

  int height() const { return ascent() + descent(); }

  bool isSpace() const { return (type_ == Type::TEXT && text_ == " "); }

 private:
  Type               type_ { Type::NONE };
  std::string        text_;
  CImagePtr          image_;
  CBrowserFormInput* input_ { nullptr };
  CPen               pen_;
  CFontPtr           font_;
  bool               breakup_ { false };
  bool               selected_ { false };
};

#endif
