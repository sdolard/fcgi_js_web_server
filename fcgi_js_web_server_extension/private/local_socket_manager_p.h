#ifndef LOCAL_SOCKET_MANAGER_P_H
#define LOCAL_SOCKET_MANAGER_P_H

#include <QLocalSocket>
#include <QCoreApplication>

#include "../local_socket_manager.h"

class LocalSocketManagerPrivate: public QObject
{
    Q_OBJECT
public:
    LocalSocketManagerPrivate()
    {
        connect(&socket, SIGNAL(connected()),
                this, SLOT(onConnected()));
        connect(&socket, SIGNAL(disconnected()),
                this, SLOT(onDisconnected()));
        connect(&socket, SIGNAL(error(QLocalSocket::LocalSocketError)),
                this, SLOT(onError(QLocalSocket::LocalSocketError)));
        connect(&socket, SIGNAL(stateChanged(QLocalSocket::LocalSocketState)),
                this, SLOT(onStateChanged(QLocalSocket::LocalSocketState)));

        connect(&socket, SIGNAL(aboutToClose()),
                this, SLOT(onAboutToClose()));
        connect(&socket, SIGNAL(bytesWritten( qint64 )),
                this, SLOT(onBytesWritten( qint64 )));
        connect(&socket, SIGNAL(readChannelFinished()),
                this, SLOT(onReadChannelFinished()));
        connect(&socket, SIGNAL(readyRead()),
                this, SLOT(onReadyRead()));
    }

    virtual ~LocalSocketManagerPrivate()
    {
    }

    bool start()
    {
        socket.connectToServer("fcgi_js_web_server_core");
        return socket.waitForConnected();
    }

    const QString toString(QLocalSocket::LocalSocketError socketError) const
    {
        switch(socketError)
        {
        case QLocalSocket::ConnectionRefusedError:
            return "The connection was refused by the peer (or timed out).";
        case QLocalSocket::PeerClosedError:
            return "The remote socket closed the connection. Note that the client socket (i.e., this socket) will be closed after the remote close notification has been sent.";
        case QLocalSocket::ServerNotFoundError:
            return "The local socket name was not found.";
        case QLocalSocket::SocketAccessError:
            return "The socket operation failed because the application lacked the required privileges.";
        case QLocalSocket::SocketResourceError:
            return "The local system ran out of resources (e.g., too many sockets).";
        case QLocalSocket::SocketTimeoutError:
            return "The socket operation timed out.";
        case QLocalSocket::DatagramTooLargeError:
            return "The datagram was larger than the operating system's limit (which can be as low as 8192 bytes).";
        case QLocalSocket::ConnectionError:
            return "An error occurred with the connection.";
        case QLocalSocket::UnsupportedSocketOperationError:
            return "The requested socket operation is not supported by the local operating system.";
        case QLocalSocket::UnknownSocketError:
            return "An unidentified error occurred";
        default:
            Q_ASSERT_X(false, "LocalSocketManagerPrivate","toString(QLocalSocket::LocalSocketError socketError)");
            return "An unmanaged error occurred";
        }
    }
    const QString toString(QLocalSocket::LocalSocketState state) const
    {
        switch(state)
        {
        case QLocalSocket::UnconnectedState:
            return "The socket is not connected.";
        case QLocalSocket::ConnectingState:
            return "The socket has started establishing a connection.";
        case QLocalSocket::ConnectedState:
            return "A connection is established.";
        case QLocalSocket::ClosingState:
            return "The socket is about to close (data may still be waiting to be written).";
        default:
            Q_ASSERT_X(false, "LocalSocketManagerPrivate","toString(QLocalSocket::LocalSocketState state)");
            return "An unmanaged error occurred";
        }
    }

    QLocalSocket socket;

public slots:
    void onConnected ()
    {
        qDebug("Connected");
    }

    void onDisconnected ()
    {
        qDebug("Disconnected");
#ifndef DEV
        qApp->exit(0);
#endif
    }

    void onError ( QLocalSocket::LocalSocketError socketError )
    {
        qDebug("%s", qPrintable(toString(socketError)));
#ifndef DEV
        qApp->exit(1);
#endif
    }

    void onStateChanged ( QLocalSocket::LocalSocketState socketState )
    {
        qDebug("%s", qPrintable(toString(socketState)));
    }

    void onAboutToClose ()
    {
        qDebug("onAboutToClose");
    }

    void onBytesWritten ( qint64 bytes )
    {
        qDebug("onBytesWritten: %lld", bytes);
    }

    void onReadChannelFinished ()
    {
        qDebug("onReadChannelFinished");
    }

    void onReadyRead()
    {
        qDebug("onReadyRead: %s", qPrintable(QString(socket.readAll())));
    }

private:
    Q_DISABLE_COPY(LocalSocketManagerPrivate);
};

#endif // LOCAL_SOCKET_MANAGER_P_H
