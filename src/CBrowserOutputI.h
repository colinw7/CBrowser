extern void  CBrowserOutputTokens
              (CBrowserWindow *, CHtmlParserTokens &tokens);
extern void  CBrowserOutputInit
              (void);
extern void  CBrowserOutputTerm
              (void);
extern void  CBrowserOutputTag
              (CBrowserWindow *window, CHtmlTag *tag);
extern void  CBrowserOutputText
              (CBrowserWindow *window, CHtmlText *text);
extern void  CBrowserOutputToken
              (CBrowserWindow *window, CHtmlToken *token);
