#include "http_request_p.h"

const QSet<QString> HttpRequestPrivate::REQUEST_HEADER_FIELD_NAMES = QSet<QString>() << "Accept" << "Accept-Charset" << "Accept-Encoding"
                                                             << "Accept-Language" << "Authorization" << "Expect" << "From" <<
                                                             "Host" << "If-Match" << "If-Modified-Since" << "If-None-Match" <<
                                                             "If-Range" << "If-Unmodified-Since" << "Max-Forwards" <<
                                                             "Proxy-Authorization" << "Range" << "Referer" << "TE" << "User-Agent";

