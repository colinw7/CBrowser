#ifndef CBrowserIFace_H
#define CBrowserIFace_H

#include <CUrl.h>

#include <QString>

class CBrowser;

class CBrowserIFace {
 public:
  CBrowserIFace(CBrowser *browser) :
   browser_(browser) {
  }

  virtual ~CBrowserIFace() = default;

  CBrowser *browser() const { return browser_; }

  virtual void addHistoryItem(const CUrl &) { }

  virtual void addDocument(const CUrl &) { }
  virtual void setDocument(const CUrl &) { }

  virtual void show() { }

  virtual void setBusy () { }
  virtual void setReady() { }

  virtual void updateTitles() { }

  virtual void setTitle (const QString &) { }
  virtual void setStatus(const QString &) { }

  virtual void errorDialog(const QString &) { }

  virtual void setObjText(const QString &) { }
  virtual void setPosText(const QString &) { }

 private:
  CBrowser* browser_ { nullptr };
};

#endif
