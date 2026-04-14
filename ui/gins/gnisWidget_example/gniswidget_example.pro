QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app
CONFIG  += c++17

SOURCES += main.cpp

LIBS += -lprotobuf

include($$PWD/../ginsUI.pri)
include($$PWD/../../../core/protobuf/gins_data_model/ginsData.pri)
include($$PWD/../../../core/protobuf/gins_data_model/ginsDataList.pri)

RESOURCES += gins.qrc
