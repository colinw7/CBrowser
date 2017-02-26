#ifndef CBrowserObjectCSSTagData_H
#define CBrowserObjectCSSTagData_H

#include <CCSS.h>
#include <CBrowserObject.h>
#include <CBrowserForm.h>
#include <CHtmlTag.h>

class CBrowserObjectCSSTagData : public CCSSTagData {
 public:
  typedef std::vector<CBrowserObject *> Children;

 public:
  explicit CBrowserObjectCSSTagData(CBrowserObject *obj) :
   obj_(obj) {
  }

  CBrowserObject *obj() const { return obj_; }

  bool isElement(const std::string &name) const override {
    return (name == obj_->typeName());
  }

  bool isClass(const std::string &name) const override {
    for (const auto &c : obj_->getClasses()) {
      if (name == c)
        return true;
    }

    return false;
  }

  bool isId(const std::string &name) const override {
    return (name == obj_->id());
  }

  bool hasAttribute(const std::string &name, CCSSAttributeOp op,
                    const std::string &value) const override {
    CHtmlTag *tag = obj_->tag();
    if (! tag) return false;

    if      (op == CCSSAttributeOp::NONE)
      return tag->hasOptionName(name);
    else if (op == CCSSAttributeOp::EQUAL)
      return tag->hasOptionNameValue(name, value);
    else if (op == CCSSAttributeOp::PARTIAL)
      return tag->hasOptionNameSubValue(name, value);
    else if (op == CCSSAttributeOp::STARTS_WITH)
      return tag->hasOptionNameStart(name, value);
    else
      return false;
  }

  bool isNthChild(int n) const override {
    if (! obj_->parent())
      return false;

    const CBrowserObject::Children &children = obj_->parent()->children();

    int i = 1;

    for (const auto &child : children) {
      if (child == obj_)
        return (i == n);

      ++i;
    }

    return false;
  }

  bool isInputValue(const std::string &name) const override {
    if (obj_->type() != CHtmlTagId::INPUT)
      return false;

    CBrowserFormInput *input = dynamic_cast<CBrowserFormInput *>(obj_);
    if (! input) return false;

    std::string value;

    return input->getNameValue(name, value);
  }

  CCSSTagDataP getParent() const override {
    if (! obj_->parent())
      return CCSSTagDataP();

    return CCSSTagDataP(new CBrowserObjectCSSTagData(obj_->parent()));
  }

  void getChildren(TagDataArray &children) const override {
    const CBrowserObject::Children &children1 = obj_->children();

    for (const auto &child : children1) {
      CCSSTagDataP dataP(new CBrowserObjectCSSTagData(child));

      children.push_back(dataP);
    }
  }

  CCSSTagDataP getPrevSibling() const override {
    if (! obj_->parent())
      return CCSSTagDataP();

    const CBrowserObject::Children &children = obj_->parent()->children();

    CBrowserObject *lastChild = nullptr;

    for (const auto &child : children) {
      if (child == obj_) {
        if (lastChild)
          return CCSSTagDataP(new CBrowserObjectCSSTagData(lastChild));
      }

      lastChild = child;
    }

    return CCSSTagDataP();
  }

  CCSSTagDataP getNextSibling() const override {
    if (! obj_->parent())
      return CCSSTagDataP();

    bool nextChild = false;

    const CBrowserObject::Children &children = obj_->parent()->children();

    for (const auto &child : children) {
      if (nextChild)
        return CCSSTagDataP(new CBrowserObjectCSSTagData(child));

      if (child == obj_)
        nextChild = true; // return next child
    }

    return CCSSTagDataP();
  }

 private:
  CBrowserObject *obj_ { nullptr };
};

#endif
