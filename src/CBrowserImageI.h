enum CBrowserImageAlignType {
  CBROWSER_IMAGE_ALIGN_NONE,
  CBROWSER_IMAGE_ALIGN_TOP,
  CBROWSER_IMAGE_ALIGN_MIDDLE,
  CBROWSER_IMAGE_ALIGN_BOTTOM,
  CBROWSER_IMAGE_ALIGN_LEFT,
  CBROWSER_IMAGE_ALIGN_RIGHT,
  CBROWSER_IMAGE_ALIGN_TEXTTOP,
  CBROWSER_IMAGE_ALIGN_ABSMIDDLE,
  CBROWSER_IMAGE_ALIGN_ABSBOTTOM
};

extern void  HtmlSetImageAlign
              (CBrowserImageAlignType type);
extern void  HtmlSetImageBorder
              (int border);
extern void  HtmlSetImageSize
              (int width, int height);
extern void  HtmlSetImageSpacing
              (int xspace, int yspace);
extern void  HtmlSetImageAlt
              (const std::string &name);
extern void  HtmlDrawImage
              (CBrowserWindow *window, const std::string &str);
extern void  HtmlDrawCBrowserNamedImage
              (CBrowserWindow *window, const std::string &str);
