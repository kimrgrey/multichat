#ifndef WEBBROWSERDIALOG_H
#define WEBBROWSERDIALOG_H

#include <QDialog>
#include <QUrl>

class QWebView;
class Accounts;

class WebBrowserDialog : public QDialog {
  Q_OBJECT
public:
  WebBrowserDialog(Accounts *accounts, QWidget *parent = 0);
public slots:
  void openAuthorizePage();
private slots:
  void handleUrlChange(const QUrl &url);
private:
  void setupDefaults();
  void createLayout();
private:
  Accounts *accounts;
  QWebView *webview;
};

#endif // WEBBROWSERDIALOG_H
