QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app
CONFIG += c++17

SOURCES += \
    main.cpp \
    mainwidget.cpp \

HEADERS += \
    mainwidget.h \
    gins.h \

FORMS += \
    mainwidget.ui

LIBS += -lprotobuf

include($$PWD/../../../proto/mcu/proto/proto.pri)
