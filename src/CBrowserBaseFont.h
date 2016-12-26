#ifndef CBrowserBaseFont_H
#define CBrowserBaseFont_H

#include <CBrowserObject.h>
#include <CBrowserData.h>

class CBrowserBaseFont : public CBrowserObject {
 public:
  CBrowserBaseFont(CBrowserWindow *window, const CBrowserBaseFontData &data);

  void initProcess() override;
  void termProcess() override;

 private:
  CBrowserBaseFontData data_;
};

#endif
