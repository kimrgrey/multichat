#include <QDebug>
#include <QUrl>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QSettings>
#include <QWebView>
#include <QMessageBox>
#include "Vkontakte.h"
#include "Accounts.h"
#include "Account.h"
#include "WebBrowserDialog.h"

WebBrowserDialog::WebBrowserDialog(Accounts *accounts, QWidget *parent) : QDialog(parent) {
  this->accounts = accounts;
  this->webview = new QWebView();
  connect(webview, SIGNAL(urlChanged(const QUrl&)), this, SLOT(handleUrlChange(const QUrl&)));
  this->setupDefaults();
  this->createLayout();
}

void WebBrowserDialog::openAuthorizePage() {
  QSettings settings;
  QString appId = settings.value("authorization/appId").toString();
  QString permissions = settings.value("authorization/permissions").toString();
  webview->load(Vkontakte::authorizeUrl(appId, permissions));
}

void WebBrowserDialog::handleUrlChange(const QUrl &url) {
  if (Vkontakte::isCallbackUrl(url)) {
    webview->hide();
    if (Vkontakte::isErrorCallback(url)) {
      QMessageBox::warning(this, QString::fromUtf8("Авторизация провалена!"), Vkontakte::extractErrorMessage(url));
      this->reject();
    } else {
      accounts->add(Account::create(url));
      this->accept();
    }
  }
}

void WebBrowserDialog::setupDefaults() {
  this->setWindowTitle(QString::fromUtf8("Авторизация - Мультичат ВК"));
  this->setMinimumSize(500, 300);
}

void WebBrowserDialog::createLayout() {
  QVBoxLayout *mainLayout = new QVBoxLayout(this);
  mainLayout->addWidget(webview);
}
