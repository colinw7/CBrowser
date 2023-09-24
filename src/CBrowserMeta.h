#ifndef CBrowserMeta_H
#define CBrowserMeta_H

#include <CBrowserObject.h>

class CBrowserMeta : public CBrowserObject {
 public:
  explicit CBrowserMeta(CBrowserWindow *window);

  void init() override;

  std::string propertyValue(int i) const override;

  void setNameValue(const std::string &name, const std::string &value) override;

 private:
  CBrowserMetaData data_;
};

#endif
