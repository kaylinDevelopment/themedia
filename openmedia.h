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

#ifndef OPENMEDIA_H
#define OPENMEDIA_H

#include <QDialog>
#include <phonon/MediaSource>
#include <QFileDialog>
#include <QMenu>
#include <QMessageBox>

#include <gpod/itdb.h>

using namespace Phonon;

namespace Ui {
class OpenMedia;
}

class OpenMedia : public QDialog
{
    Q_OBJECT

public:
    explicit OpenMedia(QList<MediaSource>* playlist, QWidget *parent = 0);
    ~OpenMedia();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_playlistWidget_customContextMenuRequested(const QPoint &pos);

    void on_actionDelete_Playlist_triggered();

private:
    Ui::OpenMedia *ui;

    QList<MediaSource>* playlist;

    void reloadPlaylists();
};

#endif // OPENMEDIA_H
