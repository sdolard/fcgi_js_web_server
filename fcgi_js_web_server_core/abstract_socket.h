#ifndef ABSTRACTSOCKET_H
#define ABSTRACTSOCKET_H

#include <QSslSocket>
#include <QTimerEvent>

// Abstract socket
class AbstractSocketPrivate;
class AbstractSocket : public QSslSocket
{
    Q_OBJECT
public:
    // Server read mode
    enum ReadMode {
        rmStream = 0x00, // as stream > no end
        rmLine = 0x01    // as line > end of line
    };

    AbstractSocket( QObject * parent = 0);
    virtual ~AbstractSocket();

    // Returns duration as milliseconds
    // Default 30000 ms
    qint64 expirationDelay() const;
    AbstractSocket & setExpirationDelay(qint64 ms);

    // Returns true if there is no activity on socket since more than expirationDelay()
    bool isExpired() const;

    // Returns a human readable version of ReadMode
    static QString toString(ReadMode);

signals:
    // Emit when socket is expired
    void expired(int socketDescriptor);

    // Emit when client has closed socket
    void clientCloseConnection(int socketDescriptor);

protected:
    // Private implementation
    // Protected cos of inheritance
    AbstractSocketPrivate * const d_ptr;

    // Private constructor used in inheritance
    AbstractSocket(AbstractSocketPrivate &dd, QObject *parent);

    // ReadMode getter
    virtual ReadMode readMode() const;
    // ReadMode setter
    virtual AbstractSocket & setReadMode(ReadMode);

    // Basic log method.
    // Write params and "this" pointer
    virtual const AbstractSocket & log(const QString &) const;

    // Used to manage socket expiration delay
    virtual void timerEvent(QTimerEvent *);

private:
    // dpointer functions
    Q_DECLARE_PRIVATE(AbstractSocket);

    // Not copiable
    Q_DISABLE_COPY(AbstractSocket);

    // Connect private signals
    void connectPrivateSignals();

private slots:
    // Those slots, emited by this, can not be manage in private implementation
    void onBytesWritten(qint64 bytes);
    void onReadyRead();
    void onConnected();
    void onDisconnected();
    void onError(QAbstractSocket::SocketError socketError);
    void onHostFound();
    void onProxyAuthenticationRequired( const QNetworkProxy & , QAuthenticator * );
    void onStateChanged( QAbstractSocket::SocketState socketState );
    void onAboutToClose();
    void onReadChannelFinished();
};

#endif // ABSTRACTSOCKET_H
