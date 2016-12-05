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
setDebug(bool debug)
{
  debug_ = debug;
}

void
CBrowserMain::
setUseAlt(bool use_alt)
{
  use_alt_ = use_alt;
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
