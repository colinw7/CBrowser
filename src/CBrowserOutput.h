#ifndef CBrowserOutput_H
#define CBrowserOutput_H

#include <CBrowserTypes.h>

class CBrowserOutputTagBase;
class CBrowserWindowIFace;

class CHtmlParserTokens;
class CHtmlTag;
class CHtmlText;
class CHtmlToken;

class CBrowserOutput {
 public:
  explicit CBrowserOutput(CBrowserWindowIFace *window);

  void processTokens(const CHtmlParserTokens &tokens);

  void layoutObjects();

 private:
  void layoutObj(CBrowserObject *obj);

  void init();
  void term();

  void processTag (CHtmlTag *tag);
  void processText(CHtmlText *text);

  void processStartTag(CHtmlTag *tag, CBrowserOutputTagBase *output_data);
  void processEndTag  (CHtmlTag *tag, CBrowserOutputTagBase *output_data);

 private:
  CBrowserWindowIFace *window_ { nullptr };
};

#endif
