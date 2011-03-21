#include <QStringList>
#include <QHash>
#include <QQueue>

#include "http_socket.h"

#include "private/http_socket_p.h"

// HttpSocket
HttpSocket::HttpSocket(QObject * parent)
    :AbstractSocket(*new HttpSocketPrivate(this), parent)
{
    setReadMode(AbstractSocket::rmLine);

    connect(this, SIGNAL(readyRead()),
            this, SLOT(onReadyRead()));
    connect(this, SIGNAL(bytesWritten(qint64)),
            this, SLOT(onBytesWritten(qint64)));
}

HttpSocket::HttpSocket(HttpSocketPrivate &dd, QObject *parent) // Used for inheritance
    :AbstractSocket(dd, parent)
{}

HttpSocket::~HttpSocket()
{}

HttpSocket & HttpSocket::setReadMode(ReadMode rm)
{
    AbstractSocket::setReadMode(rm);
    return *this;
}

const HttpSocket & HttpSocket::log(const QString & l) const
{
    qDebug("(HttpSocket %p) %s", this, qPrintable(l));
    return *this;
}

void HttpSocket::onReadyRead()
{
    Q_D(HttpSocket);
    switch(readMode())
    {
    case AbstractSocket::rmLine:
        d->readMessageHeader();
        break;
    case AbstractSocket::rmStream:
        d->readMessageBody();
        break;
    default:
        Q_ASSERT(false);
    }
}

void HttpSocket::onBytesWritten ( qint64 bytes )
{
    // This signal is emitted every time a payload of data has been written to the device.
    // The bytes argument is set to the number of bytes that were written in this payload.
    Q_D(HttpSocket);
    d->bytesWritten(bytes);
}

