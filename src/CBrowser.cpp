#include <CQApp.h>
#include <CArgs.h>
#include <CBrowserMain.h>
#include <QTimer>

int
main(int argc, char **argv)
{
  CQApp app(argc, argv);

  CArgs cargs("-debug:f -use_alt:f -batch:f -old_layout:f");

  cargs.parse(&argc, argv);

  bool debug   = cargs.getBooleanArg("-debug");
  bool use_alt = cargs.getBooleanArg("-use_alt");
  bool batch   = cargs.getBooleanArg("-batch");
  bool old     = cargs.getBooleanArg("-old_layout");

  CBrowserMain *browser = CBrowserMainInst;

  browser->setDebug (debug);
  browser->setUseAlt(use_alt);
  browser->setOldLayout(old);

  if (argc > 1) {
    for (int i = 1; i < argc; ++i) {
      std::string fileName = argv[i];

      if (i == 1)
        browser->setDocument(fileName);
      else
        browser->addDocument(fileName);
    }
  }
  else {
    browser->setDocument("");
  }

  if (batch) {
    QTimer *timer = new QTimer;

    QObject::connect(timer, SIGNAL(timeout()), browser, SLOT(exitSlot()));

    timer->start(100);
  }

  return app.exec();
}
