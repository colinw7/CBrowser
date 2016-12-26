#include <CBrowserFont.h>

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
