#ifndef CBrowserObjProp_H
#define CBrowserObjProp_H

#include <QTreeView>

class CBrowserWindowIFace;
class CBrowserObject;

class CBrowserObjProp : public QTreeView {
  Q_OBJECT

 public:
  CBrowserObjProp(QWidget *parent, CBrowserWindowIFace *window);

  CBrowserWindowIFace *window() const { return window_; }

  CBrowserObject *obj() const { return obj_; }

  void setObject(CBrowserObject *obj);

 private slots:
  void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected) override;

 private:
  CBrowserWindowIFace* window_ { nullptr };
  CBrowserObject*      obj_    { nullptr };
};

#endif
