#include <QDebug>
#include <QUrlQuery>
#include "Vkontakte.h"
#include "Messages.h"

Messages::Messages(QNetworkAccessManager *nam, QObject *parent) : QObject(parent) {
  this->nam = nam;
  connect(nam, SIGNAL(finished(QNetworkReply*)), this, SLOT(handleResult(QNetworkReply*)));
}

void Messages::sendMessage(const Account &a, const Friend &f, const QString &text) {
  qDebug() << "Send" << text << "from" << a.getUid() << "to" << f.getFullName() << "(" << f.getUid() << ")";
  QUrlQuery params;
  params.addQueryItem("uid", f.getUid());
  params.addQueryItem("message", text);
  params.addQueryItem("access_token", a.getAccessToken());
  QUrl url = Vkontakte::methodUrl("messages.send");
  url.setQuery(params.query());
  this->reply = nam->get(QNetworkRequest(url));
}

bool Messages::checkReply(QNetworkReply *r) {
  return reply == r;
}

void Messages::handleResult(QNetworkReply *reply) {
  if (!checkReply(reply)) {
    return;
  }
  qDebug() << reply->readAll();
  delete reply;
}
