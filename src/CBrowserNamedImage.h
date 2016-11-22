#ifndef CBrowserNamedImage_H
#define CBrowserNamedImage_H

#include <CImageLib.h>
#include <CBrowserTypes.h>

class CBrowserNamedImage {
 public:
  CBrowserNamedImage(const char *name, const char **data, uint num_lines);

  static CImagePtr lookup(const std::string &name);

  static CImagePtr genNoImage();

 private:
  static CBrowserNamedImage named_images_[];
  static uint               num_named_images_;

  const char  *name_ { nullptr };
  const char **data_ { nullptr };
  uint         num_lines_ { 0 };
  CImagePtr    image_;
};

#endif
