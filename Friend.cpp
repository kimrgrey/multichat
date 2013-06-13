#include "Friend.h"

Friend::Friend(QObject *parent) : QObject(parent) {
  this->uid = QString();
  this->firstName = QString();
  this->lastName = QString();
  this->smallPhotoUrl = QUrl();
  this->mediumPhotoUrl = QUrl();
  this->bigPhotoUrl = QUrl();
}

Friend::Friend(const Friend &f) : QObject(f.parent()) {
  this->uid = f.uid;
  this->firstName = f.firstName;
  this->lastName = f.lastName;
  this->smallPhotoUrl = f.smallPhotoUrl;
  this->mediumPhotoUrl = f.mediumPhotoUrl;
  this->bigPhotoUrl = f.bigPhotoUrl;
}

Friend& Friend::operator=(const Friend &f) {
  this->uid = f.uid;
  this->firstName = f.firstName;
  this->lastName = f.lastName;
  this->smallPhotoUrl = f.smallPhotoUrl;
  this->mediumPhotoUrl = f.mediumPhotoUrl;
  this->bigPhotoUrl = f.bigPhotoUrl;
  return *this;
}

void Friend::setUid(const QString &uid) {
  this->uid = uid;
}

QString Friend::getUid() const {
  return uid;
}

void Friend::setPhotoUrls(const QString &smallUrl, const QString &mediumUrl, const QString &bigUrl) {
  this->setSmallPhotoUrl(smallUrl);
  this->setMediumPhotoUrl(mediumUrl);
  this->setBigPhotoUrl(bigUrl);
}

void Friend::setPhotoUrls(const QUrl &smallUrl, const QUrl &mediumUrl, const QUrl &bigUrl) {
  this->setSmallPhotoUrl(smallUrl);
  this->setMediumPhotoUrl(mediumUrl);
  this->setBigPhotoUrl(bigUrl);
}

void Friend::setSmallPhotoUrl(const QString &url) {
  this->setSmallPhotoUrl(QUrl(url));
}

void Friend::setSmallPhotoUrl(const QUrl &url) {
  this->smallPhotoUrl = url;
}

QUrl Friend::getSmallPhotoUrl() const {
  return smallPhotoUrl;
}

void Friend::setMediumPhotoUrl(const QString &url) {
  this->setMediumPhotoUrl(QUrl(url));
}

void Friend::setMediumPhotoUrl(const QUrl &url) {
  this->mediumPhotoUrl = url;
}

QUrl Friend::getMediumPhotoUrl() const {
  return mediumPhotoUrl;
}

void Friend::setBigPhotoUrl(const QString &url) {
  this->setBigPhotoUrl(QUrl(url));
}

void Friend::setBigPhotoUrl(const QUrl &url) {
  this->bigPhotoUrl = url;
}

QUrl Friend::getBigPhotoUrl() const {
  return bigPhotoUrl;
}

void Friend::setFullName(const QString &firstName, const QString &lastName) {
  this->setFirstName(firstName);
  this->setLastName(lastName);
}

QString Friend::getFullName() const {
  return QString("%1 %2").arg(firstName).arg(lastName);
}

void Friend::setFirstName(const QString &firstName) {
  this->firstName = firstName;
}

QString Friend::getFirstName() const {
  return firstName;
}

void Friend::setLastName(const QString &lastName) {
  this->lastName = lastName;
}

QString Friend::getLastName() const {
  return lastName;
}

bool Friend::isValid() const {
  return !uid.isEmpty();
}


