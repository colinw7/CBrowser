#ifndef CBrowserXmp_H
#define CBrowserXmp_H

#include <CBrowserObject.h>

class CBrowserXmp : public CBrowserObject {
 public:
  CBrowserXmp(CBrowserWindow *window);

  void initProcess() override;
  void termProcess() override;

  void initLayout() override;
  void termLayout() override;
};

#endif
