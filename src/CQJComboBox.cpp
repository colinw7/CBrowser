#include <CQJComboBox.h>
#include <CBrowserForm.h>
#include <CBrowserJS.h>
#include <CBrowserIFace.h>
#include <CBrowserWindowWidget.h>
#include <CQJWindow.h>
#include <CQJEvent.h>
#include <CQJHtmlObj.h>

CQJComboBox::
CQJComboBox(CBrowserWindowWidget *w, CBrowserFormInput *input) :
 QComboBox(w), w_(w), input_(input)
{
  setObjectName(input->getName().c_str());

  connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(currentIndexSlot(int)));
}

void
CQJComboBox::
currentIndexSlot(int)
{
  CQJHtmlObj *obj = input_->getJObj();
  if (! obj) return;

  CJValueP edit = obj->shared_from_this();

  CJValueP event(new CQJEvent(CBrowserJSInst->js(), edit, 0));

  callEventListener("change", "onchange", event);
}

void
CQJComboBox::
callEventListener(const std::string &name, const std::string &prop, CJValueP event)
{
  CQJObject::EventArgs  args;
  CQJObject::NameValues nameValues;

  args.push_back(event);

  CQJHtmlObj *obj = input_->getJObj();

  if (obj && obj->callEventListener(name, prop, args, nameValues))
    return;
}
