#ifndef CBrowserBox_H
#define CBrowserBox_H

#include <CBrowserWord.h>
#include <CBrowserMargin.h>
#include <CBrowserBorder.h>
#include <CBrowserPadding.h>
#include <CBrowserPosition.h>
#include <CBrowserSize.h>
#include <CHtmlTypes.h>
#include <CIBBox2D.h>
#include <CTextBox.h>
#include <vector>

class CBrowserWindowIFace;

class CBrowserBox {
 public:
  using Words = std::vector<CBrowserWord>;

 public:
  explicit CBrowserBox(CBrowserWindowIFace *window);

  virtual ~CBrowserBox() { }

  CBrowserBox *parent() const { return parent_; }
  void setParent(CBrowserBox *p) { parent_ = p; }

  int x() const { return x_; }
  void setX(int x) { x_ = x; }

  int y() const { return y_; }
  void setY(int y) { y_ = y; }

  int ascent() const { return ascent_; }
  void setAscent(int a) { ascent_ = a; }

  int descent() const { return descent_; }
  void setDescent(int d) { descent_ = d; }

  //---

  const CBrowserMargin &margin() const { return margin_; }
  void setMargin(const CBrowserMargin &v) { margin_ = v; }

  CBrowserMargin &marginRef() { return margin_; }

  double marginLeft  () const { return margin().left  ().pxValue(); }
  double marginRight () const { return margin().right ().pxValue(); }
  double marginTop   () const { return margin().top   ().pxValue(); }
  double marginBottom() const { return margin().bottom().pxValue(); }

  //---

  const CBrowserBorder &border() const { return border_; }
  void setBorder(const CBrowserBorder &v) { border_ = v; }

  CBrowserBorder &borderRef() { return border_; }

  double borderLeft  () const { return border().left  ().width.value().pxValue(); }
  double borderRight () const { return border().right ().width.value().pxValue(); }
  double borderTop   () const { return border().top   ().width.value().pxValue(); }
  double borderBottom() const { return border().bottom().width.value().pxValue(); }

  //---

  const CBrowserPadding &padding() const { return padding_; }
  void setPadding(const CBrowserPadding &v) { padding_ = v; }

  CBrowserPadding &paddingRef() { return padding_; }

  double paddingLeft  () const { return padding().left  ().pxValue(); }
  double paddingRight () const { return padding().right ().pxValue(); }
  double paddingTop   () const { return padding().top   ().pxValue(); }
  double paddingBottom() const { return padding().bottom().pxValue(); }

  //---

  const CIBBox2D &content() const { return content_; }
  void setContent(const CIBBox2D &v) { content_ = v; }

  int contentX() const { return paddingLeft() + borderLeft() + marginLeft(); }
  int contentY() const { return paddingTop () + borderTop () + marginTop (); }

  int contentWidth() const { return content_.getWidth(); }
  void setContentWidth(int w) { content_.setWidth(w); }

  int contentHeight() const { return content_.getHeight(); }
  void setContentHeight(int h) { content_.setHeight(h); }

  void setContentSize(int w, int h) { content_.setSize(CISize2D(w, h)); }

  CTextBox contentBox() const { return CTextBox(x(), y(), contentWidth(), contentHeight()); }

  //---

  void addChild(CBrowserBox *box) { children_.push_back(box); }

  //---

  int width() const { return content_.getWidth() + nonContentWidth(); }
  void setWidth(int w) { content_.setWidth(w - nonContentWidth()); }

  int height() const { return content_.getHeight() + nonContentHeight(); }
  void setHeight(int h) { content_.setHeight(h - nonContentHeight()); }

  int nonContentWidth() const {
    return padding().width() + border().width() + marginLeft() + marginRight();
  }
  int nonContentHeight() const {
    return padding().height() + border().height() + marginTop() + marginBottom();
  }

  void setSize(int w, int h) {
    int w1 = w - padding().width () - border().width () - marginLeft() - marginRight ();
    int h1 = h - padding().height() - border().height() - marginTop () - marginBottom();

    content_.setSize(CISize2D(w1, h1));
  }

  CIBBox2D bbox() const { return CIBBox2D(x(), y(), x() + width(), y() + height()); }

  //---

  void hierMove(int dx, int dy);

  //---

  const CHAlignType &halign() const { return halign_; }
  void setHAlign(const CHAlignType &v) { halign_ = v; }

  const CVAlignType &valign() const { return valign_; }
  void setVAlign(const CVAlignType &v) { valign_ = v; }

  //---

  bool isFixedWidth() const { return fixedWidth_; }
  void setFixedWidth(bool b) { fixedWidth_ = b; }

  bool isFixedHeight() const { return fixedHeight_; }
  void setFixedHeight(bool b) { fixedHeight_ = b; }

  //---

  virtual bool isVisible() const = 0;

  virtual bool isInline() const = 0;

  virtual bool isBreak() const = 0;

  //---

  virtual const CBrowserPosition &position() const = 0;

  virtual const CBrowserSize &size() const = 0;

  //---

  virtual CIBBox2D calcBBox() const = 0;

  virtual void calcHeightForWidth(CTextBox &box);

  virtual void heightForWidth(CTextBox &box) const = 0;

  //---

  virtual CHtmlTagId type() const = 0;

  virtual std::string typeName() const = 0;

  //---

  virtual bool layoutChildren() const = 0;

  virtual void layout();

  virtual void getInlineWords(Words &words) const = 0;

  void getHierWords(Words &words) const;

  bool allChildrenInline() const;

  CIPoint2D parentPosition() const;

  CISize2D parentSize() const;

  void setHierVisible(bool visible);

  virtual void show() = 0;
  virtual void hide() = 0;

  //---

  virtual bool renderChildren() const = 0;

  void render(int dx, int dy);

  void renderWords(const Words &words, const CTextBox &box);

  void renderLineWords (const Words &words, const CTextBox &box);
  void renderFloatWords(const Words &words, const CTextBox &box);

  bool hasFloatWords(const Words &words) const;

  //---

  virtual void fillBackground(const CTextBox &box) = 0;

  virtual void draw(const CTextBox &box) = 0;

  virtual void drawBorder(const CTextBox &box) = 0;

  virtual void drawSelected(const CTextBox &box) = 0;

  //---

  void boxAt(const CIPoint2D &p, CBrowserBox* &box, double &area);

 private:
  using Boxes = std::vector<CBrowserBox *>;

  CBrowserWindowIFace* window_ { nullptr };
  CBrowserBox*         parent_ { nullptr };
  Boxes                children_;

  int             x_           { 0 };
  int             y_           { 0 };
  int             ascent_      { 0 };
  int             descent_     { 0 };
  CBrowserMargin  margin_;
  CBrowserBorder  border_;
  CBrowserPadding padding_;
  CIBBox2D        content_;
  CHAlignType     halign_      { CHALIGN_TYPE_LEFT };
  CVAlignType     valign_      { CVALIGN_TYPE_TOP };
  bool            fixedWidth_  { false };
  bool            fixedHeight_ { false };
};

class CBrowserBoxNode {
 public:
  CBrowserBoxNode() { }
};

#endif
