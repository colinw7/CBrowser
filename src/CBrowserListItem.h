#ifndef CBrowserListItem_H
#define CBrowserListItem_H

#include <CBrowserObject.h>
#include <CBrowserList.h>
#include <CBrowserData.h>

class CBrowserList;

class CBrowserListItem : public CBrowserObject {
 public:
  struct LabelSymbolData {
    enum class Type {
      NONE,
      LABEL,
      SYMBOL
    };

    LabelSymbolData(const std::string &text1, int width1, CHAlignType align1, const CRGBA &color1) :
     type(Type::LABEL), text(text1), width(width1), align(align1), color(color1) {
    }

    explicit LabelSymbolData(CBrowserList::SymbolType symbol1) :
     type(Type::SYMBOL), symbol(symbol1) {
    }

    Type                     type { Type::NONE };
    std::string              text;
    int                      width { 0 };
    CHAlignType              align { CHALIGN_TYPE_LEFT };
    CRGBA                    color;
    CBrowserList::SymbolType symbol { CBrowserList::SymbolType::NONE };
  };

  typedef std::vector<LabelSymbolData> LabelSymbolDatas;

 public:
  explicit CBrowserListItem(CBrowserWindow *window);

  void init() override;

  void setNameValue(const std::string &name, const std::string &value) override;

  void draw(const CTextBox &region) override;

  const CBrowserList *currentList() const;

 private:
  std::string orderedListText(const std::string &symbol, int item_num) const;

  CBrowserList::SymbolType unorderedListType(const std::string &symbol, int depth) const;

 private:
  CBrowserOutputListItemData data_;
  LabelSymbolDatas           labelSymbolDatas_;
  CBrowserList::SymbolType   symbolType_ { CBrowserList::SymbolType::NONE };
  std::string                symbol_;
  int                        itemNum_ { 0 };
};

class CBrowserDataListData : public CBrowserObject {
 public:
  explicit CBrowserDataListData(CBrowserWindow *window);
};

class CBrowserDataListTerm : public CBrowserObject {
 public:
  explicit CBrowserDataListTerm(CBrowserWindow *window);
};

#endif
