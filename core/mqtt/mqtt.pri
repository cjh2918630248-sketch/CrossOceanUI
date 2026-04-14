# mqtt

#将当前目录加入到头文件路径
INCLUDEPATH += $$PWD

SOURCES += \
    $$PWD/mqtt.cpp

HEADERS += \
    $$PWD/mqtt.h

# 3rdparty 
include($$PWD/../../3rdparty/qtmqtt.pri)