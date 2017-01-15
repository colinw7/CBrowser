#ifndef CBrowserJS_H
#define CBrowserJS_H

#include <CBrowserTypes.h>
#include <CJavaScript.h>

#define CBrowserJSInst CBrowserJS::instance()

class CBrowserJS {
 public:
  typedef std::vector<CJValueP> EventArgs;
  typedef std::set<CQJObject *> Objects;

 public:
  static CBrowserJS *instance();

  CJavaScript *js() const { return js_; }

  CJValueP jsDocument() const { return jsDocument_; }
  CJValueP jsWindow() const { return jsWindow_; }

  void addHtmlObject(CBrowserObject *obj);
  CJValueP lookupHtmlObject(CBrowserObject *htmlObj) const;

  void addJSObject(CQJObject *obj);
  void removeJSObject(CQJObject *obj);

  const Objects &objects() const { return objects_; }

  void runScript(CBrowserWindow *window, const std::string &text);
  void runScriptFile(CBrowserWindow *window, const std::string &filename);

  void onLoad();

  void callEventListeners(const std::string &name, const std::string &prop,
                          const EventArgs &args=EventArgs());

 private:
  CBrowserJS();

  void init();

 private:
  typedef std::map<CBrowserObject *, CJValueP> ObjMap;

  CJavaScript *js_ { 0 };
  CJValueP     jsDocument_;
  CJValueP     jsWindow_;
  Objects      objects_;
  ObjMap       objMap_;
};

#endif
