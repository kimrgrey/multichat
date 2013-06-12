#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QSettings>

class QLineEdit;

class SettingsDialog : public QDialog {
  Q_OBJECT
public:
  SettingsDialog(QWidget *parent = 0);
public slots:
  void saveAndClose();
  void cancelAndClose();
private:
  void setupDefaults();
  void createLayout();
private:
  QSettings settings;
  QLineEdit *appIdEdit;
  QLineEdit *permissionsEdit;
  QLineEdit *dbpathEdit;
};


#endif // SETTINGSDIALOG_H
