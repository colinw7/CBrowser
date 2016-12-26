#ifndef CBrowserObjProp_H
#define CBrowserObjProp_H

#include <QTreeView>

class CBrowserWindow;
class CBrowserObject;

class CBrowserObjProp : public QTreeView {
  Q_OBJECT

 public:
  CBrowserObjProp(QWidget *parent, CBrowserWindow *window);

  CBrowserWindow *window() const { return window_; }
  CBrowserObject *obj() const { return obj_; }

  void setObject(CBrowserObject *obj);

 private slots:
  void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);

 private:
  CBrowserWindow *window_ { nullptr };
  CBrowserObject *obj_ { nullptr };
};

#endif
