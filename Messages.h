#ifndef MESSAGES_H
#define MESSAGES_H

#include <QObject>
#include <QList>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>
#include "Account.h"
#include "Friend.h"

class Messages : public QObject {
    Q_OBJECT
public:
  Messages(QNetworkAccessManager *nam, QObject *parent = 0);
  void sendMessage(const Account &a, const Friend &f, const QString &text);
private:
  bool checkReply(QNetworkReply *r);
private slots:
  void handleResult(QNetworkReply *reply);
private:
  QNetworkAccessManager *nam;
  QNetworkReply *reply;
};

#endif // MESSAGES_H
