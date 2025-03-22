#include <CBrowserDomTree.h>
#include <CBrowserObjProp.h>
#include <CBrowserWindow.h>
#include <CBrowserText.h>
#include <CBrowserScript.h>
#include <CBrowserObject.h>

#include <QSplitter>
#include <QHeaderView>
#include <QAbstractItemModel>
#include <QStyledItemDelegate>
#include <QTextEdit>
#include <QVBoxLayout>

class CBrowserDomTreeModel : public QAbstractItemModel {
 public:
  explicit CBrowserDomTreeModel(CBrowserDomTree *tree);

  QModelIndex index(int row, int column, const QModelIndex &) const override;

  QModelIndex parent(const QModelIndex &index) const override;

  int rowCount(const QModelIndex &parent) const override;

  int columnCount(const QModelIndex &parent) const override;

  QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

  QVariant data(const QModelIndex &index, int role) const override;

 private:
  CBrowserDomTree *tree_ { nullptr };
};

//---

class CBrowserDomTreeDelegate : public QStyledItemDelegate {
 public:
  explicit CBrowserDomTreeDelegate(CBrowserDomTree *tree);

  void paint(QPainter *painter, const QStyleOptionViewItem &option,
             const QModelIndex &index) const override;

  QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

 private:
  CBrowserDomTree *tree_ { nullptr };
};

//---

CBrowserDomTreeDlg::
CBrowserDomTreeDlg(CBrowserWindowIFace *window) :
 QWidget()
{
  auto *layout = new QVBoxLayout(this);

  auto *tab = new QTabWidget;

  layout->addWidget(tab);

  //---

  auto *splitter = new QSplitter(this);
  splitter->setObjectName("splitter");

  domTree_ = new CBrowserDomTree(this, window);
  prop_    = new CBrowserObjProp(this, window);

  splitter->addWidget(domTree_);
  splitter->addWidget(prop_);

  layout->addWidget(splitter);

  //---

  tab->addTab(splitter, "DOM");

  //---

  text_ = new QTextEdit;

  tab->addTab(text_, "Text");

  //---

  resize(800, 800);
}

CBrowserWindowIFace *
CBrowserDomTreeDlg::
window() const
{
  return domTree_->window();
}

//---

CBrowserDomTree::
CBrowserDomTree(CBrowserDomTreeDlg *dlg, CBrowserWindowIFace *window) :
 QTreeView(nullptr), dlg_(dlg), window_(window)
{
  setObjectName("domTree");

  setUniformRowHeights(true);

  header()->setStretchLastSection(true);

  setAllColumnsShowFocus(true);
  setAlternatingRowColors(true);

  // create simple model for a tree view
  auto *model = new CBrowserDomTreeModel(this);

  // create custom delegate
  auto *delegate = new CBrowserDomTreeDelegate(this);

  // set model and delegate to the treeview object
  setModel(model);

  setItemDelegate(delegate);
}

void
CBrowserDomTree::
selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
  for (int i = 0; i < selected.indexes().length(); ++i) {
    QModelIndex ind = selected.indexes().at(i);

    auto *obj = static_cast<CBrowserObject *>(ind.internalPointer());
    if (! obj) continue;

    obj->setSelected(true);

    dlg_->prop()->setObject(obj);

    std::string text;

    if (obj->type() == CHtmlTagId::SCRIPT)
      text = dynamic_cast<CBrowserScript *>(obj)->text();
    else
      text = obj->text();

    dlg_->text()->setPlainText(text.c_str());
  }

  auto inds = deselected.indexes();

  for (int i = 0; i < inds.length(); ++i) {
    const QModelIndex &ind = inds[i];

    auto *obj = static_cast<CBrowserObject *>(ind.internalPointer());
    if (! obj) continue;

    obj->setSelected(false);
  }

  window_->redraw();
}

//---

CBrowserDomTreeModel::
CBrowserDomTreeModel(CBrowserDomTree *tree) :
 QAbstractItemModel(nullptr), tree_(tree)
{
}

int
CBrowserDomTreeModel::
rowCount(const QModelIndex &parent) const
{
  auto *window = tree_->window();

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
  auto *window = tree_->window();

  const CBrowserObject *obj { nullptr };

  if (parent.internalPointer())
    obj = static_cast<CBrowserObject *>(parent.internalPointer());
  else
    obj = window->rootObject();

  if (! obj)
    return QModelIndex();

  auto *child = obj->child(row);
  if (! child) return QModelIndex();

  return createIndex(row, column, child);
}

QModelIndex
CBrowserDomTreeModel::
parent(const QModelIndex &index) const
{
  if (! index.isValid())
    return QModelIndex();

  auto *window = tree_->window();

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

  //auto *window = tree_->window();

  if      (role == Qt::DisplayRole) {
    auto *obj = static_cast<CBrowserObject *>(index.internalPointer());
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
