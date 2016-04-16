#-------------------------------------------------
#
# Project created by QtCreator 2016-02-15T17:55:26
#
#-------------------------------------------------

QT       += core gui phonon4qt5 dbus x11extras KParts
CONFIG   += C++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

LIBS += -lmusicbrainz5 -lX11 -lxcb -lxcb-keysyms

TARGET = themedia
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    openmedia.cpp \
    cddbworker.cpp \
    visualisationframe.cpp \
    importcd.cpp

HEADERS  += mainwindow.h \
    openmedia.h \
    cddbworker.h \
    visualisationframe.h \
    importcd.h

FORMS    += mainwindow.ui \
    openmedia.ui \
    importcd.ui

RESOURCES += \
    icons.qrc
