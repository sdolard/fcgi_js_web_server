#include "http_response_p.h"

const QSet<QString> HttpResponsePrivate::RESPONSE_HEADER_FIELD_NAMES = QSet<QString>() << "Accept-Ranges" << "Age"
                                                               << "ETag" << "Location" << "Proxy-Authenticate" <<
                                                               "Retry-After" << "Server" << "Vary" << "WWW-Authenticate";


const QSet<int> HttpResponsePrivate::NO_MESSAGE_BODY_REPONSES = QSet<int>()  <<  HttpResponse::scContinue <<
                                                       HttpResponse::scSwitchingProtocols <<
                                                       HttpResponse::scNoContent << HttpResponse::scNotModified;


