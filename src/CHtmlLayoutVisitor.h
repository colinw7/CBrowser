#ifndef CHtmlLayoutVisitor_H
#define CHtmlLayoutVisitor_H

class CHtmlLayoutArea;
class CHtmlLayoutCell;
class CHtmlLayoutSubCell;
class CHtmlLayoutBox;

class CHtmlLayoutVisitor {
 public:
  virtual ~CHtmlLayoutVisitor() { }

  virtual void enter(CHtmlLayoutArea *) { }
  virtual void leave(CHtmlLayoutArea *) { }

  virtual void enter(CHtmlLayoutCell *) { }
  virtual void leave(CHtmlLayoutCell *) { }

  virtual void enter(CHtmlLayoutSubCell *) { }
  virtual void leave(CHtmlLayoutSubCell *) { }

  virtual void enter(CHtmlLayoutBox *) { }
  virtual void leave(CHtmlLayoutBox *) { }
};

#endif
