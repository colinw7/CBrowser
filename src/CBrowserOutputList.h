#ifndef CBrowserOutputList_H
#define CBrowserOutputList_H

#include <CBrowserObject.h>
#include <CBrowserData.h>
#include <string>
#include <vector>

class CBrowserOutputList : public CBrowserObject {
 public:
  CBrowserOutputList(CBrowserWindow *window, CHtmlTagId id, const CBrowserOutputListData &data);

  const std::string &getSymbol() const { return data_.symbol; }
  void setSymbol(const std::string &symbol) { data_.symbol = symbol; }

  int  getItemNum() const { return data_.item_num; }
  void setItemNum(int item_num) { data_.item_num = item_num; }

  bool getCompact() const { return data_.compact; }
  void setCompact(bool compact) { data_.compact = compact; }

  void initLayout() override;
  void termLayout() override;

  int listDepth() const;

 private:
  CBrowserOutputListData data_;
};

#endif
