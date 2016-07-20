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

#include "mprisdbus.h"

mprisDbus::mprisDbus(QObject *parent) : QObject(parent)
{
    new MediaPlayer2Adaptor(parent);
}

void mprisDbus::Raise() {}

void mprisDbus::Quit() {}

bool mprisDbus::CanQuit() {}

bool mprisDbus::CanRaise() {}

bool mprisDbus::HasTrackList() {}

QString mprisDbus::Identity() {}

QString mprisDbus::DesktopEntry() {}

QStringList mprisDbus::SupportedMimeTypes() {}

QStringList mprisDbus::SupportedUriSchemes() {}

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

quint64 mprisDbusPlayer::Position() {}

double mprisDbusPlayer::Volume() {}

double mprisDbusPlayer::Rate() {}

double mprisDbusPlayer::MinimumRate() {}

double mprisDbusPlayer::MaximumRate() {}

bool mprisDbusPlayer::CanControl() {}

bool mprisDbusPlayer::CanPlay() {}

bool mprisDbusPlayer::CanPause() {}

bool mprisDbusPlayer::CanGoPrevious() {}

bool mprisDbusPlayer::CanGoNext() {}
