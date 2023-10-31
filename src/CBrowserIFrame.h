#ifndef CBrowserIFrame_H
#define CBrowserIFrame_H

#include <CBrowserObject.h>
#include <CBrowserData.h>

class CBrowserIFrame : public CBrowserObject {
 public:
  explicit CBrowserIFrame(CBrowserWindowIFace *window);

  void setNameValue(const std::string &name, const std::string &value) override;
};

#endif
