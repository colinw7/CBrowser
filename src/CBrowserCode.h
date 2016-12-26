#ifndef CBrowserCode_H
#define CBrowserCode_H

#include <CBrowserObject.h>

class CBrowserCode : public CBrowserObject {
 public:
  CBrowserCode(CBrowserWindow *window);

  void initProcess() override;
  void termProcess() override;
};

#endif
