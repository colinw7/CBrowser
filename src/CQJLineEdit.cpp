#include <CQJLineEdit.h>
#include <CBrowserForm.h>
#include <CBrowserJS.h>
#include <CBrowserIFace.h>
#include <CBrowserWindowWidget.h>
#include <CQJWindow.h>
#include <CQJEvent.h>
#include <CQJHtmlObj.h>

CQJLineEdit::
CQJLineEdit(CBrowserWindowWidget *w, CBrowserFormInput *input) :
 QLineEdit(w), w_(w), input_(input)
{
  setObjectName(input->getName().c_str());

  connect(this, SIGNAL(textChanged(const QString &)), this, SLOT(changeSlot()));
}

void
CQJLineEdit::
changeSlot()
{
  CQJHtmlObj *obj = input_->getJObj();
  if (! obj) return;

  CJValueP edit = obj->shared_from_this();

  CJValueP event(new CQJEvent(CBrowserJSInst->js(), edit, 0));

  callEventListener("change", "onchange", event);
}

void
CQJLineEdit::
keyPressEvent(QKeyEvent *e)
{
  QLineEdit::keyPressEvent(e);

  CQJHtmlObj *obj = input_->getJObj();
  if (! obj) return;

  CJValueP edit = obj->shared_from_this();

  CJValueP event(new CQJEvent(CBrowserJSInst->js(), edit, e));

  callEventListener("keydown", "onkeydown", event);
}

void
CQJLineEdit::
keyReleaseEvent(QKeyEvent *e)
{
  QLineEdit::keyReleaseEvent(e);

  CQJHtmlObj *obj = input_->getJObj();
  if (! obj) return;

  CJValueP edit = obj->shared_from_this();
  //CQJLineEditP edit = CJValue::cast<CQJLineEdit>(shared_from_this());

  CJValueP event(new CQJEvent(CBrowserJSInst->js(), edit, e));

  callEventListener("keyup", "onkeyup", event);
}

void
CQJLineEdit::
callEventListener(const std::string &name, const std::string &prop, CJValueP event)
{
  CQJObject::EventArgs  args;
  CQJObject::NameValues nameValues;

  args.push_back(event);

  CQJHtmlObj *obj = input_->getJObj();

  if (obj && obj->callEventListener(name, prop, args, nameValues))
    return;
}
