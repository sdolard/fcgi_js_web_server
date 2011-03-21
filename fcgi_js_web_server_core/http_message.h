#ifndef HTTPMESSAGE_H
#define HTTPMESSAGE_H

#include <QHash>
#include <QString>

// http://www.w3.org/Protocols/rfc2616/rfc2616.html
// Base class for http request and http response
class HttpMessagePrivate;
class HttpMessage
{
public:
    HttpMessage(); // ctor
    virtual ~HttpMessage(); // dtor
    HttpMessage(const HttpMessage &); // copy ctor
    HttpMessage & operator=(const HttpMessage &); //

    // Clear message (all internal data, generalHeaders, entityHeaders, messageBody)
    virtual void clear();

    // General Header fields: http://www.w3.org/Protocols/rfc2616/rfc2616-sec4.html#sec4.5
    const QHash<QString, QString> & getGeneralHeaders() const;

    // Entity Header fields: http://www.w3.org/Protocols/rfc2616/rfc2616-sec7.html#sec7.1
    const QHash<QString, QString> & getEntityHeaders() const;

    // http://www.w3.org/Protocols/rfc2616/rfc2616-sec4.html#sec4.3
    // The message-body (if any) of an HTTP message is used to carry the entity-body associated with the request or response.
    const QByteArray & messageBody() const;

protected:
    HttpMessagePrivate * const pImpl;
    HttpMessage(HttpMessagePrivate &dp); //inheritance

    // Add message name and value in typed container
    // Return false if name do not correspond to:
    //   - General Header fields
    //   - Entity Header fields
    // TODO: Limit name and value lenght?
    virtual bool addMessage(const QString & name, const QString & value);
};

#endif // HTTPMESSAGE_H
