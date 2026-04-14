# qtmqtt.pri

# 设置库的包含路径
INCLUDEPATH += $$PWD/qtmqtt/install/include

# Linux 无独立 debug 库，debug/release 均链接同一 libQt5Mqtt.so
LIBS += -L$$PWD/qtmqtt/install/lib -lQt5Mqtt