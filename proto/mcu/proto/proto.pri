!defined(PROTO_PRI_INCLUDED, var) {
PROTO_PRI_INCLUDED = 1

# mcu protobuf 生成物位于 $$PWD/mcu/（与 protoc 中 import "mcu/..." 一致）
# - $$PWD：解析生成代码里的 #include "mcu/xxx.pb.h"
# - $$PWD/mcu：业务代码可继续 #include "thruster.pb.h" 等短路径

INCLUDEPATH += $$PWD
INCLUDEPATH += $$PWD/mcu

SOURCES += \
    $$PWD/mcu/port.pb.cc \
    $$PWD/mcu/battery.pb.cc \
    $$PWD/mcu/thruster.pb.cc

HEADERS += \
    $$PWD/mcu/port.pb.h \
    $$PWD/mcu/battery.pb.h \
    $$PWD/mcu/thruster.pb.h

}
