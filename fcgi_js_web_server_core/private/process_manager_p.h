#ifndef PROCESS_MANAGER_P_H
#define PROCESS_MANAGER_P_H

#include <QProcess>
#include <QCoreApplication>
#include <QTime>
#include <QLocalServer>
#include <QLocalSocket>

#include "../process_manager.h"

#ifndef Q_OS_WIN32 > linux, mac...
#   ifdef DEBUG
#      define WEB_SERVER_EXT_NAME "../fcgi_js_web_server_extension/fcgi_js_web_server_extensiond"
#   else
#      define WEB_SERVER_EXT_NAME "../fcgi_js_web_server_extension/fcgi_js_web_server_extension"
#   endif
#else //Q_OS_WIN32 > mingw
#   ifdef DEBUG
#      define WEB_SERVER_EXT_NAME "../../fcgi_js_web_server_extension/debug/fcgi_js_web_server_extensiond"
#   else
#      define WEB_SERVER_EXT_NAME "../../fcgi_js_web_server_extension/release/fcgi_js_web_server_extension"
#   endif
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

        // extension
        webServerExtension.setWorkingDirectory(QCoreApplication::applicationDirPath());
        webServerExtensionFilePath = QString("%1/%2").arg(QCoreApplication::applicationDirPath()).arg(WEB_SERVER_EXT_NAME);

        connect(&webServerExtension, SIGNAL(readyReadStandardOutput()),
                this, SLOT(onProcessReadyReadStandardOutput()));
        connect(&webServerExtension, SIGNAL(readyReadStandardError()),
                this, SLOT(onProcessReadyReadStandardError()));
        connect(&webServerExtension, SIGNAL(error(QProcess::ProcessError)),
                this, SLOT(onProcessError(QProcess::ProcessError)));
        connect(&webServerExtension, SIGNAL(finished(int, QProcess::ExitStatus)),
                this, SLOT(onProcessFinished(int, QProcess::ExitStatus)));
        connect(&webServerExtension, SIGNAL(stateChanged(QProcess::ProcessState)),
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
        if (QLocalServer::removeServer("fcgi_js_web_server_core"))
            qDebug("Previous LocalServer instance cleaned up");

        if (server.listen("fcgi_js_web_server_core"))
        {
            qDebug("LocalServer listening...");
            return true;
        }
        qDebug("LocalServer error: %s", qPrintable(server.errorString()));
        return false;
    }

    bool startProcess(bool blocking = true)
    {
#ifndef DEV
        if (errorCountPerSecond > 3)
        {
            qDebug("At least 3 errors occured on %s in 1 sec. Process will not be auto-restarted", WEB_SERVER_EXT_NAME);
            return false;
        }

        webServerExtension.start(webServerExtensionFilePath, QIODevice::ReadOnly);
        if (blocking)
            return webServerExtension.waitForStarted();
        return true;
#else
        // In DEV mode, extension is not run by core
        blocking = blocking; // no warning
        return true;
#endif
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
        switch(webServerExtension.state())
        {
        case QProcess::Starting:
        case QProcess::Running:
            webServerExtension.terminate();
            if(webServerExtension.waitForFinished(5000)) // 5s
            {
                qDebug("%s terminated", WEB_SERVER_EXT_NAME);
                return;
            }
            webServerExtension.close();
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

    QProcess webServerExtension;
    QString webServerExtensionFilePath;
    int errorCountPerSecond;
    QTime lastErrorTime;
    QLocalServer server;

public slots:
    void onServerNewConnection()
    {
        qDebug("onServerNewConnection");
        QScopedPointer<QLocalSocket> localSocket(server.nextPendingConnection());
        localSocket->write("toto");
        localSocket->flush();
    }

    void onProcessReadyReadStandardOutput ()
    {
        webServerExtension.setReadChannel(QProcess::StandardOutput);
        while(webServerExtension.canReadLine())
            qDebug("webServerExtension: %s", qPrintable(QString(webServerExtension.readLine().trimmed())));
    }
    void onProcessReadyReadStandardError ()
    {
        webServerExtension.setReadChannel(QProcess::StandardError);
        while(webServerExtension.canReadLine())
            qDebug("webServerExtension (err): %s", qPrintable(QString(webServerExtension.readLine().trimmed())));
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
