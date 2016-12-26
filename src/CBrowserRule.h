#ifndef CBrowserRule_H
#define CBrowserRule_H

#include <CBrowserObject.h>

struct CBrowserRuleData {
  CHAlignType       align = CHALIGN_TYPE_CENTER;
  bool              shade = true;
  int               size  = -1;
  int               width = -1;
  CBrowserUnitsType unit  = CBrowserUnitsType::PIXEL;
};

class CBrowserRule : public CBrowserObject {
 public:
  CBrowserRule(CBrowserWindow *window, const CBrowserRuleData &data);
 ~CBrowserRule();

  void initLayout() override;
  void termLayout() override;

  void format(CHtmlLayoutMgr *) override;

  void draw(CHtmlLayoutMgr *, const CHtmlLayoutRegion &) override;

 private:
  CBrowserRuleData data_;
};

#endif
