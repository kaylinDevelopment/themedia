#include "importcd.h"
#include "ui_importcd.h"

ImportCd::ImportCd(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ImportCd)
{
    ui->setupUi(this);

    cdrom_drive* drive;
    drive->cdda_device_name = "cdda://0";

    //cdrom_paranoia* cdrom = paranoia_init(drive);

}

ImportCd::~ImportCd()
{
    delete ui;
}
