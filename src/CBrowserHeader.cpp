#include <CBrowserHeader.h>
#include <CBrowserWindow.h>
#include <CBrowserFont.h>

CBrowserHeader::
CBrowserHeader(CBrowserWindow *window, CHtmlTagId id, const CBrowserHeaderData &data) :
 CBrowserObject(window, id), data_(data), ind_(-1)
{
  static std::vector<CHtmlTagId> ids =
    {{ CHtmlTagId::H1, CHtmlTagId::H2, CHtmlTagId::H3,
       CHtmlTagId::H4, CHtmlTagId::H3, CHtmlTagId::H4 }};

  for (uint i = 0; i < ids.size(); ++i) {
    if (ids[i] != id) continue;

    ind_ = i;

    break;
  }

  font_ = window_->currentFontFace()->getBoldFont(6 - ind_);
}

CBrowserHeader::
~CBrowserHeader()
{
}

void
CBrowserHeader::
initProcess()
{
  window_->setFontSize(6 - ind_);

  window_->startBold();
}

void
CBrowserHeader::
termProcess()
{
  window_->endBold();

  window_->resetFontSize();
}

void
CBrowserHeader::
initLayout()
{
  window_->skipLine();

  window_->setAlign(data_.align, CVALIGN_TYPE_TOP);

  window_->addCellRedrawData(this);
}

void
CBrowserHeader::
termLayout()
{
  window_->skipLine();

  window_->setAlign(CHALIGN_TYPE_LEFT, CVALIGN_TYPE_TOP);
}

void
CBrowserHeader::
format(CHtmlLayoutMgr *)
{
  window_->newSubCellRight(true);

  int ascent, descent;

  window_->getTextHeight(font_, &ascent, &descent);

  window_->updateSubCellHeight(ascent, descent);

  int width;

  window_->getTextWidth(font_, text_, &width);

  window_->updateSubCellWidth(width);

  //---

  window_->addSubCellRedrawData(this);
}

void
CBrowserHeader::
draw(CHtmlLayoutMgr *, const CHtmlLayoutRegion &region)
{
  window_->setFont(font_);

  CHtmlLayoutSubCell *sub_cell = window_->getCurrentSubCell();

  int x1 = region.x;
  int y1 = region.y + sub_cell->getAscent();

  window_->drawString(x1, y1, text_);

  if (isSelected())
    window_->drawSelected(region.getX(), region.getY(), region.getWidth(), region.getHeight());
}
