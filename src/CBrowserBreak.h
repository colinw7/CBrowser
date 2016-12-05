#ifndef CBrowserBreak_H
#define CBrowserBreak_H

#include <CBrowserObject.h>
#include <CHtmlLayout.h>

class CBrowserBreak : public CBrowserObject {
 public:
  CBrowserBreak(CBrowserWindow *window, CHtmlLayoutClearType clear=CHtmlLayoutClearType::NONE);
 ~CBrowserBreak();

  CBrowserWindow *getWindow() const { return window_; }

  CHtmlLayoutClearType getClear() const { return clear_; }

  void format(CHtmlLayoutMgr *) override;

  void draw(CHtmlLayoutMgr *, const CHtmlLayoutRegion &) override;

 private:
  CBrowserWindow*      window_ { nullptr };
  CHtmlLayoutClearType clear_ { CHtmlLayoutClearType::NONE };
};

#endif
