#include "mprisdbus.h"

mprisDbus::mprisDbus(QObject *parent) : QObject(parent)
{
    new MediaPlayer2Adaptor(parent);
}

void mprisDbus::raise() {}

void mprisDbus::quit() {}

bool mprisDbus::CanQuit() {}

bool mprisDbus::CanRaise() {}

bool mprisDbus::HasTrackList() {}

QString mprisDbus::Identity() {}

QString mprisDbus::DesktopEntry() {}

QStringList mprisDbus::SupportedMimeTypes() {}

mprisDbusPlayer::mprisDbusPlayer(QObject* parent) : QObject(parent) {
    new PlayerAdaptor(parent);
}

void mprisDbusPlayer::Next() {}

void mprisDbusPlayer::Previous() {}

void mprisDbusPlayer::Pause() {}

void mprisDbusPlayer::PlayPause() {}

void mprisDbusPlayer::Stop() {}

void mprisDbusPlayer::Play() {}

void mprisDbusPlayer::SetPosition() {}

void mprisDbusPlayer::OpenUri(QUrl uri) {}

QString mprisDbusPlayer::PlaybackStatus() {}

QVariantMap mprisDbusPlayer::Metadata() {}
