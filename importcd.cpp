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
