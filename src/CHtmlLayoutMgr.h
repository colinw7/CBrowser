#ifndef CHTML_LAYOUT_MGR
#define CHTML_LAYOUT_MGR

#include <CAlignType.h>
#include <vector>

class CHtmlLayoutArea;
class CHtmlLayoutCell;
class CHtmlLayoutSubCell;

class CHtmlLayoutMgr {
 public:
  CHtmlLayoutMgr();
 ~CHtmlLayoutMgr();

  void init();

  void startArea(CHtmlLayoutArea *area);
  void endArea();

  CHtmlLayoutArea *getCurrentArea() const { return current_area_; }

  CHAlignType getHAlign() const { return halign_; }
  CVAlignType getVAlign() const { return valign_; }

  void setAlign(CHAlignType halign, CVAlignType valign);

  CHtmlLayoutCell    *getCurrentCell() const;
  CHtmlLayoutSubCell *getCurrentSubCell() const;

 private:
  typedef std::vector<CHtmlLayoutArea *> AreaList;

  CHtmlLayoutArea *current_area_;
  AreaList         current_area_stack_;
  CHAlignType      halign_;
  CVAlignType      valign_;
};

#endif
