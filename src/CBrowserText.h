#ifndef CBrowserText_H
#define CBrowserText_H

#include <CBrowserObject.h>
#include <CFont.h>
#include <CRGBA.h>

//------

class CBrowserText : public CBrowserObject {
 public:
  CBrowserText(CBrowserWindow *window, const std::string &text, const CRGBA &color,
               bool underline, bool strike, CBrowserTextPlaceType place, bool breakup,
               bool format);

  CBrowserText(CBrowserWindow *window, const CBrowserText &draw_text, const std::string &text);

 ~CBrowserText();

  CFontPtr getFont() const { return font_; }

  void format(CHtmlLayoutMgr *) override;
  void draw(CHtmlLayoutMgr *, const CHtmlLayoutRegion &) override;

  void print(std::ostream &os) const override {
    os << "text '" << CStrUtil::stripSpaces(text_) << "'";
  }

  static void getTextBounds(CFontPtr, const std::string &, int *, int *, int *);

 private:
  CBrowserWindow*       window_ { nullptr };
  std::string           text_;
  CFontPtr              font_;
  CRGBA                 color_ { 0, 0, 0 };
  bool                  underline_ { false };
  bool                  strike_ { false };
  CBrowserTextPlaceType place_ { CBrowserTextPlaceType::NORMAL };
  bool                  breakup_ { false };
  bool                  format_ { false };
  CBrowserLink*         link_ { nullptr };
};

#endif
