#ifndef CBrowserPre_H
#define CBrowserPre_H

#include <CBrowserObject.h>
#include <CBrowserData.h>

class CBrowserPre : public CBrowserObject {
 public:
  CBrowserPre(CBrowserWindow *window, const CBrowserPreData &data);

  void initProcess() override;
  void termProcess() override;

  void initLayout() override;
  void termLayout() override;

 private:
  CBrowserPreData data_;
};

#endif
