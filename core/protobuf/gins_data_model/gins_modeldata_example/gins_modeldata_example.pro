QT       += core
QT       -= gui

TEMPLATE = app
CONFIG   += c++17 console
CONFIG   -= app_bundle

SOURCES += main.cpp

LIBS += -lprotobuf

include($$PWD/../ginsData.pri)
