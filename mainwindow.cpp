#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setMouseTracking(true);
    ui->VideoWidget->installEventFilter(this);
    ui->VideoWidget->setMouseTracking(true);

    ui->artistImage->setPixmap(QIcon::fromTheme("view-media-artist").pixmap(16, 16));
    ui->albumImage->setPixmap(QIcon::fromTheme("media-album-cover").pixmap(16, 16));
    ui->trackImage->setPixmap(QIcon::fromTheme("gtk-cdrom").pixmap(16, 16));
    ui->VideoWidget->setVisible(false);
    ui->dvdControls->setVisible(false);
    ui->cddbProgressFrame->setVisible(false);

    player = new MediaObject(this);
    createPath(player, new AudioOutput(Phonon::MusicCategory, this));
    createPath(player, ui->VideoWidget);

    connect(player, &MediaObject::metaDataChanged, [=]() {
        if (playlist.first().type() == MediaSource::Disc) {

            if (playlist.first().discType() == Phonon::Cd && cddbinfo.count() == 0) {
                ui->cddbProgressFrame->setVisible(true);

                //Read data from Musicbrainz
                QStringList discid = player->metaData(Phonon::MusicBrainzDiscIdMetaData);
                if (discid.count() > 0) {
                    QThread *thread = new QThread;
                    CddbWorker *worker = new CddbWorker(&cddbinfo, discid, this);
                    worker->moveToThread(thread);
                    connect(thread, SIGNAL (started()), worker, SLOT (process()));
                    connect(worker, SIGNAL (finished()), thread, SLOT (quit()));
                    connect(worker, SIGNAL (finished()), worker, SLOT (deleteLater()));
                    connect(thread, SIGNAL (finished()), thread, SLOT (deleteLater()));
                    connect(worker, &CddbWorker::finished, [=]() {
                        on_controller_availableTitlesChanged(controller->availableTitles());
                        on_controller_titleChanged(controller->currentTitle());
                        ui->cddbProgressFrame->setVisible(false);
                    });
                    thread->start();
                }
            }

        } else {
            ui->artist->setVisible(true);
            ui->artistImage->setVisible(true);
            ui->albumImage->setVisible(true);
            ui->albumImage->setVisible(true);

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
                ui->track->setVisible(true);
                ui->trackImage->setVisible(true);
            } else {
                ui->trackImage->setVisible(false);
                ui->trackImage->setVisible(false);
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
        }
    });
    connect(player, SIGNAL(totalTimeChanged(qint64)), this, SLOT(on_player_totalTimeChanged(qint64)));
    connect(player, SIGNAL(aboutToFinish()), this, SLOT(on_pushButton_2_clicked()));
    connect(player, SIGNAL(tick(qint64)), this, SLOT(on_player_tick(qint64)));
    connect(player, SIGNAL(stateChanged(Phonon::State,Phonon::State)), this, SLOT(on_player_stateChanged(Phonon::State)));
    connect(player, SIGNAL(hasVideoChanged(bool)), this, SLOT(on_player_hasVideoChanged(bool)));

    ui->seekSlider->setMediaObject(player);

    controller = new MediaController(player);
    connect(controller, SIGNAL(availableTitlesChanged(int)), this, SLOT(on_controller_availableTitlesChanged(int)));
    connect(controller, SIGNAL(titleChanged(int)), this, SLOT(on_controller_titleChanged(int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::eventFilter(QObject *, QEvent *event) {
    if (event->type() == QEvent::MouseMove) {
        if (player->hasVideo()) {
            if (mouseTimer != NULL) {
                mouseTimer->stop();
                delete mouseTimer;
                mouseTimer = NULL;
            }

            mouseTimer = new QTimer(this);
            mouseTimer->setInterval(5000);
            mouseTimer->setSingleShot(true);
            connect(mouseTimer, &QTimer::timeout, [=]() {
                mouseTimer->stop();
                delete mouseTimer;
                mouseTimer = NULL;
                ui->mediaControlFrame->setVisible(false);
                ui->menuBar->setVisible(false);
                QApplication::changeOverrideCursor(Qt::BlankCursor);
            });
            mouseTimer->start();

            ui->mediaControlFrame->setVisible(true);
            ui->menuBar->setVisible(true);
            QApplication::restoreOverrideCursor();
            event->ignore();
            return false;
        }
    }
    event->ignore();

    return false;
}

void MainWindow::on_controller_titleChanged(int titleNumber) {
    if (playlist.first().discType() == Phonon::Cd) {
        if (cddbinfo.count() == 0) {
            ui->title->setText("Track " + QString::number(titleNumber));
            ui->artist->setText("");
            ui->album->setText("");
        } else {
            ui->title->setText(cddbinfo.at(titleNumber - 1).value("name"));
            ui->artist->setText(cddbinfo.at(titleNumber - 1).value("artist"));
            ui->album->setText(cddbinfo.at(titleNumber - 1).value("album"));
        }
        for (int i = 0; i < ui->playlistWidget->count(); i++) {
            if (i + 1 == titleNumber) {
                ui->playlistWidget->item(i)->setIcon(QIcon::fromTheme("media-playback-start"));
            } else {
                ui->playlistWidget->item(i)->setIcon(QIcon::fromTheme("media-optical-audio"));
            }
        }
    }
}

void MainWindow::on_controller_availableTitlesChanged(int availableTitles) {
    if (playlist.first().discType() == Phonon::Cd) {
        ui->playlistWidget->clear();

        if (cddbinfo.count() == 0) {
            for (int i = 1; i <= availableTitles; i++) {
                QListWidgetItem* item = new QListWidgetItem();
                item->setText("Track " + QString::number(i));
                item->setIcon(QIcon::fromTheme("media-optical-audio"));

                ui->playlistWidget->addItem(item);
            }
        } else {
            for (QMap<QString, QString> info : cddbinfo) {
                QListWidgetItem* item = new QListWidgetItem();
                item->setText(info.value("name"));
                item->setIcon(QIcon::fromTheme("media-optical-audio"));
                ui->playlistWidget->addItem(item);
            }
        }
    }
}

void MainWindow::on_player_hasVideoChanged(bool hasVideo) {
    if (hasVideo) {
        ui->VideoWidget->setVisible(true);
        ui->mediaInfoFrame->setVisible(false);
        ui->spacerFrame->setVisible(false);
        ui->mediaControlFrame->setVisible(false);

        if (this->isFullScreen()) {
            QApplication::changeOverrideCursor(Qt::BlankCursor);
        } else {
            QApplication::restoreOverrideCursor();
        }
    } else {
        ui->VideoWidget->setVisible(false);
        ui->mediaInfoFrame->setVisible(true);
        ui->mediaControlFrame->setVisible(true);
        ui->spacerFrame->setVisible(true);

        if (mouseTimer != NULL) {
            mouseTimer->stop();
            delete mouseTimer;
            mouseTimer = NULL;
        }
        QApplication::restoreOverrideCursor();
    }
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
    if (playlist.count() > 0) {
        if (playlist.first().type() == MediaSource::Disc) {
            if (playlist.first().discType() == Phonon::Cd) {

            } else {
                ui->dvdControls->setVisible(true);
            }
        } else {
            ui->dvdControls->setVisible(false);
            ui->playlistWidget->clear();
            for (MediaSource source : playlist) {
                QListWidgetItem* item = new QListWidgetItem();

                switch (source.type()) {
                case MediaSource::LocalFile:
                    item->setText(source.fileName());
                    break;
                case MediaSource::Url:
                    if (source.url().isLocalFile()) {
                        item->setText(QFileInfo(source.url().path() + source.url().fileName()).baseName());
                    } else {
                        item->setText(source.url().toDisplayString());
                    }
                    break;
                }

                if (playlist.indexOf(source) == currentPlaylist) {
                    item->setIcon(QIcon::fromTheme("media-playback-start"));
                } else {
                    item->setIcon(QIcon::fromTheme("audio-x-generic"));
                }

                ui->playlistWidget->addItem(item);
            }
        }
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
    if (playlist.first().type() == MediaSource::Disc) {
        if (playlist.first().discType() == Phonon::Cd) {
            controller->nextTitle();
        } else {
            if (controller->availableChapters() == controller->currentChapter()) {
                controller->nextTitle();
            } else {
                controller->setCurrentChapter(controller->currentChapter() + 1);
            }
        }
    } else {
        currentPlaylist++;

        if (currentPlaylist == playlist.length()) {
            currentPlaylist = 0;
        }

        if (sender() == player) {
            player->enqueue(playlist.at(currentPlaylist));
        } else {
            player->setCurrentSource(playlist.at(currentPlaylist));
            player->play();
        }

        updatePlaylist();
    }
}

void MainWindow::on_pushButton_clicked()
{

    if (playlist.first().type() == MediaSource::Disc) {
        if (playlist.first().discType() == Phonon::Cd) {
            controller->previousTitle();
        } else {
            if (controller->currentChapter() == 1) {
                controller->previousTitle();
            } else {
                controller->setCurrentChapter(controller->currentChapter() - 1);
            }
        }
    } else {
        currentPlaylist--;

        if (currentPlaylist == -1) {
            currentPlaylist = playlist.length() - 1;
        }

        player->setCurrentSource(playlist.at(currentPlaylist));
        player->play();

        updatePlaylist();
    }
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::exit(0);
}

void MainWindow::on_pushButton_3_clicked()
{
    if (this->isFullScreen()) {
        this->showNormal();
        ui->pushButton_3->setIcon(QIcon::fromTheme("view-fullscreen"));
        ui->mediaControlFrame->setVisible(true);
        ui->playlistWidget->setVisible(true);
        ui->menuBar->setVisible(true);
        QApplication::restoreOverrideCursor();
    } else {
        this->showFullScreen();
        ui->pushButton_3->setIcon(QIcon::fromTheme("view-restore"));
        ui->playlistWidget->setVisible(false);
        if (player->hasVideo()) {
            ui->mediaControlFrame->setVisible(false);
            QApplication::changeOverrideCursor(Qt::BlankCursor);
        } else {
            ui->mediaControlFrame->setVisible(true);
            QApplication::restoreOverrideCursor();
        }
        ui->menuBar->setVisible(false);
    }
}

void MainWindow::on_playlistWidget_itemClicked(QListWidgetItem *item)
{
    if (playlist.first().type() == MediaSource::Disc) {
        if (playlist.first().discType() == Phonon::Cd) {
            controller->setCurrentTitle(ui->playlistWidget->row(item) + 1);
        }
    } else {
        currentPlaylist = ui->playlistWidget->row(item);
        player->setCurrentSource(playlist.at(currentPlaylist));
        player->play();

        updatePlaylist();
    }
}

void MainWindow::on_dvdRootMenu_clicked()
{
    controller->setCurrentMenu(MediaController::RootMenu);
}

void MainWindow::on_dvdTitleMenu_clicked()
{
    controller->setCurrentMenu(MediaController::TitleMenu);
}
