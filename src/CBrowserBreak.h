#ifndef CBrowserBreak_H
#define CBrowserBreak_H

#include <CBrowserObject.h>
#include <CBrowserData.h>

class CBrowserBreak : public CBrowserObject {
 public:
  explicit CBrowserBreak(CBrowserWindowIFace *window);

 ~CBrowserBreak();

  CBrowserClear::Type getClear() const { return data_.clear; }

  void init() override;

  void setNameValue(const std::string &name, const std::string &value) override;

  void getInlineWords(Words &words) const override;

  CBrowserRegion calcRegion() const override;

  bool isBreak() const override { return true; }

 private:
  CBrowserBreakData data_;
};

//---

class CBrowserWbr : public CBrowserObject {
 public:
  explicit CBrowserWbr(CBrowserWindowIFace *window);

 ~CBrowserWbr();
};

#endif
