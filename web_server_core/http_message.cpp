#include "http_message.h"

#include "private/http_message_p.h"

HttpMessage::HttpMessage()
    :pImpl(new HttpMessagePrivate())// ctor
{}

HttpMessage::HttpMessage(const HttpMessage & other) // copy ctor
    :pImpl(new HttpMessagePrivate(*other.pImpl))
{}

HttpMessage::~HttpMessage() // dtor
{
    delete pImpl;
}

HttpMessage & HttpMessage::operator=(const HttpMessage & other)
                                    {
    if (this == &other)
        return *this;
    *pImpl = *other.pImpl;
    return *this;
}

HttpMessage::HttpMessage(HttpMessagePrivate & dp) // inheritance
    :pImpl(&dp)
{}

void HttpMessage::clear()
{
    pImpl->clear();
}

const QHash<QString, QString> & HttpMessage::getGeneralHeaders() const
{
    return pImpl->generalHeaders;
}

const QHash<QString, QString> & HttpMessage::getEntityHeaders() const
{
    return pImpl->entityHeaders;
}

bool HttpMessage::addMessage(const QString & name, const QString & value)
{
   return pImpl->addMessage(name, value);
}

const QByteArray & HttpMessage::messageBody() const
{
    return pImpl->messageBody;
}
