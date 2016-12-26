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
  CBrowserText(CBrowserWindow *window, const std::string &text, const CBrowserTextData &data);

  CBrowserText(CBrowserWindow *window, const CBrowserText &draw_text, const std::string &text);

 ~CBrowserText();

  const std::string &text() const { return text_; }

  const CFontPtr &font() const { return font_; }

  const CBrowserTextPos &pos() const { return pos_; }
  void setPos(const CBrowserTextPos &pos) { pos_ = pos; }

  bool isHierSelected() const override;

  void initLayout() override;
  void termLayout() override;

  void format(CHtmlLayoutMgr *) override;

  void draw(CHtmlLayoutMgr *, const CHtmlLayoutRegion &) override;

  void print(std::ostream &os) const override {
    os << "text '" << CStrUtil::stripSpaces(text_) << "'";
  }

  static void getTextBounds(CFontPtr, const std::string &, int *, int *, int *);

 private:
  void addText(const std::string &str, CBrowserTextPos pos, bool breakup);

 private:
  typedef std::vector<CBrowserText *> Texts;

  std::string      text_;
  CFontPtr         font_;
  CBrowserTextData data_;
  CBrowserLink*    link_ { nullptr };
  CBrowserTextPos  pos_ { CBrowserTextPos::RIGHT };
  Texts            texts_;
  CBrowserText*    parentText_ { nullptr };
};

#endif
