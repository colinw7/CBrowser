#ifndef CBrowserHistory_H
#define CBrowserHistory_H

#include <CBrowserTypes.h>
#include <CUrl.h>

class QMenu;

class CBrowserHistory {
 public:
  explicit CBrowserHistory(CBrowserWindowIFace *window);

 ~CBrowserHistory();

  void setMenu(QMenu *menu);

  void addUrl(const CUrl &url);

  CUrl goBack();
  CUrl goForward();

  int goTo(const CUrl &url);

 private:
  using Urls = std::vector<CUrl>;

  CBrowserWindowIFace* window_ { nullptr };

  Urls urls_;
  int  url_num_ { 0 };

  QMenu* menu_ { nullptr };
};

#endif
