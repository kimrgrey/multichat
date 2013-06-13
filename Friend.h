#ifndef FRIEND_H
#define FRIEND_H

#include <QObject>
#include <QUrl>

class Friend : public QObject {
  Q_OBJECT
public:
  Friend(QObject *parent = 0);
  Friend(const Friend &f);
  Friend& operator=(const Friend &f);
  void setUid(const QString &uid);
  QString getUid() const;
  void setPhotoUrls(const QString &smallUrl, const QString &mediumUrl, const QString &bigUrl);
  void setPhotoUrls(const QUrl &smallUrl, const QUrl &mediumUrl, const QUrl &bigUrl);
  void setSmallPhotoUrl(const QString &url);
  void setSmallPhotoUrl(const QUrl &url);
  QUrl getSmallPhotoUrl() const;
  void setMediumPhotoUrl(const QString &url);
  void setMediumPhotoUrl(const QUrl &url);
  QUrl getMediumPhotoUrl() const;
  void setBigPhotoUrl(const QString &url);
  void setBigPhotoUrl(const QUrl &url);
  QUrl getBigPhotoUrl() const;
  void setFullName(const QString &firstName, const QString &lastName);
  QString getFullName() const;
  void setFirstName(const QString &firstName);
  QString getFirstName() const;
  void setLastName(const QString &lastName);
  QString getLastName() const;
  bool isValid() const;
private:
  QString uid;
  QString firstName;
  QString lastName;
  QUrl smallPhotoUrl;
  QUrl mediumPhotoUrl;
  QUrl bigPhotoUrl;
};

#endif // FRIEND_H
