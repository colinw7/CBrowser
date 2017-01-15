#ifndef CBrowserParagraph_H
#define CBrowserParagraph_H

#include <CBrowserObject.h>
#include <CBrowserData.h>

class CBrowserParagraph : public CBrowserObject {
 public:
  CBrowserParagraph(CBrowserWindow *window,
                    const CBrowserParagraphData &data=CBrowserParagraphData());

  void init() override;

  void setNameValue(const std::string &name, const std::string &value) override;

  std::string propertyValue(int i) const override;

 private:
  CBrowserParagraphData data_;
};

#endif
