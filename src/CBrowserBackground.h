#ifndef CBrowserBackground_H
#define CBrowserBackground_H

#include <CBrowserColor.h>
#include <CImageMgr.h>
#include <CStrUtil.h>

class CBrowserBackgroundImage {
 public:
  enum class Type {
    INVALID,
    URL,
    NONE,
    INITIAL,
    INHERIT
  };

 public:
  CBrowserBackgroundImage() { }

  explicit CBrowserBackgroundImage(const std::string &str) :
   str_(str) {
    auto lstr = CStrUtil::toLower(str);

    if      (lstr == "none") {
      type_ = Type::NONE;
    }
    else if (lstr == "initial") {
      type_ = Type::INITIAL;
    }
    else if (lstr == "inherit") {
      type_ = Type::INHERIT;
    }
    else if (lstr.size() >= 5 && lstr.substr(0, 4) == "url(" && lstr[lstr.size() - 1] == ')') {
      type_  = Type::URL;
      value_ = str.substr(4, str.size() - 5);

      if (value_.size() >= 2) {
        if      (value_[0] == '"' && value_[value_.size() - 1] == '"')
          value_ = value_.substr(1, value_.size() - 2);
        else if (value_[0] == '\'' && value_[value_.size() - 1] == '\'')
          value_ = value_.substr(1, value_.size() - 2);
      }
    }
    else {
      type_  = Type::URL;
      value_ = str;
    }
  }

  bool isValid() const { return type_ != Type::INVALID; }

  const std::string &string() const { return str_; }

  const Type &type() const { return type_; }

  const std::string &value() const { return value_; }

  CImagePtr image() const {
    if (type_ == Type::URL) {
      CImageFileSrc file(value_);

      return CImageMgrInst->createImage(file);
    }
    else
      return CImagePtr();
  }

 private:
  std::string str_;
  Type        type_ { Type::INVALID };
  std::string value_;
};

//---

class CBrowserBackgroundPosition {
 public:
  enum class Type {
    INVALID,
    NONE,
    INITIAL,
    INHERIT,
    LEFT_TOP,
    LEFT_CENTER,
    LEFT_BOTTOM,
    RIGHT_TOP,
    RIGHT_CENTER,
    RIGHT_BOTTOM,
    CENTER_TOP,
    CENTER_CENTER,
    CENTER_BOTTOM,
    X_Y_POS
  };

 public:
  CBrowserBackgroundPosition() { }

  explicit CBrowserBackgroundPosition(const std::string &str) :
   str_(str) {
    std::string lstr = CStrUtil::toLower(str);

    std::vector<std::string> words;

    CStrUtil::toWords(lstr, words);

    if      (words.size() == 2) {
      if      (words[0] == "left") {
        if      (words[1] == "top")
          type_ = Type::LEFT_TOP;
        else if (words[1] == "center")
          type_ = Type::LEFT_CENTER;
        else if (words[1] == "bottom")
          type_ = Type::LEFT_BOTTOM;
      }
      else if (words[0] == "right") {
        if      (words[1] == "top")
          type_ = Type::RIGHT_TOP;
        else if (words[1] == "center")
          type_ = Type::RIGHT_CENTER;
        else if (words[1] == "bottom")
          type_ = Type::RIGHT_BOTTOM;
      }
      else if (words[0] == "center") {
        if      (words[1] == "top")
          type_ = Type::CENTER_TOP;
        else if (words[1] == "center")
          type_ = Type::CENTER_CENTER;
        else if (words[1] == "bottom")
          type_ = Type::CENTER_BOTTOM;
      }
      else {
        type_ = Type::X_Y_POS;
        xpos_ = CBrowserUnitValue(words[0]);
        ypos_ = CBrowserUnitValue(words[1]);
      }
    }
    else if (lstr == "initial") {
      type_ = Type::INITIAL;
    }
    else if (lstr == "inherit") {
      type_ = Type::INHERIT;
    }
    else {
      type_ = Type::INVALID;
    }
  }

 private:
  std::string       str_;
  Type              type_ { Type::NONE };
  CBrowserUnitValue xpos_;
  CBrowserUnitValue ypos_;
};

//---

class CBrowserBackgroundSize {
 public:
  enum class Type {
    INVALID,
    NONE,
    INITIAL,
    INHERIT,
    AUTO,
    COVER,
    CONTAIN,
    VALUE
  };

 public:
  CBrowserBackgroundSize() { }

  explicit CBrowserBackgroundSize(const std::string &str) :
   str_(str) {
    std::string lstr = CStrUtil::toLower(str);

    std::vector<std::string> words;

    CStrUtil::toWords(lstr, words);

    if      (words.size() == 2) {
      width_  = CBrowserUnitValue(words[0]);
      height_ = CBrowserUnitValue(words[1]);
    }
    else if (lstr == "auto")
      type_ = Type::AUTO;
    else if (lstr == "cover")
      type_ = Type::COVER;
    else if (lstr == "contain")
      type_ = Type::CONTAIN;
    else if (lstr == "initial")
      type_ = Type::INITIAL;
    else if (lstr == "inherit")
      type_ = Type::INHERIT;
    else {
      type_   = Type::VALUE;
      width_  = CBrowserUnitValue(lstr);
      height_ = CBrowserUnitValue();
    }
  }

 private:
  std::string       str_;
  Type              type_ { Type::NONE };
  CBrowserUnitValue width_;
  CBrowserUnitValue height_;
};

//---

class CBrowserBackgroundRepeat {
 public:
  enum class Type {
    INVALID,
    NONE,
    INITIAL,
    INHERIT,
    REPEAT,
    REPEAT_X,
    REPEAT_Y,
    NO_REPEAT
  };

 public:
  CBrowserBackgroundRepeat() { }

  explicit CBrowserBackgroundRepeat(const std::string &str) :
   str_(str) {
    std::string lstr = CStrUtil::toLower(str);

    if      (lstr == "repeat")
      type_ = Type::REPEAT;
    else if (lstr == "repeat-x")
      type_ = Type::REPEAT_X;
    else if (lstr == "repeat-y")
      type_ = Type::REPEAT_Y;
    else if (lstr == "no-repeat")
      type_ = Type::NO_REPEAT;
    else if (lstr == "initial")
      type_ = Type::INITIAL;
    else if (lstr == "inherit")
      type_ = Type::INHERIT;
    else
      type_ = Type::INVALID;
  }

 private:
  std::string str_;
  Type        type_ { Type::NONE };
};

//---

class CBrowserBackgroundOrigin {
 public:
  enum class Type {
    INVALID,
    NONE,
    INITIAL,
    INHERIT,
    PADDING_BOX,
    BORDER_BOX,
    CONTENT_BOX
  };

 public:
  CBrowserBackgroundOrigin() { }

  explicit CBrowserBackgroundOrigin(const std::string &str) :
   str_(str) {
    std::string lstr = CStrUtil::toLower(str);

    if      (lstr == "padding-box")
      type_ = Type::PADDING_BOX;
    else if (lstr == "border-box")
      type_ = Type::BORDER_BOX;
    else if (lstr == "content-box")
      type_ = Type::CONTENT_BOX;
    else if (lstr == "initial")
      type_ = Type::INITIAL;
    else if (lstr == "inherit")
      type_ = Type::INHERIT;
    else
      type_ = Type::INVALID;
  }

 private:
  std::string str_;
  Type        type_ { Type::NONE };
};

//---

class CBrowserBackgroundClip {
 public:
  enum class Type {
    INVALID,
    NONE,
    INITIAL,
    INHERIT,
    BORDER_BOX,
    PADDING_BOX,
    CONTENT_BOX
  };

 public:
  CBrowserBackgroundClip() { }

  explicit CBrowserBackgroundClip(const std::string &str) :
   str_(str) {
    std::string lstr = CStrUtil::toLower(str);

    if      (lstr == "border-box")
      type_ = Type::BORDER_BOX;
    else if (lstr == "padding-box")
      type_ = Type::PADDING_BOX;
    else if (lstr == "content-box")
      type_ = Type::CONTENT_BOX;
    else if (lstr == "initial")
      type_ = Type::INITIAL;
    else if (lstr == "inherit")
      type_ = Type::INHERIT;
    else
      type_ = Type::INVALID;
  }

 private:
  std::string str_;
  Type        type_ { Type::NONE };
};

//---

class CBrowserBackgroundAttachment {
 public:
  enum class Type {
    INVALID,
    NONE,
    INITIAL,
    INHERIT,
    SCROLL,
    FIXED,
    LOCAL
  };

 public:
  CBrowserBackgroundAttachment() { }

  explicit CBrowserBackgroundAttachment(const std::string &str) :
   str_(str) {
    std::string lstr = CStrUtil::toLower(str);

    if      (lstr == "scroll")
      type_ = Type::SCROLL;
    else if (lstr == "fixed")
      type_ = Type::FIXED;
    else if (lstr == "local")
      type_ = Type::LOCAL;
    else if (lstr == "initial")
      type_ = Type::INITIAL;
    else if (lstr == "inherit")
      type_ = Type::INHERIT;
    else
      type_ = Type::INVALID;
  }

 private:
  std::string str_;
  Type        type_ { Type::NONE };
};

//---

class CBrowserBackground {
 public:
  CBrowserBackground() { }

  explicit CBrowserBackground(const std::string &str) :
   str_(str) {
  }

  std::string string() const {
    std::string str;

    if (! color_.string().empty())
      str += "color: " + color_.string() + "; ";

    return str;
  }

  void setColor(const CBrowserColor &c) { color_ = c; }
  const CBrowserColor &color() const { return color_; }

  const CBrowserBackgroundImage &image() const { return image_; }
  void setImage(const CBrowserBackgroundImage &v) { image_ = v; }

  const CBrowserBackgroundPosition &position() const { return position_; }
  void setPosition(const CBrowserBackgroundPosition &p) { position_ = p; }

  const CBrowserBackgroundSize &size() const { return size_; }
  void setSize(const CBrowserBackgroundSize &s) { size_ = s; }

  const CBrowserBackgroundRepeat &repeat() const { return repeat_; }
  void setRepeat(const CBrowserBackgroundRepeat &r) { repeat_ = r; }

  const CBrowserBackgroundOrigin &origin() const { return origin_; }
  void setOrigin(const CBrowserBackgroundOrigin &o) { origin_ = o; }

  const CBrowserBackgroundClip &clip() const { return clip_; }
  void setClip(const CBrowserBackgroundClip &c) { clip_ = c; }

  const CBrowserBackgroundAttachment &attachment() const { return attachment_; }
  void setAttachment(const CBrowserBackgroundAttachment &a) { attachment_ = a; }

 private:
  std::string                  str_;
  CBrowserColor                color_;
  CBrowserBackgroundImage      image_;
  CBrowserBackgroundPosition   position_;
  CBrowserBackgroundSize       size_;
  CBrowserBackgroundRepeat     repeat_;
  CBrowserBackgroundOrigin     origin_;
  CBrowserBackgroundClip       clip_;
  CBrowserBackgroundAttachment attachment_;
};

//---

#include <CBrowserProperty.h>

template<>
class CBrowserPropertyTraits<CBrowserBackground> {
 public:
  std::string toString(const CBrowserBackground &bg) {
    return bg.string();
  }

  bool fromString(const std::string &str, CBrowserBackground &bg) {
    bg = CBrowserBackground(str);

    return true;
  }
};

#endif
