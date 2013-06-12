#include <QDebug>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QListView>
#include <QPushButton>
#include "Account.h"
#include "Accounts.h"
#include "WebBrowserDialog.h"
#include "AccountsDialog.h"

AccountsDialog::AccountsDialog(Accounts *accounts, QWidget *parent) : QDialog(parent) {
  this->accounts = accounts;
  this->webBrowserDialog = new WebBrowserDialog(this);
  this->createAccountsList();
  this->setupDefaults();
  this->createLayout();
}

void AccountsDialog::createAccountsList() {
  this->accountsList = new QListView();
  accountsList->setModel(accounts);
}

void AccountsDialog::createLayout() {
  QVBoxLayout *actionsLayout = new QVBoxLayout();
  QPushButton *addAccountButton = new QPushButton(QString::fromUtf8("Добавить"));
  connect(addAccountButton, SIGNAL(clicked()), this, SLOT(openBrowserWindow()));
  actionsLayout->addWidget(addAccountButton);
  // TODO Sad, but removing is not working in this version of code =(
  actionsLayout->addWidget(new QPushButton(QString::fromUtf8("Удалить")));
  actionsLayout->addStretch();
  QHBoxLayout *topLayout = new QHBoxLayout();
  topLayout->addWidget(accountsList);
  topLayout->addLayout(actionsLayout);
  QHBoxLayout *bottomLayout = new QHBoxLayout();
  bottomLayout->addStretch();
  QPushButton *closeButton = new QPushButton(QString::fromUtf8("Закрыть"));
  connect(closeButton, SIGNAL(clicked()), this, SLOT(accept()));
  bottomLayout->addWidget(closeButton);
  QVBoxLayout *mainLayout = new QVBoxLayout(this);
  mainLayout->addLayout(topLayout);
  mainLayout->addLayout(bottomLayout);
}

void AccountsDialog::openBrowserWindow() {
  webBrowserDialog->openAuthorizePage();
  webBrowserDialog->exec();
}

void AccountsDialog::setupDefaults() {
  this->setWindowTitle(QString::fromUtf8("Менеджер аккаунтов - Мультичат ВК"));
  this->setMinimumSize(500, 300);
}
