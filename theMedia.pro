#-------------------------------------------------
#
# Project created by QtCreator 2016-02-15T17:55:26
#
#-------------------------------------------------

QT       += core gui phonon4qt5 dbus

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = themedia
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    openmedia.cpp

HEADERS  += mainwindow.h \
    openmedia.h

FORMS    += mainwindow.ui \
    openmedia.ui

RESOURCES += \
    icons.qrc
