#ifndef CBrowserHtml_H
#define CBrowserHtml_H

#include <CBrowserObject.h>
#include <CBrowserData.h>

class CBrowserHtml : public CBrowserObject {
 public:
  CBrowserHtml(CBrowserWindow *window);

  void initLayout() override;
  void termLayout() override;
};

#endif
