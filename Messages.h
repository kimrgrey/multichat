#ifndef MESSAGES_H
#define MESSAGES_H

#include <QAbstractListModel>
#include <QList>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>
#include "Account.h"
#include "Friend.h"
#include "Message.h"

class Messages : public QAbstractListModel {
    Q_OBJECT
public:
  Messages(QNetworkAccessManager *nam, QObject *parent = 0);
  int rowCount(const QModelIndex &parent = QModelIndex()) const;
  QVariant data(const QModelIndex &index, int role) const;
  void sendMessage(const Account &a, const Friend &f, const QString &text);
  void setAccount(const Account &a);
  void setFriend(const Friend &f);
public slots:
  void loadHistory(const QString& lastMid = QString());
private slots:
  void handleReply(QNetworkReply *reply);
private:
  Account a;
  Friend f;
  QNetworkAccessManager *nam;
  QNetworkReply *historyReply;
  QNetworkReply *sendingReply;
  QList<Message> messages;
};

#endif // MESSAGES_H
