#ifndef CBrowserHistory_H
#define CBrowserHistory_H

#include <CBrowserTypes.h>
#include <CUrl.h>

class QMenu;

class CBrowserHistory {
 public:
  explicit CBrowserHistory(CBrowserWindow *window);

 ~CBrowserHistory();

  void setMenu(QMenu *menu);

  void addUrl(const CUrl &url);

  CUrl goBack();
  CUrl goForward();

  int goTo(const CUrl &url);

 private:
  typedef std::vector<CUrl> Urls;

  CBrowserWindow* window_ { nullptr };
  Urls            urls_;
  int             url_num_ { 0 };
  QMenu*          menu_ { nullptr };
};

#endif
