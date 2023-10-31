#ifndef CBrowserScript_H
#define CBrowserScript_H

#include <CBrowserObject.h>
#include <CBrowserData.h>

class CBrowserScript : public CBrowserObject {
 public:
  CBrowserScript(CBrowserWindowIFace *window, const CBrowserScriptData &data);

  const std::string &text() const { return text_; }
  void setText(const std::string &t) { text_ = t; }

  const CBrowserScriptData &data() const { return data_; }

  void init() override;

  void setNameValue(const std::string &name, const std::string &value) override;

  void initProcess() override;
  void termProcess() override;

  std::string propertyValue(int i) const override;

 private:
  CBrowserWindowIFace* window_ { nullptr };
  CBrowserScriptData   data_;
  std::string          text_;
};

#endif
