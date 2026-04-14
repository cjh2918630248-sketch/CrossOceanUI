QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app
CONFIG += c++17

TARGET = thrustermodel

SOURCES += \
    main.cpp \
    thrusterview.cpp

HEADERS += \
    thrusterview.h

LIBS += -lprotobuf

include($$PWD/../../../core/protobuf/thruster_model/thrusterData.pri)
