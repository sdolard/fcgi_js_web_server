#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include "http_message.h"

// Request: http://www.w3.org/Protocols/rfc2616/rfc2616-sec5.html#sec5
// Request = Request-Line              ; Section 5.1
//           *(( general-header        ; Section 4.5
//            | request-header         ; Section 5.3
//            | entity-header ) CRLF)  ; Section 7.1
//           CRLF
//           [ message-body ]          ; Section 4.3

class HttpRequestPrivate;
class HttpRequest: public HttpMessage
{
public:
    // Request methods: http://www.w3.org/Protocols/rfc2616/rfc2616-sec5.html#sec5.1.1
    // Method         = "OPTIONS"                ; Section 9.2
    //                | "GET"                    ; Section 9.3
    //                | "HEAD"                   ; Section 9.4
    //                | "POST"                   ; Section 9.5
    //                | "PUT"                    ; Section 9.6
    //                | "DELETE"                 ; Section 9.7
    //                | "TRACE"                  ; Section 9.8
    //                | "CONNECT"                ; Section 9.9
    //                | extension-method
    // extension-method = token
    enum RequestMethod {
        rmUnknown,
        rmOptions,
        rmGet,
        rmHead,
        rmPost,
        rmPut,
        rmDelete,
        rmTrace,
        rmConnect
    };

    // Different error type when adding line form socket
    // See #appendMessageLine
    enum AppendError {
        aeRequestLineNotRead, // when first line do not correspond to a request line
        aeInvalidRequestLine, // when request line is not well formated
        aeInvalidMessage,     // when message is not valid (invalid token or value)
        aeHeaderIncomplete,   // line is valid, but header is still not complete
        aeHeaderComplete      // header is complete, next data must be message body
    };

    HttpRequest(); // ctor
    HttpRequest(const HttpRequest &); // copy ctor
    HttpRequest & operator=(const HttpRequest &);
    virtual ~HttpRequest(); // dtor

    // Returns true if header is complete and valid
    bool isHeaderComplete() const;

    // Clear request
    virtual void clear();

    // Add a new line, and parse it
    // See #AppendError
    AppendError appendMessageLine(const QByteArray & line);

    // Append message data and returns new size of message body
    int appendMessageBody(const QByteArray & message);

    // Returns request method
    // Request must me complete. See isHeaderComplete()
    HttpRequest::RequestMethod method() const;

    // Returns request URI
    // Request must me complete. See isHeaderComplete()
    QString URI() const;

    // Returns HTTP major version
    // aeRequestLineNotRead state must has been passed
    int majorVersion() const;

    // Returns HTTP minor version
    // aeRequestLineNotRead state must has been passed
    int minorVersion() const;

    // Returns request headers
    QHash<QString, QString> requestHeaders () const;

    // Returns last append error
    HttpRequest::AppendError lastError() const;

    // Convert s to a RequestMethod
    // Returns rmUnknown if method do not match
    static RequestMethod toMethod(const QString & s);

    // Static request methods QHash
    // See #toMethod
    static const QHash<QString, RequestMethod> requestMethodHash;
protected:
    HttpRequest(HttpRequestPrivate & dp);

    // Add message name and value in typed container
    // Return false if name do not correspond to:
    //   - General Header fields
    //   - Entity Header fields
    //   - Request Header fields
    // TODO: Limit name and value lenght?
    virtual bool addMessage(const QString & name, const QString & value);
};

#endif // HTTPREQUEST_H
