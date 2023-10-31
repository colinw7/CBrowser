#include <CBrowserMain.h>
#include <CBrowserIFace.h>
#include <CBrowserWindowIFace.h>
#include <CBrowserWindowWidgetIFace.h>

#include <CQApp.h>
#include <CArgs.h>
#include <QTimer>
#include <QFile>
#include <QPixmap>

class CBrowserTextWindowWidgetIFace : public CBrowserWindowWidgetIFace {
 public:
  CBrowserTextWindowWidgetIFace(CBrowser *browser) :
   CBrowserWindowWidgetIFace(browser) {
  }

  int windowWidth () override { return 1000; }
  int windowHeight() override { return 1000; }

  void drawPixmap(QPixmap *pixmap) override {
    QFile file("temp.png");

    if (! file.open(QIODevice::WriteOnly))
      return;

    pixmap->save(&file, "PNG");
  }
};

class CBrowserTextWindowIFace : public CBrowserWindowIFace {
 public:
  CBrowserTextWindowIFace(CBrowser *browser) :
   CBrowserWindowIFace(browser) {
  }

 ~CBrowserTextWindowIFace() override { }

  CBrowserWindowWidgetIFace *makeWidget() override {
    return new CBrowserTextWindowWidgetIFace(browser_);
  }
};

//---

class CBrowserTextIFace : public CBrowserIFace {
 public:
  CBrowserTextIFace(CBrowser *browser) :
   CBrowserIFace(browser) {
    window_ = new CBrowserTextWindowIFace(browser);

    window_->init();
  }

  void addDocument(const CUrl &url) override {
    setDocument(url);
  }

  void setDocument(const CUrl &url) override {
    window_->setDocument(url);
  }

  void draw() { window_->draw(); }

 private:
  CBrowserTextWindowIFace *window_ { nullptr };
};

//---

int
main(int argc, char **argv)
{
  CQApp app(argc, argv);

  CArgs cargs("-debug:f -use_alt:f -batch:f -old_layout:f -text:f");

  cargs.parse(&argc, argv);

  bool debug   = cargs.getBooleanArg("-debug");
  bool use_alt = cargs.getBooleanArg("-use_alt");
  bool batch   = cargs.getBooleanArg("-batch");
  bool old     = cargs.getBooleanArg("-old_layout");
  bool text    = cargs.getBooleanArg("-text");

  auto *browser = new CBrowser;

  browser->setDebug(debug);
  browser->setUseAlt(use_alt);
  browser->setOldLayout(old);

  CBrowserTextIFace *textIFace = nullptr;

  if (text) {
    textIFace = new CBrowserTextIFace(browser);

    browser->setIFace(textIFace);

    batch = true;
  }

  if (argc > 1) {
    for (int i = 1; i < argc; ++i) {
      std::string fileName = argv[i];

      CUrl url(fileName);

      if (i == 1)
        browser->setDocument(url);
      else
        browser->addDocument(url);
    }
  }
  else {
    CUrl url;

    browser->setDocument(url);
  }

  if (text)
    textIFace->draw();

  if (batch) {
    auto *timer = new QTimer;

    QObject::connect(timer, SIGNAL(timeout()), browser, SLOT(exitSlot()));

    timer->start(100);
  }

  return app.exec();
}
