#ifndef CBrowserLayout_H
#define CBrowserLayout_H

class CBrowserWindow;
class CBrowserBox;

#include <CIBBox2D.h>
#include <vector>

class CBrowserLayout {
 public:
  explicit CBrowserLayout(CBrowserWindow *window);

  void startBox(CBrowserBox *box);
  void endBox  (CBrowserBox *box);

  CBrowserBox *currentBox() const;

  void layout(CBrowserBox *root, const CIBBox2D &bbox);

  void render(int dx=0, int dy=0);

  CBrowserBox *boxAt(const CIPoint2D &p);

 private:
  typedef std::vector<CBrowserBox *> Boxes;

  CBrowserWindow* window_ { nullptr };
  CBrowserBox*    root_ { nullptr };
  Boxes           boxes_;
};

#endif
