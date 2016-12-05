#ifndef CBrowserDomTree_H
#define CBrowserDomTree_H

#include <QTreeView>

class CBrowserDomTree;
class CBrowserWindow;

class CBrowserDomTreeDlg : public QWidget {
  Q_OBJECT

 public:
  CBrowserDomTreeDlg(CBrowserWindow *window);

 private:
  CBrowserDomTree *tree_ { nullptr };
};

//---

class CBrowserDomTree : public QTreeView {
  Q_OBJECT

 public:
  CBrowserDomTree(QWidget *parent, CBrowserWindow *window);

  CBrowserWindow *window() const { return window_; }

 private slots:
  void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);

 private:
  CBrowserWindow *window_ { nullptr };
};

#endif
