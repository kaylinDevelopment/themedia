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

#include "nativeeventfilter.h"

NativeEventFilter::NativeEventFilter(QObject *parent) : QObject(parent)
{
    lastPress.start();
}

bool NativeEventFilter::nativeEventFilter(const QByteArray &eventType, void *message, long *result) {
    if (eventType == "xcb_generic_event_t") {
        xcb_generic_event_t* event = static_cast<xcb_generic_event_t*>(message);
        if (event->response_type == XCB_KEY_PRESS) {
            if (lastPress.restart() > 100) {
                xcb_key_press_event_t* button = static_cast<xcb_key_press_event_t*>(message);

                if (button->detail == XKeysymToKeycode(QX11Info::display(), XF86XK_AudioPlay) || button->detail == XKeysymToKeycode(QX11Info::display(), XF86XK_AudioStop)) {
                    emit PlayPause();
                } else if (button->detail == XKeysymToKeycode(QX11Info::display(), XF86XK_AudioNext)) {
                    emit Next();
                } else if (button->detail == XKeysymToKeycode(QX11Info::display(), XF86XK_AudioPrev)) {
                    emit Previous();
                }
            }
        }
    }
    return false;
}
