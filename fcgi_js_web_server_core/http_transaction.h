#ifndef HTTPTRANSACTION_H
#define HTTPTRANSACTION_H

#include <QByteArray>
#include <QSharedPointer>
#include <QQueue>

#include "http_response.h"
#include "http_request.h"

// Class transaction
// Contains HttpRequest, HttpResponse pair
// Manage those function of transaction states
class HttpTransactionPrivate;
class HttpTransaction
{
public:
    // Transaction states
    enum State {
        tsReadHeader = 0x00,       // Reading header
        tsReadMessageBody = 0x01,  // Reading message body
        tsWriteHeader = 0x02,      // Writing header
        tsWriteBody = 0x03         // Writing message body
    };

    HttpTransaction(); // ctor
    HttpTransaction(const HttpTransaction &); // copy ctor
    virtual ~HttpTransaction(); // dtor
    // We are using a pointer, so we need to define a copy ctor
    // in order to not copy pImpl pointer but data
    HttpTransaction & operator=(const HttpTransaction &);

    // Transaction state getter
    State state() const;
    // Transaction state setter
    void setState(State state);

    // Returns HttpResponse
    HttpResponse & response();

    // Returns HttpRequest
    HttpRequest & request();

protected:
    HttpTransactionPrivate * const pImpl;
    HttpTransaction(HttpTransactionPrivate & dp);
};

#endif // HTTPTRANSACTION_H
