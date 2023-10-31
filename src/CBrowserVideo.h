#ifndef CBrowserVideo_H
#define CBrowserVideo_H

#include <CBrowserObject.h>
#include <CBrowserData.h>

class CBrowserVideo : public CBrowserObject {
 public:
  explicit CBrowserVideo(CBrowserWindowIFace *window);

  void setNameValue(const std::string &name, const std::string &value) override;
};

#endif
