#include <QDebug>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonObject>
#include "Vkontakte.h"
#include "Messages.h"

Messages::Messages(QNetworkAccessManager *nam, QObject *parent) : QAbstractListModel(parent) {
  this->nam = nam;
  this->a = Account();
  this->f = Friend();
  connect(nam, SIGNAL(finished(QNetworkReply*)), this, SLOT(handleReply(QNetworkReply*)));
}

int Messages::rowCount(const QModelIndex &parent) const {
  return messages.size();
}

QVariant Messages::data(const QModelIndex &index, int role) const {
  if (!index.isValid()) {
    return QVariant();
  }
  if (index.row() >= messages.size()) {
    return QVariant();
  }
  if (role == Qt::DisplayRole) {
    return messages.at(index.row()).getDisplayText();
  } else {
    return QVariant();
  }
}

void Messages::setAccount(const Account &account) {
  this->a = account;
  this->f = new Friend();
  loadHistory();
}

void Messages::setFriend(const Friend &frnd) {
  this->f = frnd;
  loadHistory();
}

void Messages::loadHistory(const QString &lastMid) {
  this->beginResetModel();
  messages.clear();
  this->endResetModel();
  if (!a.isSaved() || !f.isValid()) {
    return;
  }
  qDebug() << "Load history for" << a.getUid() << "and" << f.getUid();
  QUrlQuery params;
  params.addQueryItem("uid", f.getUid());
  params.addQueryItem("access_token", a.getAccessToken());
  params.addQueryItem("rev", "0");
  if (!lastMid.isEmpty()) {
    params.addQueryItem("mid", lastMid);
  }
  QUrl url = Vkontakte::methodUrl("messages.getHistory");
  url.setQuery(params.query());
  this->historyReply = nam->get(QNetworkRequest(url));
}

void Messages::sendMessage(const Account &a, const Friend &f, const QString &text) {
  qDebug() << "Send" << text << "from" << a.getUid() << "to" << f.getFullName() << "(" << f.getUid() << ")";
  QUrlQuery params;
  params.addQueryItem("uid", f.getUid());
  params.addQueryItem("message", text);
  params.addQueryItem("access_token", a.getAccessToken());
  QUrl url = Vkontakte::methodUrl("messages.send");
  url.setQuery(params.query());
  this->sendingReply = nam->get(QNetworkRequest(url));
}

void Messages::handleReply(QNetworkReply *reply) {
  if (reply && reply == sendingReply) {
    qDebug() << reply->readAll();
    this->sendingReply = 0;
    delete reply;
  } else if (reply && reply == historyReply) {
    QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
    QJsonArray response = doc.object().value("response").toArray();
    this->beginResetModel();
    messages.clear();
    for (int i = response.size() - 1; i >= 0; --i) {
      QJsonObject o = response.at(i).toObject();
      Message m;
      m.setMid(o.value("mid").toVariant().toString());
      m.setUid(o.value("uid").toVariant().toString());
      m.setText(o.value("body").toVariant().toString());
      switch (o.value("out").toVariant().toInt()) {
        case 0:
          m.setType(Message::INCOMING_MESSAGE);
          break;
        case 1:
          m.setType(Message::OUTGOING_MESSAGE);
          break;
        default:
          m.setType(Message::UNKNOWN_TYPE);
      }
      messages.append(m);
    }
    this->endResetModel();
    this->historyReply = 0;
    delete reply;
  }
}
