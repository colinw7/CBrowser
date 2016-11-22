#define SOURCE_LINK 1
#define DEST_LINK   2

struct CBrowserLinkRect {
  int x1;
  int y1;
  int x2;
  int y2;
};

class CBrowserLink {
 public:
  CBrowserLink(int type, const std::string &name,
               const std::string &dest, const std::string &title);
 ~CBrowserLink();

  int         getType () const { return type_ ; }
  std::string getName () const { return name_ ; }
  std::string getDest () const { return dest_ ; }
  std::string getTitle() const { return title_; }

  int isSource() { return type_ == SOURCE_LINK; }

  int getNumRects();

  CBrowserLinkRect *getRect(int i);

  void addRect(int x1, int y1, int x2, int y2);

  void deleteRects();

 private:
  typedef std::vector<CBrowserLinkRect *> LinkRects;

  int         type_;
  std::string name_;
  std::string dest_;
  std::string title_;
  LinkRects   rects_;
};

extern void          HtmlStartSourceLink
                      (CBrowserWindow *, const std::string &, const std::string &);
extern void          HtmlStartDestLink
                      (CBrowserWindow *, const std::string &, const std::string &);
extern void          HtmlEndLink
                      (CBrowserWindow *);
extern CBrowserLink *HtmlGetCurrentLink
                      (CBrowserWindow *);
extern void          HtmlDeleteLinkRects
                      (CBrowserWindow *);
extern void          HtmlFreeLinks
                      (CBrowserWindow *);
extern CBrowserLink *HtmlGetSourceLink
                      (CBrowserWindow *, int, int);
extern int           HtmlGetDestLinkPos
                      (CBrowserWindow *, const std::string &, int *, int *);
