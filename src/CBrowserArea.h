#ifndef CBrowserArea_H
#define CBrowserArea_H

#include <CBrowserObject.h>

class CBrowserArea : public CBrowserObject {
 public:
  explicit CBrowserArea(CBrowserWindowIFace *window);

  void init() override;

  void setNameValue(const std::string &name, const std::string &value) override;
};

#endif
