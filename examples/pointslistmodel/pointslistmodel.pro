QT += widgets
greaterThan(QT_MAJOR_VERSION, 5): QT += core5compat

CONFIG += c++17
CONFIG += warn_off
CONFIG += resources_big
CONFIG += console
CONFIG -= debug_and_release
DEFINES += QT_MESSAGELOGCONTEXT

TEMPLATE    = app
TARGET      = example_pointslistmodel

SOURCES += \
    $$PWD/main.cpp \
    $$PWD/mainwindow.cpp \
    $$PWD/mapview.cpp \
    $$PWD/datatableview.cpp \
    $$PWD/directlistview.cpp

HEADERS += \
    $$PWD/mainwindow.h \
    $$PWD/mapview.h \
    $$PWD/datatableview.h \
    $$PWD/directlistview.h \
    $$PWD/datagenerator.h

INCLUDEPATH += $$PWD

# core datamodel
include($$PWD/../../core/datamodel/datamodel.pri)

# SOURCES += \
#     $$PWD/../../core/datamodel/pointslistmodel.cpp

# HEADERS += \
#     $$PWD/../../core/datamodel/pointslistmodel.h

#指定编译生成的可执行文件放到根目录下的bin目录
!android:!ios {
DESTDIR = $$PWD/../../bin/examples/pointslistmodel
}

MOC_DIR     = temp/moc
RCC_DIR     = temp/rcc
UI_DIR      = temp/ui
OBJECTS_DIR = temp/obj
