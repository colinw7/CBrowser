#ifndef CBrowserCSSStyle_H
#define CBrowserCSSStyle_H

#include <CBrowserObject.h>
#include <CBrowserData.h>

class CBrowserCSSStyle : public CBrowserObject {
 public:
  CBrowserCSSStyle(CBrowserWindow *window);

  const std::string &text() const { return text_; }
  void setText(const std::string &t) { text_ = t; }

  void initProcess() override;
  void termProcess() override;

 private:
  std::string text_;
};

#endif
