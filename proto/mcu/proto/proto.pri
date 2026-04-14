!defined(PROTO_PRI_INCLUDED, var) {
PROTO_PRI_INCLUDED = 1

# proto

#将当前目录加入到头文件路径
INCLUDEPATH += $$PWD

SOURCES += \
    $$PWD/msg_gins_data.pb.cc \
    $$PWD/msg_battery.pb.cc \
    $$PWD/msg_thruster.pb.cc

HEADERS += \
    $$PWD/msg_gins_data.pb.h \
    $$PWD/msg_battery.pb.h \
    $$PWD/msg_thruster.pb.h

}