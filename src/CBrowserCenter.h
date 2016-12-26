#ifndef CBrowserCenter_H
#define CBrowserCenter_H

#include <CBrowserObject.h>

class CBrowserCenter : public CBrowserObject {
 public:
  CBrowserCenter(CBrowserWindow *window);

  void initLayout() override;
  void termLayout() override;
};

#endif
