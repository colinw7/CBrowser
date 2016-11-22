#ifndef CBrowserOutput_H
#define CBrowserOutput_H

#include <CBrowserTypes.h>

class CHtmlParserTokens;
class CHtmlTag;
class CHtmlText;
class CHtmlToken;

extern void  CBrowserOutputTokens
              (CBrowserWindow *, CHtmlParserTokens &tokens);
extern void  CBrowserOutputInit
              (CBrowserWindow *);
extern void  CBrowserOutputTerm
              ();
extern void  CBrowserOutputTag
              (CBrowserWindow *window, CHtmlTag *tag);
extern void  CBrowserOutputText
              (CBrowserWindow *window, CHtmlText *text);
extern void  CBrowserOutputToken
              (CBrowserWindow *window, CHtmlToken *token);

#endif
