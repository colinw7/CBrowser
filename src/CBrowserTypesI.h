#ifndef CBROWSER_TYPES_H
#define CBROWSER_TYPES_H

#include <CHtmlLayout.h>

enum CBrowserBorderType {
  CBROWSER_BORDER_TYPE_NONE,
  CBROWSER_BORDER_TYPE_IN,
  CBROWSER_BORDER_TYPE_OUT
};

class CBrowserDocument;
class CBrowserForm;
class CBrowserHistory;
class CBrowserLink;
class CBrowserWindow;

class CBrowserLinkRect;
class CBrowserFontFace;

template<class T>
class CBrowserFormatProc : public CHtmlLayoutCellFormatProc  {
 public:
  CBrowserFormatProc(T *object) :
   object_(object) {
  }

  void execute(CHtmlLayoutMgr *) {
    object_->formatProc();
  }

 private:
  T *object_;
};

template<class T>
class CBrowserRedrawProc : public CHtmlLayoutCellRedrawProc {
 public:
  CBrowserRedrawProc(T *object) :
   object_(object) {
  }

  void execute(CHtmlLayoutMgr *, int *x, int *y) {
    object_->redrawProc(x, y);
  }

 private:
  T *object_;
};

template<class T>
class CBrowserFreeProc : public CHtmlLayoutCellFreeProc {
 public:
  CBrowserFreeProc(T *object) :
   object_(object) {
  }

  void execute() {
    object_->freeProc();
  }

 private:
  T *object_;
};

#endif
