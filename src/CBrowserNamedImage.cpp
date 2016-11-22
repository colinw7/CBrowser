#include <CBrowserNamedImage.h>
#include <CBrowserImage.h>
#include <CBrowserPixmaps.h>
#include <cstring>

//------

#define ImageLen(a) (sizeof(a)/sizeof(char *))

CBrowserNamedImage CBrowserNamedImage::named_images_[] = {
  CBrowserNamedImage("dir_file"   , dir_file_image   , ImageLen(dir_file_image   )),
  CBrowserNamedImage("html_file"  , html_file_image  , ImageLen(html_file_image  )),
  CBrowserNamedImage("image_file" , image_file_image , ImageLen(image_file_image )),
  CBrowserNamedImage("normal_file", normal_file_image, ImageLen(normal_file_image)),
  CBrowserNamedImage("blank_file" , blank_file_image , ImageLen(blank_file_image )),
};

uint CBrowserNamedImage::num_named_images_ = sizeof(named_images_)/sizeof(CBrowserNamedImage);

//------

CBrowserNamedImage::
CBrowserNamedImage(const char *name, const char **data, uint num_lines) :
 name_(name), data_(data), num_lines_(num_lines)
{
}

CImagePtr
CBrowserNamedImage::
lookup(const std::string &name)
{
  const char *name1 = name.c_str();

  CBrowserNamedImage *named_image = nullptr;

  for (uint i = 0; i < num_named_images_; ++i)
    if (strcmp(named_images_[i].name_, name1) == 0) {
      named_image = &named_images_[i];
      break;
    }

  if (! named_image)
    return CImagePtr();

  if (! named_image->image_) {
    CImageNameSrc src("CBrowserNamedImage/" + std::string(name));

    named_image->image_ = CImageMgrInst->createImage(src);

    named_image->image_->readXPM((const char **) named_image->data_, named_image->num_lines_);
  }

  return named_image->image_;
}

CImagePtr
CBrowserNamedImage::
genNoImage()
{
  CImageNameSrc src("CBrowserImage/noimage");

  CImagePtr image = CImageMgrInst->createImage(src);

  image->readXPM((const char **) noimage, sizeof(noimage)/sizeof(char *));

  return image;
}
