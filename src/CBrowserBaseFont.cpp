#include <CBrowserBaseFont.h>
#include <CBrowserWindow.h>

CBrowserBaseFont::
CBrowserBaseFont(CBrowserWindow *window, const CBrowserBaseFontData &data) :
 CBrowserObject(window, CHtmlTagId::BASEFONT), data_(data)
{
  setDisplay(CBrowserObject::Display::INLINE);
}

void
CBrowserBaseFont::
init()
{
  CBrowserObject::init();
}

void
CBrowserBaseFont::
setNameValue(const std::string &name, const std::string &value)
{
  std::string lname = CStrUtil::toLower(name);

  if (lname == "size") {
    std::string value1 = value;

    if      (value1[0] == '+' || value1[0] == '-') {
      data_.sizeSign = (value1[0] == '+' ? 1 : -1);

      value1 = value1.substr(1);
    }
    else
      data_.sizeSign = 0;

    if (CStrUtil::isInteger(value1)) {
      data_.size    = CStrUtil::toInteger(value1);
      data_.sizeSet = true;
    }
    else
      window_->displayError("Illegal 'basefont' Value for Size '%s'\n", value.c_str());
  }
  else {
    CBrowserObject::setNameValue(name, value);
  }
}

void
CBrowserBaseFont::
initProcess()
{
  apply();
}

void
CBrowserBaseFont::
termProcess()
{
  window_->resetBaseFontSize();
}

void
CBrowserBaseFont::
apply() const
{
  if (data_.sizeSet) {
    if      (data_.sizeSign == 1)
      window_->increaseBaseFontSize(data_.size);
    else if (data_.sizeSign == -1)
      window_->decreaseBaseFontSize(data_.size);
    else
      window_->setBaseFontSize(data_.size);
  }
  else
    window_->resetBaseFontSize();
}
