#include <CBrowserMain.h>
#include <CBrowserIFace.h>
#include <CBrowserWindow.h>
#include <CBrowserScrolledWindow.h>

#include <CQApp.h>

CBrowserMain *
CBrowserMain::
getInstance()
{
  static CBrowserMain *instance;

  if (! instance)
    instance = new CBrowserMain;

  return instance;
}

CBrowserMain::
CBrowserMain()
{
}

CBrowserMain::
~CBrowserMain()
{
  delete iface_;
}

void
CBrowserMain::
setDebug(bool b)
{
  debug_ = b;
}

void
CBrowserMain::
setUseAlt(bool b)
{
  useAlt_ = b;
}

void
CBrowserMain::
setOldLayout(bool b)
{
  oldLayout_ = b;
}

void
CBrowserMain::
setShowBoxes(bool b)
{
  showBoxes_ = b;
}

void
CBrowserMain::
setDocument(const CUrl &url)
{
  CBrowserIFace *iface = this->iface();

  iface->setDocument(url);

  iface_->show();
}

void
CBrowserMain::
addDocument(const CUrl &url)
{
  CBrowserIFace *iface = this->iface();

  iface->addDocument(url);

  iface_->show();
}

CBrowserIFace *
CBrowserMain::
iface() const
{
  if (! iface_) {
    CBrowserMain *th = const_cast<CBrowserMain *>(this);

    th->iface_ = new CBrowserIFace;

    th->iface_->init();
  }

  return iface_;
}

void
CBrowserMain::
exitSlot()
{
  exit(0);
}
