#ifndef CBrowserArea_H
#define CBrowserArea_H

#include <CBrowserObject.h>

class CBrowserArea : public CBrowserObject {
 public:
  CBrowserArea(CBrowserWindow *window);

  void init();

  void setNameValue(const std::string &name, const std::string &value);
};

#endif
