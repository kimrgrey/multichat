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
  this->initLongPoll();
  this->loadHistory();
}

void Messages::setFriend(const Friend &frnd) {
  this->f = frnd;
  this->loadHistory();
}

void Messages::add(const Message &message) {
  this->beginInsertRows(QModelIndex(), messages.size(), messages.size());
  messages.append(message);
  this->endInsertRows();
}

void Messages::loadHistory(const QString &lastMid) {
  this->beginResetModel();
  messages.clear();
  this->endResetModel();
  if (!a.isSaved() || !f.isValid()) {
    return;
  }
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
  QUrlQuery params;
  params.addQueryItem("uid", f.getUid());
  params.addQueryItem("message", text);
  params.addQueryItem("access_token", a.getAccessToken());
  QUrl url = Vkontakte::methodUrl("messages.send");
  url.setQuery(params.query());
  this->sendingReply = nam->get(QNetworkRequest(url));
  Message message;
  message.setUid(f.getUid());
  message.setType(Message::OUTGOING_MESSAGE);
  message.setText(text);
  this->add(message);
}

void Messages::initLongPoll() {
  QUrlQuery params;
  params.addQueryItem("use_ssl", "1");
  params.addQueryItem("need_pts", "0");
  params.addQueryItem("access_token", a.getAccessToken());
  QUrl url = Vkontakte::methodUrl("messages.getLongPollServer");
  url.setQuery(params.query());
  this->pollReply = nam->get(QNetworkRequest(url));
}

void Messages::startLongPoll() {
  QUrlQuery params;
  params.addQueryItem("act", "a_check");
  params.addQueryItem("key", pollKey);
  params.addQueryItem("ts", pollTs);
  params.addQueryItem("wait", "25");
  params.addQueryItem("mode", "2");
  QUrl url(QString("https://%1").arg(pollServer));
  url.setQuery(params.query());
  this->pollReply = nam->get(QNetworkRequest(url));
}

void Messages::handleReply(QNetworkReply *reply) {
  if (reply == pollReply) {
    QJsonObject doc = QJsonDocument::fromJson(reply->readAll()).object();
    if (doc.keys().contains("response", Qt::CaseInsensitive)) {
      QJsonObject response = doc.value("response").toObject();
      this->pollServer = response.value("server").toVariant().toString();
      this->pollKey = response.value("key").toVariant().toString();
      this->pollTs = response.value("ts").toVariant().toString();
      this->startLongPoll();
    } else if (doc.keys().contains("ts", Qt::CaseInsensitive)){
      this->pollTs = doc.value("ts").toVariant().toString();
      QJsonArray events = doc.value("updates").toArray();
      for (int i = 0; i < events.size(); ++i) {
        QJsonArray event = events.at(i).toArray();
        int eventType = event.first().toVariant().toInt();
        if (eventType == 4) {
          QString senderUid = event.at(3).toVariant().toString();
          if (f.isValid() && f.getUid() == senderUid) {
            Message message;
            message.setMid(event.at(1).toVariant().toString());
            message.setUid(event.at(3).toVariant().toString());
            message.setText(event.at(6).toVariant().toString());
            message.setType(Message::INCOMING_MESSAGE);
            this->add(message);
          }
        }
      }
      this->startLongPoll();
    } else if (doc.keys().contains("failed", Qt::CaseInsensitive)) {
      this->initLongPoll();
    }
    reply->deleteLater();
  } else if (reply == sendingReply) {
    // TODO We should set mid for sent message here
    reply->deleteLater();
  } else if (reply == historyReply) {
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
    reply->deleteLater();
  }
}
