#ifndef HTTPRESPONSE_H
#define HTTPRESPONSE_H

#include "http_message.h"

// Response: http://www.w3.org/Protocols/rfc2616/rfc2616-sec6.html#sec6
class HttpResponsePrivate;
class HttpResponse: public HttpMessage
{
public:
    // Response status code  http://www.w3.org/Protocols/rfc2616/rfc2616-sec6.html#sec6.1.1
    enum StatusCode {
        scContinue = 100,
        scSwitchingProtocols = 101,
        scOk = 200,
        scCreated = 201,
        scAccepted = 202,
        scNonAuthoritativeInformation = 203,
        scNoContent = 204,
        scResetContent = 205,
        scPartielContent = 206,
        scMultipleChoices = 300,
        scMovedPermanently = 301,
        scFound = 302,
        scSeeOther = 303,
        scNotModified = 304,
        scUseProxy = 305,
        scTemporaryRedirect = 307,
        scBadRequest = 400,
        scUnauthorized = 401, // Constant denoting HTTP Unauthorized status. The request requires user authentication.
        scPaymentRequired = 402,
        scForbidden = 403,
        scNotFound = 404,
        scMethodNotAllowed = 405,
        scNotAcceptable = 406,
        scProxyAuthenticationRequired = 407,
        scRequestTimeOut = 408,
        scConflict = 409,
        scGone = 410,
        scLengthRequired = 411,
        scPreconditionFailed = 412,
        scRequestEntityTooLarge = 413,
        scRequestURITooLarge = 414,
        scUnsupportedMediaType = 415,
        scRequestRangeNotStatisfiable = 416,
        scExpectationFailed = 417,
        scInternalServerError = 500,
        scNotImplemented = 501,
        scBadGateway = 502,
        scServiceUnavailable = 503,
        scGatewayTimeOut = 504,
        scHttpVersionNotSupported = 505
    };

    HttpResponse();// ctor
    HttpResponse(const HttpResponse &); // copy ctor
    HttpResponse & operator=(const HttpResponse &);

    // Clear response
    virtual void clear();

    // Add a new field/value to response/
    // Returns false if field name is not valid
    bool writeMessageHeader(const QString & name, const QString & value);

    // Returns formated status line
    // Returns an empty QString setStatusCode has not been set
    QString & statusLine() const;

    // Status code setter
    void setStatusCode(HttpResponse::StatusCode sc);
    // Status code getter
    StatusCode statusCode() const;

    // Returns reason phrase status code (not localized)
    static const QString toReasonPhrase(HttpResponse::StatusCode sc);

    // Returns true if response has to contains a message body (function of status code)
    bool mustContainsAMessageBody() const;

    const QByteArray & writeBuffer();

protected:
    HttpResponse(HttpResponsePrivate & dp);

    // Add message name and value in typed container
    // Return false if name do not correspond to:
    //   - General Header fields
    //   - Entity Header fields
    //   - Response Header fields
    bool addMessage(const QString & name, const QString & value);
};

#endif // HTTPRESPONSE_H
