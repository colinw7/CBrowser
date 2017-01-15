#ifndef CBrowserList_H
#define CBrowserList_H

#include <CBrowserObject.h>
#include <CBrowserData.h>
#include <string>
#include <vector>

class CBrowserList : public CBrowserObject {
 public:
  enum class SymbolType {
    NONE,
    BLOCK,
    CIRCLE,
    DECIMAL,
    DECIMAL_ZERO,
    DISC,
    LOWER_ALPHA,
    LOWER_LATIN,
    LOWER_ROMAN,
    SQUARE,
    UPPER_ALPHA,
    UPPER_LATIN,
    UPPER_ROMAN,
    INITIAL,
    INHERIT
  };

 public:
  static SymbolType stringToSymbol(const std::string &value);

  CBrowserList(CBrowserWindow *window, CHtmlTagId id, const CBrowserOutputListData &data);

  const std::string &getSymbol() const { return data_.symbol; }
  void setSymbol(const std::string &symbol) { data_.symbol = symbol; }

  int  getItemNum() const { return data_.item_num; }
  void setItemNum(int item_num) { data_.item_num = item_num; }

  bool getCompact() const { return data_.compact; }
  void setCompact(bool compact) { data_.compact = compact; }

  void setNameValue(const std::string &name, const std::string &value) override;

  void init() override;

  int listDepth() const;

 private:
  CBrowserOutputListData data_;
  SymbolType             symbolType_ { SymbolType::NONE };
  int                    indent_ { 0 };
};

#endif
