#include "process_manager.h"

#include "private/process_manager_p.h"

ProcessManager::ProcessManager()
    :pImpl(new ProcessManagerPrivate())
{
}

ProcessManager::~ProcessManager()
{
    delete pImpl;
}

ProcessManager::ProcessManager(ProcessManagerPrivate & dp)
    :pImpl(&dp)
{
}

bool ProcessManager::start()
{
    return pImpl->start();
}

void ProcessManager::onAboutToQuit()
{
    pImpl->stop();
}
