QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app
CONFIG += c++17

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
include($$PWD/../ginsDataList.pri)
