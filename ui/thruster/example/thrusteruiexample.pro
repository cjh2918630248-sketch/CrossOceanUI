QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app
CONFIG  += c++17

TARGET = thrusteruiexample

SOURCES += \
    main.cpp \
    mainwidget.cpp

HEADERS += \
    mainwidget.h

LIBS += -lprotobuf

# thrusterUI 视图层（ThrusterDataListView + ThrusterDataPanel）
include($$PWD/../thrusterUI.pri)

# thrusterData 数据模型（ThrusterDataModel + proto）
include($$PWD/../../../core/protobuf/thruster_model/thrusterData.pri)
