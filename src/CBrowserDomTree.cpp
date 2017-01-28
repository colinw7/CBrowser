#include <CBrowserDomTree.h>
#include <CBrowserObjProp.h>
#include <CBrowserWindow.h>
#include <CBrowserText.h>
#include <CBrowserObject.h>

#include <QSplitter>
#include <QHeaderView>
#include <QAbstractItemModel>
#include <QStyledItemDelegate>
#include <QVBoxLayout>

class CBrowserDomTreeModel : public QAbstractItemModel {
 public:
  explicit CBrowserDomTreeModel(CBrowserDomTree *tree);

  QModelIndex index(int row, int column, const QModelIndex &) const;

  QModelIndex parent(const QModelIndex &index) const;

  int rowCount(const QModelIndex &parent) const;

  int columnCount(const QModelIndex &parent) const;

  QVariant headerData(int section, Qt::Orientation orientation, int role) const;

  QVariant data(const QModelIndex &index, int role) const;

 private:
  CBrowserDomTree *tree_ { nullptr };
};

//---

class CBrowserDomTreeDelegate : public QStyledItemDelegate {
 public:
  explicit CBrowserDomTreeDelegate(CBrowserDomTree *tree);

  void paint(QPainter *painter, const QStyleOptionViewItem &option,
             const QModelIndex &index) const;

  QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;

 private:
  CBrowserDomTree *tree_ { nullptr };
};

//---

CBrowserDomTreeDlg::
CBrowserDomTreeDlg(CBrowserWindow *window) :
 QWidget()
{
  QVBoxLayout *layout = new QVBoxLayout(this);

  QTabWidget *tab = new QTabWidget;

  layout->addWidget(tab);

  //---

  QSplitter *splitter = new QSplitter(this);
  splitter->setObjectName("splitter");

  domTree_ = new CBrowserDomTree(this, window);
  prop_    = new CBrowserObjProp(this, window);

  splitter->addWidget(domTree_);
  splitter->addWidget(prop_);

  layout->addWidget(splitter);

  //---

  tab->addTab(splitter, "DOM");
}

CBrowserWindow *
CBrowserDomTreeDlg::
window() const
{
  return domTree_->window();
}

//---

CBrowserDomTree::
CBrowserDomTree(CBrowserDomTreeDlg *dlg, CBrowserWindow *window) :
 QTreeView(nullptr), dlg_(dlg), window_(window)
{
  setObjectName("domTree");

  setUniformRowHeights(true);

  header()->setStretchLastSection(true);

  setAllColumnsShowFocus(true);
  setAlternatingRowColors(true);

  // create simple model for a tree view
  CBrowserDomTreeModel *model = new CBrowserDomTreeModel(this);

  // create custom delegate
  CBrowserDomTreeDelegate* delegate = new CBrowserDomTreeDelegate(this);

  // set model and delegate to the treeview object
  setModel(model);

  setItemDelegate(delegate);
}

void
CBrowserDomTree::
selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
  for (int i = 0; i < selected.indexes().length(); ++i) {
    const QModelIndex &ind = selected.indexes()[i];

    CBrowserObject *obj = static_cast<CBrowserObject *>(ind.internalPointer());
    if (! obj) continue;

    obj->setSelected(true);

    dlg_->prop()->setObject(obj);
  }

  for (int i = 0; i < deselected.indexes().length(); ++i) {
    const QModelIndex &ind = deselected.indexes()[i];

    CBrowserObject *obj = static_cast<CBrowserObject *>(ind.internalPointer());
    if (! obj) continue;

    obj->setSelected(false);
  }

  window_->redraw();
}

//---

CBrowserDomTreeModel::
CBrowserDomTreeModel(CBrowserDomTree *tree) :
 QAbstractItemModel(0), tree_(tree)
{
}

int
CBrowserDomTreeModel::
rowCount(const QModelIndex &parent) const
{
  CBrowserWindow *window = tree_->window();

  const CBrowserObject *obj { nullptr };

  if (parent.internalPointer())
    obj = static_cast<CBrowserObject *>(parent.internalPointer());
  else
    obj = window->rootObject();

  if (! obj)
    return 0;

  return obj->numChildren();
}

int
CBrowserDomTreeModel::
columnCount(const QModelIndex &) const
{
  return 2;
}

QModelIndex
CBrowserDomTreeModel::
index(int row, int column, const QModelIndex &parent) const
{
  CBrowserWindow *window = tree_->window();

  const CBrowserObject *obj { nullptr };

  if (parent.internalPointer())
    obj = static_cast<CBrowserObject *>(parent.internalPointer());
  else
    obj = window->rootObject();

  if (! obj)
    return QModelIndex();

  CBrowserObject *child = obj->child(row);
  if (! child) return QModelIndex();

  return createIndex(row, column, child);
}

QModelIndex
CBrowserDomTreeModel::
parent(const QModelIndex &index) const
{
  if (! index.isValid())
    return QModelIndex();

  CBrowserWindow *window = tree_->window();

  CBrowserObject *obj { nullptr };

  if (index.internalPointer())
    obj = static_cast<CBrowserObject *>(index.internalPointer());
  else
    return QModelIndex();

  if (! obj)
    return QModelIndex();

  const CBrowserObject *parent = obj->parent();

  if (! parent || parent == window->rootObject())
    return QModelIndex();

  const CBrowserObject *parentParent = parent->parent();
  if (! parentParent) return QModelIndex();

  int parentRow = parentParent->childIndex(parent);
  if (parentRow < 0) return QModelIndex();

  return createIndex(parentRow, 0, const_cast<CBrowserObject *>(parent));
}

QVariant
CBrowserDomTreeModel::
headerData(int section, Qt::Orientation, int role) const
{
  if (role != Qt::DisplayRole)
    return QVariant();

  if      (section == 0)
    return "Id";
  else if (section == 1)
    return "Text";

  return QVariant();
}

QVariant
CBrowserDomTreeModel::
data(const QModelIndex &index, int role) const
{
  if (! index.isValid())
    return QVariant();

  //CBrowserWindow *window = tree_->window();

  if      (role == Qt::DisplayRole) {
    CBrowserObject *obj = static_cast<CBrowserObject *>(index.internalPointer());
    if (! obj) return QVariant();

    std::string name;

    if      (index.column() == 0) {
      name = obj->typeName();

      if (obj->id() != "")
        name = name + ":" + obj->id();
    }
    else if (index.column() == 1) {
      if (obj->type() == CHtmlTagId::TEXT)
        name = dynamic_cast<CBrowserText *>(obj)->text();
      else
        name = obj->text();
    }

    return QVariant(QString(name.c_str()));
  }
  else if (role == Qt::FontRole) {
    if (index.column() == 0) {
      QFont font = tree_->font();

      font.setBold(true);

      return QVariant(font);
    }
  }

  return QVariant();
}

//---

CBrowserDomTreeDelegate::
CBrowserDomTreeDelegate(CBrowserDomTree *tree) :
 tree_(tree)
{
}


void
CBrowserDomTreeDelegate::
paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
  QStyledItemDelegate::paint(painter, option, index);
}

QSize
CBrowserDomTreeDelegate::
sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
  return QStyledItemDelegate::sizeHint(option, index);
}
