#include <QDebug>
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
  if (index < 0 || index >= accounts.size()) {
    return Account();
  }
  return accounts.at(index);
}

void Accounts::add(const Account &account) {
  this->beginInsertRows(QModelIndex(), accounts.size(), accounts.size());
  accounts.append(account);
  this->endInsertRows();
}

void Accounts::remove(int index) {
  if (index < 0 || index >= accounts.size()) {
    return;
  }
  Account account = accounts.at(index);
  if (account.destroy()) {
    beginRemoveRows(QModelIndex(), index, index);
    accounts.removeAt(index);
    endRemoveRows();
  }
}
