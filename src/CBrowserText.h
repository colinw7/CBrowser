#ifndef CBrowserText_H
#define CBrowserText_H

#include <CBrowserObject.h>
#include <CBrowserData.h>
#include <CFont.h>

//------

enum CBrowserTextPos {
  RIGHT,
  BELOW
};

class CBrowserText : public CBrowserObject {
 public:
  CBrowserText(CBrowserWindow *window, const std::string &text);

  CBrowserText(CBrowserWindow *window, const CBrowserText &draw_text, const std::string &text);

 ~CBrowserText();

  std::string typeName() const override { return "text"; }

  const std::string &text() const { return text_; }

  const CBrowserTextPos &pos() const { return pos_; }
  void setPos(const CBrowserTextPos &pos) { pos_ = pos; }

  bool isHierSelected() const override;

  CBrowserRegion calcRegion() const override;

  void getInlineWords(Words &words) const override;

  void draw(const CTextBox &) override;

  void print(std::ostream &os) const override {
    os << "text '" << CStrUtil::stripSpaces(text_) << "'";
  }

  static void getTextBounds(CFontPtr, const std::string &, int *, int *, int *);

  CFontPtr hierFont() const override;

  CRGBA hierFgColor() const override;

  CBrowserObject::WhiteSpace hierWhiteSpace() const override;

 private:
  typedef std::vector<CBrowserText *> Texts;

  std::string         text_;
  CBrowserAnchorLink* link_ { nullptr };
  CBrowserTextPos     pos_ { CBrowserTextPos::RIGHT };
  Texts               texts_;
};

#endif
