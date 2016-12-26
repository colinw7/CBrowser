#ifndef CBrowserScript_H
#define CBrowserScript_H

#include <CBrowserObject.h>
#include <CBrowserData.h>

class CBrowserScript : public CBrowserObject {
 public:
  CBrowserScript(CBrowserWindow *window, const CBrowserScriptData &data);

  const std::string &text() const { return text_; }
  void setText(const std::string &t) { text_ = t; }

  const CBrowserScriptData &data() const { return data_; }

  void initProcess() override;
  void termProcess() override;

 private:
  std::string        text_;
  CBrowserScriptData data_;
};

#endif
