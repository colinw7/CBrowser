#include <CBrowserWindow.h>
#include <CBrowserDocument.h>
#include <CBrowserFile.h>
#include <CBrowserOutput.h>
#include <CBrowserIFace.h>
#include <CBrowserWindowWidget.h>
#include <CBrowserCeil.h>
#include <CBrowserJS.h>

#include <CTempFile.h>
#include <CCeilL.h>

#include <QMessageBox>

static void             HtmlRunCeilScriptFile
                         (CBrowserWindow *window, const std::string &);
static void             HtmlRunCeilScript
                         (CBrowserWindow *window, const std::string &);
static std::string      HtmlRunCeilScriptCommand
                         (CBrowserWindow *window, const std::string &);
static ClParserValuePtr HtmlScriptDocumentSetTitle
                         ();
static ClParserValuePtr HtmlScriptDocumentSetBgColor
                         ();
static ClParserValuePtr HtmlScriptDocumentSetFgColor
                         ();
static ClParserValuePtr HtmlScriptDocumentSetLinkColor
                         ();
static ClParserValuePtr HtmlScriptDocumentOpen
                         ();
static ClParserValuePtr HtmlScriptDocumentWrite
                         ();
static ClParserValuePtr HtmlScriptDocumentWriteLn
                         ();
static ClParserValuePtr HtmlScriptDocumentClose
                         ();
static ClParserValuePtr HtmlScriptWindowOpen
                         ();
static ClParserValuePtr HtmlScriptWindowClose
                         ();
static ClParserValuePtr HtmlScriptWindowSetStatus
                         ();
static ClParserValuePtr HtmlScriptWindowSetTimeout
                         ();
#if 0
static void             HtmlScriptWindowTimeoutProc
                         (void *);
#endif
static ClParserValuePtr HtmlScriptWindowClearTimeout
                         ();
static ClParserValuePtr HtmlScriptWindowConfirm
                         ();
static ClParserValuePtr HtmlScriptWindowAlert
                         ();
static ClParserValuePtr HtmlScriptWindowPrompt
                         ();

//------

#define UPROC(a,b,c) \
 { a, (ClParserUserFnProc) b, nullptr, c, nullptr, }

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
 UPROC(nullptr                       , nullptr                       , nullptr),
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
runScriptFile(CBrowserWindow *window, const std::string &file)
{
  HtmlRunCeilScriptFile(window, file);
}

void
CBrowserCeil::
runScript(CBrowserWindow *window, const std::string &text)
{
  HtmlRunCeilScript(window, text);
}

std::string
CBrowserCeil::
runScriptCommand(CBrowserWindow *window, const std::string &cmd)
{
  return HtmlRunCeilScriptCommand(window, cmd);
}

//------

static void HtmlRunCeilScriptLines
             (CBrowserWindow *, std::vector<std::string> &lines, int);

void
HtmlRunCeilScriptFile(CBrowserWindow *window, const std::string &filename)
{
  CFile file(filename);

  std::vector<std::string> lines;

  file.toLines(lines);

  ulong num_lines = lines.size();

  HtmlRunCeilScriptLines(window, lines, num_lines);
}

void
HtmlRunCeilScript(CBrowserWindow *window, const std::string &str)
{
  std::vector<std::string> lines;

  CStrUtil::addLines(str, lines);

  uint num_lines = lines.size();

  HtmlRunCeilScriptLines(window, lines, num_lines);
}

std::string
HtmlRunCeilScriptCommand(CBrowserWindow *window, const std::string &str)
{
  HtmlRunCeilScript(window, str);

  ClParserValuePtr value = ClLanguageMgrInst->getLastValue();

  if (! value.isValid())
    return nullptr;

  std::string str1;

  if (value->stringValue(str1))
    return nullptr;

  return str1;
}

static void
HtmlRunCeilScriptLines(CBrowserWindow *window, std::vector<std::string> &lines, int num_lines)
{
  CBrowserDocument *document = window->getDocument();

  ClParserInst->createVar("window"  , ClParserValueMgrInst->createValue((long) window  ));
  ClParserInst->createVar("document", ClParserValueMgrInst->createValue((long) document));

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

      ClLanguageMgrInst->runCommand((char *) buffer.c_str());

      buffer = "";
    }
    else
      ClLanguageMgrInst->runCommand((char *) lines[i].c_str());
  }

  if (buffer[0] != '\0')
    ClLanguageMgrInst->runCommand((char *) buffer.c_str());

  CTempFile *output_document_file = CBrowserCeilInst->outputDocumentFile();

  if (output_document_file) {
    output_document_file->getFile()->close();

    CHtmlParserTokens tokens;

    HtmlReadFile(output_document_file->getFile()->getPath(), tokens);

    CBrowserOutput output(window);

    output.processTokens(tokens);

    CBrowserCeilInst->deleteOutputDocumentFile();
  }
}

static ClParserValuePtr
HtmlScriptDocumentSetTitle()
{
  char *title;
  long  idocument;

  if (ClParserInst->getUserFnArgList(CL_ARG_TYPE_INTEGER, &idocument,
                                     CL_ARG_TYPE_STRING , &title   , -1)) {
    CBrowserDocument *document = (CBrowserDocument *) idocument;

    document->setTitle(title);
  }

  ClParserValuePtr value = ClParserValueMgrInst->createValue(0L);

  return value;
}

static ClParserValuePtr
HtmlScriptDocumentSetBgColor()
{
  char *color;
  long  idocument;

  if (ClParserInst->getUserFnArgList(CL_ARG_TYPE_INTEGER, &idocument,
                                     CL_ARG_TYPE_STRING , &color  , -1)) {
    CBrowserDocument *document = (CBrowserDocument *) idocument;

    document->setBgColor(color);

    CBrowserWindow *window = document->getWindow();

    window->resize();
  }

  ClParserValuePtr value = ClParserValueMgrInst->createValue(0L);

  return value;
}

static ClParserValuePtr
HtmlScriptDocumentSetFgColor()
{
  char *color;
  long  idocument;

  if (ClParserInst->getUserFnArgList(CL_ARG_TYPE_INTEGER, &idocument,
                                     CL_ARG_TYPE_STRING , &color  , -1)) {
    CBrowserDocument *document = (CBrowserDocument *) idocument;

    document->setFgColor(color);
  }

  ClParserValuePtr value = ClParserValueMgrInst->createValue(0L);

  return value;

}

static ClParserValuePtr
HtmlScriptDocumentSetLinkColor()
{
  char *color;
  long  idocument;

  if (ClParserInst->getUserFnArgList(CL_ARG_TYPE_INTEGER, &idocument,
                                     CL_ARG_TYPE_STRING , &color  , -1)) {
    CBrowserDocument *document = (CBrowserDocument *) idocument;

    document->setLinkColor(color);
  }

  ClParserValuePtr value = ClParserValueMgrInst->createValue(0L);

  return value;
}

static ClParserValuePtr
HtmlScriptDocumentOpen()
{
  char *type;
  long  idocument;

  if (ClParserInst->getUserFnArgList(CL_ARG_TYPE_INTEGER, &idocument,
                                     CL_ARG_TYPE_STRING , &type    , -1)) {
    CBrowserDocument *document = (CBrowserDocument *) idocument;

    CTempFile *output_document_file = CBrowserCeilInst->outputDocumentFile();

    if (output_document_file) {
      output_document_file->getFile()->close();

      CBrowserWindow *window = document->getWindow();

      CHtmlParserTokens tokens;

      HtmlReadFile(output_document_file->getFile()->getPath(), tokens);

      CBrowserOutput output(window);

      output.processTokens(tokens);

      CBrowserCeilInst->deleteOutputDocumentFile();
    }

    CBrowserCeilInst->initOutputDocumentFile();
  }

  ClParserValuePtr value = ClParserValueMgrInst->createValue(0L);

  return value;
}

static ClParserValuePtr
HtmlScriptDocumentWrite()
{
  char *text;
  long  idocument;

  if (ClParserInst->getUserFnArgList(CL_ARG_TYPE_INTEGER, &idocument,
                                     CL_ARG_TYPE_STRING , &text     , -1)) {
    CTempFile *output_document_file = CBrowserCeilInst->initOutputDocumentFile();

    output_document_file->getFile()->write(text);
  }

  ClParserValuePtr value = ClParserValueMgrInst->createValue(0L);

  return value;
}

static ClParserValuePtr
HtmlScriptDocumentWriteLn()
{
  char *text;
  long  idocument;

  if (ClParserInst->getUserFnArgList(CL_ARG_TYPE_INTEGER, &idocument,
                                     CL_ARG_TYPE_STRING , &text    , -1)) {
    CTempFile *output_document_file = CBrowserCeilInst->initOutputDocumentFile();

    output_document_file->getFile()->write(text);
    output_document_file->getFile()->write("\n");
  }

  ClParserValuePtr value = ClParserValueMgrInst->createValue(0L);

  return value;
}

static ClParserValuePtr
HtmlScriptDocumentClose()
{
  long idocument;

  if (ClParserInst->getUserFnArgList(CL_ARG_TYPE_INTEGER, &idocument, -1)) {
    CTempFile *output_document_file = CBrowserCeilInst->outputDocumentFile();

    if (output_document_file) {
      output_document_file->getFile()->close();

      CBrowserDocument *document = (CBrowserDocument *) idocument;

      CBrowserWindow *window = document->getWindow();

      CHtmlParserTokens tokens;

      HtmlReadFile(output_document_file->getFile()->getPath(), tokens);

      CBrowserOutput output(window);

      output.processTokens(tokens);

      CBrowserCeilInst->deleteOutputDocumentFile();
    }
  }

  ClParserValuePtr value = ClParserValueMgrInst->createValue(0L);

  return value;
}

static ClParserValuePtr
HtmlScriptWindowOpen()
{
  char *url;
  char *name;

  CBrowserWindow *window = nullptr;

  if (ClParserInst->getUserFnArgList(CL_ARG_TYPE_STRING, &url ,
                                     CL_ARG_TYPE_STRING, &name, -1)) {
    CBrowserIFace *iface = new CBrowserIFace;

    CBrowserWindow *window = iface->getWindow();

    window->setName(name);

    window->setDocument(url);
  }

  ClParserValuePtr value = ClParserValueMgrInst->createValue((long) window);

  return value;
}

static ClParserValuePtr
HtmlScriptWindowClose()
{
  long window;

  if (ClParserInst->getUserFnArgList(CL_ARG_TYPE_INTEGER, &window, -1))
    ((CBrowserWindow *) window)->close();

  ClParserValuePtr value = ClParserValueMgrInst->createValue(0L);

  return value;
}

static ClParserValuePtr
HtmlScriptWindowSetStatus()
{
  long  window;
  char *status;

  if (ClParserInst->getUserFnArgList(CL_ARG_TYPE_INTEGER, &window,
                                     CL_ARG_TYPE_STRING , &status, -1))
    ((CBrowserWindow *) window)->setStatus(status);

  ClParserValuePtr value = ClParserValueMgrInst->createValue(0L);

  return value;
}

static ClParserValuePtr
HtmlScriptWindowSetTimeout()
{
#if 0
  int   delay;
  long  window;
  char *command;

  QTimer *timer = nullptr;

  if (ClParserInst->getUserFnArgList(CL_ARG_TYPE_INTEGER, &window, CL_ARG_TYPE_STRING, &command,
                                     CL_ARG_TYPE_INTEGER, &delay , -1)) {
    timer = new QTimer(command);

    timer->setInterval(delay);

    connect(timer, SIGNAL(timeout()), this, SLOT(HtmlScriptWindowTimeoutProc()));
  }
#else
  int id = 0;
#endif

  ClParserValuePtr value = ClParserValueMgrInst->createValue((long) id);

  return value;
}

#if 0
static void
HtmlScriptWindowTimeoutProc(void *command)
{
  ClLanguageMgrInst->runCommand((char *) command);
}
#endif

static ClParserValuePtr
HtmlScriptWindowClearTimeout()
{
#if 0
  long id;
  long window;

  if (ClParserInst->getUserFnArgList(CL_ARG_TYPE_INTEGER, &window, CL_ARG_TYPE_INTEGER, &id, -1))
    delete timer;
#endif

  ClParserValuePtr value = ClParserValueMgrInst->createValue(0L);

  return value;
}

static ClParserValuePtr
HtmlScriptWindowConfirm()
{
  char *str;
  long  window;

  bool flag = false;

  if (ClParserInst->getUserFnArgList(CL_ARG_TYPE_INTEGER, &window, CL_ARG_TYPE_STRING , &str, -1))
    flag = QMessageBox::question(((CBrowserWindow *) window)->widget(), "Confirm", str);

  ClParserValuePtr value = ClParserValueMgrInst->createValue(long(flag));

  return value;
}

static ClParserValuePtr
HtmlScriptWindowAlert()
{
  char *str;
  long  window;

  if (ClParserInst->getUserFnArgList(CL_ARG_TYPE_INTEGER, &window, CL_ARG_TYPE_STRING , &str, -1))
    QMessageBox::warning(((CBrowserWindow *) window)->widget(), "Alert", str);

  ClParserValuePtr value = ClParserValueMgrInst->createValue(0L);

  return value;
}

static ClParserValuePtr
HtmlScriptWindowPrompt()
{
  char *str;
  char *prompt;
  long  window;

  std::string reply = "";

  if (ClParserInst->getUserFnArgList(CL_ARG_TYPE_INTEGER, &window,
                                     CL_ARG_TYPE_STRING , &prompt,
                                     CL_ARG_TYPE_STRING , &str , -1)) {
#if 0
    XtSetArg(args[0], XmNtitle      , "Prompt");
    XtSetArg(args[1], XmNlabelString, prompt  );
    XtSetArg(args[2], XmNvalue      , str     );

    reply = XAskForString(((CBrowserWindow *) window)->widget(), "prompt", args, 5);
#endif
  }

  ClParserValuePtr value;

  if (reply != "")
    value = ClParserValueMgrInst->createValue(reply);
  else
    value = ClParserValueMgrInst->createValue("<null>");

  return value;
}
