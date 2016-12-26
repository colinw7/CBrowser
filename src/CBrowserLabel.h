#ifndef CBrowserLabel_H
#define CBrowserLabel_H

#include <CBrowserObject.h>
#include <CFont.h>
#include <CRGBA.h>

class CBrowserLabel : public CBrowserObject {
 public:
  CBrowserLabel(CBrowserWindow *window, const std::string &text, int width,
                CHAlignType align, const CRGBA &color);
 ~CBrowserLabel();

  void initLayout() override;
  void termLayout() override;

  void format(CHtmlLayoutMgr *) override;

  void draw(CHtmlLayoutMgr *, const CHtmlLayoutRegion &) override;

  void print(std::ostream &os) const override { os << "label '" << text_ << "'"; }

 private:
  std::string text_;
  int         width_ { 0 };
  CHAlignType align_ { CHALIGN_TYPE_NONE };
  CFontPtr    font_;
  CRGBA       color_ { 0, 0, 0 };
};

#endif
