#ifndef CBrowserImage_H
#define CBrowserImage_H

#include <CBrowserObject.h>
#include <CBrowserData.h>
#include <CImageLib.h>

class CBrowserImage : public CBrowserObject {
 public:
  explicit CBrowserImage(CBrowserWindow *window);

 ~CBrowserImage();

  const CImagePtr &image() const { return image_; }
  void setImage(const CImagePtr &v) { image_ = v; }

  void init() override;

  void setNameValue(const std::string &name, const std::string &value) override;

  std::string propertyValue(int i) const override;

  void getInlineWords(Words &words) const override;

  CBrowserRegion calcRegion() const override;

  void draw(const CTextBox &) override;

 private:
  CImagePtr           image_;
  CBrowserImageData   data_;
  CBrowserAnchorLink* link_ { nullptr };
};

#endif
