#ifndef CBrowserWord_H
#define CBrowserWord_H

#include <CImagePtr.h>
#include <CFont.h>
#include <CPen.h>

class CBrowserText;
class CBrowserBreak;
class CBrowserImage;
class CBrowserFormInput;
class CBrowserObject;

class CBrowserWord {
 public:
  enum class Type {
    NONE,
    TEXT,
    BREAK,
    IMAGE,
    INPUT
  };

  enum class Float {
    NONE,
    LEFT,
    RIGHT
  };

 public:
  CBrowserWord(CBrowserText *text, const std::string &str, const CPen &pen,
               const CFontPtr &font, bool breakup=false, bool selected=false);

  CBrowserWord(CBrowserBreak *br, bool selected=false);

  CBrowserWord(CBrowserImage *obj, const CImagePtr &image, bool selected=false);

  CBrowserWord(CBrowserFormInput *input, bool selected=false);

  const Type &type() const { return type_; }

  const std::string &text() const { assert(type_ == Type::TEXT); return text_; }

  const CImagePtr &image() const { assert(type_ == Type::IMAGE); return image_; }

  CBrowserObject *obj() const { return obj_; }

  template<typename T> T *objT() const { return dynamic_cast<T *>(obj_); }

  CBrowserText *textObj() const;

  CBrowserBreak *breakObj() const;

  CBrowserImage *imageObj() const;

  CBrowserFormInput *inputObj() const;

  const CPen &pen() const { assert(type_ == Type::TEXT); return pen_; }

  const CFontPtr &font() const { assert(type_ == Type::TEXT); return font_; }

  void setBreakup(bool b) { breakup_ = b; }
  bool isBreakup() const { return breakup_; }

  void setFloat(Float f) { float_ = f; }
  Float getFloat() const { return float_; }

  bool isSelected() const { return selected_; }

  int width() const;

  int ascent() const;
  int descent() const;

  int height() const { return ascent() + descent(); }

  bool isSpace() const { return (type_ == Type::TEXT && text_ == " "); }

 private:
  Type            type_ { Type::NONE };
  std::string     text_;
  CImagePtr       image_;
  CBrowserObject* obj_ { nullptr };
  CPen            pen_;
  CFontPtr        font_;
  bool            breakup_ { false };
  bool            selected_ { false };
  Float           float_ { Float::NONE };
};

#endif
