#ifndef CBrowserLink_H
#define CBrowserLink_H

#include <CBrowserTypes.h>

#define SOURCE_LINK 1
#define DEST_LINK   2

struct CBrowserLinkRect {
  int x1;
  int y1;
  int x2;
  int y2;
};

//------

class CBrowserLinkMgr {
 public:
  CBrowserLinkMgr(CBrowserWindow *window);

  void startSourceLink(const std::string &dest, const std::string &title);
  void startDestLink  (const std::string &name, const std::string &title);
  void endLink();

  CBrowserLink *getCurrentLink();

  void deleteLinkRects();
  void freeLinks();

  CBrowserLink *getSourceLink(int, int);

  int getDestLinkPos(const std::string &, int *, int *);

 private:
  std::string expandDestLink(const std::string &dest) const;

 private:
  CBrowserWindow *window_       { nullptr };
  CBrowserLink   *current_link_ { nullptr };
};

//------

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

#endif
