#ifndef CBrowserDiv_H
#define CBrowserDiv_H

#include <CBrowserObject.h>
#include <CBrowserData.h>

class CBrowserDiv : public CBrowserObject {
 public:
  CBrowserDiv(CBrowserWindow *window, const CBrowserDivData &data);

  void setNameValue(const std::string &name, const std::string &value) override;

  void init() override;

 private:
  CBrowserDivData data_;
};

#endif
