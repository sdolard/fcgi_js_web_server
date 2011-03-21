#ifndef ABSTRACTSOCKET_P_H
#define ABSTRACTSOCKET_P_H

#include <QDateTime>

// This class can not inherits from QObject in order to
// manager AbstractSocket signals, this his to early
class AbstractSocketPrivate
{
public:
    AbstractSocketPrivate(AbstractSocket * parent)
        :q_ptr( parent),
          expirationDelay(30000), // ms, 30s
          expirationDelayTimerId(0),
          lastUsage(QDateTime::currentDateTime()),
          readMode(AbstractSocket::rmStream)
    {}

    virtual void log(const QString & l) const
    {
        qDebug("(AbstractSocketPrivate %p) %s", this, qPrintable(l));
    }

    void restartExpirationDelayTimer() {
        Q_Q(AbstractSocket);
        if (expirationDelayTimerId != 0) {
            q->killTimer(expirationDelayTimerId);
        }
        expirationDelayTimerId = q->startTimer(30000);
    }

    AbstractSocket * const q_ptr;
    qint64 expirationDelay; // ms
    int  expirationDelayTimerId;
    QDateTime lastUsage;
    AbstractSocket::ReadMode readMode;
    Q_DECLARE_PUBLIC(AbstractSocket);
};

#endif // ABSTRACTSOCKET_P_H
