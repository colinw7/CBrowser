#include <CBrowserObjProp.h>
#include <CBrowserWindow.h>
#include <CBrowserText.h>
#include <CBrowserObject.h>
#include <QHeaderView>
#include <QAbstractItemModel>
#include <QStyledItemDelegate>
#include <QVBoxLayout>

class CBrowserObjPropModel : public QAbstractItemModel {
 public:
  explicit CBrowserObjPropModel(CBrowserObjProp *tree);

  QModelIndex index(int row, int column, const QModelIndex &) const;

  QModelIndex parent(const QModelIndex &index) const;

  int rowCount(const QModelIndex &parent) const;

  int columnCount(const QModelIndex &parent) const;

  QVariant headerData(int section, Qt::Orientation orientation, int role) const;

  QVariant data(const QModelIndex &index, int role) const;

  void emitDataChanged();

  void updateRows();

 private:
  CBrowserObjProp *tree_ { nullptr };
};

//---

class CBrowserObjPropDelegate : public QStyledItemDelegate {
 public:
  explicit CBrowserObjPropDelegate(CBrowserObjProp *tree);

  void paint(QPainter *painter, const QStyleOptionViewItem &option,
             const QModelIndex &index) const;

  QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;

 private:
  CBrowserObjProp *tree_ { nullptr };
};

//---

CBrowserObjProp::
CBrowserObjProp(QWidget *parent, CBrowserWindow *window) :
 QTreeView(parent), window_(window)
{
  setObjectName("objProp");

  setUniformRowHeights(true);

  header()->setStretchLastSection(true);

  setAllColumnsShowFocus(true);
  setAlternatingRowColors(true);

  // create simple model for a tree view
  auto *model = new CBrowserObjPropModel(this);

  // create custom delegate
  auto *delegate = new CBrowserObjPropDelegate(this);

  // set model and delegate to the treeview object
  setModel(model);

  setItemDelegate(delegate);
}

void
CBrowserObjProp::
setObject(CBrowserObject *obj)
{
  auto *model = dynamic_cast<CBrowserObjPropModel *>(this->model());

  obj_ = obj;

  model->updateRows();
}

void
CBrowserObjProp::
selectionChanged(const QItemSelection & /*selected*/, const QItemSelection & /*deselected*/)
{
#if 0
  for (int i = 0; i < selected.indexes().length(); ++i) {
    const QModelIndex &ind = selected.indexes()[i];

    CBrowserObject *obj = static_cast<CBrowserObject *>(ind.internalPointer());
    if (! obj) continue;

    obj->setSelected(true);
  }

  for (int i = 0; i < deselected.indexes().length(); ++i) {
    const QModelIndex &ind = deselected.indexes()[i];

    CBrowserObject *obj = static_cast<CBrowserObject *>(ind.internalPointer());
    if (! obj) continue;

    obj->setSelected(false);
  }

  window_->redraw();
#endif
}

//---

CBrowserObjPropModel::
CBrowserObjPropModel(CBrowserObjProp *tree) :
 QAbstractItemModel(nullptr), tree_(tree)
{
}

int
CBrowserObjPropModel::
rowCount(const QModelIndex &parent) const
{
  if (parent.isValid())
    return 0;

  auto *obj = tree_->obj();

  return (obj ? obj->numProperties() : 0);
}

int
CBrowserObjPropModel::
columnCount(const QModelIndex &) const
{
  return 2;
}

QModelIndex
CBrowserObjPropModel::
index(int row, int column, const QModelIndex & /*parent*/) const
{
  CBrowserObject *obj = tree_->obj();

  if (! obj)
    return QModelIndex();

  return createIndex(row, column, nullptr);
}

QModelIndex
CBrowserObjPropModel::
parent(const QModelIndex & /*index*/) const
{
  return QModelIndex();
}

QVariant
CBrowserObjPropModel::
headerData(int section, Qt::Orientation, int role) const
{
  if (role != Qt::DisplayRole)
    return QVariant();

  if      (section == 0)
    return "Name";
  else if (section == 1)
    return "Value";

  return QVariant();
}

QVariant
CBrowserObjPropModel::
data(const QModelIndex &index, int role) const
{
  if (! index.isValid())
    return QVariant();

  CBrowserObject *obj = tree_->obj();

  if (! obj)
    return QVariant();

  if      (role == Qt::DisplayRole) {
    std::string value;

    if (index.column() == 0)
      value = obj->propertyName(index.row());
    else
      value = obj->propertyValue(index.row());

    return QVariant(QString(value.c_str()));
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

void
CBrowserObjPropModel::
emitDataChanged()
{
  QModelIndex ind1 = index(0, 0, QModelIndex());
  QModelIndex ind2 = index(rowCount(QModelIndex()), 1, QModelIndex());

  emit dataChanged(ind1, ind2);
}

void
CBrowserObjPropModel::
updateRows()
{
  int nr = rowCount(QModelIndex());

  beginInsertRows(QModelIndex(), 0, nr);
  endInsertRows  ();
}

//---

CBrowserObjPropDelegate::
CBrowserObjPropDelegate(CBrowserObjProp *tree) :
 tree_(tree)
{
}


void
CBrowserObjPropDelegate::
paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
  QStyledItemDelegate::paint(painter, option, index);
}

QSize
CBrowserObjPropDelegate::
sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
  return QStyledItemDelegate::sizeHint(option, index);
}
