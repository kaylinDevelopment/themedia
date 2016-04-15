#include "cddbworker.h"

CddbWorker::CddbWorker(QList<QMap<QString, QString>> *cddbList, QStringList discIds, QObject *parent) : QObject(parent)
{
    this->cddbList = cddbList;
    this->discIds = discIds;
}

void CddbWorker::process() {
    MusicBrainz5::CQuery query("themedia-1.0");
    try {
        MusicBrainz5::CMetadata data = query.Query("discid", discIds.at(0).toStdString());
        if (data.Disc() && data.Disc()->ReleaseList()) {
            MusicBrainz5::CReleaseList *releaseList = data.Disc()->ReleaseList();

            for (int count = 0; count < releaseList->NumItems(); count++) {
                    MusicBrainz5::CRelease *release = releaseList->Item(count);
                    //The releases returned from LookupDiscID don't contain full information
                    MusicBrainz5::CQuery::tParamMap Params;
                    Params["inc"] = "artists labels recordings release-groups url-rels discids artist-credits";
                    MusicBrainz5::CMetadata fullData = query.Query("release", release->ID(), "", Params);
                    if (fullData.Release()) {
                            MusicBrainz5::CRelease *FullRelease = fullData.Release();
                            MusicBrainz5::CMediumList MediaList = FullRelease->MediaMatchingDiscID(discIds.at(0).toStdString());

                            if (0 != MediaList.NumItems()) {
                                for (int count = 0; count < MediaList.NumItems(); count++) {
                                    MusicBrainz5::CMedium *Medium = MediaList.Item(count);

                                    MusicBrainz5::CTrackList *TrackList = Medium->TrackList();
                                    if (TrackList) {

                                        cddbList->clear();

                                        for (int count = 0; count < TrackList->NumItems(); count++) {
                                            MusicBrainz5::CTrack *Track = TrackList->Item(count);
                                            MusicBrainz5::CRecording *Recording = Track->Recording();

                                            QMap<QString, QString> trackInfo;

                                            if (Recording) {
                                                trackInfo.insert("name", QString::fromStdString(Recording->Title()));
                                                trackInfo.insert("artist", QString::fromStdString(Recording->ArtistCredit()->NameCreditList()->Item(0)->Artist()->Name()));
                                            } else {
                                                trackInfo.insert("name", QString::fromStdString(Track->Title()));
                                                trackInfo.insert("artist", QString::fromStdString(Track->ArtistCredit()->NameCreditList()->Item(0)->Artist()->Name()));
                                            }

                                            trackInfo.insert("album", QString::fromStdString(FullRelease->ReleaseGroup()->Title()));
                                            //trackInfo.insert("artist", QString::fromStdString(FullRelease->ArtistCredit()->NameCreditList()->Item(0)->Artist()->Name()));

                                            cddbList->append(trackInfo);
                                            //Track->ArtistCredit()->NameCreditList()->Item(0)->Name();
                                        }

                                    }
                                }
                            }

                    }
            }
    }
    } catch (...) {
    }

    emit finished();
}
