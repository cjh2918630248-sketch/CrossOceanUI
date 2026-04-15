QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app
CONFIG += c++17
CONFIG += warn_off
CONFIG += resources_big
CONFIG += console
CONFIG -= debug_and_release
DEFINES += QT_MESSAGELOGCONTEXT

# 构建说明：
# - 该 demo 同时编译业务模型与 protobuf 生成文件；
# - 直接链接 libprotobuf，运行时即可完成序列化/反序列化演示。
SOURCES += \
    main.cpp \
    widget.cpp \

HEADERS += \
    widget.h \

LIBS += -lprotobuf

INCLUDEPATH += $$PWD

# core
include($$PWD/../../core/ginsmodeldata/ginsdatalist.pri)

#指定编译生成的可执行文件放到根目录下的bin目录
!android:!ios {
DESTDIR = $$PWD/../../bin/examples/ginsmodeldatalist
}

MOC_DIR     = temp/moc
RCC_DIR     = temp/rcc
UI_DIR      = temp/ui
OBJECTS_DIR = temp/obj
