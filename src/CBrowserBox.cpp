#include <CBrowserBox.h>
#include <CBrowserWindow.h>
#include <CBrowserMain.h>
#include <CBrowserLine.h>

CBrowserBox::
CBrowserBox(CBrowserWindow *window) :
 window_(window)
{
}

void
CBrowserBox::
layout()
{
  if (! isVisible())
    return;

  CTextBox box(this->width(), 0);

  (void) calcHeightForWidth(box);

  //---

  bool allInline = allChildrenInline();

  if (allInline)
    return;

  int x = this->x() + this->contentX();
  int y = this->y() + this->contentY();

  if (isInline() && ! isBreak()) {
    CIBBox2D bbox = calcBBox();

    x += bbox.getWidth();
  }

  if (! layoutChildren())
    return;

  for (auto &child : children_) {
    if (! child->isVisible())
      continue;

    const CBrowserPosition &pos = child->position();

    if (pos.type() == CBrowserPosition::Type::ABSOLUTE) {
      x = pos.left().value();
      y = pos.top ().value();
    }

    child->setX(x);
    child->setY(y);

    if (! child->isInline() || child->isBreak()) {
      x  = this->x() + this->contentX();
      y += child->height();
    }
    else
      x += child->width();
  }

  for (auto &child : children_) {
    if (! child->isVisible())
      continue;

    child->layout();
  }
}

void
CBrowserBox::
setHierVisible(bool visible)
{
  if (visible)
    show();
  else
    hide();

  if (! layoutChildren())
    return;

  for (auto &child : children_)
    child->setHierVisible(visible);
}

void
CBrowserBox::
render(int dx, int dy)
{
  CIBBox2D cbox(0, 0, window_->getCanvasWidth(), window_->getCanvasHeight());

  if (! isVisible()) {
    setHierVisible(false);
    return;
  }

  //---

  int bx = x() + dx;
  int by = y() + dy;

  CIBBox2D bbox(bx, by, bx + width(), by + height());

  if (bbox.getYMax() < cbox.getYMin() || bbox.getYMin() > cbox.getYMax()) {
    setHierVisible(false);
    return;
  }

  //---

  if (! height())
    return;

  //---

  CTextBox box(bx, by, width(), height() - descent(), descent());

  if (CBrowserMainInst->getShowBoxes()) {
    CPen pen1(CRGBA(1,0,0));

    window_->drawRectangle(box.x(), box.y(), box.width(), box.height(), pen1);

    CPen pen2(CRGBA(0,1,0));

    window_->drawRectangle(box.x() + contentX(), box.y() + contentY(),
                           content().getWidth(), content().getHeight(), pen2);
  }

#ifdef BOX_NAME
  int twidth, ascent, descent;

  window_->getTextSize("X", &twidth, &ascent, &descent);

  window_->drawText(box.x(), box.y() + ascent, typeName(), pen, window_->getFont());
#endif

  draw(box);

  //---

  if (! layoutChildren())
    return;

  //---

  bool allInline = allChildrenInline();

  if (! allInline) {
    for (auto &child : children_) {
      if (! child->isVisible())
        continue;

      child->render(dx, dy);
    }
  }
  else {
    Words words;

    getHierWords(words);

    //---

    CBrowserLine line;

    int width1 = content().getWidth();

    int x = 0;
    int y = 0;

    int xo = box.x() + this->contentX();
    int yo = box.y() + this->contentY();

    for (const auto &word : words) {
      if (x == 0 && word.isSpace())
        continue;

      if (! word.isBreakup()) {
        if (! line.isEmpty() && x + word.width() > width1 && ! word.isSpace()) {
          line.draw(window_, width1, halign());

          x  = 0;
          y += line.height();

          line.clear();
        }

        line.addWord(x + xo, y + yo, word);

        x += word.width();
      }
      else {
        line.addWord(x + xo, y + yo, word);

        line.draw(window_, width1, halign());

        x  = 0;
        y += line.height();

        line.clear();
      }
    }

    if (! line.isEmpty()) {
      line.draw(window_, width1, halign());

      x  = 0;
      y += line.height();
    }
  }
}

void
CBrowserBox::
calcHeightForWidth(CTextBox &box)
{
  setSize(box.width(), 0);

  int width, ascent, descent;

  if (! isInline() || isBreak()) {
    width = box.width();

    CTextBox box1(width, 0);

    heightForWidth(box1);

    ascent  = box1.ascent();
    descent = box1.descent();
  }
  else {
    CIBBox2D bbox = calcBBox();

    width   = bbox.getWidth ();
    ascent  = bbox.getHeight();
    descent = 0;
  }

  int height = ascent + descent;

  //---

  if (layoutChildren()) {
    bool allInline = allChildrenInline();

    if (! allInline) {
      for (const auto &child : children_) {
        if (! child->isVisible())
          continue;

        CTextBox childBox(content().getWidth(), 0);

        child->calcHeightForWidth(childBox);

        if (! child->isInline() || child->isBreak())
          height += childBox.height();
        else
          height = std::max(height, childBox.height());

        width = std::max(width, childBox.width());
      }
    }
    else {
      Words words;

      getHierWords(words);

      //---

      int width1 = content().getWidth();

      int ascent  = 0;
      int descent = 0;

      int x = 0;
      int y = 0;

      for (const auto &word : words) {
        if (x == 0 && word.isSpace())
          continue;

        int w = word.width();

        ascent  = std::max(ascent , word.ascent ());
        descent = std::max(descent, word.descent());

        if (! word.isBreakup()) {
          if (x > 0 && x + w > width1) {
            x  = 0;
            y += ascent + descent;

            ascent  = word.ascent ();
            descent = word.descent();
          }

          x += w;
        }
        else {
          x  = 0;
          y += ascent + descent;

          ascent  = 0;
          descent = 0;
        }
      }

      if (x > 0)
        y += ascent + descent;

      height += y;
    }
  }

  height += nonContentHeight();

  //---

  setSize(width, height);

  setAscent (ascent);
  setDescent(descent);

  box.setSize(width, height);
}

void
CBrowserBox::
getHierWords(Words &words) const
{
  getInlineWords(words);

  //---

  if (! layoutChildren())
    return;

  //---

  for (const auto &child : children_) {
    if (! child->isVisible())
      continue;

    child->getHierWords(words);
  }
}

bool
CBrowserBox::
allChildrenInline() const
{
  if (children_.empty())
    return true;

  //---

  if (! layoutChildren())
    return true;

  //---

  for (const auto &child : children_) {
    if (! child->isVisible())
      continue;

    if (! child->isInline())
      return false;

    if (! child->allChildrenInline())
      return false;
  }

  return true;
}

void
CBrowserBox::
boxAt(const CIPoint2D &p, CBrowserBox* &box, double &area)
{
  if (! isVisible())
    return;

  //---

  if (! layoutChildren())
    return;

  //---

  for (const auto &child : children_)
    child->boxAt(p, box, area);

  if (! content_.isSet())
    return;

  CIBBox2D bbox(x(), y(), x() + contentWidth(), y() + contentHeight());

  if (bbox.inside(p)) {
    double area1 = bbox.area();

    if (! box || area1 < area) {
      area = area1;
      box  = this;
    }
  }
}
