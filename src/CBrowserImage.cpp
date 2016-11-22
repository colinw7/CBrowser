#include <CBrowserImage.h>
#include <CBrowserNamedImage.h>
#include <CBrowserMain.h>
#include <CBrowserText.h>
#include <CBrowserLink.h>
#include <CBrowserWindow.h>
#include <CBrowserDocument.h>
#include <cstring>

CBrowserImage::
CBrowserImage(CBrowserWindow *window, const CImagePtr &image, CBrowserImageAlign align,
              int border, int width, int height, int hspace, int vspace) :
 CBrowserObject(Type::IMAGE), window_(window), image_(image), align_(align), border_(border),
 width_(width), height_(height), hspace_(hspace), vspace_(vspace)
{
  link_ = window->linkMgr()->getCurrentLink();
}

CBrowserImage::
~CBrowserImage()
{
}

void
CBrowserImage::
format(CHtmlLayoutMgr *)
{
  CHtmlLayoutSubCell::newCellRight(window_->getLayoutMgr(), true);

  CHtmlLayoutSubCell *sub_cell = window_->getCurrentSubCell();

  if      (align_ == CBrowserImageAlign::LEFT)
    sub_cell->setAlign(CHALIGN_TYPE_LEFT);
  else if (align_ == CBrowserImageAlign::RIGHT)
    sub_cell->setAlign(CHALIGN_TYPE_RIGHT);
  else
    sub_cell->setAlign(CHALIGN_TYPE_NONE);

  /*-------------*/

  int hspace = std::max(hspace_, border_);
  int vspace = std::max(vspace_, border_);

  /*-------------*/

  if      (align_ == CBrowserImageAlign::TOP ||
           align_ == CBrowserImageAlign::TEXTTOP)
    window_->updateSubCellHeight(vspace, image_->getHeight() + vspace);
  else if (align_ == CBrowserImageAlign::MIDDLE ||
           align_ == CBrowserImageAlign::ABSMIDDLE)
    window_->updateSubCellHeight((image_->getHeight() + 1)/2 + vspace,
                                  image_->getHeight()     /2 + vspace);
  else
    window_->updateSubCellHeight(image_->getHeight() + vspace, vspace);

  window_->updateSubCellWidth(image_->getWidth() + 2*hspace);

  /*-------------*/

  window_->addSubCellRedrawData(this);
}

void
CBrowserImage::
draw(CHtmlLayoutMgr *, const CHtmlLayoutRegion &region)
{
  int hspace = std::max(hspace_, border_);
  int vspace = std::max(vspace_, border_);

  /*-------------*/

  CHtmlLayoutSubCell *sub_cell = window_->getCurrentSubCell();

  int x1 = region.x + hspace;
  int y1 = region.y + vspace;

  if      (align_ == CBrowserImageAlign::TOP   )
    y1 += sub_cell->getAscent();
  else if (align_ == CBrowserImageAlign::MIDDLE)
    y1 += sub_cell->getAscent() - image_->getHeight()/2;
  else if (align_ == CBrowserImageAlign::ABSMIDDLE)
    y1 += (sub_cell->getHeight() - image_->getHeight())/2;
  else if (align_ == CBrowserImageAlign::ABSBOTTOM)
    y1 += sub_cell->getDescent();
  else
    y1 += 0;

  window_->drawImage(x1, y1, image_);

  if (link_) {
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

  //region.x += image_->getWidth() + 2*hspace;
}
