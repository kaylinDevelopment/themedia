#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //XGrabKey(QX11Info::display(), XKeysymToKeycode(QX11Info::display(), XK_Pause), AnyModifier, RootWindow(QX11Info::display(), 0), true, GrabModeAsync, GrabModeAsync);

    this->setMouseTracking(true);
    this->setAcceptDrops(true);
    ui->VideoWidget->installEventFilter(this);
    ui->VideoWidget->setMouseTracking(true);

    ui->artistImage->setPixmap(QIcon::fromTheme("view-media-artist").pixmap(16, 16));
    ui->albumImage->setPixmap(QIcon::fromTheme("media-album-cover").pixmap(16, 16));
    ui->trackImage->setPixmap(QIcon::fromTheme("gtk-cdrom").pixmap(16, 16));
    ui->VideoWidget->setVisible(false);
    ui->dvdControls->setVisible(false);
    ui->cddbProgressFrame->setVisible(false);
    ui->errorFrame->setVisible(false);
    ui->errorImage->setPixmap(QIcon::fromTheme("dialog-error").pixmap(24, 24));

    player = new MediaObject(this);
    player->setPrefinishMark(1000);
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

                QVariantMap hints;
                hints.insert("transient", true);
                QList<QVariant> args;
                args << "theMedia"<< (uint) 0 << "media-playback-start" << "theMedia" <<
                                        "Now Playing: " + ui->title->text() + " by " + ui->artist->text() <<
                                        QStringList() << hints << (int) -1;

                QDBusMessage message = interface.callWithArgumentList(QDBus::AutoDetect, "Notify", args);
                //QMessageBox::warning(this, "Message", message.errorMessage(), QMessageBox::Ok, QMessageBox::Ok);

                //QSystemTrayIcon* notification = new QSystemTrayIcon(this);
                //notification->showMessage("theMedia", "Now Playing: " + ui->title->text() + " by " + ui->artist->text());
            }
        }
    });
    connect(player, SIGNAL(totalTimeChanged(qint64)), this, SLOT(on_player_totalTimeChanged(qint64)));
    //connect(player, SIGNAL(aboutToFinish()), this, SLOT(on_pushButton_2_clicked()));
    //connect(player, SIGNAL(prefinishMarkReached(qint32)), this, SLOT(on_pushButton_2_clicked()));
    connect(player, SIGNAL(finished()), this, SLOT(on_pushButton_2_clicked()));
    connect(player, SIGNAL(tick(qint64)), this, SLOT(on_player_tick(qint64)));
    connect(player, SIGNAL(stateChanged(Phonon::State,Phonon::State)), this, SLOT(on_player_stateChanged(Phonon::State)));
    connect(player, SIGNAL(hasVideoChanged(bool)), this, SLOT(on_player_hasVideoChanged(bool)));

    ui->seekSlider->setMediaObject(player);

    controller = new MediaController(player);
    connect(controller, SIGNAL(availableTitlesChanged(int)), this, SLOT(on_controller_availableTitlesChanged(int)));
    connect(controller, SIGNAL(titleChanged(int)), this, SLOT(on_controller_titleChanged(int)));

    dataOut = new AudioDataOutput(this);
    dataOut->setDataSize(this->width());
    connect(dataOut, SIGNAL(dataReady(QMap<Phonon::AudioDataOutput::Channel,QVector<qint16>>)), this, SLOT(on_dataOut_dataReady(QMap<Phonon::AudioDataOutput::Channel,QVector<qint16>>)));
    createPath(player, dataOut);

    /*KAction* playPauseGlobalButton = new KAction(this);
    playPauseGlobalButton->setGlobalShortcut(KShortcut(Qt::Key_MediaPause));
    connect(playPauseGlobalButton, SIGNAL(triggered(Qt::MouseButtons,Qt::KeyboardModifiers)), this, SLOT(on_playPause_clicked()));*/

    QStringList arguments = QApplication::arguments();
    arguments.removeFirst();
    for (QUrl argument : QUrl::fromStringList(arguments)) {
        MediaSource source(argument);
        playlist.append(source);
    }

    if (playlist.count() > 0) {
        player->enqueue(playlist.at(0));
        player->play();
        currentPlaylist = 0;
    }
}

MainWindow::~MainWindow()
{
    //XUngrabKey(QX11Info::display(), XKeysymToKeycode(QX11Info::display(), XK_Pause), AnyModifier, QX11Info::appRootWindow());
    delete ui;
}

void MainWindow::on_dataOut_dataReady(QMap<AudioDataOutput::Channel, QVector<qint16>> data) {
    /*quint64 visualisationRecord;
    for (qint64 sample : data.value(AudioDataOutput::LeftChannel)) {
        visualisationRecord = visualisationRecord + (sample ^ 2);

    }

    quint64 visualisation = qSqrt(visualisationRecord / 11025);*/
    //ui->->setValue(visualisation);

    ui->spacerFrame->setVisualisation(data.value(AudioDataOutput::LeftChannel));
    ui->spacerFrame->update();
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
            for (int i = 1; i <= availableTitles; i++) {
                QMap<QString, QString> info = cddbinfo.at(i - 1);
                QListWidgetItem* item = new QListWidgetItem();
                item->setText("(" + QString::number(i) + ") " + info.value("name"));
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

    if (newState == Phonon::ErrorState) {
        ui->errorText->setText(player->errorString());
        ui->errorFrame->setVisible(true);
    } else {
        ui->errorFrame->setVisible(false);
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

            ui->createPlaylistButton->setVisible(false);
        } else {
            ui->createPlaylistButton->setVisible(true);
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
    if (time == 0) {
        ui->totalTime->setText("∞");
    } else {
        QTime total(0, 0, 0);
        total = total.addMSecs(time);
        ui->totalTime->setText(total.toString("mm:ss"));
    }
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
        if (ui->repeatButton->isChecked()) {
        } else if (ui->shuffleButton->isChecked()) {
            currentPlaylist = qrand() % playlist.count();
        } else {
            currentPlaylist++;
        }

        if (currentPlaylist == playlist.length()) {
            currentPlaylist = 0;
        }

        /*if (sender() == player) {
            player->enqueue(playlist.at(currentPlaylist));
        } else {*/
            player->setCurrentSource(playlist.at(currentPlaylist));
            player->play();
        //}

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
        ui->playlistFrame->setVisible(true);
        ui->menuBar->setVisible(true);
        QApplication::restoreOverrideCursor();
    } else {
        this->showFullScreen();
        ui->pushButton_3->setIcon(QIcon::fromTheme("view-restore"));
        ui->playlistFrame->setVisible(false);
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

void MainWindow::on_dvdRootMenu_clicked() //Go to DVD Root Menu
{
    controller->setCurrentMenu(MediaController::RootMenu);
}

void MainWindow::on_dvdTitleMenu_clicked() //Go to DVD Title Menu
{
    controller->setCurrentMenu(MediaController::TitleMenu);
}

void MainWindow::on_spacerFrame_visualisationRateChanged(int size) //Change Data Rate
{
    dataOut->setDataSize(size);
}

void MainWindow::on_actionImport_CD_triggered() //Open Import CD Dialog
{
    ImportCd* cdImport = new ImportCd(this);
    cdImport->exec();
}

void MainWindow::on_actionEject_Disc_triggered() //Eject Disc
{
    QProcess::startDetached("eject");
}

void MainWindow::on_dvdGoToChapter_clicked() //Go to DVD Chapter
{
    controller->setCurrentChapter(QInputDialog::getInt(this, "Go To Chapter", "Enter chapter number to go to", controller->currentChapter(), 1, controller->availableChapters()));
}

void MainWindow::on_dvdGoToTitle_clicked() //Go to DVD Title
{
    controller->setCurrentTitle(QInputDialog::getInt(this, "Go To Title", "Enter title number to go to", controller->currentTitle(), 1, controller->availableTitles()));
}

void MainWindow::on_createPlaylistButton_clicked() //Create a playlist
{
    QString playlistName = QInputDialog::getText(this, "Save Playlist", "What do you want to name this playlist?");
    if (playlistName != "") {
        QDir::home().mkpath(".themedia/playlists");
        QFile output(QDir::homePath() + "/.themedia/playlists/" + playlistName);
        if (output.exists()) {
            if (QMessageBox::warning(this, "Conflict", "You already have a playlist with the name " + playlistName + ". Do you want to overwrite it?", QMessageBox::Yes, QMessageBox::No, QMessageBox::No) == QMessageBox::No) {
                return;
            }
        }
        output.open(QFile::WriteOnly);
        for (MediaSource source : playlist) {
            QString write;

            if (source.type() == MediaSource::Url) {
                write = source.url().toString() + "\n";
            }

            output.write(write.toUtf8());
        }
        output.close();
    }
}

void MainWindow::on_actionSave_Playlist_triggered()
{
    on_createPlaylistButton_clicked();
}

void MainWindow::on_playlistWidget_customContextMenuRequested(const QPoint &pos)
{
    QMenu* menu = new QMenu(this);
    if (ui->playlistWidget->selectedItems().count() > 0) {
        menu->addSection("For \"" + ui->playlistWidget->selectedItems().first()->text() + "\"");
        menu->addAction(ui->actionRemove_from_Playlist);
    }
    menu->exec(ui->playlistWidget->mapToGlobal(pos));
}

void MainWindow::on_actionRemove_from_Playlist_triggered()
{
    if (ui->playlistWidget->selectedItems().count() > 0) {
        QListWidgetItem* item = ui->playlistWidget->selectedItems().first();
        playlist.removeAt(ui->playlistWidget->row(item));
        delete item;
    }
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
    } else if (event->type() == QEvent::KeyPress) {
        QKeyEvent* keyEvent = (QKeyEvent*) event;
        if (keyEvent->key() == Qt::Key_MediaPlay || keyEvent->key() == Qt::Key_Space) {
            on_playPause_clicked();
            return true;
        }
        return false;
    } else if (event->type() == QEvent::DragEnter) {
        QDragEnterEvent* dragEvent = (QDragEnterEvent*) event;
        if (dragEvent->mimeData()->hasUrls()) {
            dragEvent->acceptProposedAction();
        }
        return true;
    } else if (event->type() == QEvent::Drop) {
        QDropEvent* dropEvent = (QDropEvent*) event;
        const QMimeData* data = dropEvent->mimeData();

        if (data->hasUrls()) {
            for (QUrl url : data->urls()) {
                MediaSource source(url);
                playlist.append(source);
            }

            updatePlaylist();
        }

        dropEvent->acceptProposedAction();
        return true;
    }

    return false;
}
