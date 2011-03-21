#-------------------------------------------------
#
# Project created by QtCreator 2010-12-25T11:05:51
#
#-------------------------------------------------


TEMPLATE = app

QT += core network
QT -= gui

CONFIG += console
CONFIG -= app_bundle

CONFIG(debug, debug|release) {
    TARGET = fcgi_js_web_server_cored
    DEFINES += DEBUG
} else {
    TARGET = fcgi_js_web_server_core
}

message($$TARGET will be installed in $$DESTDIR)


SOURCES += main.cpp \
    listener.cpp \
    abstract_socket.cpp \
    http_socket.cpp \
    http_message.cpp \
    http_request.cpp \
    http_response.cpp \
    http_transaction.cpp \
    http_transaction_list.cpp \
    private/http_message_p.cpp \
    private/http_request_p.cpp \
    private/http_response_p.cpp \
    process_manager.cpp

HEADERS += \
    listener.h \
    abstract_socket.h \
    http_socket.h \
    http_message.h \
    http_request.h \
    http_response.h \
    http_transaction.h \
    http_transaction_list.h \
    private/listener_p.h \
    private/abstract_socket_p.h \
    private/http_message_p.h \
    private/http_request_p.h \
    private/http_response_p.h \
    private/http_transaction_p.h \
    private/http_socket_p.h \
    private/process_manager_p.h \
    process_manager.h

OTHER_FILES += ../README
