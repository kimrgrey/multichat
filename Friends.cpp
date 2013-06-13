#include <QDebug>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonObject>
#include "Friends.h"

Friends::Friends(QNetworkAccessManager *nam, QObject *parent) : QAbstractListModel(parent) {
  this->nam = nam;
  connect(nam, SIGNAL(finished(QNetworkReply*)),this, SLOT(handleReply(QNetworkReply*)));
}

int Friends::rowCount(const QModelIndex &parent) const {
  return friends.size();
}

QVariant Friends::data(const QModelIndex &index, int role) const {
  if (!index.isValid()) {
    return QVariant();
  }
  if (index.row() >= friends.size()) {
    return QVariant();
  }
  if (role == Qt::DisplayRole) {
    return friends.at(index.row()).getFullName();
  } else {
    return QVariant();
  }
}

Friend Friends::at(int index) const {
  if (index < 0 || index >= friends.size()) {
    return new Friend();
  }
  return friends.at(index);
}

void Friends::setAccount(const Account &account) {
  if (account.isSaved()) {
    QUrlQuery params;
    params.addQueryItem("uid", account.getUid());
    params.addQueryItem("access_token", account.getAccessToken());
    params.addQueryItem("order", "name");
    params.addQueryItem("fields", "uid,first_name,last_name,photo,photo_medium,photo_big");
    QUrl url = Vkontakte::methodUrl("friends.get");
    url.setQuery(params.query());
    this->reply = nam->get(QNetworkRequest(url));
  } else {
    this->beginResetModel();
    friends.clear();
    this->endResetModel();
  }
}

bool Friends::checkReply(QNetworkReply *r) {
    return reply == r;
}

void Friends::handleReply(QNetworkReply *reply) {
  if (!checkReply(reply)) {
    return;
  }
  QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
  QJsonArray response = doc.object().value("response").toArray();
  this->beginResetModel();
  friends.clear();
  for (int i = 0; i < response.size(); ++i) {
      QJsonObject o = response.at(i).toObject();
      Friend f;
      f.setFullName(o.value("first_name").toVariant().toString(), o.value("last_name").toVariant().toString());
      f.setUid(o.value("uid").toVariant().toString());
      friends.append(f);
  }
  this->endResetModel();
  delete reply;
}
