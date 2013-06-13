#ifndef MESSAGE_H
#define MESSAGE_H

#include <QObject>

class Message : public QObject {
  Q_OBJECT
public:
  enum MessageType {
    INCOMING_MESSAGE = 0,
    OUTGOING_MESSAGE = 1,
    UNKNOWN_TYPE = 3
  };
public:
  Message(QObject *parent = 0);
  Message(const Message &message);
  Message& operator=(const Message &message);
  bool operator==(const Message &message);
  void setMid(const QString &mid);
  QString getMid() const;
  void setUid(const QString &uid);
  QString getUid() const;
  void setText(const QString &text);
  QString getText() const;
  void setType(MessageType type);
  MessageType getType() const;
  bool isValid() const;
  QString getDisplayText() const;
private:
  QString mid;
  QString uid;
  QString text;
  MessageType type;
};

#endif // MESSAGE_H
