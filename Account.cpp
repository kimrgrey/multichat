#include <QDebug>
#include <QMap>
#include <QSqlQuery>
#include "Vkontakte.h"
#include "Account.h"

Account Account::create(const QString &accessToken, const QString &uid, int expirationPeriod) {
  Account account(accessToken, uid, expirationPeriod);
  if (!account.save()) {
    // TODO I think exception will look pretty nice here, but Qt is against it... =(
  }
  return account;
}

Account Account::create(const QUrl &url) {
  QMap<QString, QString> fragments = Vkontakte::extractFragments(url);
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

bool Account::save() {
  QSqlQuery query;
  query.prepare("INSERT INTO accounts(access_token, uid, expiration_period) VALUES (:access_token, :uid, :expiration_period)");
  query.bindValue(":access_token", accessToken);
  query.bindValue(":uid", uid);
  query.bindValue(":expiration_period", expirationPeriod);
  return query.exec();
}

bool Account::isSaved() {
  return true;
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
