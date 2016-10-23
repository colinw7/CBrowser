#include <CBrowserHtmlI.h>

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

  if (fileName != "")
    iface->getWindow()->setDocument(fileName);

  iface->show();

  ifaces_.push_back(iface);
}
