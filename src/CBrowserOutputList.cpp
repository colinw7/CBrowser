#include <CBrowserOutputList.h>
#include <CBrowserWindow.h>

CBrowserOutputList::
CBrowserOutputList(CBrowserWindow *window, CHtmlTagId id, const CBrowserOutputListData &data) :
 CBrowserObject(window, id), data_(data)
{
}

void
CBrowserOutputList::
initLayout()
{
  window_->indentLeft(2);

  if (listDepth() == 1 || ! getCompact())
    window_->skipLine();
  else
    window_->newLine();
}

void
CBrowserOutputList::
termLayout()
{
  window_->indentLeft(-2);

  if (listDepth() == 1)
    window_->skipLine();
  else
    window_->newLine();
}

int
CBrowserOutputList::
listDepth() const
{
  int depth = 1;

  const CBrowserObject *parent = window_->currentObj();

  while (parent) {
    if (dynamic_cast<const CBrowserOutputList *>(parent))
      ++depth;

    parent = parent->parent();
  }

  return depth;
}
