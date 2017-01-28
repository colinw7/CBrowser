#ifndef CBrowserBaseFont_H
#define CBrowserBaseFont_H

#include <CBrowserObject.h>
#include <CBrowserData.h>

class CBrowserBaseFont : public CBrowserObject {
 public:
  explicit CBrowserBaseFont(CBrowserWindow *window);

  void init();

  void setNameValue(const std::string &name, const std::string &value) override;

  void initProcess() override;
  void termProcess() override;

  void apply() const;

 private:
  CBrowserBaseFontData data_;
};

#endif
