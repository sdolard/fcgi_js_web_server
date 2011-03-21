#include "local_socket_manager.h"

#include "private/local_socket_manager_p.h"

LocalSocketManager::LocalSocketManager()
    :pImpl(new LocalSocketManagerPrivate())
{
}

LocalSocketManager::~LocalSocketManager()
{
    delete pImpl;
}

LocalSocketManager::LocalSocketManager(LocalSocketManagerPrivate & dp)
    :pImpl(&dp)
{
}

bool LocalSocketManager::start()
{
    return pImpl->start();
}
