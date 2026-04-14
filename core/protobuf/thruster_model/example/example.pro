QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app
CONFIG += c++17

SOURCES += \
    main.cpp \
    widget.cpp

HEADERS += \
    widget.h

LIBS += -lprotobuf

INCLUDEPATH += $$PWD

include($$PWD/../thrusterData.pri)
