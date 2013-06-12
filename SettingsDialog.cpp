#include <QDebug>
#include <QFormLayout>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include "SettingsDialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) : QDialog(parent) {
  this->appIdEdit = new QLineEdit(settings.value("authorization/appId").toString());
  this->permissionsEdit = new QLineEdit(settings.value("authorization/permissions").toString());
  this->dbpathEdit = new QLineEdit(settings.value("db/path").toString());
  this->setupDefaults();
  this->createLayout();
}

void SettingsDialog::saveAndClose() {
  settings.setValue("authorization/appId", appIdEdit->text());
  settings.setValue("authorization/permissions", permissionsEdit->text());
  settings.setValue("db/path", dbpathEdit->text());
  this->accept();
}

void SettingsDialog::cancelAndClose() {
  this->reject();
}

void SettingsDialog::setupDefaults() {
  this->setMinimumSize(500, 300);
  this->setWindowTitle(QString::fromUtf8("Настройки - Мультичат ВК"));
}

void SettingsDialog::createLayout() {
  QFormLayout *formLayout = new QFormLayout();
  formLayout->addRow(QString::fromUtf8("Приложение"), appIdEdit);
  formLayout->addRow(QString::fromUtf8("Права доступа"), permissionsEdit);
  formLayout->addRow(QString::fromUtf8("База данных"), dbpathEdit);
  QHBoxLayout *actionsLayout = new QHBoxLayout();
  actionsLayout->addStretch();
  QPushButton *applyButton = new QPushButton(QString::fromUtf8("Принять"));
  applyButton->setDefault(true);
  connect(applyButton, SIGNAL(clicked()), this, SLOT(saveAndClose()));
  QPushButton *cancelButton = new QPushButton(QString::fromUtf8("Отмена"));
  connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelAndClose()));
  actionsLayout->addWidget(applyButton);
  actionsLayout->addWidget(cancelButton);
  QVBoxLayout *mainLayout = new QVBoxLayout(this);
  mainLayout->addLayout(formLayout);
  mainLayout->addLayout(actionsLayout);
}
