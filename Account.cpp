#include <QDebug>
#include <QMap>
#include <QSqlQuery>
#include <QUrlQuery>
#include "Account.h"

QString Account::AUTHORIZE_URL = "https://oauth.vk.com/authorize";
QString Account::CALLBACK_URL = "https://oauth.vk.com/blank.html";
QString Account::METHOD_URL = "https://api.vk.com/method";

QMap<QString, QString> Account::extractFragments(const QUrl &url) {
  QStringList fragmentsList = url.fragment().split("&");
  QMap<QString, QString> fragmentsMap;
  for (int i = 0; i < fragmentsList.size(); ++i) {
    QStringList fragment = fragmentsList[i].split("=");
    fragmentsMap.insert(fragment.first(), fragment.last());
  }
  return fragmentsMap;
}

QUrl Account::authorizeUrl(const QString &appId, const QString &permissions) {
  QUrlQuery params;
  params.addQueryItem("client_id", appId);
  params.addQueryItem("scope", permissions);
  params.addQueryItem("redirect_uri", CALLBACK_URL);
  params.addQueryItem("display", "page");
  params.addQueryItem("response_type", "token");
  QUrl url(AUTHORIZE_URL);
  url.setQuery(params);
  return url;
}

QUrl Account::authorizeUrl(const QString &appId, const QStringList &permissions) {
  return authorizeUrl(appId, permissions.join(","));
}

QUrl Account::methodUrl(const QString& methodName) {
  return QString("%1/%2").arg(METHOD_URL).arg(methodName);
}

bool Account::isCallbackUrl(const QUrl &url) {
  QUrl callbackUrl(CALLBACK_URL);
  return url.host() == callbackUrl.host() && url.path() == callbackUrl.path();
}

bool Account::isErrorCallback(const QUrl &url) {
  return extractFragments(url).contains("error");
}

QString Account::extractErrorMessage(const QUrl& url) {
  return extractFragments(url).value("error_description");
}

QString Account::extractToken(const QUrl &url) {
  return extractFragments(url).value("access_token");
}

Account Account::create(const QString &accessToken, const QString &uid, int expirationPeriod) {
  Account account(accessToken, uid, expirationPeriod);
  if (!account.save()) {
    // TODO I think exception will look pretty nice here, but Qt is against it... =(
  }
  return account;
}

Account Account::create(const QUrl &url) {
  QMap<QString, QString> fragments = Account::extractFragments(url);
  return create(fragments["access_token"], fragments["user_id"], fragments["expires_in"].toInt());
}

QList<Account> Account::list() {
  QList<Account> accounts;
  QSqlQuery query("SELECT id, access_token, uid, expiration_period FROM accounts");
  while (query.next()) {
    Account account = Account(query.value(0).toInt(), query.value(1).toString(), query.value(2).toString(), query.value(3).toInt());
    accounts.append(account);
  }
  return accounts;
}

int Account::count() {
  QSqlQuery query("SELECT count(*) FROM accounts");
  if (query.next()) {
    return query.value(0).toInt();
  }
  return 0; // just in case of some errors
}

int Account::lastId() {
  QSqlQuery query;
  query.prepare("SELECT seq FROM sqlite_sequence WHERE name = :table");
  query.bindValue(":table", "accounts");
  if (query.exec() && query.next()) {
   return query.value(0).toInt();
  }
  return -1; // just in case of some errors
}

Account Account::find(int id) {
  QSqlQuery query;
  query.prepare("SELECT id, access_token, uid, expiration_period FROM accounts WHERE id = :id");
  query.bindValue(":id", id);
  if (query.exec() && query.next()) {
    return Account(query.value(0).toInt(), query.value(1).toString(), query.value(2).toString(), query.value(3).toInt());
  }
  return Account(); // just in case of some errors
}

Account::Account(QObject *parent) : QObject(parent) {
  this->id = -1;
  this->accessToken = QString();
  this->uid = QString();
  this->expirationPeriod = -1;
}

Account::Account(const Account &account) : QObject(account.parent()) {
  this->id = account.id;
  this->accessToken = account.accessToken;
  this->uid = account.uid;
  this->expirationPeriod = account.expirationPeriod;
}

Account::Account(const QString &accessToken, const QString &uid, int expirationPeriod, QObject *parent) : QObject(parent) {
  this->id = -1;
  this->accessToken = accessToken;
  this->uid = uid;
  this->expirationPeriod = expirationPeriod;
}

Account::Account(int id, const QString &accessToken, const QString &uid, int expirationPeriod, QObject *parent) : QObject(parent) {
  this->id = id;
  this->accessToken = accessToken;
  this->uid = uid;
  this->expirationPeriod = expirationPeriod;
}

Account& Account::operator=(const Account &account) {
  this->id = account.id;
  this->accessToken = account.accessToken;
  this->uid = account.uid;
  this->expirationPeriod = account.expirationPeriod;
  return *this;
}

bool Account::operator ==(const Account &account) const {
  return id == account.id;
}

bool Account::save() {
  QSqlQuery query;
  query.prepare("INSERT INTO accounts(access_token, uid, expiration_period) VALUES (:access_token, :uid, :expiration_period)");
  query.bindValue(":access_token", accessToken);
  query.bindValue(":uid", uid);
  query.bindValue(":expiration_period", expirationPeriod);
  if (query.exec()) {
    this->id = Account::lastId();
    return true;
  }
  return false;
}

bool Account::destroy() {
  QSqlQuery query;
  query.prepare("DELETE FROM accounts WHERE id = :id");
  query.bindValue(":id", id);
  if (query.exec()) {
    this->id = -1;
    return true;
  }
  return false;
}

bool Account::isSaved() const {
  return id > 0;
}

int Account::getId() const {
  return id;
}

QString Account::getUid() const {
  return uid;
}

void Account::setUid(const QString &uid) {
  this->uid = uid;
}

QString Account::getAccessToken() const {
  return accessToken;
}
