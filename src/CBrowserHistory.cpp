#include <CBrowserHistory.h>
#include <CBrowserWindow.h>

CBrowserHistory::
CBrowserHistory(CBrowserWindow *window) :
 window_(window)
{
  url_num_ = 0;
  menu_    = NULL;
}

CBrowserHistory::
~CBrowserHistory()
{
}

void
CBrowserHistory::
setMenu(QMenu *menu)
{
  menu_ = menu;
}

void
CBrowserHistory::
addUrl(const CUrl &url)
{
  urls_.resize(url_num_);

  urls_.push_back(url);

  window_->addHistoryItem(url);
}

CUrl
CBrowserHistory::
goBack()
{
  if (url_num_ <= 1)
    return CUrl();

  url_num_--;

  return urls_[url_num_ - 1];
}

CUrl
CBrowserHistory::
goForward()
{
  int num_urls = urls_.size();

  if (url_num_ >= num_urls)
    return CUrl();

  url_num_++;

  return urls_[url_num_ - 1];
}

int
CBrowserHistory::
goTo(const CUrl &url)
{
  int num_urls = urls_.size();

  int i = 0;

  for ( ; i < num_urls; i++)
    if (urls_[i] == url)
      break;

  if (i >= num_urls)
    return false;

  url_num_ = i + 1;

  return true;
}
