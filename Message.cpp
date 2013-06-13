#include "Message.h"

Message::Message(QObject *parent) : QObject(parent) {
  this->mid = QString();
  this->uid = QString();
  this->text = QString();
  this->type = Message::UNKNOWN_TYPE;
}

Message::Message(const Message &message) : QObject(message.parent()) {
  this->mid = message.mid;
  this->uid = message.uid;
  this->text = message.text;
  this->type = message.type;
}

Message& Message::operator=(const Message &message) {
  this->mid = message.mid;
  this->uid = message.uid;
  this->text = message.text;
  this->type = message.type;
  return *this;
}

bool Message::operator==(const Message &message) {
  return isValid() && mid == message.mid;
}

void Message::setMid(const QString &mid) {
  this->mid = mid;
}

QString Message::getMid() const {
  return mid;
}

void Message::setUid(const QString &uid) {
  this->uid = uid;
}

QString Message::getUid() const {
  return uid;
}

void Message::setText(const QString &text) {
  this->text = text;
}

QString Message::getText() const {
  return text;
}

QString Message::getDisplayText() const {
  QString result;
  switch (type) {
    case INCOMING_MESSAGE:
      result = QString(">> %1");
      break;
    case OUTGOING_MESSAGE:
      result = QString("<< %1");
      break;
    default:
      result = QString();
  }
  return result.arg(text);
}

void Message::setType(Message::MessageType type) {
  this->type = type;
}

Message::MessageType Message::getType() const {
  return type;
}

bool Message::isValid() const {
  return !mid.isEmpty();
}
