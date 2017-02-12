#ifndef CBrowserFile_H
#define CBrowserFile_H

#include <CBrowserTypes.h>
#include <CUrl.h>

class CHtmlParserTokens;

class CBrowserFileMgr {
 public:
  explicit CBrowserFileMgr(CBrowserWindow *window);

  bool readURL(const CUrl &url, CHtmlParserTokens &tokens);

  bool readHttp(const std::string &site, const std::string &file,
                const std::string &target, const CUrl::SearchList &searches,
                CHtmlParserTokens &tokens);

  bool readScript(const std::string &filename, CHtmlParserTokens &tokens);

  bool readFile(const std::string &filename, CHtmlParserTokens &tokens);

  bool readDirectory(const std::string &dirname, CHtmlParserTokens &tokens);

  bool readImageFile(const std::string &filename, CHtmlParserTokens &tokens);

  bool readTextFile(const std::string &filename, CHtmlParserTokens &tokens);

  bool readBinaryFile(const std::string &filename, CHtmlParserTokens &tokens);

  bool readScriptFile(const std::string &filename, CHtmlParserTokens &tokens);

  bool readHTMLString(const std::string &str, CHtmlParserTokens &tokens);

  bool readHTMLFile(const std::string &filename, CHtmlParserTokens &tokens);

 private:
  CBrowserWindow *window_ { nullptr };
};

#endif
