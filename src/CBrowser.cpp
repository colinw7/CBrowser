#include <CQApp.h>
#include <CArgs.h>
#include <CBrowserMain.h>

int
main(int argc, char **argv)
{
  CQApp app(argc, argv);

  CArgs cargs("-debug:f -use_alt:f");

  cargs.parse(&argc, argv);

  bool debug   = cargs.getBooleanArg("-debug");
  bool use_alt = cargs.getBooleanArg("-use_alt");

  CBrowserMain *browser = CBrowserMainInst;

  browser->setDebug (debug);
  browser->setUseAlt(use_alt);

  std::string fileName;

  if (argc > 1)
    fileName = argv[1];

  browser->openDocument(fileName);

  return app.exec();
}
