#include <CBrowserHtmlI.h>

class CBrowserImage {
 private:
  CBrowserWindow                    *window_;
  CImagePtr                          image_;
  CBrowserImageAlignType             align_;
  int                                border_;
  int                                width_;
  int                                height_;
  int                                hspace_;
  int                                vspace_;
  CBrowserLink                      *link_;
  CBrowserFormatProc<CBrowserImage> *format_proc_;
  CBrowserRedrawProc<CBrowserImage> *redraw_proc_;
  CBrowserFreeProc<CBrowserImage>   *free_proc_;

 public:
  CBrowserImage(CBrowserWindow *window, const CImagePtr &image,
                CBrowserImageAlignType align, int border, int width,
                int height, int hspace, int vspace);
 ~CBrowserImage();

  void formatProc();
  void redrawProc(int *x, int *y);
  void freeProc();
};

class CBrowserNamedImage {
 private:
  static CBrowserNamedImage named_images_[];
  static uint               num_named_images_;

  const char  *name_;
  const char **data_;
  uint         num_lines_;
  CImagePtr    image_;

 public:
  CBrowserNamedImage(const char *name, const char **data, uint num_lines) :
   name_(name), data_(data), num_lines_(num_lines) {
  }

  static CImagePtr lookup(const std::string &name) {
    const char *name1 = name.c_str();

    CBrowserNamedImage *named_image = NULL;

    for (uint i = 0; i < num_named_images_; ++i)
      if (strcmp(named_images_[i].name_, name1) == 0) {
        named_image = &named_images_[i];
        break;
      }

    if (named_image == NULL)
      return CImagePtr();

    if (! named_image->image_) {
      CImageNameSrc src("CBrowserNamedImage/" + std::string(name));

      named_image->image_ = CImageMgrInst->createImage(src);

      named_image->image_->readXPM((const char **) named_image->data_,
                                   named_image->num_lines_);
    }

    return named_image->image_;
  }
};

static const char *
noimage[] = {
  "35 35 8 1",
  ". c #000000",
  "# c #ffffff",
  "a c #baa0a7",
  "b c #c0c0c0",
  "c c #808080",
  "d c #2f7e88",
  "e c #1a5f67",
  "  c none",
  "                                   ",
  "                                   ",
  "  #######################ba        ",
  "  ######################b##a       ",
  "  ##bbbbbbbbbbbbbbbbbbbbb###a      ",
  "  ##bbbbbbbbbbbbbbbbbbbbb####a     ",
  "  ##bbbbbbbbbbbbbbbbbbbbb#####a    ",
  "  ##bbbbbbbbbbbbbbbbbbbbb######a   ",
  "  ##bbbbbbbbbbb###bbaabbb######b   ",
  "  ##bbbbbbbbb#######ccabbbccc.dbc  ",
  "  ##bbbbbbbb#########ccabbbcccab.  ",
  "  ##bbbbbbbb##becc###bcaabbbaabb.  ",
  "  ##bbbbbbb###eccca###ecaabbbb##.  ",
  "  ##bbbbbbb###cccab###ecabbbbb##.  ",
  "  ##bbbbbbbbbeccabb##becaabbbb##.  ",
  "  ##bbbbbbbbbbbbbb###becabbbbb##.  ",
  "  ##bbbbbbbbbbbbb####eccaabbbb##.  ",
  "  ##bbbbbbbbbbbb####eccaabbbbb##.  ",
  "  ##bbbbbbbbbbb####eccaaabbbbb##.  ",
  "  ##bbbbbbbbbbb###eccaaabbbbbb##.  ",
  "  ##bbbbbbbbbbb###ecaaabbbbbbb##.  ",
  "  ##bbbbbbbbbbbbeeccaabbbbbbbb##.  ",
  "  ##bbbbbbbbbbbbaccaababbbbbbb##.  ",
  "  ##bbbbbbbbbbb###aaaabbbbbbbb##.  ",
  "  ##bbbbbbbbbbb###eabbbbbbbbbb##.  ",
  "  ##bbbbbbbbbbb###ecaabbbbbbbb##.  ",
  "  ##bbbbbbbbbbbbeeecababbbbbbb##.  ",
  "  ##bbbbbbbbbbbbbcccaabbbbbbbb##.  ",
  "  ##bbbbbbbbbbbbbbaaabbbbbbbbb##.  ",
  "  ##bbbbbbbbbbbbbbbbbbbbbbbbbb##.  ",
  "  ##############################.  ",
  "  ##############################.  ",
  "   c.............................  ",
  "                                   ",
  "                                   ",
};

static const char *
dir_file_image[] = {
  "20 21 8 1",
  ". c #ffffff",
  "# c #ffff00",
  "a c #c0c0c0",
  "b c #808080",
  "c c #000080",
  "d c #429aa7",
  "e c #baa0a7",
  "  c none",
  "                    ",
  "                    ",
  "                    ",
  "                    ",
  "  ccccccc           ",
  " bc#a#a#ac          ",
  " c#e#e#e#ecccccccccd",
  "c#e#e#e#e#a##a#a#aac",
  "ceaeaeaeaa##a#a#a#ec",
  "c#.#.#.#.##a#a#a#aec",
  "c#a#a#a#a#a#a#a#a#ec",
  "c##a#a#a#a#a#a#a#aec",
  "c#a#a#a#a#a#a#a#a#ec",
  "c##a#a#a#a#a#a#a#aec",
  "c#a#a#a#a#a#a#a#a#ec",
  "c##a#a#a#a#a#a#a#aec",
  "c#a#a#a#a#a#a#a#a#ec",
  "ca#e#e#e#e#e#e#e#eec",
  "c#a#a#a#a#a#a#a#a#ec",
  "ceeeeeeeeeeeeeeeeeec",
  "cccccccccccccccccccc",
};

static const char *
normal_file_image[] = {
  "20 21 7 1",
  ". c #000000",
  "# c #ffffff",
  "a c #c0c0c0",
  "b c #808080",
  "c c #000080",
  "d c #429aa7",
  "  c none",
  "  ccccccccccb       ",
  "  c#########ac      ",
  "  c#########aac     ",
  "  c#########a#ac    ",
  "  c#########a##ac   ",
  "  c#########a###ab  ",
  "  c#########dcccc.  ",
  "  c##########bbbb.  ",
  "  c###########aaac  ",
  "  c#############ac  ",
  "  c##############c  ",
  "  c##############c  ",
  "  c##############c  ",
  "  c##############c  ",
  "  c##############c  ",
  "  c##############c  ",
  "  c##############c  ",
  "  c##############c  ",
  "  c##############c  ",
  "  c##############c  ",
  "  cccccccccccccccc  "
};

static const char *
image_file_image[] = {
  " 20 21 12 1",
  ". c #000000",
  "# c #ffffff",
  "a c #c0c0c0",
  "b c #808080",
  "c c #000080",
  "d c #429aa7",
  "e c #baa0a7",
  "f c #0000ff",
  "g c #008000",
  "h c #1a5f67",
  "i c #2f7e88",
  "  c none",
  "  ccccccccccb       ",
  "  c#########ac      ",
  "  c#########aac     ",
  "  c#########a#ac    ",
  "  c#########a##ac   ",
  "  c######dgia###ab  ",
  "  c######giidcccc.  ",
  "  c######iiihhbbb.  ",
  "  c######giiihaaac  ",
  "  c####a#dgig###ac  ",
  "  c####e#########c  ",
  "  c###aea########c  ",
  "  c###eee##ffff##c  ",
  "  c##aeeea#ffff##c  ",
  "  c##eeeee#ffff##c  ",
  "  c########ffff##c  ",
  "  c##############c  ",
  "  c##############c  ",
  "  c##############c  ",
  "  c##############c  ",
  "  cccccccccccccccc  ",
};

static const char *
html_file_image[] = {
  " 20 21 9 1",
  ". c #000000",
  "# c #ffffff",
  "a c #c0c0c0",
  "b c #808080",
  "c c #000080",
  "d c #429aa7",
  "e c #baa0a7",
  "f c #1a5f67",
  "  c none",
  "aaccccccccccbaaaaaaa",
  "aac#########acaaaaaa",
  "aac#########aacaaaaa",
  "aac##eabababa#acaaaa",
  "aac#########a##acaaa",
  "aac##eb#abbaa###abaa",
  "aac#########dcccc.aa",
  "aac##ebab#baffbfb.aa",
  "aac##########aaaacaa",
  "aac##eabbab#baba#caa",
  "aac##############caa",
  "aac##ebab#bab####caa",
  "aac##############caa",
  "aac##eb#ab#ababa#caa",
  "aac##############caa",
  "aac##eabb#ba#bab#caa",
  "aac##############caa",
  "aac##eb#bab######caa",
  "aac##############caa",
  "aac##############caa",
  "aaccccccccccccccccaa",
};

static const char *
blank_file_image[] = {
  "20 21 1 1",
  "  c none",
  "                    ",
  "                    ",
  "                    ",
  "                    ",
  "                    ",
  "                    ",
  "                    ",
  "                    ",
  "                    ",
  "                    ",
  "                    ",
  "                    ",
  "                    ",
  "                    ",
  "                    ",
  "                    ",
  "                    ",
  "                    ",
  "                    ",
  "                    ",
  "                    ",
};

#define ImageLen(a) (sizeof(a)/sizeof(char *))

CBrowserNamedImage CBrowserNamedImage::named_images_[] = {
  CBrowserNamedImage("dir_file"   , dir_file_image   ,
                     ImageLen(dir_file_image   )),
  CBrowserNamedImage("html_file"  , html_file_image  ,
                     ImageLen(html_file_image  )),
  CBrowserNamedImage("image_file" , image_file_image ,
                     ImageLen(image_file_image )),
  CBrowserNamedImage("normal_file", normal_file_image,
                     ImageLen(normal_file_image)),
  CBrowserNamedImage("blank_file" , blank_file_image ,
                     ImageLen(blank_file_image )),
};

uint CBrowserNamedImage::num_named_images_ = sizeof(named_images_)/sizeof(CBrowserNamedImage);

static CBrowserImageAlignType image_align  = CBROWSER_IMAGE_ALIGN_BOTTOM;
static int                    image_border = 2;
static int                    image_width  = -1;
static int                    image_height = -1;
static int                    image_hspace = 2;
static int                    image_vspace = 2;
static std::string            image_alt    = "";

static std::vector<CBrowserImage *> draw_images_;

void
HtmlSetImageAlign(CBrowserImageAlignType align)
{
  image_align = align;
}

void
HtmlSetImageBorder(int border)
{
  image_border = border;
}

void
HtmlSetImageSize(int width, int height)
{
  image_width  = width;
  image_height = height;
}

void
HtmlSetImageSpacing(int hspace, int vspace)
{
  image_hspace = hspace;
  image_vspace = vspace;
}

void
HtmlSetImageAlt(const std::string &alt)
{
  image_alt = alt;
}

void
HtmlDrawImage(CBrowserWindow *window, const std::string &filename)
{
  if (filename.substr(0, 6) == "_html_") {
    HtmlDrawCBrowserNamedImage(window, filename.substr(6));
    return;
  }

  CImageFileSrc file(filename);

  CImagePtr image = CImageMgrInst->createImage(file);

  if (image.isValid() && image_width != -1 && image_height != -1 &&
      ((int) image->getWidth () != image_width ||
       (int) image->getHeight() != image_height)) {
    CImagePtr image1 = image->resize(image_width, image_height);

    image = image1;
  }

  if (! image.isValid()) {
    if (image_alt != "" && CBrowserMainInst->getUseAlt()) {
      HtmlDrawText(window, image_alt);

      return;
    }

    CImageNameSrc src("HtmlDrawImage/noimage");

    image = CImageMgrInst->createImage(src);

    image->readXPM((const char **) noimage, sizeof(noimage)/sizeof(char *));
  }

  if (! image)
    return;

  CBrowserImage *draw_image =
    new CBrowserImage(window, image, image_align, image_border,
                      image_width, image_height, image_hspace, image_vspace);

  draw_images_.push_back(draw_image);
}

void
HtmlDrawCBrowserNamedImage(CBrowserWindow *window, const std::string &name)
{
  CImagePtr image = CBrowserNamedImage::lookup(name);

  if (! image)
    return;

  CBrowserImage *draw_image =
    new CBrowserImage(window, image, CBROWSER_IMAGE_ALIGN_ABSBOTTOM, 0,
                      -1, -1, 0, 0);

  draw_images_.push_back(draw_image);
}

CBrowserImage::
CBrowserImage(CBrowserWindow *window, const CImagePtr &image,
              CBrowserImageAlignType align, int border, int width,
              int height, int hspace, int vspace) :
 window_(window), image_(image), align_(align), border_(border),
 width_(width), height_(height), hspace_(hspace), vspace_(vspace),
 link_(NULL)
{
  link_ = HtmlGetCurrentLink(window);

  format_proc_ = new CBrowserFormatProc<CBrowserImage>(this);
  redraw_proc_ = NULL;
  free_proc_   = new CBrowserFreeProc<CBrowserImage>(this);

  window->addCellRedrawData(format_proc_, free_proc_);
}

CBrowserImage::
~CBrowserImage()
{
  delete format_proc_;
  delete redraw_proc_;
  delete free_proc_;
}

void
CBrowserImage::
formatProc()
{
  CHtmlLayoutSubCell::newCellRight(window_->getLayoutMgr(), true);

  CHtmlLayoutSubCell *sub_cell = window_->getCurrentSubCell();

  if      (align_ == CBROWSER_IMAGE_ALIGN_LEFT)
    sub_cell->setAlign(CHALIGN_TYPE_LEFT);
  else if (align_ == CBROWSER_IMAGE_ALIGN_RIGHT)
    sub_cell->setAlign(CHALIGN_TYPE_RIGHT);
  else
    sub_cell->setAlign(CHALIGN_TYPE_NONE);

  /*-------------*/

  int hspace = std::max(hspace_, border_);
  int vspace = std::max(vspace_, border_);

  /*-------------*/

  if      (align_ == CBROWSER_IMAGE_ALIGN_TOP ||
           align_ == CBROWSER_IMAGE_ALIGN_TEXTTOP)
    window_->updateSubCellHeight(vspace, image_->getHeight() + vspace);
  else if (align_ == CBROWSER_IMAGE_ALIGN_MIDDLE ||
           align_ == CBROWSER_IMAGE_ALIGN_ABSMIDDLE)
    window_->updateSubCellHeight((image_->getHeight() + 1)/2 + vspace,
                                  image_->getHeight()     /2 + vspace);
  else
    window_->updateSubCellHeight(image_->getHeight() + vspace, vspace);

  window_->updateSubCellWidth(image_->getWidth() + 2*hspace);

  /*-------------*/

  delete redraw_proc_;

  redraw_proc_ = new CBrowserRedrawProc<CBrowserImage>(this);

  window_->addSubCellRedrawData(redraw_proc_);
}

void
CBrowserImage::
redrawProc(int *x, int *y)
{
  int hspace = std::max(hspace_, border_);
  int vspace = std::max(vspace_, border_);

  /*-------------*/

  CHtmlLayoutSubCell *sub_cell = window_->getCurrentSubCell();

  int x1 = *x + hspace;
  int y1 = *y + vspace;

  if      (align_ == CBROWSER_IMAGE_ALIGN_TOP   )
    y1 += sub_cell->getAscent();
  else if (align_ == CBROWSER_IMAGE_ALIGN_MIDDLE)
    y1 += sub_cell->getAscent() - image_->getHeight()/2;
  else if (align_ == CBROWSER_IMAGE_ALIGN_ABSMIDDLE)
    y1 += (sub_cell->getHeight() - image_->getHeight())/2;
  else if (align_ == CBROWSER_IMAGE_ALIGN_ABSBOTTOM)
    y1 += sub_cell->getDescent();
  else
    y1 += 0;

  window_->drawImage(x1, y1, image_);

  if (link_ != NULL) {
    if (link_->isSource()) {
      CRGBA color = window_->getDocument()->getLinkColor();

      window_->setForeground(color);

      for (int i = 0; i < border_; i++)
        window_->drawRectangle(x1 - i - 1, y1 - i - 1,
                              image_->getWidth()  + 2*i + 1,
                              image_->getHeight() + 2*i + 1);
    }

    int x2 = x1 + image_->getWidth();
    int y2 = y1 + image_->getHeight();

    link_->addRect(x1, y1, x2, y2);
  }

  *x += image_->getWidth() + 2*hspace;
}

void
CBrowserImage::
freeProc()
{
  delete this;
}
