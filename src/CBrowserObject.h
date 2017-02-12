#ifndef CBrowserObject_H
#define CBrowserObject_H

#include <CBrowserTypes.h>
#include <CBrowserBox.h>
#include <CBrowserRegion.h>
#include <CBrowserData.h>
#include <CBrowserBackground.h>
#include <CBrowserColor.h>
#include <CBrowserFont.h>
#include <CBrowserTextProp.h>
#include <CBrowserClear.h>
#include <CBrowserFloat.h>
#include <CBrowserPosition.h>
#include <CBrowserSize.h>
#include <CHtmlTypes.h>
#include <CJavaScript.h>

class CHtmlTag;

//---

class CBrowserObject : public CBrowserBox {
 public:
  enum class Display {
    INVALID,
    NONE,
    BLOCK,
    INLINE,
    TABLE,
    TABLE_ROW,
    TABLE_CELL,
    TABLE_CAPTION,
    LIST_ITEM
  };

  enum class WhiteSpace {
    INVALID,
    NORMAL,
    NOWRAP,
    PRE,
    PRE_LINE,
    PRE_WRAP,
    INITIAL,
    INHERIT
  };

 public:
  typedef std::vector<CBrowserObject *> Children;
  typedef std::vector<std::string>      Classes;
  typedef std::vector<std::string>      Properties;

 public:
  CBrowserObject(CBrowserWindow *window, CHtmlTagId type,
                 const CBrowserBaseData &data=CBrowserBaseData());
 ~CBrowserObject();

  virtual void init();

  CBrowserWindow *getWindow() const { return window_; }

  virtual CHtmlTagId type() const override { return type_; }

  CHtmlTag *tag() const { return tag_; }
  void setTag(CHtmlTag *v) { tag_ = v; }

  const std::string &id() const { return id_; }
  void setId(const std::string &s) { id_ = s; }

  const std::string &getName() const { return name_; }
  void setName(const std::string &s) { name_ = s; }

  const std::string &getClass() const { return class_; }
  void setClass(const std::string &s);
  const Classes &getClasses() const { return classes_; }

  const std::string &text() const { return text_; }
  void setText(const std::string &s) { text_ = s; }

  bool isSelected() const { return selected_; }
  void setSelected(bool b) { selected_ = b; }

  CQJHtmlObj *getJObj() const;
  void setJObj(CQJHtmlObj *obj);

  CJValueP getJObjValue() const { return objValue_; }

  CBrowserObject *parent() const { return parent_; }
  void setParent(CBrowserObject *p) { parent_ = p; }

  std::string typeName() const override;
  std::string hierTypeName() const;

  void addChild(CBrowserObject *child);

  const Children &children() const { return children_; }
  int numChildren() const { return children_.size(); }
  CBrowserObject *child(int i) const { return children_[i]; }
  int childIndex(const CBrowserObject *child) const;

  Display display() const;
  void setDisplay(const Display &v) { display_ = v; }

  WhiteSpace whiteSpace() const { return whiteSpace_; }
  void setWhiteSpace(const WhiteSpace &v) { whiteSpace_ = v; }

  const CBrowserBackground &background() const { return background_; }
  void setBackground(const CBrowserBackground &bg) { background_ = bg; }

  const CBrowserColor &foreground() const { return foreground_; }
  void setForeground(const CBrowserColor &c) { foreground_ = c; }

  const CBrowserClear &clear() const { return clear_; }
  void setClear(const CBrowserClear &v) { clear_ = v; }

  const CBrowserFloat &getFloat() const { return float_; }
  void setFloat(const CBrowserFloat &v) { float_ = v; }

  const CBrowserPosition &position() const override { return position_; }
  void setPosition(const CBrowserPosition &v) { position_ = v; }

  const std::string &title() const { return title_; }
  void setTitle(const std::string &v) { title_ = v; }

  const CBrowserFont &font() const { return font_; }

  const CBrowserTextProp &textProp() const { return textProp_; }

  virtual WhiteSpace hierWhiteSpace() const;

  virtual CFontPtr hierFont() const;

  virtual CRGBA hierFgColor() const;

  //--

  const CBrowserSize &size() const { return size_; }
  void setSize(const CBrowserSize &v) { size_ = v; }

  const CBrowserUnitValue &width () const { return size_.width; }
  void setWidth(const CBrowserUnitValue &w) { size_.width = w; }

  const CBrowserUnitValue &height() const { return size_.height; }
  void setHeight(const CBrowserUnitValue &h) { size_.height = h; }

  //--

  virtual void setNameValue(const std::string &name, const std::string &value);

  virtual void setStyleValue(const std::string &name, const std::string &value);

  void styleValueToWords(const std::string &value, std::vector<std::string> &words);

  //---

  void addProperties(const Properties &properties);
  virtual int numProperties() const { return properties_.size(); }
  virtual std::string propertyName(int i) const { return properties_[i]; }
  virtual std::string propertyValue(int i) const;

  //---

  void processStyleAttribute(const std::string &value);

  //---

  virtual bool isHierSelected() const;

  //---

  virtual void initProcess() { }
  virtual void termProcess() { }

  virtual void initLayout() { }
  virtual void termLayout() { }

  //---

  template<typename T>
  T *parentType() const {
    CBrowserObject *obj = this->parent();

    while (obj && ! dynamic_cast<T *>(obj))
      obj = obj->parent();

    return dynamic_cast<T *>(obj);
  }

  //---

  bool layoutChildren() const override { return true; }

  bool renderChildren() const override { return true; }

  //---

  virtual CBrowserRegion calcRegion() const { return CBrowserRegion(); }

  virtual CIBBox2D calcBBox() const override;

  //---

  bool isVisible() const override;

  bool isInline() const override;

  bool isBreak() const override { return false; }

  bool hasInlineChildren() const;

  //---

  virtual void heightForWidth(CTextBox &box) const override;

  virtual void getInlineWords(Words &words) const override;

  void show() override { }
  void hide() override { }

  //---

  void fillBackground(const CTextBox &region) override;

  void draw(const CTextBox &) override;

  void drawBorder(const CTextBox &region) override;

  void drawBorderLine(double x1, double y1, double x2, double y2,
                      CBrowserBorderStyle style, const CPen &pen, CBrowserBorderSide side);

  //---

  virtual void print(std::ostream &os) const { os << typeName(); }

 protected:
  CBrowserWindow*    window_ { nullptr };
  CHtmlTagId         type_;
  CBrowserBaseData   data_;
  CHtmlTag*          tag_ { nullptr };
  std::string        id_;
  std::string        name_;
  std::string        class_;
  Classes            classes_;
  std::string        text_;
  bool               selected_ { false };
  CJValueP           objValue_;
  CBrowserObject*    parent_ { nullptr };
  Children           children_;
  Display            display_ { Display::INVALID };
  WhiteSpace         whiteSpace_ { WhiteSpace::NORMAL };
  CBrowserBackground background_;
  CBrowserColor      foreground_;
  CBrowserClear      clear_;
  CBrowserFloat      float_;
  CBrowserPosition   position_;
  CBrowserUnitValue  maxWidth_;
  CBrowserUnitValue  maxHeight_;
  CBrowserUnitValue  minWidth_;
  CBrowserUnitValue  minHeight_;
  int                zIndex_ { -1 };
  std::string        title_;
  CBrowserFont       font_;
  CBrowserTextProp   textProp_;
  CBrowserSize       size_;
  Properties         properties_;
};

//------

#include <CBrowserProperty.h>

template<>
class CBrowserPropertyTraits<CBrowserObject::Display> {
 public:
  std::string toString(const CBrowserObject::Display &d) {
    if      (d == CBrowserObject::Display::NONE         ) return "none";
    else if (d == CBrowserObject::Display::BLOCK        ) return "block";
    else if (d == CBrowserObject::Display::INLINE       ) return "inline";
    else if (d == CBrowserObject::Display::TABLE        ) return "tabel";
    else if (d == CBrowserObject::Display::TABLE_ROW    ) return "table-row";
    else if (d == CBrowserObject::Display::TABLE_CELL   ) return "table-cell";
    else if (d == CBrowserObject::Display::TABLE_CAPTION) return "table-caption";

    return "";
  }

  bool fromString(const std::string &str, CBrowserObject::Display &d) {
    std::string lstr = CStrUtil::toLower(str);

    d = CBrowserObject::Display::INVALID;

    if      (lstr == "none"         ) d = CBrowserObject::Display::NONE;
    else if (lstr == "block"        ) d = CBrowserObject::Display::BLOCK;
    else if (lstr == "inline"       ) d = CBrowserObject::Display::INLINE;
    else if (lstr == "table"        ) d = CBrowserObject::Display::TABLE;
    else if (lstr == "table-row"    ) d = CBrowserObject::Display::TABLE_ROW;
    else if (lstr == "table-cell"   ) d = CBrowserObject::Display::TABLE_CELL;
    else if (lstr == "table-caption") d = CBrowserObject::Display::TABLE_CAPTION;
    else                              return false;

    return true;
  }
};

template<>
class CBrowserPropertyTraits<CBrowserObject::WhiteSpace> {
 public:
  std::string toString(const CBrowserObject::WhiteSpace &d) {
    if      (d == CBrowserObject::WhiteSpace::NORMAL  ) return "normal";
    else if (d == CBrowserObject::WhiteSpace::NOWRAP  ) return "nowrap";
    else if (d == CBrowserObject::WhiteSpace::PRE     ) return "pre";
    else if (d == CBrowserObject::WhiteSpace::PRE_LINE) return "pre-line";
    else if (d == CBrowserObject::WhiteSpace::PRE_WRAP) return "pre-wrap";
    else if (d == CBrowserObject::WhiteSpace::INITIAL ) return "initial";
    else if (d == CBrowserObject::WhiteSpace::INHERIT ) return "inherit";

    return "";
  }

  bool fromString(const std::string &str, CBrowserObject::WhiteSpace &d) {
    std::string lstr = CStrUtil::toLower(str);

    d = CBrowserObject::WhiteSpace::INVALID;

    if      (lstr == "normal"  ) d = CBrowserObject::WhiteSpace::NORMAL;
    else if (lstr == "nowrap"  ) d = CBrowserObject::WhiteSpace::NOWRAP;
    else if (lstr == "pre"     ) d = CBrowserObject::WhiteSpace::PRE;
    else if (lstr == "pre-line") d = CBrowserObject::WhiteSpace::PRE_LINE;
    else if (lstr == "pre-wrap") d = CBrowserObject::WhiteSpace::PRE_WRAP;
    else if (lstr == "initial" ) d = CBrowserObject::WhiteSpace::INITIAL;
    else if (lstr == "inherit" ) d = CBrowserObject::WhiteSpace::INHERIT;
    else                         return false;

    return true;
  }
};

#endif
