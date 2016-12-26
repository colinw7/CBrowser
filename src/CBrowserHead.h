#ifndef CBrowserHead_H
#define CBrowserHead_H

#include <CBrowserObject.h>

class CBrowserHead : public CBrowserObject {
 public:
  CBrowserHead(CBrowserWindow *window);

  void initLayout() override;
  void termLayout() override;
};

#endif
