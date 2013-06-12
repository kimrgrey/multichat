#include "Accounts.h"

Accounts::Accounts(QObject *parent) : QAbstractListModel(parent) {
  this->accounts = Account::list();
}

int Accounts::rowCount(const QModelIndex &parent) const {
  return accounts.size();
}

QVariant Accounts::data(const QModelIndex &index, int role) const {
  if (!index.isValid()) {
    return QVariant();
  }
  if (index.row() >= accounts.size()) {
    return QVariant();
  }
  if (role == Qt::DisplayRole) {
    return accounts.at(index.row()).getUid();
  } else {
    return QVariant();
  }
}

Account Accounts::at(int index) const {
  return accounts.at(index);
}
