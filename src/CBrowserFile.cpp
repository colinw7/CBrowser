#include <CBrowserFile.h>
#include <CBrowserWindow.h>
#include <CBrowserCeil.h>
#include <CBrowserJS.h>
#include <CHtmlUtil.h>
#include <CHtml.h>
#include <CUrl.h>
#include <CTempFile.h>
#include <CFileUtil.h>
#include <CDir.h>
#include <CEnv.h>

class CBrowserHtmlFileMgr : public CHtmlFileMgr {
 public:
  std::string getTypeName (CFileType file_type);
  std::string getTypeImage(CFileType file_type);
};

CBrowserFileMgr::
CBrowserFileMgr(CBrowserWindow *window) :
 window_(window)
{
}

bool
CBrowserFileMgr::
readURL(const std::string &url_name, CHtmlParserTokens &tokens)
{
  CUrl url(url_name);

  std::string prefix = url.getPrefix();
  std::string file   = url.getFile();
  std::string target = url.getTarget();

  if      (prefix == "ceil")
    readScript(file, tokens);
  else if (prefix == "file") {

    if (! CFile::exists(file)) {
      window_->errorDialog("File '" + file + "' does not exist");
      return false;
    }
    else {
      if (! readFile(file, tokens))
        return false;
    }
  }
  else if (prefix == "http") {
    std::string      site     = url.getSite    ();
    CUrl::SearchList searches = url.getSearches();

    if (! readHttp(site, file, target, searches, tokens))
      return false;
  }
  else
    return false;

  if (target != "")
    window_->setTarget(target);

  return true;
}

bool
CBrowserFileMgr::
readHttp(const std::string &site, const std::string &file,
         const std::string & /*target*/, const CUrl::SearchList & /*searches*/,
         CHtmlParserTokens &tokens)
{
  if (site != "localhost")
    return false;

  std::string resource = "cgi-bin+";

  resource += site;

  char *dir = getenv(resource.c_str());

  if (dir == NULL)
    return false;

  std::string url = "file://";

  url += dir;
  url += "/";
  url += file;

  CEnvInst.set("HTTP_USER_AGENT"  , "html");
  CEnvInst.set("DOCUMENT_ROOT"    , "/var/lib/httpd/htdocs");
  CEnvInst.set("SERVER_SOFTWARE"  , "None");
  CEnvInst.set("SERVER_NAME"      , "None");
  CEnvInst.set("GATEWAY_INTERFACE", "CGI/1.1");
  CEnvInst.set("SERVER_PROTOCOL"  , "HTTP/1.0");
  CEnvInst.set("SERVER_PORT"      , "None");
  CEnvInst.set("REQUEST_METHOD"   , "GET");
  CEnvInst.set("HTTP_ACCEPT"      , "*/*");
  CEnvInst.set("PATH_INFO"        , "");
  CEnvInst.set("PATH_TRANSLATED"  , "");
  CEnvInst.set("SCRIPT_NAME"      , file);
  CEnvInst.set("QUERY_STRING"     , "");
  CEnvInst.set("REMOTE_HOST"      , "localhost");
  CEnvInst.set("REMOTE_ADDR"      , "127.0.0.1");
  CEnvInst.set("REMOTE_USER"      , "");
  CEnvInst.set("AUTH_TYPE"        , "");
  CEnvInst.set("CONTENT_TYPE"     , "");
  CEnvInst.set("CONTENT_LENGTH"   , "");

  return readURL(url, tokens);
}

bool
CBrowserFileMgr::
readScript(const std::string &filename, CHtmlParserTokens &tokens)
{
  std::string str = CBrowserCeilInst->runScriptCommand(window_, filename);

  return readHTMLString(str, tokens);
}

bool
CBrowserFileMgr::
readFile(const std::string &filename, CHtmlParserTokens &tokens)
{
  CFile file(filename);

  CFileType type = CFileUtil::getType(&file);

  if (type & CFILE_TYPE_IMAGE)
    return readImageFile(filename, tokens);

  switch (type) {
    case CFILE_TYPE_INODE_DIR:
      return readDirectory(filename, tokens);
    case CFILE_TYPE_TEXT_HTML:
    case CFILE_TYPE_TEXT_XML:
      return readHTMLFile(filename, tokens);
    case CFILE_TYPE_APP_SH:
      return readScriptFile(filename, tokens);
    case CFILE_TYPE_TEXT_PLAIN:
      return readTextFile(filename, tokens);
    case CFILE_TYPE_TEXT_BINARY:
      return readBinaryFile(filename, tokens);
    default:
      break;
  }

  return false;
}

bool
CBrowserFileMgr::
readDirectory(const std::string &directory, CHtmlParserTokens &tokens)
{
  CDir dir(directory);

  CTempFile temp_file;

  CBrowserHtmlFileMgr mgr;

  CHtmlUtil::listDirectory(dir, *temp_file.getFile(), mgr);

  /*------------*/

  bool flag = readHTMLFile(temp_file.getFile()->getPath(), tokens);

  /*------------*/

  temp_file.getFile()->remove();

  /*------------*/

  return flag;
}

bool
CBrowserFileMgr::
readImageFile(const std::string &filename, CHtmlParserTokens &tokens)
{
  CTempFile temp_file;

  /*------------*/

  CHtmlUtil::listImage(filename, *temp_file.getFile());

  /*------------*/

  bool flag = readHTMLFile(temp_file.getFile()->getPath(), tokens);

  /*------------*/

  temp_file.getFile()->remove();

  /*------------*/

  return flag;

}

bool
CBrowserFileMgr::
readTextFile(const std::string &filename, CHtmlParserTokens &tokens)
{
  CTempFile temp_file;

  /*------------*/

  CHtmlUtil::listTextFile(filename, *temp_file.getFile());

  /*------------*/

  bool flag = readHTMLFile(temp_file.getFile()->getPath(), tokens);

  /*------------*/

  temp_file.getFile()->remove();

  return flag;
}

bool
CBrowserFileMgr::
readBinaryFile(const std::string &filename, CHtmlParserTokens &tokens)
{
  CTempFile temp_file;

  /*------------*/

  CHtmlUtil::listBinaryFile(filename, *temp_file.getFile());

  /*------------*/

  bool flag = readHTMLFile(temp_file.getFile()->getPath(), tokens);

  /*------------*/

  temp_file.getFile()->remove();

  /*------------*/

  return flag;
}

bool
CBrowserFileMgr::
readScriptFile(const std::string &filename, CHtmlParserTokens &tokens)
{
  CTempFile temp_file;

  /*------------*/

  CHtmlUtil::listScriptFile(filename, *temp_file.getFile());

  /*------------*/

  bool flag = readHTMLFile(temp_file.getFile()->getPath(), tokens);

  /*------------*/

  temp_file.getFile()->remove();

  /*------------*/

  return flag;
}

bool
CBrowserFileMgr::
readHTMLString(const std::string &str, CHtmlParserTokens &tokens)
{
  CHtml html;

  if (! html.readString(str, tokens))
    return false;

  return true;
}

bool
CBrowserFileMgr::
readHTMLFile(const std::string &filename, CHtmlParserTokens &tokens)
{
  CHtml html;

  if (! html.read(filename, tokens))
    return false;

  return true;
}

std::string
CBrowserHtmlFileMgr::
getTypeName(CFileType file_type)
{
  if (file_type & CFILE_TYPE_IMAGE)
    return "Image";

  switch (file_type) {
    case CFILE_TYPE_INODE_DIR:
      return "Directory";
      break;
    case CFILE_TYPE_TEXT_HTML:
      return "Html Document";
      break;
    case CFILE_TYPE_APP_SH:
      return "Script";
    default:
      break;
  }

  return "";
}

std::string
CBrowserHtmlFileMgr::
getTypeImage(CFileType file_type)
{
  if (file_type & CFILE_TYPE_IMAGE)
    return "_html_image_file";

  switch (file_type) {
    case CFILE_TYPE_INODE_DIR:
      return "_html_dir_file";
    case CFILE_TYPE_TEXT_HTML:
      return "_html_html_file";
    case CFILE_TYPE_APP_SH:
      return "_html_normal_file";
    default:
      break;
  }

  return "_html_normal_file";
}
