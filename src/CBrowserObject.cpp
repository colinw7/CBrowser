#include <CBrowserObject.h>
#include <CHtmlTagDef.h>

CBrowserObject::
CBrowserObject(CBrowserWindow *window, CHtmlTagId type) :
 window_(window), type_(type)
{
}

CBrowserObject::
~CBrowserObject()
{
  for (auto &c : children_)
    delete c;
}

std::string
CBrowserObject::
typeName() const
{
  const CHtmlTagDef &tagDef = CHtmlTagDefLookupInst->lookup(type_);

  return tagDef.getName();
}

void
CBrowserObject::
addChild(CBrowserObject *child)
{
  child->setParent(this);

  children_.push_back(child);
}

int
CBrowserObject::
childIndex(const CBrowserObject *child) const
{
  for (uint i = 0; i < children_.size(); ++i)
    if (children_[i] == child)
      return i;

  return -1;
}

bool
CBrowserObject::
isHierSelected() const
{
  if (isSelected())
    return true;

  const CBrowserObject *parent = this->parent();

  while (parent) {
    if (parent->isSelected())
      return true;

    parent = parent->parent();
  }

  return false;
}
