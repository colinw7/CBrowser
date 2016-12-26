#ifndef CBrowserList_H
#define CBrowserList_H

#include <CBrowserObject.h>
#include <CBrowserData.h>
#include <CBrowserSymbol.h>

class CBrowserOutputList;

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

    LabelSymbolData(CBrowserSymbol::Type symbol1) :
     type(Type::SYMBOL), symbol(symbol1) {
    }

    Type                 type { Type::NONE };
    std::string          text;
    int                  width { 0 };
    CHAlignType          align { CHALIGN_TYPE_LEFT };
    CRGBA                color;
    CBrowserSymbol::Type symbol { CBrowserSymbol::Type::NONE };
  };

  typedef std::vector<LabelSymbolData> LabelSymbolDatas;

 public:
  CBrowserListItem(CBrowserWindow *window, const CBrowserOutputListItemData &data);

  void init();

  void addLabelSymbols();

  void initLayout() override;
  void termLayout() override;

  const CBrowserOutputList *currentList() const;

 private:
  CBrowserOutputListItemData data_;
  LabelSymbolDatas           labelSymbolDatas_;
};

class CBrowserDataListData : public CBrowserObject {
 public:
  CBrowserDataListData(CBrowserWindow *window);

  void initLayout() override;
  void termLayout() override;
};

class CBrowserDataListTerm : public CBrowserObject {
 public:
  CBrowserDataListTerm(CBrowserWindow *window);

  void initProcess() override;
  void termProcess() override;
};

#endif
