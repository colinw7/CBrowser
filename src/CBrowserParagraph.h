#ifndef CBrowserParagraph_H
#define CBrowserParagraph_H

#include <CBrowserObject.h>
#include <CBrowserData.h>

class CBrowserParagraph : public CBrowserObject {
 public:
  CBrowserParagraph(CBrowserWindow *window, const CBrowserParagraphData &data);

  std::string propertyValue(int i) const override;

  void initLayout() override;
  void termLayout() override;

 private:
  CBrowserParagraphData data_;
};

#endif
