#ifndef CBrowserHead_H
#define CBrowserHead_H

#include <CBrowserObject.h>

class CBrowserHead : public CBrowserObject {
 public:
  explicit CBrowserHead(CBrowserWindow *window);

  void init() override;

  void setNameValue(const std::string &name, const std::string &value) override;
};

#endif
