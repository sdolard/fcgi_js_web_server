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
    TARGET = web_server_extentiond
    DEFINES += DEBUG
} else {
    TARGET = web_server_extention
}

message($$TARGET will be installed in $$DESTDIR)

SOURCES += main.cpp \
    local_socket_manager.cpp
#HEADERS +=

HEADERS += \
    local_socket_manager.h \
    private/local_socket_manager_p.h
