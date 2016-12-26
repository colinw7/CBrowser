#ifndef CBrowserAddress_H
#define CBrowserAddress_H

#include <CBrowserObject.h>

class CBrowserAddress : public CBrowserObject {
 public:
  CBrowserAddress(CBrowserWindow *window);
 ~CBrowserAddress();

  void initProcess() override;
  void termProcess() override;
};

#endif
