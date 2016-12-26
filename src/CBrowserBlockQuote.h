#ifndef CBrowserBlockQuote_H
#define CBrowserBlockQuote_H

#include <CBrowserObject.h>

class CBrowserBlockQuote : public CBrowserObject {
 public:
  CBrowserBlockQuote(CBrowserWindow *window);

  void initProcess() override;
  void termProcess() override;

  void initLayout() override;
  void termLayout() override;
};

#endif
