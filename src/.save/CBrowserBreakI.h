class CBrowserBreak {
 public:
  CBrowserBreak(CBrowserWindow *window, CHtmlLayoutClearType clear=CHTML_LAYOUT_CLEAR_NONE);
 ~CBrowserBreak();

  CBrowserWindow *getWindow() const { return window_; }

  CHtmlLayoutClearType getClear() const { return clear_; }

  void formatProc();
  void redrawProc(int *x, int *y);
  void freeProc();

 private:
  CBrowserWindow                    *window_;
  CHtmlLayoutClearType               clear_;
  CBrowserFormatProc<CBrowserBreak> *format_proc_;
  CBrowserRedrawProc<CBrowserBreak> *redraw_proc_;
  CBrowserFreeProc<CBrowserBreak>   *free_proc_;
};
