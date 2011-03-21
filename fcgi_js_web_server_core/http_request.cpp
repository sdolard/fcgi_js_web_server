#include "http_message.h"

#include "http_request.h"
#include "private/http_request_p.h"

// Internal initialisation of HttpRequest::requestMethodHash
QHash<QString, HttpRequest::RequestMethod> initRequestMethodHash()
{
    QHash<QString, HttpRequest::RequestMethod> hash;

    hash.insert("OPTIONS", HttpRequest::rmOptions);
    hash.insert("GET", HttpRequest::rmGet);
    hash.insert("HEAD", HttpRequest::rmHead);
    hash.insert("POST", HttpRequest::rmPost);
    hash.insert("PUT", HttpRequest::rmPut);
    hash.insert("DELETE", HttpRequest::rmDelete);
    hash.insert("TRACE", HttpRequest::rmTrace);
    hash.insert("CONNECT", HttpRequest::rmConnect);

    return hash;
}

HttpRequest::HttpRequest()// ctor
    :HttpMessage(*new HttpRequestPrivate())
{}

HttpRequest::HttpRequest(const HttpRequest & other) // copy ctor
    :HttpMessage(*new HttpRequestPrivate(*static_cast<const HttpRequestPrivate*>(other.pImpl)))
{}

HttpRequest & HttpRequest::operator=(const HttpRequest & other)
{
    if (this == &other)
        return *this;

    const HttpRequestPrivate * otherP = static_cast<const HttpRequestPrivate*>(other.pImpl);
    HttpRequestPrivate * p = static_cast<HttpRequestPrivate*>(pImpl);
    *p = *otherP;
    return *this;
}

HttpRequest::~HttpRequest()
{}

HttpRequest::HttpRequest(HttpRequestPrivate & dp)
    :HttpMessage(dp)
{}

bool HttpRequest::isHeaderComplete() const
{
    return static_cast<const HttpRequestPrivate*>(pImpl)->isHeaderComplete();
}

void HttpRequest::clear()
{
    static_cast<HttpRequestPrivate*>(pImpl)->clear();
}

HttpRequest::AppendError HttpRequest::appendMessageLine(const QByteArray & line)
{
    return static_cast<HttpRequestPrivate*>(pImpl)->appendMessageLine(line);
}

int HttpRequest::appendMessageBody(const QByteArray & message)
{
    return static_cast<HttpRequestPrivate*>(pImpl)->appendMessageBody(message);
}

bool HttpRequest::addMessage(const QString & name, const QString & value)
{
    return static_cast<HttpRequestPrivate*>(pImpl)->addMessage(name, value);
}

const QHash<QString, HttpRequest::RequestMethod> HttpRequest::requestMethodHash = initRequestMethodHash();


HttpRequest::RequestMethod HttpRequest::toMethod(const QString & s)
{
    return requestMethodHash.value(s, rmUnknown);
}

HttpRequest::RequestMethod  HttpRequest::method() const
{
    return static_cast<const HttpRequestPrivate*>(pImpl)->method;
}

QString  HttpRequest::URI() const
{
    return static_cast<const HttpRequestPrivate*>(pImpl)->URI;
}

int  HttpRequest::majorVersion() const
{
    return static_cast<const HttpRequestPrivate*>(pImpl)->majorVersion;
}

int  HttpRequest::minorVersion() const
{
    return static_cast<const HttpRequestPrivate*>(pImpl)->minorVersion;
}

QHash<QString, QString>  HttpRequest::requestHeaders () const
{
    return static_cast<const HttpRequestPrivate*>(pImpl)->requestHeaders;
}

HttpRequest::AppendError  HttpRequest::lastError() const
{
    return static_cast<const HttpRequestPrivate*>(pImpl)->lastError;
}
