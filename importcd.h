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
