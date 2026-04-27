# shipui
#
# 使用方式：在工程 .pro 文件中 include(path/to/shipui.pri)

INCLUDEPATH += $$PWD

SOURCES += \
    $$PWD/main/icondye.cpp \
    $$PWD/main/shipuiwidget.cpp \
    $$PWD/left/leftwidget.cpp \
    $$PWD/top/topwidget.cpp \
    $$PWD/middle/midwidget.cpp \
    $$PWD/right/rightwidget.cpp \
    $$PWD/left/speedgaugewidget.cpp \
    $$PWD/right/rightthrusterwidget.cpp \
    $$PWD/right/ruddergaugewidget.cpp \
    $$PWD/main/speedmaxconfig.cpp \
    $$PWD/top/vesselstatus.cpp \
    $$PWD/top/clocktoolbar.cpp \
    $$PWD/main/scaledview.cpp

HEADERS += \
    $$PWD/main/icondye.h \
    $$PWD/main/shipuiwidget.h \
    $$PWD/left/leftwidget.h \
    $$PWD/top//topwidget.h \
    $$PWD/middle/midwidget.h \
    $$PWD/right/rightwidget.h \
    $$PWD/left/speedgaugewidget.h \
    $$PWD/right/rightthrusterwidget.h \
    $$PWD/right/ruddergaugewidget.h \
    $$PWD/main/speedmaxconfig.h \
    $$PWD/top/vesselstatus.h \
    $$PWD/top/clocktoolbar.h \
    $$PWD/main/scaledview.h

RESOURCES += \
    $$PWD/shipui.qrc
