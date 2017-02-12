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

    explicit LabelSymbolData(CBrowserListStyleType::Type symbol1) :
     type(Type::SYMBOL), symbol(symbol1) {
    }

    Type                  type { Type::NONE };
    std::string           text;
    int                   width { 0 };
    CHAlignType           align { CHALIGN_TYPE_LEFT };
    CRGBA                 color;
    CBrowserListStyleType symbol;
  };

  typedef std::vector<LabelSymbolData> LabelSymbolDatas;

 public:
  explicit CBrowserListItem(CBrowserWindow *window);

  void init() override;

  const CBrowserListStyleType &styleType() const { return styleType_; }
  void setStyleType(const CBrowserListStyleType &v) { styleType_ = v; }

  void setNameValue(const std::string &name, const std::string &value) override;

  void draw(const CTextBox &region) override;

  const CBrowserList *currentList() const;

  CBrowserListStyleType calcStyleStype() const;

 private:
  std::string orderedListText(const std::string &symbol, int item_num) const;

  CBrowserListStyleType unorderedListType(const std::string &symbol, int depth) const;

 private:
  CBrowserOutputListItemData data_;
  LabelSymbolDatas           labelSymbolDatas_;
  CBrowserListStyleType      styleType_;
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
