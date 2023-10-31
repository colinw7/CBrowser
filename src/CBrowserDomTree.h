#ifndef CBrowserDomTree_H
#define CBrowserDomTree_H

#include <QTreeView>

class CBrowserDomTree;
class CBrowserObjProp;
class CBrowserWindowIFace;
class QTextEdit;

class CBrowserDomTreeDlg : public QWidget {
  Q_OBJECT

 public:
  explicit CBrowserDomTreeDlg(CBrowserWindowIFace *window);

  CBrowserWindowIFace *window() const;

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
  CBrowserDomTree(CBrowserDomTreeDlg *dlg, CBrowserWindowIFace *window);

  CBrowserWindowIFace *window() const { return window_; }

 private slots:
  void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected) override;

 private:
  CBrowserDomTreeDlg*  dlg_    { nullptr };
  CBrowserWindowIFace* window_ { nullptr };
};

#endif
