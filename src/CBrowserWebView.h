#ifndef CBrowserWebView_H
#define CBrowserWebView_H

#include <QFrame>

class QWebView;

class CBrowserWebView : public QFrame {
  Q_OBJECT

 public:
  explicit CBrowserWebView(QWidget *parent=0);

  void setFilename(const std::string &filename);

 private:
  QWebView *view_ { nullptr };
};

#endif
