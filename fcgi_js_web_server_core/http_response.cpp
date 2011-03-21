#include "http_response.h"

#include "private/http_response_p.h"

HttpResponse::HttpResponse()// ctor
    :HttpMessage(*new HttpResponsePrivate())
{}

HttpResponse::HttpResponse(const HttpResponse & other) // copy ctor
    :HttpMessage(*new HttpResponsePrivate(*static_cast<HttpResponsePrivate*>(other.pImpl)))
{}

HttpResponse & HttpResponse::operator=(const HttpResponse & other)
                                      {
    if (this == &other)
        return *this;

    const HttpResponsePrivate * o = static_cast<const HttpResponsePrivate*>(other.pImpl);
    HttpResponsePrivate * p = static_cast<HttpResponsePrivate*>(pImpl);
    *p = *o;
    return *this;
}

HttpResponse::HttpResponse(HttpResponsePrivate & dp)
    :HttpMessage(dp)
{}

void HttpResponse::clear()
{
    static_cast<HttpResponsePrivate*>(pImpl)->clear();
}

bool HttpResponse::writeMessageHeader(const QString & name, const QString & value)
{
    return addMessage(name, value);
}

bool HttpResponse::addMessage(const QString & name, const QString & value)
{
    return static_cast<HttpResponsePrivate*>(pImpl)->addMessage(name, value);
}

QString & HttpResponse::statusLine() const
{
    return static_cast<const HttpResponsePrivate*>(pImpl)->statusLine;
}

HttpResponse::StatusCode HttpResponse::statusCode() const
{
   return static_cast<const HttpResponsePrivate*>(pImpl)->statusCode;
}

void HttpResponse::setStatusCode(HttpResponse::StatusCode sc)
{
    static_cast<HttpResponsePrivate*>(pImpl)->setStatusCode(sc);
}

const QString HttpResponse::toReasonPhrase(HttpResponse::StatusCode sc)
{
    switch(sc)
    {
    case scContinue: return "Continue";
    case scSwitchingProtocols: return "Switching Protocols";
    case scOk: return "OK";
    case scCreated: return "Created";
    case scAccepted: return "Accepted";
    case scNonAuthoritativeInformation: return "Non-Authoritative Information";
    case scNoContent: return "No Content";
    case scResetContent: return "Reset Content";
    case scPartielContent: return "Partial Content";
    case scMultipleChoices: return "Multiple Choices";
    case scMovedPermanently: return "Moved Permanently";
    case scFound: return "Found";
    case scSeeOther: return "See Other";
    case scNotModified: return "Not Modified";
    case scUseProxy: return "Use Proxy";
    case scTemporaryRedirect: return "Temporary Redirect";
    case scBadRequest: return "Bad Request";
    case scUnauthorized: return "Unauthorized";
    case scPaymentRequired: return "Payment Required";
    case scForbidden: return "Forbidden";
    case scNotFound: return "Not Found";
    case scMethodNotAllowed: return "Method Not Allowed";
    case scNotAcceptable: return "Not Acceptable";
    case scProxyAuthenticationRequired: return "Proxy Authentication Required";
    case scRequestTimeOut: return "Request Time-out";
    case scConflict: return "Conflict";
    case scGone: return "Gone";
    case scLengthRequired: return "Length Required";
    case scPreconditionFailed: return "Precondition Failed";
    case scRequestEntityTooLarge: return "Request Entity Too Large";
    case scRequestURITooLarge: return "Request-URI Too Large";
    case scUnsupportedMediaType: return "Unsupported Media Type";
    case scRequestRangeNotStatisfiable: return "Requested range not satisfiable";
    case scExpectationFailed: return "Expectation Failed";
    case scInternalServerError: return "Internal Server Error";
    case scNotImplemented: return "Not Implemented";
    case scBadGateway: return "Bad Gateway";
    case scServiceUnavailable: return "Service Unavailable";
    case scGatewayTimeOut: return "Gateway Time-out";
    case scHttpVersionNotSupported: return "HTTP Version not supported";
    default:
        Q_ASSERT(false);
        return "Not defined http status code";
    }
}

bool HttpResponse::mustContainsAMessageBody() const
{
    return static_cast<const HttpResponsePrivate*>(pImpl)->mustContainsAMessageBody();
}
