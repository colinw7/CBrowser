#include <CBrowserFont.h>
#include <CBrowserWindow.h>
#include <CBrowserBaseFont.h>

#include <CFontMgr.h>

CBrowserFontFace::
CBrowserFontFace(CBrowserWindow *window, const std::string &face) :
 face_(face)
{
  loadFonts(window);
}

CBrowserFontFace::
~CBrowserFontFace()
{
}

void
CBrowserFontFace::
loadFonts(CBrowserWindow *)
{
  int size = 12;

  normal_fonts_[0] = CFontMgrInst->lookupFont(face_, CFONT_STYLE_NORMAL     , size);
  bold_fonts_  [0] = CFontMgrInst->lookupFont(face_, CFONT_STYLE_BOLD       , size);
  italic_fonts_[0] = CFontMgrInst->lookupFont(face_, CFONT_STYLE_ITALIC     , size);
  boldi_fonts_ [0] = CFontMgrInst->lookupFont(face_, CFONT_STYLE_BOLD_ITALIC, size);

  for (int i = 1; i < 7; ++i) {
    int size1 = size + i;

    normal_fonts_[i] = CFontMgrInst->lookupFont(face_, CFONT_STYLE_NORMAL     , size1);
    bold_fonts_  [i] = CFontMgrInst->lookupFont(face_, CFONT_STYLE_BOLD       , size1);
    italic_fonts_[i] = CFontMgrInst->lookupFont(face_, CFONT_STYLE_ITALIC     , size1);
    boldi_fonts_ [i] = CFontMgrInst->lookupFont(face_, CFONT_STYLE_BOLD_ITALIC, size1);

    //normal_fonts_[i] = normal_fonts_[0];
    //bold_fonts_  [i] = bold_fonts_  [0];
    //italic_fonts_[i] = italic_fonts_[0];
    //boldi_fonts_ [i] = boldi_fonts_ [0];
  }
}

//---

CFontPtr
CBrowserFont::
font(const CBrowserObject *obj) const
{
  std::string family = fontFamily(obj);

  int isize = fontSize(obj);

  uint style = CFONT_STYLE_NORMAL;

  updateFontStyle(obj, style);

  CFontPtr font = CFontMgrInst->lookupFont(family, (CFontStyle) style, isize);

  return font;
}

std::string
CBrowserFont::
fontFamily(const CBrowserObject *obj) const
{
  CBrowserFontFamily family = (obj ? obj->font().family_ : family_);

  if (family.isValid()) {
    if (family.isMonospace())
      return "courier";
    else if (family.isSerif())
      return "times";
    else if (family.isSansSerif())
      return "helvetica";
    else
      return family.value();
  }

  CBrowserObject *pobj = (obj ? obj->parent() : nullptr);

  if (pobj)
    return fontFamily(pobj);

  return "helvetica";
}

int
CBrowserFont::
fontSize(const CBrowserObject *obj) const
{
  const CBrowserBaseFont *baseFont = dynamic_cast<const CBrowserBaseFont *>(obj);

  if (baseFont)
    baseFont->apply();

  //---

  CBrowserFontSize size = (obj ? obj->font().size_ : size_);

  // inherit from parent
  if (size.type() == CBrowserFontSize::Type::INHERIT) {
    CBrowserObject *parent = (obj ? obj->parent() : 0);

    return fontSize(parent);
  }

  //---

  // get parent size for relative
  int relSize = 0;

  if (size.isRelative()) {
    CBrowserObject *parent = (obj ? obj->parent() : 0);

    relSize = fontSize(parent);
  }

  //---

  if (size.isValid())
    return size.value(relSize);

  //---

  if (obj) {
    int baseSize = obj->getWindow()->getBaseFontSize();

    if (baseSize < 0)
      baseSize = 3;

    return obj->getWindow()->sizeToFontSize(baseSize).value();
  }

  return CBrowserFontSize::defSize();
}

void
CBrowserFont::
updateFontStyle(const CBrowserObject *obj, uint &currentStyle) const
{
  CBrowserFontStyle  style  = (obj ? obj->font().style_  : style_);
  CBrowserFontWeight weight = (obj ? obj->font().weight_ : weight_);

  if (style.type() == CBrowserFontStyle::Type::ITALIC ||
      style.type() == CBrowserFontStyle::Type::OBLIQUE) {
    if (weight.value() > 500)
      currentStyle |= CFONT_STYLE_BOLD_ITALIC;
    else
      currentStyle |= CFONT_STYLE_ITALIC;
  }
  else {
    if (weight.value() > 500)
      currentStyle |= CFONT_STYLE_BOLD;
    else
      currentStyle |= CFONT_STYLE_NORMAL;
  }

  if (isUnderline())
    currentStyle |= CFONT_STYLE_UNDERLINE;

  if (isStrike())
    currentStyle |= CFONT_STYLE_STRIKEOUT;

  if (isSuperscript())
    currentStyle |= CFONT_STYLE_SUPERSCRIPT;

  if (isSubscript())
    currentStyle |= CFONT_STYLE_SUBSCRIPT;

  CBrowserObject *pobj = obj->parent();

  if (pobj)
    updateFontStyle(pobj, currentStyle);
}
