#ifndef VKONTAKTE_H
#define VKONTAKTE_H

#include <QObject>
#include <QList>
#include <QStringList>
#include <QMap>
#include <QUrl>

class Vkontakte : public QObject {
public:
  static QString AUTHORIZE_URL;
  static QString CALLBACK_URL;
  static QString METHOD_URL;
  static QMap<QString, QString> extractFragments(const QUrl &url);
  static QUrl authorizeUrl(const QString &appId, const QString &permissions = QString());
  static QUrl authorizeUrl(const QString &appId, const QStringList &permissions = QStringList());
  static QUrl methodUrl(const QString &methodName);
  static bool isCallbackUrl(const QUrl &url);
  static bool isErrorCallback(const QUrl &url);
  static QString extractErrorMessage(const QUrl &url);
  static QString extractToken(const QUrl &url);
public:
  Vkontakte(QObject * parent = 0);
};

#endif // VKONTAKTE_H
