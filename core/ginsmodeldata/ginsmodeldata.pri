!defined(GINSDATA_PRI_INCLUDED, var) {
GINSDATA_PRI_INCLUDED = 1

# ginsData

#将当前目录加入到头文件路径
INCLUDEPATH += $$PWD

SOURCES += \
    $$PWD/ginsdatamodel.cpp

HEADERS += \
    $$PWD/ginsdatamodel.h

include($$PWD/../../proto/mcu/proto/proto.pri)

}