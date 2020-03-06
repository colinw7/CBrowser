#ifndef CBrowserSVG_H
#define CBrowserSVG_H

#include <CBrowserObject.h>
#include <CSVG.h>

class CQSVGRenderer;

class CBrowserSVG : public CBrowserObject {
 public:
  explicit CBrowserSVG(CBrowserWindow *window);

  bool processTag(CHtmlTag *) override;

  void setNameValue(const std::string &name, const std::string &value) override;

  CBrowserRegion calcRegion() const override;

  void draw(const CTextBox &region) override;

 private:
  CSVG           svg_;
  CSVGObject*    block_ { nullptr };
  CSVGObject*    currentObj_ { nullptr };
  CQSVGRenderer* renderer_ { nullptr };
};

#endif
