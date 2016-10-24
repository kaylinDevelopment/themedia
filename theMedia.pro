#-------------------------------------------------
#
# Project created by QtCreator 2016-02-15T17:55:26
#
#-------------------------------------------------

QT       += core gui phonon4qt5 dbus x11extras KParts
CONFIG   += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

LIBS += -lmusicbrainz5 -lX11 -lxcb -lxcb-keysyms -lcdda_interface -lcdda_paranoia -lgpod
INCLUDEPATH += /usr/include/gpod-1.0/ /usr/include/glib-2.0/ /usr/lib/glib-2.0/include/

TARGET = themedia
TEMPLATE = app

mpris.files = org.mpris.MediaPlayer2.theMedia.xml
DBUS_ADAPTORS += mpris

SOURCES += main.cpp\
        mainwindow.cpp \
    openmedia.cpp \
    cddbworker.cpp \
    visualisationframe.cpp \
    importcd.cpp \
    nativeeventfilter.cpp \
    mprisdbus.cpp \
    aboutdialog.cpp

HEADERS  += mainwindow.h \
    openmedia.h \
    cddbworker.h \
    visualisationframe.h \
    importcd.h \
    nativeeventfilter.h \
    mprisdbus.h \
    aboutdialog.h

FORMS    += mainwindow.ui \
    openmedia.ui \
    importcd.ui \
    aboutdialog.ui

RESOURCES += \
    icons.qrc

DISTFILES += \
    themedia.desktop
