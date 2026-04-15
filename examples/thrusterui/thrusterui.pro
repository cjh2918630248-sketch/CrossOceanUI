QT += widgets

CONFIG += c++17
CONFIG += warn_off #关闭编译警告
CONFIG += resources_big #允许大体积资源文件
CONFIG += console #运行程序时弹出控制台
CONFIG -= debug_and_release #禁止同时生成 Debug + Release 版本
#让 qDebug () /qInfo () 等日志输出，自动带上 文件名 + 行号 + 函数名
DEFINES += QT_MESSAGELOGCONTEXT

include(../../ui/thrusterrpm/thrusterrpm.pri)

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    widget.cpp

HEADERS += \
    widget.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

#指定编译生成的可执行文件放到根目录下的bin目录
!android:!ios {
DESTDIR = $$PWD/../../bin/examples/thrusterui
}

MOC_DIR     = temp/moc
RCC_DIR     = temp/rcc
UI_DIR      = temp/ui
OBJECTS_DIR = temp/obj
