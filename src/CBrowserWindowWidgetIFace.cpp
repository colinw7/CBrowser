#include <CBrowserWindowWidgetIFace.h>
#include <CBrowserGraphics.h>

#include <QPixmap>

CBrowserWindowWidgetIFace::
CBrowserWindowWidgetIFace(CBrowser *browser) :
 browser_(browser)
{
}

void
CBrowserWindowWidgetIFace::
init()
{
  graphics_ = new CBrowserGraphics(this);
}

void
CBrowserWindowWidgetIFace::
saveImage(const QString &filename)
{
  auto *pixmap = graphics_->pixmap();

  pixmap->save(filename);
}
