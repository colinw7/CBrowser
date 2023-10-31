#include <CBrowserSVG.h>
#include <CBrowserWindow.h>
#include <CBrowserGraphics.h>

#include <CQSVGRenderer.h>
#include <CHtmlTag.h>

CBrowserSVG::
CBrowserSVG(CBrowserWindowIFace *window) :
 CBrowserObject(window, CHtmlTagId::SVG)
{
  renderer_ = new CQSVGRenderer;

  svg_.setRenderer(renderer_);

  block_ = svg_.getRoot();

  currentObj_ = block_;
}

bool
CBrowserSVG::
processTag(CHtmlTag *tag)
{
  if (tag->isStartTag()) {
    CSVGObject *obj = svg_.createObjectByName(tag->getName());

    if (obj) {
      for (const auto &opt : tag->getOptions()) {
        const std::string &name  = opt->getName ();
        const std::string &value = opt->getValue();

        if (! obj->processOption(name, value))
          window_->displayError("Unhandled tag option %s=%s for %s\n",
                                name.c_str(), value.c_str(), obj->getTagName().c_str());
      }

      if (currentObj_)
        currentObj_->addChildObject(obj);

      currentObj_ = obj;
    }
    else {
      window_->displayError("SVG Tag '%s' not implemented\n", tag->getName().c_str());
      return false;
    }
  }
  else {
    currentObj_ = currentObj_->getParent();
  }

  return true;
}

void
CBrowserSVG::
setNameValue(const std::string &name, const std::string &value)
{
  if (! block_->processOption(name, value))
    window_->displayError("Unhandled tag option %s=%s for %s\n",
                          name.c_str(), value.c_str(), block_->getTagName().c_str());

  CBrowserObject::setNameValue(name, value);
}

CBrowserRegion
CBrowserSVG::
calcRegion() const
{
  int width  = svg_.getWidth ();
  int height = svg_.getHeight();

  return CBrowserRegion(width, height, 0);
}

void
CBrowserSVG::
draw(const CTextBox &region)
{
  svg_.draw();

  const QImage &qimage = renderer_->qimage();

  auto *graphics = window_->graphics();

  graphics->drawImage(region.x(), region.y(), qimage);
}
