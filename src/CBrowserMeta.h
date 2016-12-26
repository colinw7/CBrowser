#ifndef CBrowserMeta_H
#define CBrowserMeta_H

#include <CBrowserObject.h>

class CBrowserMeta : public CBrowserObject {
 public:
  CBrowserMeta(CBrowserWindow *window);

  void initLayout() override;
  void termLayout() override;
};

#endif