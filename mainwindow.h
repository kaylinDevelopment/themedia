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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "openmedia.h"
#include <phonon/MediaObject>
#include <phonon/AudioOutput>
#include <QFileDialog>
#include <QList>
#include <QListWidgetItem>
#include <QTime>
#include <QSystemTrayIcon>
#include <QPixmap>
#include <QDBusMessage>
#include <QDBusConnection>
#include <QDBusObjectPath>
#include <QMessageBox>
#include <QDBusInterface>
#include <QTimer>
#include <QMouseEvent>
#include <QProgressDialog>
#include <QThread>
#include <QProcess>
#include <QInputDialog>
#include <QMimeData>
#include <QtX11Extras/QX11Info>
#include <QtMath>
#include <phonon/AbstractMediaStream>
#include <phonon/MediaController>
#include <phonon/AudioDataOutput>
#include "cddbworker.h"
#include "importcd.h"
#include "nativeeventfilter.h"
#include "mprisdbus.h"
#include "aboutdialog.h"

#include <X11/keysym.h>
#include <X11/XF86keysym.h>
#include <X11/Xlib.h>


using namespace Phonon;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    Q_PROPERTY(bool CanQuit READ CanQuit)
    Q_PROPERTY(bool CanRaise READ CanRaise)
    Q_PROPERTY(bool HasTrackList READ HasTrackList)
    Q_PROPERTY(QString Identity READ Identity)
    Q_PROPERTY(QString DesktopEntry READ DesktopEntry)
    Q_PROPERTY(QStringList SupportedMimeTypes READ SupportedMimeTypes)
    Q_PROPERTY(QStringList SupportedUriSchemes READ SupportedUriSchemes)

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
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QStringList mimeTypes;

private slots:
    void on_actionOpen_Media_triggered();

    void on_player_stateChanged(Phonon::State newState);

    void on_player_tick(qint64 time);

    void updatePlaylist();

    void on_player_totalTimeChanged(qint64 time);

    void on_playPause_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_actionExit_triggered();

    void on_player_hasVideoChanged(bool hasVideo);

    void on_pushButton_3_clicked();

    void on_controller_titleChanged(int titleNumber);

    void on_controller_availableTitlesChanged(int availableTitles);

    void on_playlistWidget_itemClicked(QListWidgetItem *item);

    void on_dvdRootMenu_clicked();

    void on_dvdTitleMenu_clicked();

    void on_dataOut_dataReady(QMap<Phonon::AudioDataOutput::Channel,QVector<qint16>> data);

    void on_spacerFrame_visualisationRateChanged(int );

    void on_actionImport_CD_triggered();

    void on_actionEject_Disc_triggered();

    void on_dvdGoToChapter_clicked();

    void on_dvdGoToTitle_clicked();

    void on_createPlaylistButton_clicked();

    void on_actionSave_Playlist_triggered();

    void on_playlistWidget_customContextMenuRequested(const QPoint &pos);

    void on_actionRemove_from_Playlist_triggered();

    void on_actionScope_triggered();

    void on_actionLines_triggered();

    void on_spacerFrame_customContextMenuRequested(const QPoint &pos);

    void on_actionCircle_triggered();

    void on_actionAbout_triggered();

public Q_SLOTS:
    void Raise();
    void Quit();

    void Next();
    void Previous();
    void Pause();
    void PlayPause();
    void Stop();
    void Play();
    void SetPosition();
    void OpenUri(QUrl uri);

private:
    Ui::MainWindow *ui;

    QList<MediaSource> playlist;
    int currentPlaylist;

    MediaObject* player;
    MediaController* controller;
    AudioDataOutput* dataOut;

    QTimer* mouseTimer = NULL;
    QList<QMap<QString, QString>> cddbinfo;

    mprisDbus* dbusInterface;
    mprisDbusPlayer* dbusInterfacePlayer;
    QVariantMap mprisMetadataMap;

    bool eventFilter(QObject *, QEvent *event);
    void closeEvent(QCloseEvent* event);

    bool CanQuit();
    bool CanRaise();
    bool HasTrackList();
    QString Identity();
    QString DesktopEntry();
    QStringList SupportedMimeTypes();
    QStringList SupportedUriSchemes();

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

#endif // MAINWINDOW_H
