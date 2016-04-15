#include "openmedia.h"
#include "ui_openmedia.h"

OpenMedia::OpenMedia(QList<MediaSource>* playlist, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OpenMedia)
{
    ui->setupUi(this);

    this->playlist = playlist;
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
        break;
    case 1: //Media Stream
        {
            MediaSource source(QUrl::fromUserInput(ui->streamUrl->text()));
            playlist->append(source);
        }
        break;
    case 2: //CD/DVD
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
