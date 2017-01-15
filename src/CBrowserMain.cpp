#include <CBrowserMain.h>
#include <CBrowserIFace.h>
#include <CBrowserWindow.h>

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
  for (auto &i : ifaces_)
    delete i;
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
openDocument(const std::string &fileName)
{
  CBrowserIFace *iface = new CBrowserIFace;

  iface->init();

  ifaces_.push_back(iface);

  if (fileName != "")
    iface->getWindow()->setDocument(fileName);

  iface->show();
}

void
CBrowserMain::
exitSlot()
{
  exit(0);
}
