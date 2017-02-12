#include <CBrowserWebView.h>
#include <CFile.h>
#include <QVBoxLayout>
#include <QWebView>
#include <iostream>

CBrowserWebView::
CBrowserWebView(QWidget *parent) :
 QFrame(parent)
{
  QVBoxLayout *layout = new QVBoxLayout(this);

  view_ = new QWebView;

  layout->addWidget(view_);
}

void
CBrowserWebView::
setFilename(const std::string &filename)
{
  setWindowTitle(filename.c_str());

  //QUrl url = QUrl::fromLocalFile(filename.c_str());

  //url.setScheme("file");
  //url.setPath(filename.c_str());

  //QUrl url(QString("file://%1").arg(filename.c_str()));

//std::cerr << url.toString().toStdString() << std::endl;
  //view_->setUrl(url);

  //view_->load(url);

  CFile file(filename);

  QString str;

  std::string line;

  while (file.readLine(line)) {
    str += line.c_str();
    str += "\n";
  }

  view_->setHtml(str);
}
