#ifndef CBrowserHtml_H
#define CBrowserHtml_H

#include <CBrowserObject.h>
#include <CBrowserData.h>

class CBrowserHtml : public CBrowserObject {
 public:
  explicit CBrowserHtml(CBrowserWindow *window);

  void init();

  void setNameValue(const std::string &name, const std::string &value) override;
};

#endif
