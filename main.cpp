#include <QDebug>
#include <QApplication>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QSettings>
#include "MainWindow.h"

int main(int argc, char **argv) {
  QApplication application(argc, argv);
  QCoreApplication::setOrganizationName("kimrgrey");
  QCoreApplication::setOrganizationDomain("kimrgrey.org");
  QCoreApplication::setApplicationName("multichat");
  QSettings settings;
  QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
  db.setDatabaseName(settings.value("db/path").toString());
  db.open();
  MainWindow mainWindow;
  mainWindow.show();
  int exitCode = application.exec();
  db.close();
  return exitCode;
}
