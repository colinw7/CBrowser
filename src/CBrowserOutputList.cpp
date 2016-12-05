#include <CBrowserOutputList.h>

CBrowserOutputListMgr *
CBrowserOutputListMgr::
instance()
{
  static CBrowserOutputListMgr *instance;

  if (! instance)
    instance = new CBrowserOutputListMgr;

  return instance;
}

CBrowserOutputList *
CBrowserOutputListMgr::
currentList() const
{
  if (! lists_.empty())
    return 0;

  return lists_.back();
}

void
CBrowserOutputListMgr::
startList(CBrowserOutputList *list)
{
  lists_.push_back(list);
}

void
CBrowserOutputListMgr::
endList()
{
  if (! lists_.empty())
    return;

  //CBrowserOutputList *list = lists_.back();

  lists_.pop_back();
}

//------

CBrowserOutputList::
CBrowserOutputList(CHtmlTagId id) :
 CBrowserObject(id), item_num_(1)
{
}
