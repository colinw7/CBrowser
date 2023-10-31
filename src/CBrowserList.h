#ifndef CBrowserList_H
#define CBrowserList_H

#include <CBrowserObject.h>
#include <CBrowserData.h>
#include <string>
#include <vector>

class CBrowserListStyleType {
 public:
  enum class Type {
    INVALID,
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
  explicit CBrowserListStyleType(const std::string &str) :
   str_(str), type_(stringToType(str_)) {
  }

  explicit CBrowserListStyleType(const Type &type=Type::INVALID) :
   type_(type) {
  }

  bool isValid() const { return type_ != Type::INVALID; }

  const Type &type() const { return type_; }

  static Type stringToType(const std::string &str) {
    std::string lstr = CStrUtil::toLower(str);

    if (lstr == "none"                ) return CBrowserListStyleType::Type::NONE;
    if (lstr == "block"               ) return CBrowserListStyleType::Type::BLOCK;
    if (lstr == "circle"              ) return CBrowserListStyleType::Type::CIRCLE;
    if (lstr == "decimal"             ) return CBrowserListStyleType::Type::DECIMAL;
    if (lstr == "decimal-leading-zero") return CBrowserListStyleType::Type::DECIMAL_ZERO;
    if (lstr == "disc"                ) return CBrowserListStyleType::Type::DISC;
    if (lstr == "lower-alpha"         ) return CBrowserListStyleType::Type::LOWER_ALPHA;
    if (lstr == "lower-latin"         ) return CBrowserListStyleType::Type::LOWER_LATIN;
    if (lstr == "lower-roman"         ) return CBrowserListStyleType::Type::LOWER_ROMAN;
    if (lstr == "square"              ) return CBrowserListStyleType::Type::SQUARE;
    if (lstr == "upper-alpha"         ) return CBrowserListStyleType::Type::UPPER_ALPHA;
    if (lstr == "upper-latin"         ) return CBrowserListStyleType::Type::UPPER_LATIN;
    if (lstr == "upper-roman"         ) return CBrowserListStyleType::Type::UPPER_ROMAN;
    if (lstr == "initial"             ) return CBrowserListStyleType::Type::INITIAL;
    if (lstr == "inherit"             ) return CBrowserListStyleType::Type::INHERIT;

    return CBrowserListStyleType::Type::INVALID;
  }

 private:
  std::string str_;
  Type        type_ { Type::INVALID };
};

//------

class CBrowserList : public CBrowserObject {
 public:
  CBrowserList(CBrowserWindowIFace *window, CHtmlTagId id);

  const std::string &getSymbol() const { return data_.symbol; }
  void setSymbol(const std::string &symbol) { data_.symbol = symbol; }

  int  getItemNum() const { return data_.item_num; }
  void setItemNum(int item_num) { data_.item_num = item_num; }

  bool getCompact() const { return data_.compact; }
  void setCompact(bool compact) { data_.compact = compact; }

  const CBrowserListStyleType &styleType() const { return styleType_; }
  void setStyleType(const CBrowserListStyleType &v) { styleType_ = v; }

  int indent() const { return indent_; }
  void setIndent(int i) { indent_ = i; }

  void setNameValue(const std::string &name, const std::string &value) override;

  void init() override;

  int listDepth() const;

 private:
  CBrowserOutputListData data_;
  CBrowserListStyleType  styleType_;
  int                    indent_ { 0 };
};

#endif
