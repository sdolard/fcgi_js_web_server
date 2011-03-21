#ifndef PROCESS_MANAGER_H
#define PROCESS_MANAGER_H

#include <QtGlobal>
#include <QObject>

class ProcessManagerPrivate;
class ProcessManager: public QObject
{
    Q_OBJECT
public:
    ProcessManager();
    virtual ~ProcessManager();

    // Start binary dependency
    bool start();

public slots:
    void onAboutToQuit();

protected:
    ProcessManagerPrivate * const pImpl;
    ProcessManager(ProcessManagerPrivate &); // Used for inheritage
private:
    Q_DISABLE_COPY(ProcessManager)
};

#endif // PROCESS_MANAGER_H
