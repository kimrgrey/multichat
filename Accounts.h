#ifndef ACCOUNTS_H
#define ACCOUNTS_H

#include <QAbstractListModel>
#include <QList>
#include "Account.h"

class Accounts : public QAbstractListModel {
  Q_OBJECT
public:
  Accounts(QObject *parent = 0);
  int rowCount(const QModelIndex &parent = QModelIndex()) const;
  QVariant data(const QModelIndex &index, int role) const;
public:
  Account at(int index) const;
private:
  QList<Account> accounts;
};

#endif
