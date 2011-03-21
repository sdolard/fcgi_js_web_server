#ifndef HTTPTRANSACTION_P_H
#define HTTPTRANSACTION_P_H

#include "../http_transaction.h"

#include "../http_request.h"
#include "../http_response.h"

class HttpTransactionPrivate
{
public:
    HttpTransactionPrivate()
    {
        state = HttpTransaction::tsReadHeader;
    }

    virtual ~HttpTransactionPrivate() // dtor
    {
        qDebug("HttpTransaction deleted");
    }

    HttpTransaction::State state;
    HttpResponse response;
    HttpRequest request;
};

#endif // HTTPTRANSACTION_P_H
