#ifndef CBrowserImage_H
#define CBrowserImage_H

#include <CBrowserObject.h>
#include <CBrowserData.h>
#include <CQJImageIFace.h>
#include <CImageLib.h>

class CBrowserImage : public CBrowserObject {
 private:
  class IFace : public CQJImageIFace {
   public:
    IFace(CBrowserImage *image) :
     image_(image) {
    }

    CImagePtr image() const override { return image_->image(); }

   private:
    CBrowserImage *image_ { nullptr };
  };

 public:
  explicit CBrowserImage(CBrowserWindowIFace *window);

 ~CBrowserImage();

  IFace *iface() { return &iface_; }

  const CImagePtr &image() const { return image_; }
  void setImage(const CImagePtr &v) { image_ = v; }

  void init() override;

  void setNameValue(const std::string &name, const std::string &value) override;

  std::string propertyValue(int i) const override;

  void getInlineWords(Words &words) const override;

  CBrowserRegion calcRegion() const override;

  void draw(const CTextBox &) override;

  CQJHtmlObj *createJObj(CJavaScript *js) override;

 private:
  IFace               iface_;
  CImagePtr           image_;
  CBrowserImageData   data_;
  CBrowserAnchorLink* link_ { nullptr };
};

#endif
