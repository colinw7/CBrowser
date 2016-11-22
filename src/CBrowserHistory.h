#ifndef CBrowserHistory_H
#define CBrowserHistory_H

#include <CBrowserTypes.h>

class QMenu;

class CBrowserHistory {
 public:
  CBrowserHistory(CBrowserWindow *window);
 ~CBrowserHistory();

  void setMenu(QMenu *menu);

  void addUrl(const std::string &url);

  std::string goBack();
  std::string goForward();

  int goTo(const std::string &url);

 private:
  typedef std::vector<std::string> Urls;

  CBrowserWindow* window_;
  Urls            urls_;
  int             url_num_;
  QMenu*          menu_;
};

#endif
