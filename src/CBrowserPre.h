#ifndef CBrowserPre_H
#define CBrowserPre_H

#include <CBrowserObject.h>
#include <CBrowserData.h>

class CBrowserPre : public CBrowserObject {
 public:
  CBrowserPre(CBrowserWindow *window, const CBrowserPreData &data);

  void init() override;

  void setNameValue(const std::string &name, const std::string &value) override;

 private:
  CBrowserPreData data_;
};

#endif
