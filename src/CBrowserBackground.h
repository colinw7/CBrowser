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
    std::string lstr = CStrUtil::toLower(str);

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

class CBrowserBackground {
 public:
  CBrowserBackground() { }

  explicit CBrowserBackground(const std::string &str) :
   str_(str) {
  }

  void setColor(const CBrowserColor &c) { color_ = c; }
  const CBrowserColor &color() const { return color_; }

  const CBrowserBackgroundImage &image() const { return image_; }
  void setImage(const CBrowserBackgroundImage &v) { image_ = v; }

  const std::string &position() const { return position_; }
  void setPosition(const std::string &v) { position_ = v; }

  const std::string &size() const { return size_; }
  void setSize(const std::string &v) { size_ = v; }

  const std::string &repeat() const { return repeat_; }
  void setRepeat(const std::string &v) { repeat_ = v; }

  const std::string &origin() const { return origin_; }
  void setOrigin(const std::string &v) { origin_ = v; }

  const std::string &clip() const { return clip_; }
  void setClip(const std::string &v) { clip_ = v; }

  const std::string &attachment() const { return attachment_; }
  void setAttachment(const std::string &v) { attachment_ = v; }

 private:
  std::string             str_;
  CBrowserColor           color_;
  CBrowserBackgroundImage image_;
  std::string             position_;
  std::string             size_;
  std::string             repeat_;
  std::string             origin_;
  std::string             clip_;
  std::string             attachment_;
};

#endif
