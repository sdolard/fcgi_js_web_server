#include <QDateTime>

#include "abstract_socket.h"

#include "private/abstract_socket_p.h"

AbstractSocket::AbstractSocket( QObject * parent)
    :QSslSocket(parent),
      d_ptr(new AbstractSocketPrivate(this))
{
    connectPrivateSignals();
}

AbstractSocket::AbstractSocket(AbstractSocketPrivate &dd, QObject *parent)
    :QSslSocket(parent),
      d_ptr(&dd) // Inheritance
{
    connectPrivateSignals();
}

AbstractSocket::~AbstractSocket()
{
    log("Deleted");
    delete d_ptr;
}

void AbstractSocket::connectPrivateSignals()
{

    // Used to manage socket expiration timeout
    // Expiration delay is reinit when a bytes is writted
    connect(this, SIGNAL(bytesWritten(qint64)),
            this, SLOT(onBytesWritten(qint64)));

    // Used to manage socket expiration timeout
    // Expiration delay is reinit when data are available is read
    connect(this, SIGNAL(readyRead()),
            this, SLOT(onReadyRead()));

    // Those next signals are used as debug, for now
    connect(this, SIGNAL(connected()),
            this, SLOT(onConnected()));
    connect(this, SIGNAL(disconnected()),
            this, SLOT(onDisconnected()));
    connect(this, SIGNAL(error ( QAbstractSocket::SocketError)),
            this, SLOT(onError ( QAbstractSocket::SocketError)));
    connect(this, SIGNAL(hostFound ()),
            this, SLOT(onHostFound ()));
    connect(this, SIGNAL(proxyAuthenticationRequired ( const QNetworkProxy &, QAuthenticator *)),
            this, SLOT(onProxyAuthenticationRequired ( const QNetworkProxy &, QAuthenticator *)));
    connect(this, SIGNAL(stateChanged (QAbstractSocket::SocketState)),
            this, SLOT(onStateChanged (QAbstractSocket::SocketState)));
    connect(this, SIGNAL(aboutToClose()),
            this, SLOT(onAboutToClose()));
    connect(this, SIGNAL(readChannelFinished()),
            this, SLOT(onReadChannelFinished()));
}

QString AbstractSocket::toString(AbstractSocket::ReadMode rm) // static
{
    switch(rm)
    {
    case AbstractSocket::rmLine: return "rmLine";
    case AbstractSocket::rmStream: return "rmStream";
    default:
        Q_ASSERT(false);
        return "Unmanaged AbstractSocket::ReadMode";
    }
}

qint64 AbstractSocket::expirationDelay() const
{
    Q_D(const AbstractSocket);

    return d->expirationDelay;
}

AbstractSocket & AbstractSocket::setExpirationDelay(qint64 ms)
{
    Q_D(AbstractSocket);

    d->expirationDelay = ms;
    return *this;
}

bool AbstractSocket::isExpired() const
{
    Q_D(const AbstractSocket);

    return d->lastUsage.msecsTo(QDateTime::currentDateTime()) > d->expirationDelay;
}

AbstractSocket::ReadMode AbstractSocket::readMode() const
{
    Q_D(const AbstractSocket);

    return d->readMode;
}

AbstractSocket & AbstractSocket::setReadMode(AbstractSocket::ReadMode rm)
{
    Q_D(AbstractSocket);

    d->readMode = rm;
    log(toString(rm));
    return *this;
}

const AbstractSocket & AbstractSocket::log(const QString & l) const
{
    qDebug("(AbstractSocket %p) %s", this, qPrintable(l));
    return *this;
}

void AbstractSocket::onBytesWritten(qint64 bytes)
{
    Q_D(AbstractSocket);
    d->lastUsage = QDateTime::currentDateTime();
    d->restartExpirationDelayTimer();
    log(QString("BytesWritten: %1").arg(bytes));
}

void AbstractSocket::onReadyRead()
{
    Q_D(AbstractSocket);
    d->lastUsage = QDateTime::currentDateTime();
    d->restartExpirationDelayTimer();
    log("onReadyRead");
}

void AbstractSocket::onConnected()
{
    log("onConnected");
}

void AbstractSocket::onDisconnected()
{
    log("onDisconnected");
    emit clientCloseConnection(socketDescriptor());
}

void AbstractSocket::onError(QAbstractSocket::SocketError socketError)
{
    switch(socketError){
    case QAbstractSocket::ConnectionRefusedError:
        log("onError: ConnectionRefusedError");
        break;
    case QAbstractSocket::RemoteHostClosedError:
        log("onError: RemoteHostClosedError");
        break;
    case QAbstractSocket::HostNotFoundError:
        log("onError: HostNotFoundError");
        break;
    case QAbstractSocket::SocketAccessError:
        log("onError: SocketAccessError");
        break;
    case QAbstractSocket::SocketResourceError:
        log("onError: SocketResourceError");
        break;
    case QAbstractSocket::SocketTimeoutError:
        log("onError: SocketTimeoutError");
        break;                     /* 5 */
    case QAbstractSocket::DatagramTooLargeError:
        log("onError: DatagramTooLargeError");
        break;
    case QAbstractSocket::NetworkError:
        log("onError: NetworkError");
        break;
    case QAbstractSocket::AddressInUseError:
        log("onError: AddressInUseError");
        break;
    case QAbstractSocket::SocketAddressNotAvailableError:
        log("onError: SocketAddressNotAvailableError");
        break;
    case QAbstractSocket::UnsupportedSocketOperationError:
        log("onError: UnsupportedSocketOperationError");
        break;       /* 10 */
    case QAbstractSocket::UnfinishedSocketOperationError:
        log("onError: UnfinishedSocketOperationError");
        break;
    case QAbstractSocket::ProxyAuthenticationRequiredError:
        log("onError: ProxyAuthenticationRequiredError");
        break;
    case QAbstractSocket::SslHandshakeFailedError:
        log("onError: SslHandshakeFailedError");
        break;
    case QAbstractSocket::ProxyConnectionRefusedError:
        log("onError: ProxyConnectionRefusedError");
        break;
    case QAbstractSocket::ProxyConnectionClosedError:
        log("onError: ProxyConnectionClosedError");
        break;            /* 15 */
    case QAbstractSocket::ProxyConnectionTimeoutError:
        log("onError: ProxyConnectionTimeoutError");
        break;
    case QAbstractSocket::ProxyNotFoundError:
        log("onError: ProxyNotFoundError");
        break;
    case QAbstractSocket::ProxyProtocolError:
        log("onError: ProxyProtocolError");
        break;
    case QAbstractSocket::UnknownSocketError:
        log("onError: UnknownSocketError");
        break;
    default:
        log("Unmanaged socketError");
        Q_ASSERT(false);
    }
}

void AbstractSocket::onHostFound()
{
    log("onHostFound");
}

void AbstractSocket::onProxyAuthenticationRequired( const QNetworkProxy & , QAuthenticator * )
{
    log("onProxyAuthenticationRequired (TODO)");
}

void AbstractSocket::onStateChanged( QAbstractSocket::SocketState socketState )
{
    switch(socketState){
    case QAbstractSocket::UnconnectedState:
        log("onStateChanged: UnconnectedState");
        break;
    case QAbstractSocket::HostLookupState:
        log("onStateChanged: HostLookupState");
        break;
    case QAbstractSocket::ConnectingState:
        log("onStateChanged: ConnectingState");
        break;
    case QAbstractSocket::ConnectedState:
        log("onStateChanged: ConnectedState");
        break;
    case QAbstractSocket::BoundState:
        log("onStateChanged: BoundState");
        break;
    case QAbstractSocket::ListeningState:
        log("onStateChanged: ListeningState");
        break;
    case QAbstractSocket::ClosingState:
        log("onStateChanged: ClosingState");
        break;
    default:
        log("onStateChanged: Unmanaged SocketState");
        Q_ASSERT(false);
    }
}

void AbstractSocket::onAboutToClose()
{
    log("onAboutToClose");
}

void AbstractSocket::onReadChannelFinished()
{
    log("onReadChannelFinished");
}

void AbstractSocket::timerEvent(QTimerEvent * event) // TODO: each socket must self managed > done?
{
    Q_D(AbstractSocket);
    if (event->timerId() == d->expirationDelayTimerId)
    {
        log("AbstractSocket::timerEvent: expired");
        emit expired(socketDescriptor());
        return;
    }
    QSslSocket::timerEvent(event);
}
