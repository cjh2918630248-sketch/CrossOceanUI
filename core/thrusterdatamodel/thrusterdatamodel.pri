# thrusterData
#
# 使用方式：在工程 .pro 文件中 include(path/to/thrusterData.pri)

INCLUDEPATH += $$PWD

SOURCES += \
    $$PWD/thrusterdatamodel.cpp

HEADERS += \
    $$PWD/thrusterdatamodel.h

include($$PWD/../../../proto/mcu/proto/proto.pri)
