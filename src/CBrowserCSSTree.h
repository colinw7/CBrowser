#ifndef CBrowserCSSTree_H
#define CBrowserCSSTree_H

#include <QTreeView>

class CBrowserWindow;
class CBrowserCSSTree;
class QComboBox;

class CBrowserCSSTreeDlg : public QWidget {
  Q_OBJECT

 public:
  explicit CBrowserCSSTreeDlg(CBrowserWindow *window);

  CBrowserWindow *window() const;

  CBrowserCSSTree* tree() const { return tree_; }

 private:
  CBrowserWindow*  window_   { nullptr };
  CBrowserCSSTree* tree_     { nullptr };
  QComboBox*       cssCombo_ { nullptr };
};

//---

class CBrowserCSSTree : public QTreeView {
  Q_OBJECT

 public:
  CBrowserCSSTree(CBrowserCSSTreeDlg *dlg, CBrowserWindow *window);

  CBrowserWindow *window() const { return window_; }

  int ind() const { return ind_; }
  void setInd(int i);

 private slots:
  void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected) override;

 private:
  CBrowserCSSTreeDlg* dlg_    { nullptr };
  CBrowserWindow*     window_ { nullptr };
  int                 ind_    { 0 };
};

#endif
