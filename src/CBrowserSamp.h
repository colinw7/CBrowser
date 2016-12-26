#ifndef CBrowserSamp_H
#define CBrowserSamp_H

#include <CBrowserObject.h>

class CBrowserSamp : public CBrowserObject {
 public:
  CBrowserSamp(CBrowserWindow *window);

  void initProcess() override;
  void termProcess() override;
};

#endif
