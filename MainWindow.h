#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QComboBox;
class QListView;
class QLineEdit;
class Accounts;
class Friends;
class Messages;

class MainWindow : public QMainWindow {
  Q_OBJECT
public:
  MainWindow(QWidget *parent = 0);
public slots:
  void showAccountsDialog();
  void showSettingsDialog();
private slots:
  void selectAccount(int index);
  void selectFriend(const QModelIndex &current, const QModelIndex &previous);
  void sendMessage();
private:
  void setupDefaults();
  void createLayout();
  void createMenuBar();
  void createFileMenu();
  void createViewMenu();
  void createHelpMenu();
  void createStatusBar();
  void createAccountsList();
  void createFriendsList();
  void createHistoryList();
  void createMessageEdit();
private:
  Accounts *accounts;
  Friends *friends;
  Messages *messages;
  QComboBox *accountsList;
  QListView *friendsList;
  QListView *historyList;
  QLineEdit *messageEdit;
};

#endif // MAINWINDOW_H
