#include <CBrowserCanvasWidget.h>
#include <CBrowserCanvas.h>
#include <CBrowserWindowWidget.h>
#include <CBrowserMain.h>
#include <CBrowserText.h>
#include <CBrowserLink.h>
#include <CBrowserWindow.h>
#include <CBrowserDocument.h>
#include <CQJEvent.h>
#include <CQJHtmlObj.h>
#include <CQImageGaussianBlur.h>
#include <QPainter>
#include <cstring>

CBrowserCanvasWidget::
CBrowserCanvasWidget(CBrowserCanvas *canvas, CBrowserWindowWidget *widget) :
 QWidget(widget), canvas_(canvas), widget_(widget)
{
}

void
CBrowserCanvasWidget::
updateSize(int w, int h)
{
  if (qimage_.width() == w && qimage_.height() == h)
    return;

  delete ip_;

  qimage_ = QImage(QSize(w, h), QImage::Format_ARGB32);

  qimage_.fill(0);

  ip_ = new QPainter(&qimage_);

  //ip_->setCompositionMode(QPainter::CompositionMode_Plus);

  ip_->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

  //CJValue::cast<CQJCanvas>(CBrowserJSInst->jsCanvas())->updateSize();
}

void
CBrowserCanvasWidget::
setFillColor(const QColor &c)
{
  fillColor_ = c;
  fillStyle_ = Qt::SolidPattern;
}

void
CBrowserCanvasWidget::
setFillGradient(const QGradient &g)
{
  fillGradient_ = g;
  fillStyle_    = Qt::LinearGradientPattern;
}

void
CBrowserCanvasWidget::
setFillPattern(const QImage &i)
{
  fillPattern_ = i;
  fillStyle_   = Qt::TexturePattern;
}

void
CBrowserCanvasWidget::
setFillAlpha(double a)
{
  fillAlpha_ = a;
}

void
CBrowserCanvasWidget::
setStrokeColor(const QColor &c)
{
  strokeColor_ = c;
  strokeStyle_ = Qt::SolidPattern;
}

void
CBrowserCanvasWidget::
setStrokeGradient(const QGradient &g)
{
  strokeGradient_ = g;
  strokeStyle_    = Qt::LinearGradientPattern;
}

void
CBrowserCanvasWidget::
setStrokeAlpha(double a)
{
  strokeAlpha_ = a;
}

void
CBrowserCanvasWidget::
setStrokeWidth(double w)
{
  strokeWidth_ = w;
}

void
CBrowserCanvasWidget::
setStrokeCap(Qt::PenCapStyle cap)
{
  strokeCap_ = cap;
}

void
CBrowserCanvasWidget::
setStrokeJoin(Qt::PenJoinStyle join)
{
  strokeJoin_ = join;
}

void
CBrowserCanvasWidget::
setOpacity(double a)
{
  ip_->setOpacity(a);
}

//------

void
CBrowserCanvasWidget::
fillRect(double x, double y, double w, double h)
{
  setBrush();

  ip_->fillRect(QRectF(x, y, w, h), fillBrush_);
}

void
CBrowserCanvasWidget::
clearRect(double x, double y, double w, double h)
{
  ip_->setCompositionMode(QPainter::CompositionMode_SourceOver);

  ip_->fillRect(QRectF(x, y, w, h), QColor(0,0,0,255));

  //ip_->eraseRect(QRectF(x, y, w, h));

  ip_->setCompositionMode(QPainter::CompositionMode_Plus);
}

void
CBrowserCanvasWidget::
drawRect(double x, double y, double w, double h)
{
  setPen();

  ip_->setPen(pen_);

  ip_->drawRect(QRectF(x, y, w, h));
}

void
CBrowserCanvasWidget::
fillShadowText(double x, double y, const std::string &text, double d, const QColor &c)
{
  setBrush();

  QFontMetrics fm(font_);

  int fw = fm.width(text.c_str());
  int fa = fm.ascent();
  int fd = fm.descent();

  QImage qimage = QImage(QSize(fw, fa + fd), QImage::Format_ARGB32);

  qimage.fill(0);

  QPainter ip(&qimage);

  QPainterPath path;

  path.addText(QPointF(0, fa), font_, text.c_str());

  ip.fillPath(path, QBrush(c));

  CQImageGaussianBlur blur(qimage);

  QImage qimageBlur;

  blur.blur(qimageBlur, d, d, 0, 0);

  //---

  if      (fontAlign_ & Qt::AlignBottom)
    y -= fa + fd;
  else if (fontAlign_ & Qt::AlignVCenter)
    y -= fa + (fa - fd)/2;

  if      (fontAlign_ & Qt::AlignRight)
    x -= fw;
  else if (fontAlign_ & Qt::AlignHCenter)
    x -= fw/2;

  ip_->drawImage(x, y, qimageBlur);
}

void
CBrowserCanvasWidget::
fillText(double x, double y, const std::string &text)
{
  setBrush();

  QFontMetrics fm(font_);

  int fw = fm.width(text.c_str());
  int fa = fm.ascent();
  int fd = fm.descent();

  if      (fontAlign_ & Qt::AlignTop)
    y += fa;
  else if (fontAlign_ & Qt::AlignBottom)
    y -= fd;
  else if (fontAlign_ & Qt::AlignVCenter)
    y -= (fa - fd)/2;

  if      (fontAlign_ & Qt::AlignRight)
    x -= fw;
  else if (fontAlign_ & Qt::AlignHCenter)
    x -= fw/2;

  path_ = QPainterPath();

  path_.addText(QPointF(x, y), font_, text.c_str());

  ip_->fillPath(path_, fillBrush_);
}

void
CBrowserCanvasWidget::
strokeText(double x, double y, const std::string &text)
{
  setPen();

  QFontMetrics fm(font_);

  if      (fontAlign_ & Qt::AlignTop)
    y += fm.ascent();
  else if (fontAlign_ & Qt::AlignBottom)
    y -= fm.descent();
  else if (fontAlign_ & Qt::AlignVCenter)
    y -= fm.ascent() - fm.height()/2;

  if      (fontAlign_ & Qt::AlignRight)
    x -= fm.width(text.c_str());
  else if (fontAlign_ & Qt::AlignHCenter)
    x -= fm.width(text.c_str())/2;

  path_ = QPainterPath();

  path_.addText(QPointF(x, y), font_, text.c_str());

  ip_->strokePath(path_, pen_);
}

void
CBrowserCanvasWidget::
drawImage(const QImage &image, double dx, double dy)
{
  ip_->drawImage(dx, dy, image);
}

void
CBrowserCanvasWidget::
drawImage(const QImage &image, double dx, double dy, double dWidth, double dHeight)
{
  ip_->drawImage(QRectF(dx, dy, dWidth, dHeight), image);
}

void
CBrowserCanvasWidget::
drawImage(const QImage &image, double sx, double sy, double sWidth, double sHeight,
          double dx, double dy, double dWidth, double dHeight)
{
  ip_->drawImage(QRectF(dx, dy, dWidth, dHeight), image, QRectF(sx, sy, sWidth, sHeight));
}

QImage
CBrowserCanvasWidget::
getImageData(double x, double y, double w, double h)
{
  return qimage_.copy(QRect(x, y, w, h));
}

void
CBrowserCanvasWidget::
putImageData(const QImage &image, double x, double y)
{
  ip_->drawImage(x, y, image);
}

void
CBrowserCanvasWidget::
beginPath()
{
  path_ = QPainterPath();
}

void
CBrowserCanvasWidget::
moveTo(double x, double y)
{
  path_.moveTo(x, y);
}

void
CBrowserCanvasWidget::
lineTo(double x, double y)
{
  path_.lineTo(x, y);
}

void
CBrowserCanvasWidget::
arc(double x, double y, double r, double a1, double a2, bool counter)
{
  double da1 = 180*a1/M_PI;
  double da2 = 180*a2/M_PI;

  QRectF rect(x - r, y - r, 2*r, 2*r);

  if (! counter) {
    if (! path_.elementCount())
      path_.arcMoveTo(rect, da1);

    path_.arcTo(rect, da1, da2 - da1);
  }
  else {
    if (! path_.elementCount())
      path_.arcMoveTo(rect, da2);

    path_.arcTo(rect, da2, da1 - da2);
  }
}

void
CBrowserCanvasWidget::
closePath()
{
  path_.closeSubpath();
}

void
CBrowserCanvasWidget::
stroke()
{
  setPen();

  if (strokeStyle_ == Qt::SolidPattern) {
    ip_->strokePath(path_, pen_);
  }
  else {
    QPainterPathStroker stroker;

    stroker.setCapStyle   (pen_.capStyle());
    stroker.setDashOffset (pen_.dashOffset());
    stroker.setDashPattern(pen_.dashPattern());
    stroker.setJoinStyle  (pen_.joinStyle());
    stroker.setMiterLimit (pen_.miterLimit());
    stroker.setWidth      (pen_.widthF());

    ip_->fillPath(stroker.createStroke(path_), strokeBrush_);
  }
}

void
CBrowserCanvasWidget::
fill()
{
  setBrush();

  ip_->fillPath(path_, fillBrush_);
}

void
CBrowserCanvasWidget::
save()
{
  ip_->save();
}

void
CBrowserCanvasWidget::
restore()
{
  ip_->restore();
}

void
CBrowserCanvasWidget::
rect(double x, double y, double w, double h)
{
  //rect_ = QRectF(x, y, w, h);

  path_.moveTo(x    , y    );
  path_.lineTo(x + w, y    );
  path_.lineTo(x + w, y + h);
  path_.lineTo(x    , y + h);
}

void
CBrowserCanvasWidget::
clip()
{
  // clip from path ?
  QRectF rect = path_.boundingRect();

  //ip_->setClipRect(rect_);
  ip_->setClipRect(rect);
}

void
CBrowserCanvasWidget::
setTransform(double a, double b, double c, double d, double e, double f)
{
  QTransform m(a, b, c, d, e, f);

  ip_->setWorldTransform(m);
}

void
CBrowserCanvasWidget::
translate(double dx, double dy)
{
  ip_->translate(dx, dy);
}

void
CBrowserCanvasWidget::
scale(double sx, double sy)
{
  ip_->scale(sx, sy);
}

void
CBrowserCanvasWidget::
rotate(double a)
{
  ip_->rotate(180*a/M_PI);
}

void
CBrowserCanvasWidget::
setPen()
{
  if      (strokeStyle_ == Qt::SolidPattern) {
    QColor c = strokeColor_;

    c.setAlpha(255*strokeAlpha_);

    pen_.setColor(c);
  }
  else {
    strokeBrush_ = QBrush(strokeGradient_);
  }

  pen_.setCapStyle(strokeCap_);
  pen_.setJoinStyle(strokeJoin_);
  pen_.setWidthF(strokeWidth_);
}

void
CBrowserCanvasWidget::
setBrush()
{
  if      (fillStyle_ == Qt::SolidPattern) {
    QColor c = fillColor_;

    c.setAlpha(255*fillAlpha_);

    fillBrush_.setStyle(Qt::SolidPattern);
    fillBrush_.setColor(c);
  }
  else if (fillStyle_ == Qt::LinearGradientPattern) {
    fillBrush_ = QBrush(fillGradient_);
  }
  else if (fillStyle_ == Qt::TexturePattern) {
    fillBrush_ = QBrush(fillPattern_);
  }
}

void
CBrowserCanvasWidget::
paintEvent(QPaintEvent *)
{
  QPainter p(this);

  p.fillRect(QWidget::rect(), Qt::black);

  p.drawImage(0, 0, qimage_);

  p.setPen(Qt::red);

  p.drawRect(QWidget::rect());
}

void
CBrowserCanvasWidget::
resizeEvent(QResizeEvent *)
{
}

void
CBrowserCanvasWidget::
enterEvent(QEvent *)
{
  CQJObject::EventArgs args;

  callEventListener("mouseover", "onmouseover", args);
}

void
CBrowserCanvasWidget::
leaveEvent(QEvent *)
{
  CQJObject::EventArgs args;

  callEventListener("mouseout", "onmouseout", args);
}

void
CBrowserCanvasWidget::
mousePressEvent(QMouseEvent *e)
{
  CJavaScript *js = canvas_->getJObj()->js();

  CQJObject::EventArgs  args;
  CQJObject::NameValues nameValues;

  nameValues.push_back(CQJObject::NameValue("mouseX", js->createNumberValue(long(e->x()))));
  nameValues.push_back(CQJObject::NameValue("mouseY", js->createNumberValue(long(e->y()))));

  callEventListener("mousedown", "onmousedown", args, nameValues);
}

void
CBrowserCanvasWidget::
mouseMoveEvent(QMouseEvent *e)
{
  CJavaScript *js = canvas_->getJObj()->js();

  CQJObject::EventArgs  args;
  CQJObject::NameValues nameValues;

  nameValues.push_back(CQJObject::NameValue("mouseX", js->createNumberValue(long(e->x()))));
  nameValues.push_back(CQJObject::NameValue("mouseY", js->createNumberValue(long(e->y()))));

  callEventListener("mousemove", "onmousemove", args, nameValues);
}

void
CBrowserCanvasWidget::
mouseReleaseEvent(QMouseEvent *e)
{
  CJavaScript *js = canvas_->getJObj()->js();

  CQJObject::EventArgs  args;
  CQJObject::NameValues nameValues;

  nameValues.push_back(CQJObject::NameValue("mouseX", js->createNumberValue(long(e->x()))));
  nameValues.push_back(CQJObject::NameValue("mouseY", js->createNumberValue(long(e->y()))));

  callEventListener("mouseup", "onmouseup", args, nameValues);

  callEventListener("click", "onclick", args, nameValues);
}

void
CBrowserCanvasWidget::
mouseDoubleClickEvent(QMouseEvent *e)
{
  CJavaScript *js = canvas_->getJObj()->js();

  CQJObject::EventArgs  args;
  CQJObject::NameValues nameValues;

  nameValues.push_back(CQJObject::NameValue("mouseX", js->createNumberValue(long(e->x()))));
  nameValues.push_back(CQJObject::NameValue("mouseY", js->createNumberValue(long(e->y()))));

  callEventListener("dblclick", "ondblclick", args, nameValues);
}

void
CBrowserCanvasWidget::
contextMenuEvent(QContextMenuEvent *e)
{
  CJavaScript *js = canvas_->getJObj()->js();

  CQJObject::EventArgs  args;
  CQJObject::NameValues nameValues;

  nameValues.push_back(CQJObject::NameValue("mouseX", js->createNumberValue(long(e->x()))));
  nameValues.push_back(CQJObject::NameValue("mouseY", js->createNumberValue(long(e->y()))));

  callEventListener("contextmenu", "oncontextmenu", args, nameValues);
}

void
CBrowserCanvasWidget::
keyPressEvent(QKeyEvent *e)
{
  CQJHtmlObj *obj = canvas_->getJObj();
  if (! obj) return;

  CJavaScript *js = obj->js();

  CJValueP value = obj->shared_from_this();

  CJValueP event(new CQJEvent(js, value, e));

  callEventListener("keydown", "onkeydown", event);
}

void
CBrowserCanvasWidget::
keyReleaseEvent(QKeyEvent *e)
{
  CQJHtmlObj *obj = canvas_->getJObj();
  if (! obj) return;

  CJavaScript *js = obj->js();

  CJValueP value = obj->shared_from_this();

  CJValueP event(new CQJEvent(js, value, e));

  callEventListener("keyup", "onkeyup", event);
}

void
CBrowserCanvasWidget::
callEventListener(const std::string &name, const std::string &prop, CJValueP event)
{
  CQJObject::EventArgs  args;
  CQJObject::NameValues nameValues;

  args.push_back(event);

  callEventListener(name, prop, args, nameValues);
}

void
CBrowserCanvasWidget::
callEventListener(const std::string &name, const std::string &prop,
                  const CQJObject::EventArgs &args, const CQJObject::NameValues &nameValues)
{
  CQJHtmlObj *obj = canvas_->getJObj();

  if (obj && obj->callEventListener(name, prop, args, nameValues))
    return;
}
