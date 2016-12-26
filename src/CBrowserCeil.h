#ifndef CBrowserCeil_H
#define CBrowserCeil_H

#include <CBrowserTypes.h>

class CTempFile;

#define CBrowserCeilInst CBrowserCeil::instance()

class CBrowserCeil {
 public:
  static CBrowserCeil *instance();

  CTempFile *outputDocumentFile() const { return outputDocumentFile_; }
  void setOutputDocumentFile(CTempFile *file) { outputDocumentFile_ = file; }

  CTempFile *initOutputDocumentFile();
  void deleteOutputDocumentFile();

  void        runScriptFile(CBrowserWindow *window, const std::string &);
  void        runScript(CBrowserWindow *window, const std::string &);
  std::string runScriptCommand(CBrowserWindow *window, const std::string &);

 private:
  CBrowserCeil();

 private:
  CTempFile *outputDocumentFile_ { nullptr };
};

#endif
