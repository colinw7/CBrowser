#ifndef CBrowserImage_H
#define CBrowserImage_H

#include <CBrowserObject.h>
#include <CImageLib.h>

class CBrowserImage : public CBrowserObject {
 public:
  CBrowserImage(CBrowserWindow *window, const CImagePtr &image,
                CBrowserImageAlign align, int border, int width,
                int height, int hspace, int vspace);
 ~CBrowserImage();

  void format(CHtmlLayoutMgr *) override;

  void draw(CHtmlLayoutMgr *, const CHtmlLayoutRegion &) override;

 private:
  CBrowserWindow*    window_ { nullptr };
  CImagePtr          image_;
  CBrowserImageAlign align_  { CBrowserImageAlign::BOTTOM };
  int                border_ { 0 };
  int                width_  { 0 };
  int                height_ { 0 };
  int                hspace_ { 0 };
  int                vspace_ { 0 };
  CBrowserLink*      link_   { nullptr };
};

#endif
