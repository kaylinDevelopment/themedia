#ifndef OPENMEDIA_H
#define OPENMEDIA_H

#include <QDialog>
#include <phonon/MediaSource>
#include <QFileDialog>
#include <QMenu>
#include <QMessageBox>

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
