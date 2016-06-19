#ifndef NATIVEEVENTFILTER_H
#define NATIVEEVENTFILTER_H

#include <QObject>
#include <QAbstractNativeEventFilter>
#include <QDebug>
#include <QX11Info>
#include <QTime>
#include <xcb/xcb.h>
#include <xcb/xproto.h>
#include <Xlib.h>
#include <X11/XF86keysym.h>

class NativeEventFilter : public QObject, public QAbstractNativeEventFilter
{
    Q_OBJECT
public:
    explicit NativeEventFilter(QObject *parent = 0);

signals:
    void PlayPause();
    void Next();
    void Previous();

public slots:

private:
    bool nativeEventFilter(const QByteArray &eventType, void *message, long *result);

    QTime lastPress;
};

#endif // NATIVEEVENTFILTER_H
