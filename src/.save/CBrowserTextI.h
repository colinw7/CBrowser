extern void  HtmlSetTextColor
              (const CRGBA &);
extern void  HtmlSetTextUnderline
              (bool flag);
extern void  HtmlSetTextStrike
              (bool flag);
extern void  HtmlSetTextSubscript
              (bool flag);
extern void  HtmlSetTextSuperscript
              (bool flag);
extern void  HtmlSetTextBreakup
              (bool breakup);
extern void  HtmlSetTextFormat
              (bool flag);
extern void  HtmlDrawText
              (CBrowserWindow *, const std::string &text);
extern void  HtmlDrawLabel
              (CBrowserWindow *, const std::string &text, int width, CHAlignType align);
extern void  HtmlGetTextSize
              (CBrowserWindow *window, const std::string &text, int *width,
               int *ascent, int *descent);
