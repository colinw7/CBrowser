#ifndef CBrowserSymbol_H
#define CBrowserSymbol_H

#include <CBrowserObject.h>

class CBrowserSymbol : public CBrowserObject {
 public:
  enum class Type {
    NONE,
    DISC,
    CIRCLE,
    BLOCK,
    SQUARE
  };

 public:
  CBrowserSymbol(CBrowserWindow *window, Type type);
 ~CBrowserSymbol();

  void initLayout() override;
  void termLayout() override;

  void format(CHtmlLayoutMgr *) override;
  void draw(CHtmlLayoutMgr *, const CHtmlLayoutRegion &) override;

 private:
  Type type_    { Type::NONE };
  int  width_   { 0 };
  int  ascent_  { 0 };
  int  descent_ { 0 };
};

#endif
