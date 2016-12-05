#ifndef CBrowserHeader_H
#define CBrowserHeader_H

#include <CBrowserObject.h>
#include <CFont.h>
#include <CStrUtil.h>

class CBrowserHeader : public CBrowserObject {
 public:
  CBrowserHeader(CBrowserWindow *window, CHtmlTagId id, CHAlignType align);
 ~CBrowserHeader();

  const std::string &text() const { return text_; }
  void setText(const std::string &v) { text_ = v; }

  void initFormat();
  void termFormat();

  void format(CHtmlLayoutMgr *) override;

  void draw(CHtmlLayoutMgr *, const CHtmlLayoutRegion &) override;

  void print(std::ostream &os) const override {
    os << "header '" << CStrUtil::stripSpaces(text_) << "'";
  }

 private:
  CBrowserWindow* window_ { nullptr };
  CHAlignType     align_ { CHALIGN_TYPE_LEFT };
  std::string     text_;
  int             ind_ { -1 };
  CFontPtr        font_;
};

#endif
