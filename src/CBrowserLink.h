#ifndef CBrowserLink_H
#define CBrowserLink_H

#include <CBrowserObject.h>
#include <CBrowserData.h>
#include <CBrowserTypes.h>

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

  void startSourceLink(const CBrowserLinkData &data);
  void startDestLink  (const CBrowserLinkData &data);
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
  enum class Type {
    SOURCE,
    DEST
  };

 public:
  CBrowserLink(Type type, const std::string &name,
               const std::string &dest, const std::string &title);
 ~CBrowserLink();

  Type        getType () const { return type_ ; }
  std::string getName () const { return name_ ; }
  std::string getDest () const { return dest_ ; }
  std::string getTitle() const { return title_; }

  int isSource() { return type_ == Type::SOURCE; }

  int getNumRects();

  CBrowserLinkRect *getRect(int i);

  void addRect(int x1, int y1, int x2, int y2);

  void deleteRects();

 private:
  typedef std::vector<CBrowserLinkRect *> LinkRects;

  Type        type_;
  std::string name_;
  std::string dest_;
  std::string title_;
  LinkRects   rects_;
};

//-----

class CBrowserLinkObj : public CBrowserObject {
 public:
  CBrowserLinkObj(CBrowserWindow *window, const CBrowserLinkData &data);
 ~CBrowserLinkObj();

  std::string propertyValue(int i) const override;

  void initProcess() override;
  void termProcess() override;

 private:
  CBrowserLinkData data_;
};

#endif
