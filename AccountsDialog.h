#ifndef ACCOUNTSDIALOG_H
#define ACCOUNTSDIALOG_H

#include <QDialog>

class WebBrowserDialog;
class QListView;
class Accounts;

class AccountsDialog : public QDialog {
  Q_OBJECT
public:
  AccountsDialog(Accounts *accounts, QWidget *parent = 0);
private slots:
  void openBrowserWindow();
private:
  void setupDefaults();
  void createAccountsList();
  void createLayout();
private:
  Accounts *accounts;
  QListView *accountsList;
  WebBrowserDialog *webBrowserDialog;
};

#endif // ACCOUNTSDIALOG_H
