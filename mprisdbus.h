#ifndef MPRISDBUS_H
#define MPRISDBUS_H

#include <QObject>
#include <QApplication>
#include <QDBusConnection>
#include "themedia_adaptor.h"

class mprisDbus : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.mpris.MediaPlayer2")

    Q_PROPERTY(bool CanQuit READ CanQuit)
    Q_PROPERTY(bool CanRaise READ CanRaise)
    Q_PROPERTY(bool HasTrackList READ HasTrackList)
    Q_PROPERTY(QString Identity READ Identity)
    Q_PROPERTY(QString DesktopEntry READ DesktopEntry)
    Q_PROPERTY(QStringList SupportedMimeTypes READ SupportedMimeTypes)


public:
    explicit mprisDbus(QObject *parent = 0);

    bool cquit = true;
    bool craise = true;
    bool trackList = false;
    QString id = "theMedia";
    QString de = "theMedia";
    QStringList mimeTypes;

signals:
    void bringToFront();

public Q_SLOTS:
    void raise();
    void quit();

private:
    bool CanQuit();
    bool CanRaise();
    bool HasTrackList();
    QString Identity();
    QString DesktopEntry();
    QStringList SupportedMimeTypes();
};

class mprisDbusPlayer : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.mpris.MediaPlayer2.Player")

    Q_PROPERTY(QString PlaybackStatus READ PlaybackStatus)
    Q_PROPERTY(QVariantMap Metadata READ Metadata)
public:
    explicit mprisDbusPlayer(QObject* parent = 0);

public Q_SLOTS:
    void Next();
    void Previous();
    void Pause();
    void PlayPause();
    void Stop();
    void Play();
    void SetPosition();
    void OpenUri(QUrl uri);

    QString PlaybackStatus();
    QVariantMap Metadata();
};

#endif // MPRISDBUS_H
