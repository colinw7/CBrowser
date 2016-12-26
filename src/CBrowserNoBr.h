#ifndef CBrowserNoBr_H
#define CBrowserNoBr_H

#include <CBrowserObject.h>

class CBrowserNoBr : public CBrowserObject {
 public:
  CBrowserNoBr(CBrowserWindow *window);

  void initProcess() override;
  void termProcess() override;
};

#endif
