#include <QtCore/QCoreApplication>

#include "process_manager.h"
#include "listener.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    ProcessManager workerManager;
    QObject::connect(&a, SIGNAL(aboutToQuit()),
                     &workerManager, SLOT(onAboutToQuit()));

    if (!workerManager.start())
    {
         qDebug("Server stopped");
        return 1;
    }
    Listener listener;
    if (!listener.listen(QHostAddress::Any, 8080))
    {
        qDebug("listen error: %s", qPrintable(listener.errorString()));
        return -1;
    }
    qDebug("Listening on %d... (max pending conn: %d)\n",
           listener.serverPort(),
           listener.maxPendingConnections ());
    return a.exec();
}
