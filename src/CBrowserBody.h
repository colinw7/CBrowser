#ifndef CBrowserBody_H
#define CBrowserBody_H

#include <CBrowserObject.h>
#include <CBrowserData.h>

class CBrowserBody : public CBrowserObject {
 public:
  explicit CBrowserBody(CBrowserWindowIFace *window);

  void setNameValue(const std::string &name, const std::string &value) override;

  void init() override;

  void initProcess() override;
  void termProcess() override;

 private:
  CBrowserBodyData data_;
};

#endif
