#ifndef LOCAL_SOCKET_MANAGER_H
#define LOCAL_SOCKET_MANAGER_H

#include <QObject>

class LocalSocketManagerPrivate;
class LocalSocketManager: public QObject
{
    Q_OBJECT
public:
    LocalSocketManager();
    virtual ~LocalSocketManager();

    bool start();

protected:
    LocalSocketManagerPrivate * const pImpl;
    LocalSocketManager(LocalSocketManagerPrivate &); // Used for inheritage
private:
    Q_DISABLE_COPY(LocalSocketManager)
};

#endif // LOCAL_SOCKET_MANAGER_H
