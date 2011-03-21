#ifndef HTTPREQUESTPRIVATE_H
#define HTTPREQUESTPRIVATE_H

#include <QStringList>

#include "http_request.h"
#include "http_message_p.h"

class HttpRequestPrivate : public HttpMessagePrivate
{
public:
    HttpRequest::RequestMethod method;
    QString URI;
    int majorVersion;
    int minorVersion;
    QHash<QString, QString> requestHeaders;
    HttpRequest::AppendError lastError;

    static const QSet<QString> REQUEST_HEADER_FIELD_NAMES;

    HttpRequestPrivate() // ctor
        :HttpMessagePrivate()
    {
        majorVersion = -1;
        minorVersion = -1;
        lastError = HttpRequest::aeRequestLineNotRead;
    }

    HttpRequest::AppendError setLastError(HttpRequest::AppendError ae)
    {
        lastError = ae;
        return lastError;
    }

    bool isHeaderComplete() const
    {
        return lastError == HttpRequest::aeHeaderComplete;
    }

    HttpRequest::AppendError readMessageLine(const QString & line)
    {
        Q_ASSERT(lastError != HttpRequest::aeRequestLineNotRead);
        if (lastError == HttpRequest::aeRequestLineNotRead) {
            return lastError;
        }
        // http://www.w3.org/Protocols/rfc2616/rfc2616-sec4.html#sec4.2
        // message-header = field-name ":" [ field-value ]
        //     field-name     = token
        //     field-value    = *( field-content | LWS )
        //     field-content  = <the OCTETs making up the field-value
        //                      and consisting of either *TEXT or combinations
        //                      of token, separators, and quoted-string>

        int pos = line.indexOf(":", 1);
        if (pos == -1)
            return setLastError(HttpRequest::aeInvalidMessage);

        QString name = line.left(pos);
        QString value = line.mid(pos + 1).trimmed();
        //qDebug("HttpMessage::name: '%s', value: '%s'", qPrintable(name), qPrintable(value));

        if(name.isEmpty())
            return setLastError(HttpRequest::aeInvalidMessage);

        // Is name in kwnown field token?
        if(!addMessage(name, value))
            return setLastError(HttpRequest::aeInvalidMessage);

        return setLastError(HttpRequest::aeHeaderIncomplete);
    }

    // Request line: http://www.w3.org/Protocols/rfc2616/rfc2616-sec5.html#sec5.1
    // Returns HttpRequest::aeHeaderIncomplete if request line is valid
    // Request-Line = Method SP Request-URI SP HTTP-Version CRLF
    virtual HttpRequest::AppendError readRequestLine(const QString & line)
    {
        Q_ASSERT(lastError == HttpRequest::aeRequestLineNotRead);
        if (lastError != HttpRequest::aeRequestLineNotRead)
            return setLastError(HttpRequest::aeInvalidRequestLine);

        const QStringList l = line.split(" ");
        if (l.count() != 3)
            return setLastError(HttpRequest::aeInvalidRequestLine);
        for (int i = 0; i < l.count(); i++)
        {
            switch(i)
            {
            case 0:// Method
                method = HttpRequest::toMethod(l.at(i));
                if(method == HttpRequest::rmUnknown)
                    return setLastError(HttpRequest::aeInvalidRequestLine);
                continue;

            case 1:// Request-URI    = "*" | absoluteURI | abs_path | authority
                URI = l.at(i);
                if (URI.isEmpty())
                    return setLastError(HttpRequest::aeInvalidRequestLine);
                continue;

            case 2:// HTTP-Version   = "HTTP" "/" 1*DIGIT "." 1*DIGIT
            {
                majorVersion = -1;
                minorVersion = -1;
                QString version = l.at(i);
                if (version.indexOf("HTTP/", 0) != 0)
                    return setLastError(HttpRequest::aeInvalidRequestLine);

                version.remove("HTTP/");
                QStringList l = version.split(".");
                if (l.count() != 2)
                    return setLastError(HttpRequest::aeInvalidRequestLine);

                bool success;
                majorVersion = l.at(0).toInt(&success);
                if (!success || majorVersion > 9) {
                    majorVersion = -1;
                    return setLastError(HttpRequest::aeInvalidRequestLine);
                }
                minorVersion = l.at(1).toInt(&success);
                if (!success || minorVersion > 9)
                {
                    minorVersion = -1;
                    return setLastError(HttpRequest::aeInvalidRequestLine);
                }
            }
            }
        }
        // qDebug("method: %d", method);
        // qDebug("URI: %s", qPrintable(URI));
        // qDebug("version: %s", qPrintable(version));
        return setLastError(HttpRequest::aeHeaderIncomplete);
    }

    // Returns HttpRequest::aeHeaderComplete if header is complete
    HttpRequest::AppendError appendMessageLine(const QByteArray & line)
    {
        if (line == "\r\n")  // End of header
            return setLastError(HttpRequest::aeHeaderComplete);

        if (lastError == HttpRequest::aeRequestLineNotRead)
            return readRequestLine(line);

        return readMessageLine(line);
    }

    int appendMessageBody(const QByteArray & data)
    {
        messageBody += data;
        return messageBody.length();
    }

protected:
    friend class HttpRequest;

    virtual void clear()
    {
        HttpMessagePrivate::clear();

        method = HttpRequest::rmUnknown;
        URI.clear();
        requestHeaders.clear();
        majorVersion = 0;
        minorVersion = 0;
        lastError = HttpRequest::aeRequestLineNotRead;
    }

    virtual bool addMessage(const QString & name, const QString & value)
    {
        if (HttpMessagePrivate::addMessage(name, value))
            return true;

        // Request header fields: http://www.w3.org/Protocols/rfc2616/rfc2616-sec5.html#sec5.3
        //   request-header = Accept                   ; Section 14.1
        //                  | Accept-Charset           ; Section 14.2
        //                  | Accept-Encoding          ; Section 14.3
        //                  | Accept-Language          ; Section 14.4
        //                  | Authorization            ; Section 14.8
        //                  | Expect                   ; Section 14.20
        //                  | From                     ; Section 14.22
        //                  | Host                     ; Section 14.23
        //                  | If-Match                 ; Section 14.24
        //                  | If-Modified-Since        ; Section 14.25
        //                  | If-None-Match            ; Section 14.26
        //                  | If-Range                 ; Section 14.27
        //                  | If-Unmodified-Since      ; Section 14.28
        //                  | Max-Forwards             ; Section 14.31
        //                  | Proxy-Authorization      ; Section 14.34
        //                  | Range                    ; Section 14.35
        //                  | Referer                  ; Section 14.36
        //                  | TE                       ; Section 14.39
        //                  | User-Agent               ; Section 14.43
        if(REQUEST_HEADER_FIELD_NAMES.contains(name))
        {
            // TODO: test value content
            // qDebug("requestHeader Field-Name: '%s', value: '%s'", qPrintable(name), qPrintable(value));
            requestHeaders.insert(name, value);
            return true;
        }
        return false;
    }
};

#endif // HTTPREQUESTPRIVATE_H
