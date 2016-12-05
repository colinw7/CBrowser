#ifndef CBrowserAddress_H
#define CBrowserAddress_H

#include <CBrowserObject.h>
#include <CHtmlLayout.h>

class CBrowserAddress : public CBrowserObject {
 public:
  CBrowserAddress(CBrowserWindow *window);
 ~CBrowserAddress();

  CBrowserWindow *getWindow() const { return window_; }

 private:
  CBrowserWindow* window_ { nullptr };
};

#endif
