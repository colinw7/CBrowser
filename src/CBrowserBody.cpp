#include <CBrowserBody.h>
#include <CBrowserWindow.h>
#include <CBrowserDocument.h>

CBrowserBody::
CBrowserBody(CBrowserWindow *window, const CBrowserBodyData &data) :
 CBrowserObject(window, CHtmlTagId::BODY), data_(data)
{
}

void
CBrowserBody::
initProcess()
{
  CBrowserDocument *document = window_->getDocument();

  document->setBgColor(data_.bgcolor);
  document->setFgColor(data_.text   );

  window_->setBackgroundImage(data_.background, data_.fixed);

  document->setLinkColor (data_.link );
  document->setALinkColor(data_.alink);
  document->setVLinkColor(data_.vlink);

  window_->setMargins(data_.leftmargin, data_.topmargin);
}

void
CBrowserBody::
termProcess()
{
}
