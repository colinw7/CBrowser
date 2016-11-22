#include <CQJSlider.h>
#include <CBrowserForm.h>
#include <CBrowserJS.h>
#include <CBrowserIFace.h>
#include <CBrowserWindowWidget.h>
#include <CQJWindow.h>
#include <CQJEvent.h>
#include <CQJHtmlObj.h>

CQJSlider::
CQJSlider(CBrowserWindowWidget *w, CBrowserFormInput *input) :
 QSlider(Qt::Horizontal, w), w_(w), input_(input)
{
  setObjectName(input->getName().c_str());

  QSlider::setRange(0, 100);
  QSlider::setSingleStep(1);

  setTickPosition(TicksBelow);

  connect(this, SIGNAL(valueChanged(int)), this, SLOT(changeSlot()));
}

void
CQJSlider::
setRange(double min, double max)
{
  min_ = min;
  max_ = max;

  if (step_ < 0)
    step_ = (max_ - min_)/100.0;
}

void
CQJSlider::
setSingleStep(double step)
{
  step_ = step;

  if (step_ > 0)
    QSlider::setSingleStep((max_ - min_)/step_);
}

double
CQJSlider::
value() const
{
  return QSlider::value()*(max_ - min_)/100.0 + min_;
}

void
CQJSlider::
setValue(double v)
{
  QSlider::setValue(100.0*(v - min_)/(max_ - min_));
}

void
CQJSlider::
changeSlot()
{
  CQJHtmlObj *obj = input_->getJObj();
  if (! obj) return;

  CJValueP edit = obj->shared_from_this();

  CJValueP event(new CQJEvent(CBrowserJSInst->js(), edit, 0));

  callEventListener("change", "onchange", event);
}

void
CQJSlider::
callEventListener(const std::string &name, const std::string &prop, CJValueP event)
{
  CQJObject::EventArgs  args;
  CQJObject::NameValues nameValues;

  args.push_back(event);

  CQJHtmlObj *obj = input_->getJObj();

  if (obj && obj->callEventListener(name, prop, args, nameValues))
    return;
}
