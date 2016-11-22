#ifndef CBrowserSymbol_H
#define CBrowserSymbol_H

#include <CBrowserObject.h>

class CBrowserSymbol : public CBrowserObject {
 public:
  CBrowserSymbol(CBrowserWindow *window, CBrowserSymbolType type, int width, int height);
 ~CBrowserSymbol();

  void format(CHtmlLayoutMgr *) override;
  void draw(CHtmlLayoutMgr *, const CHtmlLayoutRegion &) override;

 private:
  CBrowserWindow*    window_ { nullptr };
  CBrowserSymbolType type_ { CBrowserSymbolType::NONE };
  int                width_ { 0 };
  int                height_ { 0 };
};

#endif
