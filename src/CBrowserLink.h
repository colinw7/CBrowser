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

  CBrowserLinkRect(int x11, int y11, int x22, int y22) :
   x1(x11), y1(y11), x2(x22), y2(y22) {
  }
};

//------

class CBrowserLinkMgr {
 public:
  explicit CBrowserLinkMgr(CBrowserWindow *window);

  void startSourceLink(CBrowserAnchor *anchor);
  void startDestLink  (CBrowserAnchor *anchor);
  void endLink();

  CBrowserAnchorLink *getCurrentLink();

  void clearLinkRects();

  CBrowserAnchorLink *getSourceLink(int, int);

  int getDestLinkPos(const std::string &, int *, int *);

  std::string expandDestLink(const std::string &dest) const;

 private:
  CBrowserWindow     *window_       { nullptr };
  CBrowserAnchorLink *current_link_ { nullptr };
};

//------

class CBrowserAnchorLink {
 public:
  enum class Type {
    SOURCE,
    DEST
  };

 public:
  CBrowserAnchorLink(Type type, const std::string &name,
                     const std::string &dest, const std::string &title);
 ~CBrowserAnchorLink();

  Type        getType () const { return type_ ; }
  std::string getName () const { return name_ ; }
  std::string getDest () const { return dest_ ; }
  std::string getTitle() const { return title_; }

  int isSource() { return type_ == Type::SOURCE; }

  int getNumRects();

  const CBrowserLinkRect &getRect(int i);

  void addRect(int x1, int y1, int x2, int y2);

  void clearRects();

 private:
  typedef std::vector<CBrowserLinkRect> LinkRects;

  Type        type_;
  std::string name_;
  std::string dest_;
  std::string title_;
  LinkRects   rects_;
};

//-----

class CBrowserAnchor : public CBrowserObject {
 public:
  explicit CBrowserAnchor(CBrowserWindow *window);

 ~CBrowserAnchor();

  void init() override;

  void setNameValue(const std::string &name, const std::string &value) override;

  std::string propertyValue(int i) const override;

  const std::string &href() const { return data_.href; }

  void initProcess() override;
  void termProcess() override;

 private:
  CBrowserLinkData data_;
};

//-----

class CBrowserLink : public CBrowserObject {
 public:
  explicit CBrowserLink(CBrowserWindow *window);

 ~CBrowserLink();

  void init() override;

  void setNameValue(const std::string &name, const std::string &value) override;

  void initProcess() override;
  void termProcess() override;

 private:
  CBrowserLinkData data_;
};

#endif
