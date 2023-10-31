#include <CBrowserMain.h>
#include <CBrowserMainWindow.h>
#include <CBrowserWindow.h>
#include <CBrowserScrolledWindow.h>
#include <CBrowserRenderer.h>

#include <CQApp.h>

CBrowser::
CBrowser()
{
}

CBrowser::
~CBrowser()
{
  delete iface_;
}

void
CBrowser::
setDebug(bool b)
{
  debug_ = b;
}

void
CBrowser::
setUseAlt(bool b)
{
  useAlt_ = b;
}

void
CBrowser::
setOldLayout(bool b)
{
  oldLayout_ = b;
}

void
CBrowser::
setShowBoxes(bool b)
{
  showBoxes_ = b;
}

void
CBrowser::
setDocument(const CUrl &url)
{
  auto *iface = this->iface();

  iface->setDocument(url);

  iface_->show();
}

void
CBrowser::
addDocument(const CUrl &url)
{
  auto *iface = this->iface();

  iface->addDocument(url);

  iface_->show();
}

CBrowserIFace *
CBrowser::
iface() const
{
  if (! iface_) {
    auto *th = const_cast<CBrowser *>(this);

    auto *mainWindow = new CBrowserMainWindow(th);

    mainWindow->init();

    th->setIFace(mainWindow);
  }

  return iface_;
}

void
CBrowser::
setIFace(CBrowserIFace *iface)
{
  iface_ = iface;
}

void
CBrowser::
exitSlot()
{
  exit(0);
}

void
CBrowser::
displayError(const char *format, ...)
{
  va_list args;

  va_start(args, format);

  displayError(format, &args);

  va_end(args);
}

void
CBrowser::
displayError(const char *format, va_list *args)
{
  if (! getQuiet())
    vfprintf(stderr, format, *args);
}

void
CBrowser::
displayError(const QString &str)
{
  if (! getQuiet())
    fprintf(stderr, "%s", str.toLatin1().constData());
}
