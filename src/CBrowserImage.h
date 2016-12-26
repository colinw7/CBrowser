#ifndef CBrowserImage_H
#define CBrowserImage_H

#include <CBrowserObject.h>
#include <CBrowserData.h>
#include <CImageLib.h>

class CBrowserImage : public CBrowserObject {
 public:
  CBrowserImage(CBrowserWindow *window, const CImagePtr &image,
                const CBrowserImageData &data);
 ~CBrowserImage();

  std::string propertyValue(int i) const override;

  void initLayout() override;
  void termLayout() override;

  void format(CHtmlLayoutMgr *) override;

  void draw(CHtmlLayoutMgr *, const CHtmlLayoutRegion &) override;

 private:
  CImagePtr         image_;
  CBrowserImageData data_;
  CBrowserLink*     link_ { nullptr };
};

#endif
