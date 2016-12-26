#ifndef CBrowserTT_H
#define CBrowserTT_H

#include <CBrowserObject.h>

class CBrowserTT : public CBrowserObject {
 public:
  CBrowserTT(CBrowserWindow *window);

  void initProcess() override;
  void termProcess() override;
};

#endif
