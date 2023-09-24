#include <CBrowserCSSTree.h>
#include <CBrowserObjProp.h>
#include <CBrowserWindow.h>
#include <CBrowserText.h>
#include <CBrowserScript.h>
#include <CBrowserObject.h>

#include <QComboBox>
#include <QHeaderView>
#include <QAbstractItemModel>
#include <QStyledItemDelegate>
#include <QVBoxLayout>
#include <QHBoxLayout>

class CBrowserCSSTreeModel : public QAbstractItemModel {
 public:
  explicit CBrowserCSSTreeModel(CBrowserCSSTree *tree);

  int ind() const { return ind_; }
  void setInd(int i) { ind_ = i; }

  QModelIndex index(int row, int column, const QModelIndex &) const override;

  QModelIndex parent(const QModelIndex &index) const override;

  int rowCount(const QModelIndex &parent) const override;

  int columnCount(const QModelIndex &parent) const override;

  QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

  QVariant data(const QModelIndex &index, int role) const override;

  std::vector<int> indexToRows(const QModelIndex &ind) const;

 private:
  bool isValidInd() const;

  std::vector<int> nameToRows(const std::string &name) const;

  int *namePtr(const std::string &name) const;

  const std::string &ptrName(int *ptr) const;

 private:
  typedef std::map<std::string,int *>  NamePtrMap;
  typedef std::map<int *, std::string> PtrNameMap;

  CBrowserCSSTree *tree_ { nullptr };
  int              ind_ { 0 };
  NamePtrMap       namePtrMap_;
  PtrNameMap       ptrNameMap_;
};

//---

class CBrowserCSSTreeDelegate : public QStyledItemDelegate {
 public:
  explicit CBrowserCSSTreeDelegate(CBrowserCSSTree *tree);

  void paint(QPainter *painter, const QStyleOptionViewItem &option,
             const QModelIndex &index) const override;

  QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

 private:
  CBrowserCSSTree *tree_ { nullptr };
};

//---

CBrowserCSSTreeDlg::
CBrowserCSSTreeDlg(CBrowserWindow *window) :
 QWidget(), window_(window)
{
  QVBoxLayout *layout = new QVBoxLayout(this);

  //---

  QHBoxLayout *chooseLayout = new QHBoxLayout;

  cssCombo_ = new QComboBox;

  for (const auto &cssData : window_->cssList()) {
    cssCombo_->addItem(cssData.url.getUrl().c_str());
  }

  chooseLayout->addWidget(cssCombo_);

  layout->addLayout(chooseLayout);

  //---

  tree_ = new CBrowserCSSTree(this, window);

  layout->addWidget(tree_);

  //---

  resize(800, 800);
}

CBrowserWindow *
CBrowserCSSTreeDlg::
window() const
{
  return tree_->window();
}

//---

CBrowserCSSTree::
CBrowserCSSTree(CBrowserCSSTreeDlg *dlg, CBrowserWindow *window) :
 QTreeView(nullptr), dlg_(dlg), window_(window), ind_(0)
{
  setObjectName("tree");

  setUniformRowHeights(true);

  header()->setStretchLastSection(true);

  setAllColumnsShowFocus(true);
  setAlternatingRowColors(true);

  // create simple model for a tree view
  CBrowserCSSTreeModel *model = new CBrowserCSSTreeModel(this);

  // create custom delegate
  CBrowserCSSTreeDelegate* delegate = new CBrowserCSSTreeDelegate(this);

  // set model and delegate to the treeview object
  setModel(model);

  setItemDelegate(delegate);
}

void
CBrowserCSSTree::
selectionChanged(const QItemSelection &selected, const QItemSelection & /*deselected*/)
{
  auto isValidInd = [&]() {
    return (ind_ >= 0 && ind_ < int(window()->cssList().size()));
  };

  if (! isValidInd())
    return;

  auto *model = dynamic_cast<CBrowserCSSTreeModel *>(this->model());

  const auto &cssData = window()->cssData(ind_);

  std::vector<CCSS::SelectorList> selectors;

  cssData.css.getSelectors(selectors);

  for (int i = 0; i < selected.indexes().length(); ++i) {
    const QModelIndex &ind = selected.indexes()[i];

    std::vector<int> rows = model->indexToRows(ind);

    if (rows.size() != 1)
      continue;

    //---

    int rootInd = rows[0];

    const CCSS::SelectorList &selectorList = selectors[rootInd];

    const CCSS::StyleData &styleData = cssData.css.getStyleData(selectorList);

    window_->selectCSSPattern(styleData);
  }
}

void
CBrowserCSSTree::
setInd(int ind)
{
  dynamic_cast<CBrowserCSSTreeModel *>(model())->setInd(ind);
}

//---

CBrowserCSSTreeModel::
CBrowserCSSTreeModel(CBrowserCSSTree *tree) :
 QAbstractItemModel(nullptr), tree_(tree), ind_(0)
{
}

int
CBrowserCSSTreeModel::
rowCount(const QModelIndex &parent) const
{
  if (! isValidInd())
    return 0;

  CBrowserWindow *window = tree_->window();

  const auto &cssData = window->cssData(ind_);

  std::vector<CCSS::SelectorList> selectors;

  cssData.css.getSelectors(selectors);

  //---

  if (! parent.internalPointer())
    return selectors.size();

  //---

  std::vector<int> rows = indexToRows(parent);

  if (rows.empty())
    return 0;

  //---

  int rootInd = rows[0];

  const CCSS::SelectorList &selectorList = selectors[rootInd];

  const CCSS::StyleData &styleData = cssData.css.getStyleData(selectorList);

  if (rows.size() == 1) {
    return styleData.getOptions().size();
  }

  //---

  return 0;
}

int
CBrowserCSSTreeModel::
columnCount(const QModelIndex &) const
{
  return 2;
}

QModelIndex
CBrowserCSSTreeModel::
index(int row, int column, const QModelIndex &parent) const
{
  int *ptr = nullptr;

  if (parent.internalPointer())
    ptr = static_cast<int *>(parent.internalPointer());
  else
    ptr = nullptr;

  std::string name1;

  if (ptr) {
    const std::string &name = ptrName(ptr);

    name1 = name + "/" + std::to_string(row);
  }
  else
    name1 = std::to_string(row);

  int *ptr1 = namePtr(name1);

  return createIndex(row, column, ptr1);
}

QModelIndex
CBrowserCSSTreeModel::
parent(const QModelIndex &index) const
{
  if (! index.isValid())
    return QModelIndex();

  int *ptr = nullptr;

  if (index.internalPointer())
    ptr = static_cast<int *>(index.internalPointer());
  else
    ptr = nullptr;

  const std::string &name = ptrName(ptr);

  auto p = name.rfind('/');

  if (p == std::string::npos)
    return QModelIndex();

  std::string name1 = name.substr(0, p);

  int *ptr1 = namePtr(name1);

  std::vector<int> rows = nameToRows(name1);

  return createIndex(rows.back(), 0, ptr1);
}

QVariant
CBrowserCSSTreeModel::
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
CBrowserCSSTreeModel::
data(const QModelIndex &index, int role) const
{
  if (! index.isValid())
    return QVariant();

  //---

  if (! isValidInd())
    return QVariant();

  CBrowserWindow *window = tree_->window();

  const auto &cssData = window->cssData(ind_);

  //---

  if (! index.internalPointer())
    return QModelIndex();

  //---

  std::vector<int> rows = indexToRows(index);

  if (rows.empty())
    return QModelIndex();

  //---

  if      (role == Qt::DisplayRole) {
    if (rows.size() < 1 || rows.size() > 2)
      return QVariant();

    std::vector<CCSS::SelectorList> selectors;

    cssData.css.getSelectors(selectors);

    int rootInd = rows[0];

    const CCSS::SelectorList &selectorList = selectors[rootInd];

    const CCSS::StyleData &styleData = cssData.css.getStyleData(selectorList);

    if (rows.size() == 1) {
      if (index.column() != 0)
        return "";

      return QString(styleData.toString().c_str());
    }

    //---

    int patternInd = rows[1];

    if (rows.size() == 2) {
      const CCSS::Option &option = styleData.getOption(patternInd);

      if      (index.column() == 0)
        return QString(option.getName().c_str());
      else if (index.column() == 1)
        return QString(option.getValue().c_str());

      return QVariant();
    }

    //---

    return QVariant();
  }
  else if (role == Qt::FontRole) {
    return QVariant();
  }

  return QVariant();
}

bool
CBrowserCSSTreeModel::
isValidInd() const
{
  auto *window = tree_->window();

  return (ind_ >= 0 && ind_ < int(window->cssList().size()));
}

std::vector<int>
CBrowserCSSTreeModel::
indexToRows(const QModelIndex &ind) const
{
  int *ptr = static_cast<int *>(ind.internalPointer());

  if (! ptr)
    return std::vector<int>();

  const std::string &name = ptrName(ptr);

  return nameToRows(name);
}

std::vector<int>
CBrowserCSSTreeModel::
nameToRows(const std::string &name) const
{
  QStringList names = QString(name.c_str()).split("/");

  std::vector<int> rows;

  rows.resize(names.size());

  for (int i = 0; i < names.length(); ++i)
    rows[i] = std::stoi(names[i].toStdString());

  return rows;
}

int *
CBrowserCSSTreeModel::
namePtr(const std::string &name) const
{
  auto p = namePtrMap_.find(name);

  if (p == namePtrMap_.end()) {
    CBrowserCSSTreeModel *th = const_cast<CBrowserCSSTreeModel *>(this);

    int *ptr = new int; *ptr = th->namePtrMap_.size();

    p = th->namePtrMap_.insert(p, NamePtrMap::value_type(name, ptr));

    th->ptrNameMap_[ptr] = name;
  }

  return (*p).second;
}

const std::string &
CBrowserCSSTreeModel::
ptrName(int *ptr) const
{
  auto p = ptrNameMap_.find(ptr);

  assert(p != ptrNameMap_.end());

  return (*p).second;
}

//---

CBrowserCSSTreeDelegate::
CBrowserCSSTreeDelegate(CBrowserCSSTree *tree) :
 tree_(tree)
{
}


void
CBrowserCSSTreeDelegate::
paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
  QStyledItemDelegate::paint(painter, option, index);
}

QSize
CBrowserCSSTreeDelegate::
sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
  return QStyledItemDelegate::sizeHint(option, index);
}
