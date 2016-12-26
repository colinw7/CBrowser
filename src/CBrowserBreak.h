#ifndef CBrowserBreak_H
#define CBrowserBreak_H

#include <CBrowserObject.h>
#include <CBrowserData.h>
#include <CHtmlLayout.h>

class CBrowserBreak : public CBrowserObject {
 public:
  CBrowserBreak(CBrowserWindow *window, const CBrowserBreakData &data);
 ~CBrowserBreak();

  CHtmlLayoutClearType getClear() const { return data_.clear; }

  void initLayout() override;
  void termLayout() override;

  void format(CHtmlLayoutMgr *) override;

  void draw(CHtmlLayoutMgr *, const CHtmlLayoutRegion &) override;

 private:
  CBrowserBreakData data_;
};

//---

class CBrowserWbr : public CBrowserObject {
 public:
  CBrowserWbr(CBrowserWindow *window);
 ~CBrowserWbr();

  void initLayout() override;
  void termLayout() override;
};

#endif
