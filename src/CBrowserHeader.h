#ifndef CBrowserHeader_H
#define CBrowserHeader_H

#include <CBrowserObject.h>
#include <CBrowserData.h>
#include <CFont.h>
#include <CStrUtil.h>

class CBrowserHeader : public CBrowserObject {
 public:
  CBrowserHeader(CBrowserWindow *window, CHtmlTagId id, const CBrowserHeaderData &data);
 ~CBrowserHeader();

  const std::string &text() const { return text_; }
  void setText(const std::string &v) { text_ = v; }

  void initProcess() override;
  void termProcess() override;

  void initLayout() override;
  void termLayout() override;

  void format(CHtmlLayoutMgr *) override;

  void draw(CHtmlLayoutMgr *, const CHtmlLayoutRegion &) override;

  void print(std::ostream &os) const override {
    os << "header '" << CStrUtil::stripSpaces(text_) << "'";
  }

 private:
  CBrowserHeaderData data_;
  std::string        text_;
  int                ind_ { -1 };
  CFontPtr           font_;
};

#endif
