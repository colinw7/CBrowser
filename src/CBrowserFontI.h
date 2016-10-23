class CBrowserFontFace {
 public:
  CBrowserFontFace(CBrowserWindow *window, const std::string &face);
 ~CBrowserFontFace();

  std::string getFace() const { return face_; }

  CFontPtr getNormalFont    (int i) { return normal_fonts_[i]; }
  CFontPtr getBoldFont      (int i) { return bold_fonts_  [i]; }
  CFontPtr getItalicFont    (int i) { return italic_fonts_[i]; }
  CFontPtr getBoldItalicFont(int i) { return boldi_fonts_ [i]; }

 private:
  void loadFonts(CBrowserWindow *window);

 private:
  std::string face_;
  CFontPtr    normal_fonts_[7];
  CFontPtr    bold_fonts_  [7];
  CFontPtr    italic_fonts_[7];
  CFontPtr    boldi_fonts_ [7];
};
