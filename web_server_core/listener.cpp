#include <QSharedPointer>
#include <QList>

#include "http_socket.h"

#include "listener.h"

#include "private/listener_p.h"

// Listener
Listener::Listener(QObject *parent)
    :QTcpServer(parent),
    d_ptr(new ListenerPrivate(this))
{}

Listener::~Listener()
{
    delete d_ptr;
}

Listener::Listener(ListenerPrivate &dd, QObject *parent)
    :QTcpServer(parent),
    d_ptr(&dd) // Inheritace
{}

void Listener::incomingConnection ( int socketDescriptor )
{
    QSharedPointer<HttpSocket> socket = QSharedPointer<HttpSocket>(new HttpSocket(this), &QObject::deleteLater);
    if (!socket->setSocketDescriptor(socketDescriptor)){
        qDebug("Listener::incomingConnection: setSocketDescriptor failed");
    }
    Q_D(Listener);
    d->append(socket);
    qDebug("Listener::incomingConnection: new connection (total: %d)",
           d->socketList.count());
}
