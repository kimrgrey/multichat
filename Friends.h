#ifndef FRIENDS_H
#define FRIENDS_H

#include <QAbstractListModel>
#include <QList>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>
#include "Account.h"
#include "Friend.h"

class Friends : public QAbstractListModel {
  Q_OBJECT
public:
  Friends(QNetworkAccessManager *nam, QObject *parent = 0);
  int rowCount(const QModelIndex &parent = QModelIndex()) const;
  QVariant data(const QModelIndex &index, int role) const;
  void setAccount(const Account &account);
  Friend at(int index) const;
private:
  bool checkReply(QNetworkReply *r);
private slots:
  void handleReply(QNetworkReply *reply);
private:
  QNetworkAccessManager *nam;
  QNetworkReply *reply;
  QList<Friend> friends;
};


#endif
