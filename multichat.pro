QT += core gui network webkit sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets webkitwidgets

TARGET = multichat
TEMPLATE = app

HEADERS = MainWindow.h \
  AccountsDialog.h \
  WebBrowserDialog.h \
  Vkontakte.h \
  SettingsDialog.h \
  Account.h \
  Accounts.h \
  Friend.h \
  Friends.h \
  Messages.h

SOURCES = main.cpp \
  MainWindow.cpp \
  AccountsDialog.cpp \
  WebBrowserDialog.cpp \
  Vkontakte.cpp \
  SettingsDialog.cpp \
  Account.cpp \
  Accounts.cpp \
  Friend.cpp \
  Friends.cpp \
  Messages.cpp
