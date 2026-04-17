QT       += core
QT       -= gui

TEMPLATE = app
CONFIG   += c++17 console
CONFIG   -= app_bundle
CONFIG += warn_off
CONFIG += resources_big
CONFIG += console
CONFIG -= debug_and_release
DEFINES += QT_MESSAGELOGCONTEXT


CONFIG += warn_off
CONFIG += resources_big
CONFIG += console
CONFIG -= debug_and_release
DEFINES += QT_MESSAGELOGCONTEXT


SOURCES += main.cpp

LIBS += -lprotobuf


include($$PWD/../../core/ginsmodeldata/ginsmodeldata.pri)

#指定编译生成的可执行文件放到根目录下的bin目录
!android:!ios {
DESTDIR = $$PWD/../../bin/examples/ginsmodeldata
}

MOC_DIR     = temp/moc
RCC_DIR     = temp/rcc
UI_DIR      = temp/ui
OBJECTS_DIR = temp/obj