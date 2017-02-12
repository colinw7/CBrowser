#ifndef CBrowserDomTree_H
#define CBrowserDomTree_H

#include <QTreeView>

class CBrowserDomTree;
class CBrowserObjProp;
class CBrowserWindow;
class QTextEdit;

class CBrowserDomTreeDlg : public QWidget {
  Q_OBJECT

 public:
  explicit CBrowserDomTreeDlg(CBrowserWindow *window);

  CBrowserWindow *window() const;

  CBrowserDomTree* domTree() const { return domTree_; }
  CBrowserObjProp* prop   () const { return prop_; }
  QTextEdit*       text   () const { return text_; }

 private:
  CBrowserDomTree* domTree_ { nullptr };
  CBrowserObjProp* prop_    { nullptr };
  QTextEdit*       text_    { nullptr };
};

//---

class CBrowserDomTree : public QTreeView {
  Q_OBJECT

 public:
  CBrowserDomTree(CBrowserDomTreeDlg *dlg, CBrowserWindow *window);

  CBrowserWindow *window() const { return window_; }

 private slots:
  void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);

 private:
  CBrowserDomTreeDlg *dlg_ { nullptr };
  CBrowserWindow     *window_ { nullptr };
};

#endif
