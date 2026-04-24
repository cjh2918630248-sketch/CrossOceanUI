!defined(PROTO_PRI_INCLUDED, var) {
PROTO_PRI_INCLUDED = 1

# proto

#将当前目录加入到头文件路径
INCLUDEPATH += $$PWD

SOURCES += \
    $$PWD/port.pb.cc \
    $$PWD/data.pb.cc

HEADERS += \
    $$PWD/port.pb.h \
    $$PWD/data.pb.h

}