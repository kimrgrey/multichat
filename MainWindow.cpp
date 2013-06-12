#include <QDebug>
#include <QApplication>
#include <QSplitter>
#include <QVBoxLayout>
#include <QListView>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QLabel>
#include <QStatusBar>
#include "Account.h"
#include "Accounts.h"
#include "Friend.h"
#include "Friends.h"
#include "Messages.h"
#include "AccountsDialog.h"
#include "SettingsDialog.h"
#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  QNetworkAccessManager *nam = new QNetworkAccessManager(this);
  this->accounts = new Accounts(this);
  this->friends = new Friends(nam, this);
  this->messages = new Messages(nam, this);
  this->setupDefaults();
  this->createMenuBar();
  this->createStatusBar();
  this->createHistoryList();
  this->createMessageEdit();
  this->createFriendsList();
  this->createAccountsList();
  this->createLayout();
}

void MainWindow::selectAccount(int index) {
  friends->setAccount(accounts->at(index));
}

void MainWindow::sendMessage() {
    QModelIndex friendIndex = friendsList->currentIndex();
    if (friendIndex.isValid()) {
      Account a = accounts->at(accountsList->currentIndex());
      Friend f =  friends->at(friendIndex.row());
      messages->sendMessage(a, f, messageEdit->text());
    }
}

void MainWindow::createAccountsList() {
  this->accountsList = new QComboBox();
  connect(accountsList, SIGNAL(currentIndexChanged(int)), this, SLOT(selectAccount(int)));
  accountsList->setModel(accounts);
}

void MainWindow::createFriendsList() {
  this->friendsList = new QListView();
  friendsList->setModel(friends);
}

void MainWindow::createHistoryList() {
  this->historyList = new QListView();
}

void MainWindow::createMessageEdit() {
  this->messageEdit = new QLineEdit();
}

void MainWindow::showAccountsDialog() {
  AccountsDialog dialog(accounts, this);
  dialog.exec();
}

void MainWindow::showSettingsDialog() {
  SettingsDialog dialog(this);
  dialog.exec();
}

void MainWindow::createMenuBar() {
  this->createFileMenu();
  this->createViewMenu();
  this->createHelpMenu();
}

void MainWindow::createFileMenu() {
  QMenu *fileMenu = this->menuBar()->addMenu(QString::fromUtf8("Файл"));
  QAction *accountsAction = fileMenu->addAction(QString::fromUtf8("Менеджер аккаунтов"));
  connect(accountsAction, SIGNAL(triggered()), this, SLOT(showAccountsDialog()));
  QAction *settingsAction = fileMenu->addAction(QString::fromUtf8("Настройки"));
  connect(settingsAction, SIGNAL(triggered()), this, SLOT(showSettingsDialog()));
  fileMenu->addSeparator();
  QAction *quitAction = fileMenu->addAction(QString::fromUtf8("Выход"));
  connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
}

void MainWindow::createViewMenu() {
  QMenu *viewMenu = this->menuBar()->addMenu(QString::fromUtf8("Вид"));
  viewMenu->addAction(QString::fromUtf8("Только онлайн"));
}

void MainWindow::createHelpMenu() {
  QMenu *helpMenu = this->menuBar()->addMenu(QString::fromUtf8("Помощь"));
  helpMenu->addAction(QString::fromUtf8("Документация"));
  helpMenu->addSeparator();
  helpMenu->addAction(QString::fromUtf8("О программе"));
}

void MainWindow::createStatusBar() {
  QLabel *chatStatus = new QLabel();
  chatStatus->setAlignment(Qt::AlignLeft);
  QLabel *connectionStatus = new QLabel();
  connectionStatus->setAlignment(Qt::AlignRight);
  this->statusBar()->addWidget(chatStatus, 2);
  this->statusBar()->addWidget(connectionStatus, 1);
}

void MainWindow::createLayout() {
  QHBoxLayout *chatLayout = new QHBoxLayout();
  chatLayout->addWidget(messageEdit);
  QPushButton *sendButton = new QPushButton(QString::fromUtf8("Отправить"));
  connect(sendButton, SIGNAL(clicked()), this, SLOT(sendMessage()));
  chatLayout->addWidget(sendButton);
  QWidget *history = new QWidget();
  QVBoxLayout *historyLayout = new QVBoxLayout(history);
  historyLayout->addWidget(historyList);
  historyLayout->addLayout(chatLayout);
  QWidget *roster = new QWidget();
  QVBoxLayout *rosterLayout = new QVBoxLayout(roster);
  rosterLayout->addWidget(accountsList);
  rosterLayout->addWidget(friendsList);
  QSplitter *splitter = new QSplitter();
  splitter->addWidget(roster);
  splitter->addWidget(history);
  splitter->setStretchFactor(0, 1);
  splitter->setStretchFactor(1, 2);
  this->setCentralWidget(splitter);
}

void MainWindow::setupDefaults() {
  this->setWindowTitle(QString::fromUtf8("Главное окно - Мультичат ВК"));
  this->setMinimumSize(800, 400);
}
