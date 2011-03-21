#include "http_transaction.h"

#include "private/http_transaction_p.h"

HttpTransaction::HttpTransaction() // ctor
    :pImpl(new HttpTransactionPrivate())
{}

HttpTransaction::HttpTransaction(const HttpTransaction &other) // copy ctor
    :pImpl(new HttpTransactionPrivate(*other.pImpl))
{}

HttpTransaction::~HttpTransaction() // dtor
{
    delete pImpl;
}

// We are using a pointer, so we need to define a copy ctor
// in order to not copy pImpl pointer but data
HttpTransaction &  HttpTransaction::operator=(const HttpTransaction & other)
{
    if (this == &other)
        return *this;

    *pImpl = *other.pImpl;
    return *this;
}

HttpTransaction::HttpTransaction(HttpTransactionPrivate & dp)
    :pImpl(&dp)
{}

HttpResponse & HttpTransaction::response()
{
    return pImpl->response;
}

HttpRequest & HttpTransaction::request()
{
    return pImpl->request;
}

HttpTransaction::State HttpTransaction::state() const
{
    return pImpl->state;
}

void HttpTransaction::setState(HttpTransaction::State state)
{
    pImpl->state = state;
}
