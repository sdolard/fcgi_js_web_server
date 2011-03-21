#include <QtCore/QCoreApplication>

//#include <QThread>

#include "local_socket_manager.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    LocalSocketManager socketManager;

    if (!socketManager.start())
    {
        return 1;
    }
    qDebug("web server extension running");
    //qDebug("idealThreadCount: %d", QThread::idealThreadCount());
    return a.exec();
}
