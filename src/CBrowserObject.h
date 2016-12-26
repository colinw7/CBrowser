#ifndef CBrowserObject_H
#define CBrowserObject_H

#include <CBrowserTypes.h>
#include <CHtmlTypes.h>
#include <CHtmlLayoutBox.h>

class CBrowserObject : public CHtmlLayoutBox {
 public:
  typedef std::vector<CBrowserObject *> Children;
  typedef std::vector<std::string>      Properties;

 public:
  CBrowserObject(CBrowserWindow *window, CHtmlTagId type);
 ~CBrowserObject();

  CBrowserWindow *getWindow() const { return window_; }

  CHtmlTagId type() const { return type_; }

  const std::string &id() const { return id_; }
  void setId(const std::string &s) { id_ = s; }

  const std::string &text() const { return text_; }
  void setText(const std::string &s) { text_ = s; }

  bool isSelected() const { return selected_; }
  void setSelected(bool b) { selected_ = b; }

  CQJHtmlObj *getJObj() { return obj_; }
  void setJObj(CQJHtmlObj *obj) { obj_ = obj; }

  CBrowserObject *parent() const { return parent_; }
  void setParent(CBrowserObject *p) { parent_ = p; }

  std::string typeName() const;

  void addChild(CBrowserObject *child);

  const Children &children() const { return children_; }
  int numChildren() const { return children_.size(); }
  CBrowserObject *child(int i) const { return children_[i]; }
  int childIndex(const CBrowserObject *child) const;

  void setProperties(const Properties &properties) { properties_ = properties; }
  virtual int numProperties() const { return properties_.size(); }

  virtual std::string propertyName(int i) const { return properties_[i]; }
  virtual std::string propertyValue(int) const { return ""; }

  virtual bool isHierSelected() const;

  virtual void initProcess() { }
  virtual void termProcess() { }

  virtual void initLayout() { }
  virtual void termLayout() { }

  template<typename T>
  T *parentType() const {
    CBrowserObject *obj = this->parent();

    while (obj && ! dynamic_cast<T *>(obj))
      obj = obj->parent();

    return dynamic_cast<T *>(obj);
  }

  void format(CHtmlLayoutMgr *) override { }

  void draw(CHtmlLayoutMgr *, const CHtmlLayoutRegion &) override { }

  virtual void print(std::ostream &os) const { os << typeName(); }

 protected:
  CBrowserWindow* window_ { nullptr };
  CHtmlTagId      type_;
  std::string     id_;
  std::string     text_;
  bool            selected_ { false };
  CQJHtmlObj*     obj_ { nullptr };
  CBrowserObject* parent_ { nullptr };
  Children        children_;
  Properties      properties_;
};

#endif
