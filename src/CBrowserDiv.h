#ifndef CBrowserDiv_H
#define CBrowserDiv_H

#include <CBrowserObject.h>
#include <CBrowserData.h>

class CBrowserDiv : public CBrowserObject {
 public:
  CBrowserDiv(CBrowserWindow *window, const CBrowserDivData &data);

  void initLayout() override;
  void termLayout() override;

 private:
  CBrowserDivData data_;
};

#endif
