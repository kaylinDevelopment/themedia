#ifndef IMPORTCD_H
#define IMPORTCD_H

#include <QDialog>
#include <cdda_interface.h>
#include <cdda_paranoia.h>


namespace Ui {
class ImportCd;
}

class ImportCd : public QDialog
{
    Q_OBJECT

public:
    explicit ImportCd(QWidget *parent = 0);
    ~ImportCd();

private:
    Ui::ImportCd *ui;
};

#endif // IMPORTCD_H
