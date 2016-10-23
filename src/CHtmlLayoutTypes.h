#ifndef CHTML_LAYOUT_TYPES_H
#define CHTML_LAYOUT_TYPES_H

enum CHtmlLayoutClearType {
  CHTML_LAYOUT_CLEAR_NONE,
  CHTML_LAYOUT_CLEAR_LEFT,
  CHTML_LAYOUT_CLEAR_RIGHT,
  CHTML_LAYOUT_CLEAR_ALL
};

class CHtmlLayoutMgr;

class CHtmlLayoutCellFormatProc {
 public:
  CHtmlLayoutCellFormatProc() { }

  virtual ~CHtmlLayoutCellFormatProc() { }

  virtual void execute(CHtmlLayoutMgr *layout) = 0;
};

class CHtmlLayoutCellRedrawProc {
 public:
  CHtmlLayoutCellRedrawProc() { }

  virtual ~CHtmlLayoutCellRedrawProc() { }

  virtual void execute(CHtmlLayoutMgr *layout, int *x, int *y) = 0;
};

class CHtmlLayoutCellFreeProc {
 public:
  CHtmlLayoutCellFreeProc() { }

  virtual ~CHtmlLayoutCellFreeProc() { }

  virtual void execute() = 0;
};

#endif
