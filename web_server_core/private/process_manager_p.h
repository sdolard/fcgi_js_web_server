#ifndef PROCESS_MANAGER_P_H
#define PROCESS_MANAGER_P_H

#include <QProcess>
#include <QCoreApplication>
#include <QTime>
#include <QLocalServer>

#include "../process_manager.h"

#ifdef DEBUG
#   define WEB_SERVER_EXT_NAME "../web_server_extention/web_server_extentiond"
#else
#   define WEB_SERVER_EXT_NAME "../web_server_extention/web_server_extention"
#endif

class ProcessManagerPrivate: public QObject
{
    Q_OBJECT
public:
    ProcessManagerPrivate()
        :errorCountPerSecond(0)
    {
        // server
        connect(&server, SIGNAL(newConnection()),
                this, SLOT(onServerNewConnection()));


        // extention
        webServerExtention.setWorkingDirectory(QCoreApplication::applicationDirPath());
        webServerExtentionFilePath = QString("%1/%2").arg(QCoreApplication::applicationDirPath()).arg(WEB_SERVER_EXT_NAME);

        connect(&webServerExtention, SIGNAL(readyReadStandardOutput()),
                this, SLOT(onProcessReadyReadStandardOutput()));
        connect(&webServerExtention, SIGNAL(readyReadStandardError()),
                this, SLOT(onProcessReadyReadStandardError()));
        connect(&webServerExtention, SIGNAL(error(QProcess::ProcessError)),
                this, SLOT(onProcessError(QProcess::ProcessError)));
        connect(&webServerExtention, SIGNAL(finished(int, QProcess::ExitStatus)),
                this, SLOT(onProcessFinished(int, QProcess::ExitStatus)));
        connect(&webServerExtention, SIGNAL(stateChanged(QProcess::ProcessState)),
                this, SLOT(onProcessStateChanged(QProcess::ProcessState)));
    }

   virtual ~ProcessManagerPrivate()
    {
        stop();
    }

    bool start()
    {
        return startServer() && startProcess();
    }

    bool startServer()
    {
        if (QLocalServer::removeServer("web_server_core"))
            qDebug("Previous LocalServer instance cleaned up");

        if (server.listen("web_server_core"))
        {
            qDebug("LocalServer listening...");
            return true;
        }
        qDebug("LocalServer error: %s", qPrintable(server.errorString()));
        return false;
    }

    bool startProcess(bool blocking = true)
    {
        if (errorCountPerSecond > 3)
        {
            qDebug("At least 3 errors occured on %s in 1 sec. Process will not be auto-restarted", WEB_SERVER_EXT_NAME);
            return false;
        }

        webServerExtention.start(webServerExtentionFilePath, QIODevice::ReadOnly);
        if (blocking)
            return webServerExtention.waitForStarted();
        return true;
    }

    void stop()
    {
        stopProcess();
        stopServer();
    }

    void stopServer()
    {
        server.close();
    }

    void stopProcess()
    {
        switch(webServerExtention.state())
        {
        case QProcess::Starting:
        case QProcess::Running:
            webServerExtention.terminate();
            if(webServerExtention.waitForFinished(5000)) // 5s
            {
                qDebug("%s terminated", WEB_SERVER_EXT_NAME);
                return;
            }
            webServerExtention.close();
            qDebug("%s killed", WEB_SERVER_EXT_NAME);
            return;
        default:
            return;
        }
    }

    const QString toString(QProcess::ProcessError error) const
    {
        switch(error){
        case QProcess::FailedToStart:
            return "The process failed to start. Either the invoked program is missing, or you may have insufficient permissions to invoke the program.";
        case QProcess::Crashed:
            return "The process crashed some time after starting successfully.";
        case QProcess::Timedout:
            return "The last waitFor...() function timed out. The state of QProcess is unchanged, and you can try calling waitFor...() again.";
        case QProcess::WriteError:
            return "An error occurred when attempting to write to the process. For example, the process may not be running, or it may have closed its input channel.";
        case QProcess::ReadError:
            return "An error occurred when attempting to read from the process. For example, the process may not be running.";
        case QProcess::UnknownError:
            return "An unknown error occurred. This is the default return value of error().";
        default:
            Q_ASSERT(false);
            return QString("toString(QProcess::ProcessError error): unmanaged QProcess::ProcessError: %1").arg(error);
        }
    }

    const QString toString(QProcess::ExitStatus exitStatus) const
    {
        switch(exitStatus)
        {
        case QProcess::NormalExit:
            return "The process exited normally";

        case QProcess::CrashExit:
            return "The process crashed.";
        default:
            Q_ASSERT(false);
            return QString("toString(QProcess::ExitStatus exitStatus): unmanaged QProcess::ExitStatus: %1").arg(exitStatus);
        }
    }


    const QString toString(QProcess::ProcessState state) const
    {
        switch(state)
        {
        case QProcess::NotRunning:
            return "The process is not running.";

        case QProcess::Starting:
            return "The process is starting, but the program has not yet been invoked.";

        case QProcess::Running:
            return "The process is running and is ready for reading and writing.";

        default:
            Q_ASSERT(false);
            return QString("toString(QProcess::ProcessState state): unmanaged QProcess::ProcessState: %1").arg(state);
        }
    }

    QProcess webServerExtention;
    QString webServerExtentionFilePath;
    int errorCountPerSecond;
    QTime lastErrorTime;
    QLocalServer server;

public slots:
    void onServerNewConnection()
    {
        qDebug("onServerNewConnection");
    }

    void onProcessReadyReadStandardOutput ()
    {
        webServerExtention.setReadChannel(QProcess::StandardOutput);
        while(webServerExtention.canReadLine())
            qDebug("webServerExtention: %s", qPrintable(QString(webServerExtention.readLine().trimmed())));
    }
    void onProcessReadyReadStandardError ()
    {
        webServerExtention.setReadChannel(QProcess::StandardError);
        while(webServerExtention.canReadLine())
            qDebug("webServerExtention (err): %s", qPrintable(QString(webServerExtention.readLine().trimmed())));
    }

    void onProcessError ( QProcess::ProcessError error )
    {
        qDebug("ProcessManagerPrivate %s error: %s", WEB_SERVER_EXT_NAME, qPrintable(toString(error)));
        if (!lastErrorTime.isValid()) {
            lastErrorTime.start();
            errorCountPerSecond++; // first crash
        } else {
            if (lastErrorTime.elapsed() < 1000) // 3 crash per sec max
            {
                errorCountPerSecond++;
            } else  {
                lastErrorTime.restart();
                errorCountPerSecond = 0;
            }
        }
    }

    void onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus)
    {
        qDebug("ProcessManagerPrivate %s finished: %s (exit code %d)", WEB_SERVER_EXT_NAME, qPrintable(toString(exitStatus)), exitCode);
        if (exitStatus == QProcess::CrashExit)
            startProcess(false);
    }

    void onProcessStateChanged(QProcess::ProcessState newState)
    {
        qDebug("ProcessManagerPrivate %s process state changed: %s", WEB_SERVER_EXT_NAME, qPrintable(toString(newState)));
    }

private:
    Q_DISABLE_COPY(ProcessManagerPrivate);
};


#endif // PROCESS_MANAGER_P_H
