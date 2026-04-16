QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app
CONFIG += c++17
CONFIG += warn_off #关闭编译警告
CONFIG += resources_big #允许大体积资源文件
CONFIG += console #运行程序时弹出控制台
CONFIG -= debug_and_release #禁止同时生成 Debug + Release 版本
#让 qDebug () /qInfo () 等日志输出，自动带上 文件名 + 行号 + 函数名
DEFINES += QT_MESSAGELOGCONTEXT

TARGET = thrustermodel

SOURCES += \
    main.cpp \
    thrusterview.cpp

HEADERS += \
    thrusterview.h

LIBS += -lprotobuf


include($$PWD/../../core/thrusterdatamodel/thrusterdatamodel.pri)

#指定编译生成的可执行文件放到根目录下的bin目录
!android:!ios {
DESTDIR = $$PWD/../../bin/examples/protobuf/thrustermodel
}

MOC_DIR     = temp/moc
RCC_DIR     = temp/rcc
UI_DIR      = temp/ui
OBJECTS_DIR = temp/obj
