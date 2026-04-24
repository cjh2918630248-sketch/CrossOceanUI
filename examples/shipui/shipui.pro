QT += widgets

CONFIG += c++17
CONFIG += warn_off
CONFIG += resources_big
CONFIG += console
CONFIG -= debug_and_release
DEFINES += QT_MESSAGELOGCONTEXT

include($$PWD/../../ui/shipui/shipui.pri)

SOURCES += \
    main.cpp \
    timerviewdemo.cpp

HEADERS += \
    timerviewdemo.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# 指定编译生成的可执行文件放到根目录下的bin目录
!android:!ios {
DESTDIR = $$PWD/../../bin/examples/shipui
}

MOC_DIR     = temp/moc
RCC_DIR     = temp/rcc
UI_DIR      = temp/ui
OBJECTS_DIR = temp/obj
