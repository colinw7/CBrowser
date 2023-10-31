#ifndef CBrowserHeader_H
#define CBrowserHeader_H

#include <CBrowserObject.h>
#include <CBrowserData.h>
#include <CFont.h>
#include <CStrUtil.h>

class CBrowserHeader : public CBrowserObject {
 public:
  CBrowserHeader(CBrowserWindowIFace *window, CHtmlTagId id);
 ~CBrowserHeader();

  void init() override;

  void setNameValue(const std::string &name, const std::string &value) override;

  void print(std::ostream &os) const override {
    os << "header '" << ind_ << "'";
  }

 private:
  CBrowserHeaderData data_;
  int                ind_ { -1 };
};

#endif
