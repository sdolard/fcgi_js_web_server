#ifndef LISTENER_H
#define LISTENER_H

#include <QTcpServer>

class ListenerPrivate;
class Listener: public QTcpServer
{
    Q_OBJECT
public:
    explicit Listener(QObject *parent = 0); // ctor
    virtual ~Listener(); // dtor
protected:
    ListenerPrivate * const d_ptr;
    Listener(ListenerPrivate &dd, QObject *parent); // Used for inheritance
    void incomingConnection ( int socketDescriptor );
private:
    Q_DECLARE_PRIVATE(Listener);
    Q_DISABLE_COPY(Listener);
};

#endif // LISTENER_H
