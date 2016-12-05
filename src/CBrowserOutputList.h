#ifndef CBrowserOutputList_H
#define CBrowserOutputList_H

#include <CBrowserObject.h>
#include <string>
#include <vector>

class CBrowserOutputList;

#define CBrowserOutputListMgrInst CBrowserOutputListMgr::instance()

class CBrowserOutputListMgr {
 public:
  static CBrowserOutputListMgr *instance();

  CBrowserOutputList *currentList() const;

  void startList(CBrowserOutputList *list);

  void endList();

  int listDepth() const { return lists_.size(); }

 private:
  typedef std::vector<CBrowserOutputList *> Lists;

  Lists lists_;
};

//---

class CBrowserOutputList : public CBrowserObject {
 public:
  CBrowserOutputList(CHtmlTagId id);

  const std::string &getSymbol() const { return symbol_; }
  void setSymbol(const std::string &symbol) { symbol_ = symbol; }

  int  getItemNum() const { return item_num_; }
  void setItemNum(int item_num) { item_num_ = item_num; }

  bool getCompact() const { return compact_; }
  void setCompact(bool compact) { compact_ = compact; }

 private:
  std::string symbol_;
  int         item_num_ { 0 };
  bool        compact_ { false };
};

#endif
