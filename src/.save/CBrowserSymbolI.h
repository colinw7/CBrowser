enum CBrowserSymbolType {
  CBROWSER_SYMBOL_TYPE_NONE,
  CBROWSER_SYMBOL_TYPE_DISC,
  CBROWSER_SYMBOL_TYPE_CIRCLE,
  CBROWSER_SYMBOL_TYPE_BLOCK,
  CBROWSER_SYMBOL_TYPE_SQUARE
};

extern void  HtmlDrawSymbol
              (CBrowserWindow *window, CBrowserSymbolType type);
