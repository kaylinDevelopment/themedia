#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->artistImage->setPixmap(QIcon::fromTheme("view-media-artist").pixmap(16, 16));
    ui->albumImage->setPixmap(QIcon::fromTheme("media-album-cover").pixmap(16, 16));
    ui->trackImage->setPixmap(QIcon::fromTheme("gtk-cdrom").pixmap(16, 16));

    player = new MediaObject(this);
    createPath(player, new AudioOutput(Phonon::MusicCategory, this));

    connect(player, &MediaObject::metaDataChanged, [=]() {
        bool showNotification = false;
        QStringList Title = player->metaData(Phonon::TitleMetaData);
        if (Title.count() > 0) {
            if (Title.at(0) != ui->title->text()) {
                showNotification = true;
                ui->title->setText(Title.at(0));
            }
        }

        QStringList Artist = player->metaData(Phonon::ArtistMetaData);
        if (Artist.count() > 0) {
            if (Artist.at(0) != ui->artist->text()) {
                showNotification = true;
                ui->artist->setText(Artist.at(0));
            }
        }

        QStringList Album = player->metaData(Phonon::AlbumMetaData);
        if (Album.count() > 0) {
            ui->album->setText(Album.at(0));
        }

        QStringList Track = player->metaData(Phonon::TracknumberMetaData);
        if (Track.count() > 0) {
            ui->track->setText(Track.at(0));
        }

        updatePlaylist();

        if (showNotification) {
            QDBusInterface interface("org.freedesktop.Notifications", "/org/freedesktop/Notifications", "org.freedesktop.Notifications");
            //QDBusMessage msg = QDBusMessage::createMethodCall("org.freedesktop.Notifications", "/org/freedesktop/Notifications/", "org.freedesktop.Notifications", "Notify");

            QList<QVariant> args;
            args << "theMedia"<< (uint) 0 << "media-playback-start" << "theMedia" <<
                                    "Now Playing: " + ui->title->text() + " by " + ui->artist->text() <<
                                    QStringList() << QVariantMap() << (int) -1;

            QDBusMessage message = interface.callWithArgumentList(QDBus::AutoDetect, "Notify", args);
            //QMessageBox::warning(this, "Message", message.errorMessage(), QMessageBox::Ok, QMessageBox::Ok);

            //QSystemTrayIcon* notification = new QSystemTrayIcon(this);
            //notification->showMessage("theMedia", "Now Playing: " + ui->title->text() + " by " + ui->artist->text());
        }
    });
    connect(player, SIGNAL(totalTimeChanged(qint64)), this, SLOT(on_player_totalTimeChanged(qint64)));
    connect(player, SIGNAL(aboutToFinish()), this, SLOT(on_pushButton_2_clicked()));
    connect(player, SIGNAL(tick(qint64)), this, SLOT(on_player_tick(qint64)));
    connect(player, SIGNAL(stateChanged(Phonon::State,Phonon::State)), this, SLOT(on_player_stateChanged(Phonon::State)));

    ui->seekSlider->setMediaObject(player);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_player_stateChanged(Phonon::State newState) {
    if (newState == Phonon::PlayingState) {
        ui->playPause->setIcon(QIcon::fromTheme("media-playback-pause"));
    } else {
        ui->playPause->setIcon(QIcon::fromTheme("media-playback-start"));
    }
}

void MainWindow::on_actionOpen_Media_triggered()
{
    OpenMedia* open = new OpenMedia(&playlist, this);
    if (open->exec() == QDialog::Accepted) {
        if (player->state() != Phonon::PlayingState) {
            player->enqueue(playlist.at(0));
            player->play();
            currentPlaylist = 0;
        }
    }
    updatePlaylist();
}

void MainWindow::updatePlaylist() {
    ui->playlistWidget->clear();

    for (MediaSource source : playlist) {
        MediaObject* tempPlayer = new MediaObject(this);
        tempPlayer->setCurrentSource(source);
        tempPlayer->play();

        QListWidgetItem* item = new QListWidgetItem();

        QStringList Title = tempPlayer->metaData(Phonon::TitleMetaData);
        if (Title.count() > 0) {
            item->setText(Title.at(0));
        }
        tempPlayer->deleteLater();

            item->setIcon(QIcon::fromTheme("media-playback-start"));
            item->setIcon(QIcon::fromTheme("audio-x-generic"));

        ui->playlistWidget->addItem(item);
    }
}

void MainWindow::on_playPause_clicked()
{
    if (player->state() == Phonon::PlayingState) {
        player->pause();
    } else {
        player->play();
    }
}

void MainWindow::on_player_tick(qint64 time) {
    QTime elapsed(0, 0, 0);
    elapsed = elapsed.addMSecs(time);
    ui->elapsedTime->setText(elapsed.toString("mm:ss"));
}

void MainWindow::on_player_totalTimeChanged(qint64 time) {
    QTime total(0, 0, 0);
    total = total.addMSecs(time);
    ui->totalTime->setText(total.toString("mm:ss"));
}

void MainWindow::on_pushButton_2_clicked()
{
    currentPlaylist++;

    if (currentPlaylist == playlist.length()) {
        currentPlaylist = 0;
    }

    player->setCurrentSource(playlist.at(currentPlaylist));
    player->play();

    updatePlaylist();
}

void MainWindow::on_pushButton_clicked()
{
    currentPlaylist--;

    if (currentPlaylist == -1) {
        currentPlaylist = playlist.length() - 1;
    }

    player->setCurrentSource(playlist.at(currentPlaylist));
    player->play();

    updatePlaylist();
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::exit(0);
}
