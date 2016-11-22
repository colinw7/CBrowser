#ifndef CBrowserFile_H
#define CBrowserFile_H

#include <CBrowserTypes.h>
#include <CUrl.h>

class CHtmlParserTokens;

extern bool HtmlReadURL
             (CBrowserWindow *window, const std::string &url, CHtmlParserTokens &tokens);
extern bool HtmlReadHttp
             (CBrowserWindow *window, const std::string &site, const std::string &file,
              const std::string &target, const CUrl::SearchList &searches,
              CHtmlParserTokens &tokens);
extern bool HtmlReadScript
             (CBrowserWindow *window, const std::string &filename, CHtmlParserTokens &tokens);
extern bool HtmlReadFile
             (const std::string &filename, CHtmlParserTokens &tokens);
extern bool HtmlReadDirectory
             (const std::string &dirname, CHtmlParserTokens &tokens);
extern bool HtmlReadImageFile
             (const std::string &filename, CHtmlParserTokens &tokens);
extern bool HtmlReadTextFile
             (const std::string &filename, CHtmlParserTokens &tokens);
extern bool HtmlReadBinaryFile
             (const std::string &filename, CHtmlParserTokens &tokens);
extern bool HtmlReadScriptFile
             (const std::string &filename, CHtmlParserTokens &tokens);
extern bool HtmlReadHTMLString
             (const std::string &str, CHtmlParserTokens &tokens);
extern bool HtmlReadHTMLFile
             (const std::string &filename, CHtmlParserTokens &tokens);

#endif
