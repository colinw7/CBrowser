#ifndef CBrowserRule_H
#define CBrowserRule_H

#include <CBrowserObject.h>
#include <CBrowserUnitValue.h>

class CBrowserRule : public CBrowserObject {
 public:
  explicit CBrowserRule(CBrowserWindow *window);

 ~CBrowserRule();

  void init() override;

  void setNameValue(const std::string &name, const std::string &value) override;

  CBrowserRegion calcRegion() const override;

  void draw(const CTextBox &) override;

 private:
  CBrowserRuleData data_;
};

#endif
