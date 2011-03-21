#ifndef HTTPSOCKET_H
#define HTTPSOCKET_H

#include "abstract_socket.h"

class HttpSocketPrivate;
class HttpSocket : public AbstractSocket
{
    Q_OBJECT
public:
    HttpSocket(QObject * parent = 0);
    virtual ~HttpSocket();

protected:
    HttpSocket(HttpSocketPrivate &dd, QObject *parent); // Used for inheritance
    virtual HttpSocket & setReadMode(AbstractSocket::ReadMode);
    virtual const HttpSocket & log(const QString &) const;

private:
    Q_DISABLE_COPY(HttpSocket);
    Q_DECLARE_PRIVATE(HttpSocket);

private slots:
    void onReadyRead();
    void onBytesWritten ( qint64 bytes );
};

#endif // HTTPSOCKET_H
