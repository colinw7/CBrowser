#ifndef CBrowserObject_H
#define CBrowserObject_H

#include <CBrowserTypes.h>
#include <CHtmlLayoutBox.h>

class CBrowserObject : public CHtmlLayoutBox {
 public:
  enum class Type {
    BREAK,
    CANVAS,
    FORM_INPUT,
    IMAGE,
    LABEL,
    RULE,
    SYMBOL,
    TABLE,
    TEXT
  };

 public:
  CBrowserObject(Type type) :
   type_(type) {
  }

  Type type() const { return type_; }

  std::string typeName() const;

  CQJHtmlObj *getJObj() { return obj_; }
  void setJObj(CQJHtmlObj *obj) { obj_ = obj; }

  virtual void print(std::ostream &os) const { os << typeName(); }

 private:
  Type        type_;
  CQJHtmlObj *obj_ { nullptr };
};

#endif
