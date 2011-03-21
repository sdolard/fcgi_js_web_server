#ifndef HTTPMESSAGE_P_H
#define HTTPMESSAGE_P_H

#include <QHash>
#include <QSet>
#include <QString>
#include <QByteArray>

class HttpMessagePrivate
{
public:
    HttpMessagePrivate()
    {}

    QHash<QString, QString> generalHeaders;
    QHash<QString, QString> entityHeaders;

    static const QSet<QString> GENERAL_HEADER_FIELD_NAMES;
    static const QSet<QString> ENTITY_HEADER_FIELD_NAMES;

protected:
    friend class HttpMessage; // in order to acces to protected parts

    QByteArray messageBody;

    virtual void clear()
    {
        generalHeaders.clear();
        entityHeaders.clear();
        messageBody.clear();
    }

    virtual bool addMessage(const QString & name, const QString & value)
    {
        // General Header fields: http://www.w3.org/Protocols/rfc2616/rfc2616-sec4.html#sec4.5
        // general-header = Cache-Control            ; Section 14.9
        //                | Connection               ; Section 14.10
        //                | Date                     ; Section 14.18
        //                | Pragma                   ; Section 14.32
        //                | Trailer                  ; Section 14.40
        //                | Transfer-Encoding        ; Section 14.41
        //                | Upgrade                  ; Section 14.42
        //                | Via                      ; Section 14.45
        //                | Warning                  ; Section 14.46

        if(GENERAL_HEADER_FIELD_NAMES.contains(name))
        {
            // TODO: test value content function of name
            // qDebug("generalHeaders Field-Name: '%s', value: '%s'", qPrintable(name), qPrintable(value));
            generalHeaders.insert(name, value);
            return true;
        }

        // Entity Header fields: http://www.w3.org/Protocols/rfc2616/rfc2616-sec7.html#sec7.1
        // entity-header  = Allow                    ; Section 14.7
        //                | Content-Encoding         ; Section 14.11
        //                | Content-Language         ; Section 14.12
        //                | Content-Length           ; Section 14.13
        //                | Content-Location         ; Section 14.14
        //                | Content-MD5              ; Section 14.15
        //                | Content-Range            ; Section 14.16
        //                | Content-Type             ; Section 14.17
        //                | Expires                  ; Section 14.21
        //                | Last-Modified            ; Section 14.29
        //                | extension-header
        // extension-header = message-header

        if(ENTITY_HEADER_FIELD_NAMES.contains(name))
        {
            // TODO: test value content
            // qDebug("entityHeaders Field-Name: '%s', value: '%s'", qPrintable(name), qPrintable(value));
            entityHeaders.insert(name, value);
            return true;
        }
        return false;
    }
};

#endif // HTTPMESSAGE_P_H
