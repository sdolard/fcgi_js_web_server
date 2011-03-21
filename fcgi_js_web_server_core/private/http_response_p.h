#ifndef HTTPREPONSE_P_H
#define HTTPREPONSE_P_H

#include "http_message_p.h"
#include "../http_response.h"

class HttpResponsePrivate : public HttpMessagePrivate
{
public:
    friend class HttpResponse;

    HttpResponsePrivate()
        :HttpMessagePrivate()
    {}

    static const QSet<QString> RESPONSE_HEADER_FIELD_NAMES;
    static const QSet<int> NO_MESSAGE_BODY_REPONSES;

    QString statusLine;
    HttpResponse::StatusCode statusCode;
    QHash<QString, QString> responseHeaders;

    // http://www.w3.org/Protocols/rfc2616/rfc2616-sec6.html#sec6.1
    // Status-Line = HTTP-Version SP Status-Code SP Reason-Phrase CRLF
    void setStatusCode(HttpResponse::StatusCode sc)
    {
        statusLine = QString("HTTP/1.1 %1 %2").arg(sc).arg(HttpResponse::toReasonPhrase(sc));
    }

    bool addMessage(const QString & name, const QString & value)
    {
        if (HttpMessagePrivate::addMessage(name, value))
            return true;

        // Response header fields: http://www.w3.org/Protocols/rfc2616/rfc2616-sec6.html#sec6.2
        // response-header = Accept-Ranges           ; Section 14.5
        //                    | Age                     ; Section 14.6
        //                    | ETag                    ; Section 14.19
        //                    | Location                ; Section 14.30
        //                    | Proxy-Authenticate      ; Section 14.33
        //                    | Retry-After             ; Section 14.37
        //                    | Server                  ; Section 14.38
        //                    | Vary                    ; Section 14.44
        //                    | WWW-Authenticate        ; Section 14.47
        if(RESPONSE_HEADER_FIELD_NAMES.contains(name))
        {
            // TODO: test value content
            // qDebug("responseHeader Field-Name: '%s', value: '%s'", qPrintable(name), qPrintable(value));
            responseHeaders.insert(name, value);
            return true;
        }
        Q_ASSERT_X(false, "HttpResponsePrivate:addMessage", "Invalid name");
        return false;
    }

    bool mustContainsAMessageBody() const
    {
        return !NO_MESSAGE_BODY_REPONSES.contains(statusCode);
    }
};

#endif // HTTPREPONSE_P_H
