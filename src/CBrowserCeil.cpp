#include <CBrowserCeil.h>
#include <CBrowserWindow.h>
#include <CBrowserDocument.h>
#include <CBrowserFile.h>
#include <CBrowserOutput.h>
#include <CBrowserWindowWidget.h>
#include <CBrowserMain.h>

#include <CTempFile.h>
#include <CCeilL.h>

#include <QMessageBox>

static void             HtmlRunCeilScriptFile
                         (CBrowserWindowIFace *window, const std::string &);
static void             HtmlRunCeilScript
                         (CBrowserWindowIFace *window, const std::string &);
static std::string      HtmlRunCeilScriptCommand
                         (CBrowserWindowIFace *window, const std::string &);
static ClParserValuePtr HtmlScriptDocumentSetTitle
                         (ClParserValuePtr *, uint, void *, int *);
static ClParserValuePtr HtmlScriptDocumentSetBgColor
                         (ClParserValuePtr *, uint, void *, int *);
static ClParserValuePtr HtmlScriptDocumentSetFgColor
                         (ClParserValuePtr *, uint, void *, int *);
static ClParserValuePtr HtmlScriptDocumentSetLinkColor
                         (ClParserValuePtr *, uint, void *, int *);
static ClParserValuePtr HtmlScriptDocumentOpen
                         (ClParserValuePtr *, uint, void *, int *);
static ClParserValuePtr HtmlScriptDocumentWrite
                         (ClParserValuePtr *, uint, void *, int *);
static ClParserValuePtr HtmlScriptDocumentWriteLn
                         (ClParserValuePtr *, uint, void *, int *);
static ClParserValuePtr HtmlScriptDocumentClose
                         (ClParserValuePtr *, uint, void *, int *);
static ClParserValuePtr HtmlScriptWindowOpen
                         (ClParserValuePtr *, uint, void *, int *);
static ClParserValuePtr HtmlScriptWindowClose
                         (ClParserValuePtr *, uint, void *, int *);
static ClParserValuePtr HtmlScriptWindowSetStatus
                         (ClParserValuePtr *, uint, void *, int *);
static ClParserValuePtr HtmlScriptWindowSetTimeout
                         (ClParserValuePtr *, uint, void *, int *);
#if 0
static void             HtmlScriptWindowTimeoutProc
                         (void *);
#endif
static ClParserValuePtr HtmlScriptWindowClearTimeout
                         (ClParserValuePtr *, uint, void *, int *);
static ClParserValuePtr HtmlScriptWindowConfirm
                         (ClParserValuePtr *, uint, void *, int *);
static ClParserValuePtr HtmlScriptWindowAlert
                         (ClParserValuePtr *, uint, void *, int *);
static ClParserValuePtr HtmlScriptWindowPrompt
                         (ClParserValuePtr *, uint, void *, int *);

//------

#define UPROC(a, b, c) \
 { a, reinterpret_cast<ClParserUserFnProc>(b), nullptr, c, nullptr, }

static ClUserFnData
userfn_data[] = {
 UPROC("CBrowserDocumentSetTitle"    , HtmlScriptDocumentSetTitle    , "i,s"  ),
 UPROC("CBrowserDocumentSetBgColor"  , HtmlScriptDocumentSetBgColor  , "i,s"  ),
 UPROC("CBrowserDocumentSetFgColor"  , HtmlScriptDocumentSetFgColor  , "i,s"  ),
 UPROC("CBrowserDocumentSetLinkColor", HtmlScriptDocumentSetLinkColor, "i,s"  ),
 UPROC("CBrowserDocumentOpen"        , HtmlScriptDocumentOpen        , "i,s"  ),
 UPROC("CBrowserDocumentWrite"       , HtmlScriptDocumentWrite       , "i,s"  ),
 UPROC("CBrowserDocumentWriteLn"     , HtmlScriptDocumentWriteLn     , "i,s"  ),
 UPROC("CBrowserDocumentClose"       , HtmlScriptDocumentClose       , "i"    ),
 UPROC("CBrowserWindowOpen"          , HtmlScriptWindowOpen          , "s,s"  ),
 UPROC("CBrowserWindowClose"         , HtmlScriptWindowClose         , "i"    ),
 UPROC("CBrowserWindowSetStatus"     , HtmlScriptWindowSetStatus     , "i,s"  ),
 UPROC("CBrowserWindowSetTimeout"    , HtmlScriptWindowSetTimeout    , "i,s,i"),
 UPROC("CBrowserWindowClearTimeout"  , HtmlScriptWindowClearTimeout  , "i,i"  ),
 UPROC("CBrowserWindowConfirm"       , HtmlScriptWindowConfirm       , "i,s"  ),
 UPROC("CBrowserWindowAlert"         , HtmlScriptWindowAlert         , "i,s"  ),
 UPROC("CBrowserWindowPrompt"        , HtmlScriptWindowPrompt        , "i,s,s"),
 UPROC(nullptr                       , ClParserUserFnProc()          , nullptr),
};

//------

CBrowserCeil *
CBrowserCeil::
instance()
{
  static CBrowserCeil *inst;

  if (! inst)
    inst = new CBrowserCeil;

  return inst;
}

CBrowserCeil::
CBrowserCeil()
{
  ClLanguageMgrInst->init(nullptr, nullptr);

  for (int i = 0; userfn_data[i].name; ++i)
    ClParserInst->createUserFn(userfn_data[i]);
}

CTempFile *
CBrowserCeil::
initOutputDocumentFile()
{
  if (! outputDocumentFile_)
    outputDocumentFile_ = new CTempFile;

  return outputDocumentFile_;
}

void
CBrowserCeil::
deleteOutputDocumentFile()
{
  if (outputDocumentFile_) {
    outputDocumentFile_->getFile()->remove();

    delete outputDocumentFile_;

    outputDocumentFile_ = nullptr;
  }
}

void
CBrowserCeil::
runScriptFile(CBrowserWindowIFace *window, const std::string &file)
{
  HtmlRunCeilScriptFile(window, file);
}

void
CBrowserCeil::
runScript(CBrowserWindowIFace *window, const std::string &text)
{
  HtmlRunCeilScript(window, text);
}

std::string
CBrowserCeil::
runScriptCommand(CBrowserWindowIFace *window, const std::string &cmd)
{
  return HtmlRunCeilScriptCommand(window, cmd);
}

//------

static void HtmlRunCeilScriptLines
             (CBrowserWindowIFace *, std::vector<std::string> &lines, int);

void
HtmlRunCeilScriptFile(CBrowserWindowIFace *window, const std::string &filename)
{
  CFile file(filename);

  std::vector<std::string> lines;

  file.toLines(lines);

  ulong num_lines = lines.size();

  HtmlRunCeilScriptLines(window, lines, num_lines);
}

void
HtmlRunCeilScript(CBrowserWindowIFace *window, const std::string &str)
{
  std::vector<std::string> lines;

  CStrUtil::addLines(str, lines);

  uint num_lines = lines.size();

  HtmlRunCeilScriptLines(window, lines, num_lines);
}

std::string
HtmlRunCeilScriptCommand(CBrowserWindowIFace *window, const std::string &str)
{
  HtmlRunCeilScript(window, str);

  auto value = ClLanguageMgrInst->getLastValue();

  if (! value)
    return nullptr;

  std::string str1;

  if (value->stringValue(str1))
    return nullptr;

  return str1;
}

static void
HtmlRunCeilScriptLines(CBrowserWindowIFace *window, std::vector<std::string> &lines, int num_lines)
{
  auto *document = window->getDocument();

  ClParserInst->createVar("window"  , ClParserValueMgrInst->createValue(long(window  )));
  ClParserInst->createVar("document", ClParserValueMgrInst->createValue(long(document)));

  CBrowserCeilInst->setOutputDocumentFile(nullptr);

  std::string buffer = "";

  for (int i = 0; i < num_lines; i++) {
    int len = lines[i].size();

    if      (len > 0 && lines[i][len - 1] == '\\') {
      lines[i][len - 1] = '\0';

      buffer += lines[i];
    }
    else if (buffer[0] != '\0') {
      buffer += lines[i];

      ClLanguageMgrInst->runCommand(const_cast<char *>(buffer.c_str()));

      buffer = "";
    }
    else
      ClLanguageMgrInst->runCommand(const_cast<char *>(lines[i].c_str()));
  }

  if (buffer[0] != '\0')
    ClLanguageMgrInst->runCommand(const_cast<char *>(buffer.c_str()));

  auto *outputDocumentFile = CBrowserCeilInst->outputDocumentFile();

  if (outputDocumentFile) {
    outputDocumentFile->getFile()->close();

    CHtmlParserTokens tokens;

    window->readFile(outputDocumentFile->getFile()->getPath(), tokens);

    CBrowserOutput output(window);

    output.processTokens(tokens);

    CBrowserCeilInst->deleteOutputDocumentFile();
  }
}

static ClParserValuePtr
HtmlScriptDocumentSetTitle(ClParserValuePtr *, uint, void *, int *)
{
  char *title;
  long  idocument;

  if (ClParserInst->getUserFnArgList(CLArgType::INTEGER, &idocument,
                                     CLArgType::STRING , &title   , -1)) {
    auto *document = reinterpret_cast<CBrowserDocument *>(idocument);

    document->setTitle(title);
  }

  auto value = ClParserValueMgrInst->createValue(0L);

  return value;
}

static ClParserValuePtr
HtmlScriptDocumentSetBgColor(ClParserValuePtr *, uint, void *, int *)
{
  char *color;
  long  idocument;

  if (ClParserInst->getUserFnArgList(CLArgType::INTEGER, &idocument,
                                     CLArgType::STRING , &color  , -1)) {
    auto *document = reinterpret_cast<CBrowserDocument *>(idocument);

    document->setBgColor(color);

    auto *window = document->getWindow();

    window->resize();
  }

  auto value = ClParserValueMgrInst->createValue(0L);

  return value;
}

static ClParserValuePtr
HtmlScriptDocumentSetFgColor(ClParserValuePtr *, uint, void *, int *)
{
  char *color;
  long  idocument;

  if (ClParserInst->getUserFnArgList(CLArgType::INTEGER, &idocument,
                                     CLArgType::STRING , &color  , -1)) {
    auto *document = reinterpret_cast<CBrowserDocument *>(idocument);

    document->setFgColor(color);
  }

  auto value = ClParserValueMgrInst->createValue(0L);

  return value;
}

static ClParserValuePtr
HtmlScriptDocumentSetLinkColor(ClParserValuePtr *, uint, void *, int *)
{
  char *color;
  long  idocument;

  if (ClParserInst->getUserFnArgList(CLArgType::INTEGER, &idocument,
                                     CLArgType::STRING , &color  , -1)) {
    auto *document = reinterpret_cast<CBrowserDocument *>(idocument);

    document->setLinkColor(color);
  }

  auto value = ClParserValueMgrInst->createValue(0L);

  return value;
}

static ClParserValuePtr
HtmlScriptDocumentOpen(ClParserValuePtr *, uint, void *, int *)
{
  char *type;
  long  idocument;

  if (ClParserInst->getUserFnArgList(CLArgType::INTEGER, &idocument,
                                     CLArgType::STRING , &type    , -1)) {
    auto *document = reinterpret_cast<CBrowserDocument *>(idocument);

    auto *outputDocumentFile = CBrowserCeilInst->outputDocumentFile();

    if (outputDocumentFile) {
      outputDocumentFile->getFile()->close();

      auto *window = document->getWindow();

      CHtmlParserTokens tokens;

      window->readFile(outputDocumentFile->getFile()->getPath(), tokens);

      CBrowserOutput output(window);

      output.processTokens(tokens);

      CBrowserCeilInst->deleteOutputDocumentFile();
    }

    CBrowserCeilInst->initOutputDocumentFile();
  }

  auto value = ClParserValueMgrInst->createValue(0L);

  return value;
}

static ClParserValuePtr
HtmlScriptDocumentWrite(ClParserValuePtr *, uint, void *, int *)
{
  char *text;
  long  idocument;

  if (ClParserInst->getUserFnArgList(CLArgType::INTEGER, &idocument,
                                     CLArgType::STRING , &text     , -1)) {
    auto *outputDocumentFile = CBrowserCeilInst->initOutputDocumentFile();

    outputDocumentFile->getFile()->write(text);
  }

  auto value = ClParserValueMgrInst->createValue(0L);

  return value;
}

static ClParserValuePtr
HtmlScriptDocumentWriteLn(ClParserValuePtr *, uint, void *, int *)
{
  char *text;
  long  idocument;

  if (ClParserInst->getUserFnArgList(CLArgType::INTEGER, &idocument,
                                     CLArgType::STRING , &text    , -1)) {
    auto *outputDocumentFile = CBrowserCeilInst->initOutputDocumentFile();

    outputDocumentFile->getFile()->write(text);
    outputDocumentFile->getFile()->write("\n");
  }

  auto value = ClParserValueMgrInst->createValue(0L);

  return value;
}

static ClParserValuePtr
HtmlScriptDocumentClose(ClParserValuePtr *, uint, void *, int *)
{
  long idocument;

  if (ClParserInst->getUserFnArgList(CLArgType::INTEGER, &idocument, -1)) {
    auto *outputDocumentFile = CBrowserCeilInst->outputDocumentFile();

    if (outputDocumentFile) {
      outputDocumentFile->getFile()->close();

      auto *document = reinterpret_cast<CBrowserDocument *>(idocument);

      auto *window = document->getWindow();

      CHtmlParserTokens tokens;

      window->readFile(outputDocumentFile->getFile()->getPath(), tokens);

      CBrowserOutput output(window);

      output.processTokens(tokens);

      CBrowserCeilInst->deleteOutputDocumentFile();
    }
  }

  auto value = ClParserValueMgrInst->createValue(0L);

  return value;
}

static ClParserValuePtr
HtmlScriptWindowOpen(ClParserValuePtr *, uint, void *, int *)
{
  char *text;
  char *name;

  CBrowserWindowIFace *window = nullptr;

  if (ClParserInst->getUserFnArgList(CLArgType::STRING, &text,
                                     CLArgType::STRING, &name, -1)) {
    CUrl url(text);

    CBrowserCeilInst->browser()->setDocument(url);

    //window->setName(name);
  }

  auto value = ClParserValueMgrInst->createValue(long(window));

  return value;
}

static ClParserValuePtr
HtmlScriptWindowClose(ClParserValuePtr *, uint, void *, int *)
{
  long iwindow;

  if (ClParserInst->getUserFnArgList(CLArgType::INTEGER, &iwindow, -1)) {
    auto *window = reinterpret_cast<CBrowserWindowIFace *>(iwindow);

    window->close();
  }

  auto value = ClParserValueMgrInst->createValue(0L);

  return value;
}

static ClParserValuePtr
HtmlScriptWindowSetStatus(ClParserValuePtr *, uint, void *, int *)
{
  long  iwindow;
  char *status;

  if (ClParserInst->getUserFnArgList(CLArgType::INTEGER, &iwindow,
                                     CLArgType::STRING , &status, -1)) {
    auto *window = reinterpret_cast<CBrowserWindowIFace *>(iwindow);

    window->setStatus(status);
  }

  auto value = ClParserValueMgrInst->createValue(0L);

  return value;
}

static ClParserValuePtr
HtmlScriptWindowSetTimeout(ClParserValuePtr *, uint, void *, int *)
{
#if 0
  int   delay;
  long  iwindow;
  char *command;

  QTimer *timer = nullptr;

  if (ClParserInst->getUserFnArgList(CLArgType::INTEGER, &iwindow, CLArgType::STRING, &command,
                                     CLArgType::INTEGER, &delay , -1)) {
    timer = new QTimer(command);

    timer->setInterval(delay);

    connect(timer, SIGNAL(timeout()), this, SLOT(HtmlScriptWindowTimeoutProc()));
  }
#else
  int id = 0;
#endif

  auto value = ClParserValueMgrInst->createValue(long(id));

  return value;
}

#if 0
static void
HtmlScriptWindowTimeoutProc(void *command)
{
  ClLanguageMgrInst->runCommand(const_cast<char *>(command));
}
#endif

static ClParserValuePtr
HtmlScriptWindowClearTimeout(ClParserValuePtr *, uint, void *, int *)
{
#if 0
  long id;
  long iwindow;

  if (ClParserInst->getUserFnArgList(CLArgType::INTEGER, &iwindow, CLArgType::INTEGER, &id, -1))
    delete timer;
#endif

  auto value = ClParserValueMgrInst->createValue(0L);

  return value;
}

static ClParserValuePtr
HtmlScriptWindowConfirm(ClParserValuePtr *, uint, void *, int *)
{
  char *str;
  long  iwindow;

  bool flag = false;

  if (ClParserInst->getUserFnArgList(CLArgType::INTEGER, &iwindow, CLArgType::STRING , &str, -1)) {
    auto *window = reinterpret_cast<CBrowserWindowIFace *>(iwindow);
    auto *widget = dynamic_cast<CBrowserWindowWidget *>(window->widget());

    if (widget)
      flag = QMessageBox::question(widget, "Confirm", str);
  }

  auto value = ClParserValueMgrInst->createValue(long(flag));

  return value;
}

static ClParserValuePtr
HtmlScriptWindowAlert(ClParserValuePtr *, uint, void *, int *)
{
  char *str;
  long  iwindow;

  if (ClParserInst->getUserFnArgList(CLArgType::INTEGER, &iwindow, CLArgType::STRING , &str, -1)) {
    auto *window = reinterpret_cast<CBrowserWindowIFace *>(iwindow);
    auto *widget = dynamic_cast<CBrowserWindowWidget *>(window->widget());

    if (widget)
      QMessageBox::warning(widget, "Alert", str);
  }

  auto value = ClParserValueMgrInst->createValue(0L);

  return value;
}

static ClParserValuePtr
HtmlScriptWindowPrompt(ClParserValuePtr *, uint, void *, int *)
{
  char *str;
  char *prompt;
  long  iwindow;

  std::string reply = "";

  if (ClParserInst->getUserFnArgList(CLArgType::INTEGER, &iwindow,
                                     CLArgType::STRING , &prompt,
                                     CLArgType::STRING , &str , -1)) {
#if 0
    XtSetArg(args[0], XmNtitle      , "Prompt");
    XtSetArg(args[1], XmNlabelString, prompt  );
    XtSetArg(args[2], XmNvalue      , str     );

    auto *window = reinterpret_cast<CBrowserWindowIFace *>(iwindow);
    auto *widget = dynamic_cast<CBrowserWindowWidget *>(window->widget());

    if (widget)
      reply = XAskForString(widget, "prompt", args, 5);
#endif
  }

  ClParserValuePtr value;

  if (reply != "")
    value = ClParserValueMgrInst->createValue(reply);
  else
    value = ClParserValueMgrInst->createValue("<null>");

  return value;
}
