#ifndef CBrowserKbd_H
#define CBrowserKbd_H

#include <CBrowserObject.h>

class CBrowserKbd : public CBrowserObject {
 public:
  CBrowserKbd(CBrowserWindow *window);

  void initProcess() override;
  void termProcess() override;
};

#endif