#ifndef CBrowserCeil_H
#define CBrowserCeil_H

#include <CBrowserTypes.h>

class CBrowser;
class CTempFile;

#define CBrowserCeilInst CBrowserCeil::instance()

class CBrowserCeil {
 public:
  static CBrowserCeil *instance();

  CBrowser *browser() const { return browser_; }
  void setBrowser(CBrowser *browser) { browser_ = browser; }

  CTempFile *outputDocumentFile() const { return outputDocumentFile_; }
  void setOutputDocumentFile(CTempFile *file) { outputDocumentFile_ = file; }

  CTempFile *initOutputDocumentFile();
  void deleteOutputDocumentFile();

  void        runScriptFile(CBrowserWindowIFace *window, const std::string &);
  void        runScript(CBrowserWindowIFace *window, const std::string &);
  std::string runScriptCommand(CBrowserWindowIFace *window, const std::string &);

 private:
  CBrowserCeil();

 private:
  CBrowser*  browser_            { nullptr };
  CTempFile* outputDocumentFile_ { nullptr };
};

#endif
