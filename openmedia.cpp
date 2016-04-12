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
    case 2: //CD
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
