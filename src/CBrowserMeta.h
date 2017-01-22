#ifndef CBrowserMeta_H
#define CBrowserMeta_H

#include <CBrowserObject.h>

class CBrowserMeta : public CBrowserObject {
 public:
  CBrowserMeta(CBrowserWindow *window);

  void setNameValue(const std::string &name, const std::string &value);
};

#endif
