#include <CQApp.h>
#include <CArgs.h>
#include <CBrowserMain.h>
#include <QTimer>

int
main(int argc, char **argv)
{
  CQApp app(argc, argv);

  CArgs cargs("-debug:f -use_alt:f -batch:f");

  cargs.parse(&argc, argv);

  bool debug   = cargs.getBooleanArg("-debug");
  bool use_alt = cargs.getBooleanArg("-use_alt");
  bool batch   = cargs.getBooleanArg("-batch");

  CBrowserMain *browser = CBrowserMainInst;

  browser->setDebug (debug);
  browser->setUseAlt(use_alt);

  std::string fileName;

  if (argc > 1)
    fileName = argv[1];

  browser->openDocument(fileName);

  if (batch) {
    QTimer *timer = new QTimer;

    QObject::connect(timer, SIGNAL(timeout()), browser, SLOT(exitSlot()));

    timer->start(100);
  }

  return app.exec();
}
