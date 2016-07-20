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

#include "openmedia.h"
#include "ui_openmedia.h"

OpenMedia::OpenMedia(QList<MediaSource>* playlist, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OpenMedia)
{
    ui->setupUi(this);

    this->playlist = playlist;

    reloadPlaylists();


}

OpenMedia::~OpenMedia()
{
    delete ui;
}

void OpenMedia::on_pushButton_2_clicked()
{
    this->reject();
}

void OpenMedia::on_pushButton_clicked()
{
    switch (ui->tabWidget->currentIndex()) {
    case 0: //Playlist
        {
            QFile open(QDir::homePath() + "/.themedia/playlists/" + ui->playlistWidget->selectedItems().first()->text());
            open.open(QFile::ReadOnly);
            QString contents(open.readAll());
            open.close();

            for (QString file : contents.split("\n")) {
                if (file != "") {
                    MediaSource source(file);
                    playlist->append(source);
                }
            }
        }
        break;
    case 1: //CD/DVD
        {
        playlist->clear();
            if (ui->CdRadio->isChecked()) { //Try to play CD
                MediaSource source(Phonon::Cd, ui->deviceLine->text());
                playlist->append(source);
            } else {
                MediaSource source(Phonon::Dvd, ui->deviceLine->text()); //Try to play DVD
                playlist->append(source);
                //TODO: Buy a BD Reader and test Blu-Ray discs.
            }
        }
        break;
    case 2: //Media Stream
        {
            MediaSource source(QUrl::fromUserInput(ui->streamUrl->text()));
            playlist->append(source);
        }
        break;
    }

    this->accept();
}

void OpenMedia::on_pushButton_3_clicked()
{
    QFileDialog* fileDialog = new QFileDialog(this);
    fileDialog->setFileMode(QFileDialog::ExistingFiles);


    if (fileDialog->exec() == QDialog::Accepted) {
        for (QUrl file : fileDialog->selectedUrls()) {
            MediaSource source(file);
            playlist->append(source);
        }
        this->accept();
    }
}

void OpenMedia::on_playlistWidget_customContextMenuRequested(const QPoint &pos)
{
    QMenu* menu = new QMenu();
    if (ui->playlistWidget->selectedItems().count() > 0) {
        menu->addSection("For \"" + ui->playlistWidget->selectedItems().first()->text() + "\"");
        menu->addAction(ui->actionDelete_Playlist);
    }

    menu->exec(ui->playlistWidget->mapToGlobal(pos));
}

void OpenMedia::on_actionDelete_Playlist_triggered()
{
    if (QMessageBox::warning(this, "Delete Playlist", "This will delete the playlist permanently. Are you sure you want to do this?", QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::Yes) {
        QFile(QDir::homePath() + "/.themedia/playlists/" + ui->playlistWidget->selectedItems().first()->text()).remove();
        reloadPlaylists();
    }
}

void OpenMedia::reloadPlaylists() {
    ui->playlistWidget->clear();
    if (QDir(QDir::homePath() + "/.themedia/playlists").exists()) {
        for (QString file : QDir(QDir::homePath() + "/.themedia/playlists").entryList()) {
            if (file != "." && file != "..") {
                ui->playlistWidget->addItem(file);
            }
        }
    }
}
