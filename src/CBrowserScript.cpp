#include <CBrowserScript.h>
#include <CBrowserWindow.h>
#include <CBrowserCeil.h>
#include <CStrUtil.h>

CBrowserScript::
CBrowserScript(CBrowserWindow *window, const CBrowserScriptData &data) :
 CBrowserObject(window, CHtmlTagId::SCRIPT), data_(data)
{
}

void
CBrowserScript::
initProcess()
{
}

void
CBrowserScript::
termProcess()
{
  if (text_ != "") {
    if      (CStrUtil::casecmp(data_.language, "ceil") == 0) {
      CBrowserCeilInst->runScript(window_, text_);
    }
    else if (CStrUtil::casecmp(data_.type, "text/javascript") == 0) {
      //CBrowserJSInst->runScript(window, text_);

      window_->addScript(text_);
    }
  }
}
