QT += widgets
greaterThan(QT_MAJOR_VERSION, 5): QT += core5compat

CONFIG += c++17
CONFIG += warn_off
CONFIG += resources_big
CONFIG += console
CONFIG -= debug_and_release
DEFINES += QT_MESSAGELOGCONTEXT

TEMPLATE    = app
TARGET      = example_mqtt

SOURCES += \
    $$PWD/main.cpp \
    $$PWD/mqttwindow.cpp

HEADERS += \
    $$PWD/mqttwindow.h

INCLUDEPATH += $$PWD

# core
include($$PWD/../../core/mqtt/mqtt.pri)

#指定编译生成的可执行文件放到根目录下的bin目录
!android:!ios {
DESTDIR = $$PWD/../../bin/examples/mqtt
}

MOC_DIR     = temp/moc
RCC_DIR     = temp/rcc
UI_DIR      = temp/ui
OBJECTS_DIR = temp/obj

