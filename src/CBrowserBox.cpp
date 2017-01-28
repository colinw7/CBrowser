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
hierMove(int dx, int dy)
{
  setX(x() + dx);
  setY(y() + dy);

  for (auto &child : children_)
    child->hierMove(dx, dy);
}

// place and size all child objects
void
CBrowserBox::
layout()
{
  if (! isVisible())
    return;

  // calc preferred height for width
  // TODO: too small for child minimum width
  CTextBox box(this->width(), 0);

  (void) calcHeightForWidth(box);

  //---

  // skip if children are ignored (e.g. canvas)
  if (! layoutChildren())
    return;

  // no need to layout of all children are inline (we flow in parent box)
  bool allInline = allChildrenInline();

  if (allInline)
    return;

  //---

  // start at top left
  int x = this->x() + this->contentX();
  int y = this->y() + this->contentY();

  // add our width if inline and not break
  if (isInline() && ! isBreak()) {
    CIBBox2D bbox = calcBBox();

    x += bbox.getWidth();
  }

  for (auto &child : children_) {
    // skip invisible
    if (! child->isVisible())
      continue;

    // update position if child is absolute positioned
    // TODO: do we start from here for next child
    const CBrowserPosition &pos = child->position();

    if (pos.type() == CBrowserPosition::Type::ABSOLUTE) {
      x = pos.left().value();
      y = pos.top ().value();
    }

    // set position
    child->setX(x);
    child->setY(y);

    // move to next line if force newline for non-inline child or break
    if (! child->isInline() || child->isBreak()) {
      x  = this->x() + this->contentX();
      y += child->height();
    }
    // place right
    else {
      x += child->width();
    }
  }

  //---

  // layout children
  int width = 0, height = 0;

  for (auto &child : children_) {
    if (! child->isVisible())
      continue;

    if (child->width() == 0)
      child->setSize(box.width(), child->height());

    child->layout();

    width  = std::max(width , child->width ());
    height = std::max(height, child->height());
  }

  if (width > this->width())
   setSize(width, this->height());

  if (height > this->height())
   setSize(this->width(), height);
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

  if (! renderChildren())
    return;

  //---

  bool allInline = allChildrenInline();

  if (! allInline) {
    //int xo = box.x() + this->contentX();
    //int yo = box.y() + this->contentY();

    for (auto &child : children_) {
      if (! child->isVisible())
        continue;

      child->render(dx, dy);
    }
  }
  else {
    Words words;

    getHierWords(words);

    renderWords(words, box);
  }
}

void
CBrowserBox::
renderWords(const Words &words, const CTextBox &box)
{
  if (hasFloatWords(words))
    return renderFloatWords(words, box);
  else
    return renderLineWords(words, box);
}

void
CBrowserBox::
renderLineWords(const Words &words, const CTextBox &box)
{
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

    //x  = 0;
    //y += line.height();
  }
}

class CBrowserBoxClear {
 public:
  CBrowserBoxClear() { }

  void addLeftPoint(const CIPoint2D &p) {
    leftPoints_.push_back(p);
  }

  void addRightPoint(const CIPoint2D &p) {
    rightPoints_.push_back(p);
  }

  int getLeftPoint(int y) {
    int x = 0;

    while (! leftPoints_.empty()) {
      const CIPoint2D &p = leftPoints_.back();

      if (y < p.y) {
        x = p.x;
        break;
      }

      leftPoints_.pop_back();
    }

    return x;
  }

  int getRightPoint(int y, int width) {
    int x = width;

    while (! rightPoints_.empty()) {
      const CIPoint2D &p = rightPoints_.back();

      if (y < p.y) {
        x = p.x;
        break;
      }

      rightPoints_.pop_back();
    }

    return x;
  }

 private:
  typedef std::vector<CIPoint2D> ClearPoints;

  ClearPoints leftPoints_;
  ClearPoints rightPoints_;
};

void
CBrowserBox::
renderFloatWords(const Words &words, const CTextBox &box)
{
  CBrowserBoxClear clear;

  CBrowserLine line;

  int width1 = content().getWidth();

  int x = 0;
  int y = 0;

  int xo = box.x() + this->contentX();
  int yo = box.y() + this->contentY();

  for (const auto &word : words) {
    if (x == 0 && word.isSpace())
      continue;

    if      (word.getFloat() == CBrowserWord::Float::LEFT) {
      // start new line if line started
      if (! line.isEmpty()) {
        x  = clear.getLeftPoint(y + yo);
        y += line.height();

        line.draw(window_, width1, halign());

        line.clear();
      }

      // add word at left
      line.addWord(x + xo, y + yo, word);

      x += word.width();

      clear.addLeftPoint(CIPoint2D(x, y + word.height()));
    }
    else if (word.getFloat() == CBrowserWord::Float::RIGHT) {
      // get right position
      int x1 = width1 - word.width();

      // if no room on right move to next line
      if (! line.isEmpty() && x > x1) {
        y += line.height();

        line.draw(window_, width1, halign());

        line.clear();
      }

      // add word at right
      line.addWord(x1 + xo, y + yo, word);

      clear.addRightPoint(CIPoint2D(x1, y + word.height()));
    }
    else if (! word.isBreakup()) {
      int right = clear.getRightPoint(y + yo, width1);

      // if word overflows line start new line
      if (! line.isEmpty() && x + word.width() > right && ! word.isSpace()) {
        x  = clear.getLeftPoint(y + yo);
        y += line.height();

        line.draw(window_, width1, halign());

        line.clear();
      }

      // add word
      line.addWord(x + xo, y + yo, word);

      x += word.width();
    }
    else { // breakup
      // add word
      line.addWord(x + xo, y + yo, word);

      // move to next line
      x  = clear.getLeftPoint(y + yo);
      y += line.height();

      line.draw(window_, width1, halign());

      line.clear();
    }
  }

  if (! line.isEmpty()) {
    line.draw(window_, width1, halign());

    //x  = 0;
    //y += line.height();
  }
}

bool
CBrowserBox::
hasFloatWords(const Words &words) const
{
  for (const auto &word : words) {
    if (word.getFloat() != CBrowserWord::Float::NONE)
      return true;
  }

  return false;
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

          if (x > width)
            width = x;
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

  if (! renderChildren())
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
