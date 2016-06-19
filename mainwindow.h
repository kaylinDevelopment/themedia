#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
//#include <QMediaPlaylist>
//#include <QMediaPlayer>
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
//#include <kaction.h>
//#include <kshortcut.h>
#include "importcd.h"
#include "nativeeventfilter.h"
#undef status
#include "mprisdbus.h"

#include <X11/keysym.h>
#include <X11/XF86keysym.h>
#include "Xlib.h"


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

    Q_PROPERTY(QString PlaybackStatus READ PlaybackStatus)
    Q_PROPERTY(QVariantMap Metadata READ Metadata)

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    bool cquit = true;
    bool craise = true;
    bool trackList = false;
    QString id = "theMedia";
    QString de = "theMedia";
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

public Q_SLOTS:
    void raise();
    void quit();

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
    //QMediaPlaylist* playlist;
    //QMediaPlayer* player;

    mprisDbus* dbusInterface;
    mprisDbusPlayer* dbusInterfacePlayer;
    QVariantMap mprisMetadataMap;

    bool eventFilter(QObject *, QEvent *event);


    bool CanQuit();
    bool CanRaise();
    bool HasTrackList();
    QString Identity();
    QString DesktopEntry();
    QStringList SupportedMimeTypes();

    QString PlaybackStatus();
    QVariantMap Metadata();
};

#endif // MAINWINDOW_H
