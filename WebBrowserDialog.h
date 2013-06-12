#ifndef WEBBROWSERDIALOG_H
#define WEBBROWSERDIALOG_H

#include <QDialog>
#include <QUrl>

class QWebView;

class WebBrowserDialog : public QDialog {
  Q_OBJECT
public:
  WebBrowserDialog(QWidget *parent = 0);
public slots:
  void openAuthorizePage();
  void handleUrlChange(const QUrl &url);
private:
  void setupDefaults();
  void createLayout();
private:
  QWebView *webview;
};

#endif // WEBBROWSERDIALOG_H
