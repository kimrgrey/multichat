#include <QDebug>
#include <QUrlQuery>
#include "Vkontakte.h"

QString Vkontakte::AUTHORIZE_URL = "https://oauth.vk.com/authorize";
QString Vkontakte::CALLBACK_URL = "https://oauth.vk.com/blank.html";
QString Vkontakte::METHOD_URL = "https://api.vk.com/method";

QMap<QString, QString> Vkontakte::extractFragments(const QUrl &url) {
  QStringList fragmentsList = url.fragment().split("&");
  QMap<QString, QString> fragmentsMap;
  for (int i = 0; i < fragmentsList.size(); ++i) {
    QStringList fragment = fragmentsList[i].split("=");
    fragmentsMap.insert(fragment.first(), fragment.last());
  }
  return fragmentsMap;
}

QUrl Vkontakte::authorizeUrl(const QString &appId, const QString &permissions) {
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

QUrl Vkontakte::authorizeUrl(const QString &appId, const QStringList &permissions) {
  return authorizeUrl(appId, permissions.join(","));
}

QUrl Vkontakte::methodUrl(const QString& methodName) {
  return QString("%1/%2").arg(METHOD_URL).arg(methodName);
}

bool Vkontakte::isCallbackUrl(const QUrl &url) {
  QUrl callbackUrl(CALLBACK_URL);
  return url.host() == callbackUrl.host() && url.path() == callbackUrl.path();
}

bool Vkontakte::isErrorCallback(const QUrl &url) {
  return extractFragments(url).contains("error");
}

QString Vkontakte::extractErrorMessage(const QUrl& url) {
  return extractFragments(url).value("error_description");
}

QString Vkontakte::extractToken(const QUrl &url) {
  return extractFragments(url).value("access_token");
}

Vkontakte::Vkontakte(QObject *parent) : QObject(parent) {
}

