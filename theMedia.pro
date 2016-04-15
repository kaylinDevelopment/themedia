#-------------------------------------------------
#
# Project created by QtCreator 2016-02-15T17:55:26
#
#-------------------------------------------------

QT       += core gui phonon4qt5 dbus

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

LIBS += -lmusicbrainz5

TARGET = themedia
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    openmedia.cpp \
    cddbworker.cpp

HEADERS  += mainwindow.h \
    openmedia.h \
    cddbworker.h

FORMS    += mainwindow.ui \
    openmedia.ui

RESOURCES += \
    icons.qrc
