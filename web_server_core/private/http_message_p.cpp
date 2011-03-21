#include "http_message_p.h"

const QSet<QString> HttpMessagePrivate::GENERAL_HEADER_FIELD_NAMES = QSet<QString>() <<
                                                              "Cache-Control" << "Connection" << "Date" << "Pragma" << "Trailer" <<
                                                              "Transfer-Encoding" << "Upgrade" << "Via" << "Warning";

const QSet<QString> HttpMessagePrivate::ENTITY_HEADER_FIELD_NAMES = QSet<QString>() <<
                                                             "Allow" << "Content-Encoding" << "Content-Language" <<  "Content-Length" <<
                                                             "Content-Location" << "Content-MD5" << "Content-Range" << "Content-Type" <<
                                                             "Expires" << "Last-Modified";
