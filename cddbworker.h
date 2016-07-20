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

#ifndef CDDBWORKER_H
#define CDDBWORKER_H

#include <QObject>
#include <QMap>
#include <musicbrainz5/Query.h>
#include <musicbrainz5/Disc.h>
#include <musicbrainz5/Release.h>
#include <musicbrainz5/ReleaseList.h>
#include <musicbrainz5/ReleaseGroup.h>
#include <musicbrainz5/Medium.h>
#include <musicbrainz5/TrackList.h>
#include <musicbrainz5/Track.h>
#include <musicbrainz5/Recording.h>
#include <musicbrainz5/ArtistCredit.h>
#include <musicbrainz5/NameCredit.h>
#include <musicbrainz5/Artist.h>

class CddbWorker : public QObject
{
    Q_OBJECT
public:
    explicit CddbWorker(QList<QMap<QString, QString>>* cddbList, QStringList discIds, QObject *parent = 0);

signals:
    void finished();

public slots:
    void process();

private:
    QList<QMap<QString, QString>>* cddbList;
    QStringList discIds;
};

#endif // CDDBWORKER_H
