#include <CBrowserHtmlI.h>

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
  normal_fonts_[0] = CFontMgrInst->lookupFont("helvetica", CFONT_STYLE_NORMAL     , 12);
  bold_fonts_  [0] = CFontMgrInst->lookupFont("helvetica", CFONT_STYLE_BOLD       , 12);
  italic_fonts_[0] = CFontMgrInst->lookupFont("helvetica", CFONT_STYLE_ITALIC     , 12);
  boldi_fonts_ [0] = CFontMgrInst->lookupFont("helvetica", CFONT_STYLE_BOLD_ITALIC, 12);

  for (int i = 1; i < 7; ++i) {
    normal_fonts_[i] = normal_fonts_[0];
    bold_fonts_  [i] = bold_fonts_  [0];
    italic_fonts_[i] = italic_fonts_[0];
    boldi_fonts_ [i] = boldi_fonts_ [0];
  }
}
