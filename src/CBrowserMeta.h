#ifndef CBrowserMeta_H
#define CBrowserMeta_H

#include <CBrowserObject.h>

class CBrowserMeta : public CBrowserObject {
 public:
  explicit CBrowserMeta(CBrowserWindow *window);

  void init();

  std::string propertyValue(int i) const;

  void setNameValue(const std::string &name, const std::string &value);

 private:
  CBrowserMetaData data_;
};

#endif
