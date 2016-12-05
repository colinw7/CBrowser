#ifndef CBrowserObject_H
#define CBrowserObject_H

#include <CBrowserTypes.h>
#include <CHtmlTypes.h>
#include <CHtmlLayoutBox.h>

class CBrowserObject : public CHtmlLayoutBox {
 public:
  CBrowserObject(CHtmlTagId type);
 ~CBrowserObject();

  CHtmlTagId type() const { return type_; }

  const std::string &id() const { return id_; }
  void setId(const std::string &s) { id_ = s; }

  const std::string &text() const { return text_; }
  void setText(const std::string &s) { text_ = s; }

  bool isSelected() const { return selected_; }
  void setSelected(bool b) { selected_ = b; }

  CQJHtmlObj *getJObj() { return obj_; }
  void setJObj(CQJHtmlObj *obj) { obj_ = obj; }

  const CBrowserObject *parent() const { return parent_; }
  void setParent(CBrowserObject *p) { parent_ = p; }

  std::string typeName() const;

  void addChild(CBrowserObject *child);

  int numChildren() const { return children_.size(); }
  CBrowserObject *child(int i) const { return children_[i]; }
  int childIndex(const CBrowserObject *child) const;

  void format(CHtmlLayoutMgr *) override { }

  void draw(CHtmlLayoutMgr *, const CHtmlLayoutRegion &) override { }

  virtual void print(std::ostream &os) const { os << typeName(); }

 private:
  typedef std::vector<CBrowserObject *> Children;

  CHtmlTagId      type_;
  std::string     id_;
  std::string     text_;
  bool            selected_ { false };
  CQJHtmlObj*     obj_ { nullptr };
  CBrowserObject* parent_ { nullptr };
  Children        children_;
};

#endif
