/***************************************************************************
 *   This file is part of theMedia.
 *
 *   theMedia is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   theMedia is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with theMedia.  If not, see <http://www.gnu.org/licenses/>.
 *
****************************************************************************/

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
    Q_PROPERTY(QStringList SupportedUriSchemes READ SupportedUriSchemes)


public:
    explicit mprisDbus(QObject *parent);

    bool cquit = true;
    bool craise = true;
    bool trackList = false;
    QString id = "theMedia";
    QString de = "theMedia";
    QStringList mimeTypes;

signals:
    void bringToFront();

public Q_SLOTS:
    void Raise();
    void Quit();

private:
    bool CanQuit();
    bool CanRaise();
    bool HasTrackList();
    QString Identity();
    QString DesktopEntry();
    QStringList SupportedMimeTypes();
    QStringList SupportedUriSchemes();
};

class mprisDbusPlayer : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.mpris.MediaPlayer2.Player")

    Q_PROPERTY(QString PlaybackStatus READ PlaybackStatus)
    Q_PROPERTY(QVariantMap Metadata READ Metadata)
    Q_PROPERTY(double Rate READ Rate)
    Q_PROPERTY(double MaximumRate READ MaximumRate)
    Q_PROPERTY(double MinimumRate READ MinimumRate)
    Q_PROPERTY(qint64 Position READ Position)
    Q_PROPERTY(double Volume READ Volume)
    Q_PROPERTY(bool CanControl READ CanControl)
    Q_PROPERTY(bool CanPlay READ CanPlay)
    Q_PROPERTY(bool CanPause READ CanPause)
    Q_PROPERTY(bool CanGoPrevious READ CanGoPrevious)
    Q_PROPERTY(bool CanGoNext READ CanGoNext)

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

private:
    QString PlaybackStatus();
    QVariantMap Metadata();
    double Rate();
    double MaximumRate();
    double MinimumRate();
    quint64 Position();
    double Volume();
    bool CanControl();
    bool CanPlay();
    bool CanPause();
    bool CanGoPrevious();
    bool CanGoNext();
};

#endif // MPRISDBUS_H
