QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app
CONFIG  += c++17

TARGET = thrusterwidgetexample

SOURCES += \
    main.cpp

HEADERS +=

LIBS += -lprotobuf

include($$PWD/../thrusterUI.pri)
include($$PWD/../../../core/protobuf/thruster_model/thrusterData.pri)
