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
    TARGET = fcgi_js_web_server_extensiond
    DEFINES += DEBUG
} else {
    TARGET = fcgi_js_web_server_extension
}

message($$TARGET will be installed in $$DESTDIR)

SOURCES += main.cpp \
    local_socket_manager.cpp

HEADERS += local_socket_manager.h \
    private/local_socket_manager_p.h

OTHER_FILES += ../README
