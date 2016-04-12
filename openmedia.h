#ifndef OPENMEDIA_H
#define OPENMEDIA_H

#include <QDialog>
#include <phonon/MediaSource>
#include <QFileDialog>

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

private:
    Ui::OpenMedia *ui;

    QList<MediaSource>* playlist;
};

#endif // OPENMEDIA_H
