#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <QObject>
#include <QUrl>
#include <QList>

#include "Friend.h"

class Vkontakte;

class Account : public QObject {
public:
  static Account create(const QString &accessToken, const QString &uid, int expirationPeriod);
  static Account create(const QUrl &url);
  static QList<Account> list();
  static Account find(int id);
  static int count();
  static int lastId();
public:
  Account(QObject *parent = 0);
  Account(const Account& account);
  Account(int id, const QString &accessToken, const QString &uid, int expirationPeriod, QObject *parent = 0);
  Account(const QString &accessToken, const QString &uid, int expirationPeriod, QObject *parent = 0);
  Account& operator= (const Account &account);
  bool operator==(const Account &account) const;
  bool save();
  bool destroy();
  bool isSaved() const;
  int getId() const;
  void setUid(const QString &uid);
  QString getUid() const;
  QString getAccessToken() const;
private:
  int id;
  QString accessToken;
  QString uid;
  int expirationPeriod;
};

#endif // ACCOUNT_H
