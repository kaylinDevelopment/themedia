#include "importcd.h"
#include "ui_importcd.h"

ImportCd::ImportCd(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ImportCd)
{
    ui->setupUi(this);
}

ImportCd::~ImportCd()
{
    delete ui;
}
