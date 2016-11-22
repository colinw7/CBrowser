#include <CHtmlLayoutBox.h>
#include <CHtmlLayoutVisitor.h>

void
CHtmlLayoutBox::
accept(CHtmlLayoutVisitor &visitor)
{
  visitor.enter(this);
  visitor.leave(this);
}
