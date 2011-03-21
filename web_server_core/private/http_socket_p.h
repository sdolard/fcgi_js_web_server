#ifndef HTTPSOCKET_P_H
#define HTTPSOCKET_P_H

#include <QtGlobal>

#include "../http_transaction_list.h"

#include "abstract_socket_p.h"

class HttpSocketPrivate: public AbstractSocketPrivate
{
public:
    HttpTransactionList transactionList;
    QByteArray line;

    HttpSocketPrivate(HttpSocket * parent)
        :AbstractSocketPrivate(parent)
    {}

    void readMessageHeader()
    {
        const QSharedPointer<HttpTransaction> & transaction = transactionList.getReadingTransaction();
        HttpRequest & request = transaction->request();

        Q_Q(HttpSocket);

        while(q->canReadLine()) {
            line = q->readLine();// TODO: limit line lenght?

            log(line); //log

            switch(request.appendMessageLine(line))
            {
            case HttpRequest::aeInvalidRequestLine:
            case HttpRequest::aeInvalidMessage:
                transaction->response().setStatusCode(HttpResponse::scBadRequest);
                transactionList.swapReadingTransactionToWritingState();
                prepareResponse();
                return;

            case HttpRequest::aeHeaderComplete:
                if (request.getEntityHeaders().contains("Content-Length"))
                {
                    // We read message body
                    q->setReadMode(AbstractSocket::rmStream);
                    if (q->bytesAvailable()) {
                        if (readMessageBody()) {
                            prepareResponse();
                            return;
                        }
                    }
                    break;
                }
                // Or we start to send response
                transactionList.swapReadingTransactionToWritingState();
                prepareResponse();
                return;

            case HttpRequest::aeHeaderIncomplete:
                continue;

            default:
                Q_ASSERT(false);
                transaction->response().setStatusCode(HttpResponse::scNotImplemented);
                transactionList.swapReadingTransactionToWritingState();
                prepareResponse();
                return;
            }

            break; // We've got an error, or read mode change (header > body) > we break.
        }
    }

    // Returns true if body is completely read
    bool readMessageBody()
    {
        const QSharedPointer<HttpTransaction> & transaction = transactionList.getReadingTransaction();
        HttpRequest & request = transaction->request();
        int contentLength = request.getEntityHeaders().value("Content-Length").toInt();
        Q_Q(HttpSocket);
        // TODO: check length to read
        while(q->bytesAvailable()) {
            request.appendMessageBody(q->readAll());
        }
        return request.messageBody().size() == contentLength;
    }

    void bytesWritten ( qint64 bytes )
    {
        // This signal is emitted every time a payload of data has been written to the device.
        // The bytes argument is set to the number of bytes that were written in this payload.
        log(QString("bytesWritten: %1").arg(bytes));
    }

    /*void writeHeader()
    {
        const QSharedPointer<HttpTransaction> & transaction = transactionList.getWritingTransaction();
        HttpRequest & request = transaction->request();
        HttpResponse & response = transaction->response();

        if (request.URI() == "/") {
            response.setStatusCode(HttpResponse::scOk);
        }

        // QByteArray data = response.statusLine().toUtf8();

        response.writeMessageHeader("Server", "web_server");

        Q_Q(HttpSocket);
        // Q_ASSERT(q->write(data) == data.length());


//         1.Any response message which "MUST NOT" include a message-body
//        (such as the 1xx, 204, and 304 responses and any response to a HEAD request)
//        is always terminated by the first empty line after the header fields,
//        regardless of the entity-header fields present in the message.


        //if (NO_MESSAGE_BODY_REPONSES.contains(transaction->response().statusCode())) {
        if (!transaction->response().mustContainsAMessageBody()) {
            q->write("\r\n");
            transactionList.dequeueWritingTransaction();
            return;
        }
    }*/

    void writeMessageBody(const QSharedPointer<HttpTransaction> &)
    {
    }


    void prepareResponse() {

    }

    virtual void log(const QString & l) const
    {
        qDebug("(HttpSocketPrivate %p) %s", this, qPrintable(l));
    }

    Q_DECLARE_PUBLIC(HttpSocket);
};

#endif // HTTPSOCKET_P_H
