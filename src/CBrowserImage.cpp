#include <CBrowserImage.h>
#include <CBrowserNamedImage.h>
#include <CBrowserMain.h>
#include <CBrowserText.h>
#include <CBrowserLink.h>
#include <CBrowserWindow.h>
#include <CBrowserDocument.h>
#include <CBrowserProperty.h>
#include <cstring>

CBrowserImage::
CBrowserImage(CBrowserWindow *window, const CImagePtr &image, const CBrowserImageData &data) :
 CBrowserObject(window, CHtmlTagId::IMG), image_(image), data_(data)
{
  std::vector<std::string> strs = {{
    "src", "align", "border", "width", "height", "usemap", "hspace", "vspace", "alt" }};

  setProperties(strs);

  //---

  link_ = window->linkMgr()->getCurrentLink();
}

CBrowserImage::
~CBrowserImage()
{
}

void
CBrowserImage::
initLayout()
{
  window_->addCellRedrawData(this);
}

void
CBrowserImage::
termLayout()
{
}

std::string
CBrowserImage::
propertyValue(int i) const
{
  if      (i == 0) return CBrowserProperty::toString(data_.src);
  else if (i == 1) return CBrowserProperty::toString(data_.align);
  else if (i == 2) return CBrowserProperty::toString(data_.border);
  else if (i == 3) return CBrowserProperty::toString(data_.width);
  else if (i == 4) return CBrowserProperty::toString(data_.height);
  else if (i == 5) return CBrowserProperty::toString(data_.usemap);
  else if (i == 6) return CBrowserProperty::toString(data_.hspace);
  else if (i == 7) return CBrowserProperty::toString(data_.vspace);
  else if (i == 8) return CBrowserProperty::toString(data_.alt);

  return "";
}

void
CBrowserImage::
format(CHtmlLayoutMgr *)
{
  window_->newSubCellRight(true);

  CHtmlLayoutSubCell *sub_cell = window_->getCurrentSubCell();

  if      (data_.align == CBrowserImageAlign::LEFT)
    sub_cell->setAlign(CHALIGN_TYPE_LEFT);
  else if (data_.align == CBrowserImageAlign::RIGHT)
    sub_cell->setAlign(CHALIGN_TYPE_RIGHT);
  else
    sub_cell->setAlign(CHALIGN_TYPE_NONE);

  //---

  int hspace = std::max(data_.hspace, data_.border);
  int vspace = std::max(data_.vspace, data_.border);

  //---

  if      (data_.align == CBrowserImageAlign::TOP || data_.align == CBrowserImageAlign::TEXTTOP)
    window_->updateSubCellHeight(vspace, image_->getHeight() + vspace);
  else if (data_.align == CBrowserImageAlign::MIDDLE ||
           data_.align == CBrowserImageAlign::ABSMIDDLE)
    window_->updateSubCellHeight((image_->getHeight() + 1)/2 + vspace,
                                  image_->getHeight()     /2 + vspace);
  else
    window_->updateSubCellHeight(image_->getHeight() + vspace, vspace);

  window_->updateSubCellWidth(image_->getWidth() + 2*hspace);

  //---

  window_->addSubCellRedrawData(this);
}

void
CBrowserImage::
draw(CHtmlLayoutMgr *, const CHtmlLayoutRegion &region)
{
  int hspace = std::max(data_.hspace, data_.border);
  int vspace = std::max(data_.vspace, data_.border);

  /*-------------*/

  CHtmlLayoutSubCell *sub_cell = window_->getCurrentSubCell();

  int x1 = region.x + hspace;
  int y1 = region.y + vspace;

  if      (data_.align == CBrowserImageAlign::TOP   )
    y1 += sub_cell->getAscent();
  else if (data_.align == CBrowserImageAlign::MIDDLE)
    y1 += sub_cell->getAscent() - image_->getHeight()/2;
  else if (data_.align == CBrowserImageAlign::ABSMIDDLE)
    y1 += (sub_cell->getHeight() - image_->getHeight())/2;
  else if (data_.align == CBrowserImageAlign::ABSBOTTOM)
    y1 += sub_cell->getDescent();
  else
    y1 += 0;

  window_->drawImage(x1, y1, image_);

  if (link_) {
    if (link_->isSource()) {
      CRGBA color = window_->getDocument()->getLinkColor();

      window_->setForeground(color);

      for (int i = 0; i < data_.border; i++)
        window_->drawRectangle(x1 - i - 1, y1 - i - 1,
                              image_->getWidth()  + 2*i + 1,
                              image_->getHeight() + 2*i + 1);
    }

    int x2 = x1 + image_->getWidth();
    int y2 = y1 + image_->getHeight();

    link_->addRect(x1, y1, x2, y2);
  }

  //region.x += image_->getWidth() + 2*hspace;

  //---

  if (isSelected())
    window_->drawSelected(region.getX(), region.getY(), region.getWidth(), region.getHeight());
}
