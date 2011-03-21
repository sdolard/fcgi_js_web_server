#ifndef LISTENER_P_H
#define LISTENER_P_H

#include <QObject>
#include <QList>
#include <QSharedPointer>

#include "../http_socket.h"

#include "../listener.h" // For "moced" part

class ListenerPrivate: public QObject
{
    Q_OBJECT
public:
    ListenerPrivate(Listener * parent)
        :QObject(parent),
        q_ptr(parent)
    {
    }

    Listener * const q_ptr;
    Q_DECLARE_PUBLIC(Listener);

    void append(QSharedPointer<HttpSocket> socket)
    {
        connect(socket.data(), SIGNAL(expired(int)), this, SLOT(onSocketExpired(int)));
        connect(socket.data(), SIGNAL(clientCloseConnection(int)), this, SLOT(onClientCloseConnection(int)));
        socketList.append(socket);
    }

    QList< QSharedPointer<HttpSocket> >socketList;

private slots:
    void onSocketExpired(int socketDescriptor)
    {
        qDebug("ListenerPrivate::onSocketExpired");
        int i = socketList.count() -1;
        while (i >= 0)
        {
            QSharedPointer<HttpSocket> socket = socketList.at(i);
            if (socket->socketDescriptor() != socketDescriptor)
            {
                --i;
                continue;
            }
            socketList.removeAt(i);
            qDebug("Socket deleted %p (timeout) (remains %d connections)",
                   socket.data(),
                   socketList.count());
            return;
        }
    }

    void onClientCloseConnection(int socketDescriptor)
    {
        qDebug("ListenerPrivate::onClientCloseConnection");
        int i = socketList.count() -1;
        while (i >= 0)
        {
            QSharedPointer<HttpSocket> socket = socketList.at(i);
            if (socket->socketDescriptor() != socketDescriptor)
            {
                --i;
                continue;
            }
            socketList.removeAt(i);
            qDebug("Socket deleted %p (client close) (remains %d connections)",
                   socket.data(),
                   socketList.count());
            return;
        }
    }
};

#endif // LISTENER_P_H
