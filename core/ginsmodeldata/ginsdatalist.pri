!defined(GINSDATALIST_PRI_INCLUDED, var) {
GINSDATALIST_PRI_INCLUDED = 1

# ginsDataList

#将当前目录加入到头文件路径
INCLUDEPATH += $$PWD

SOURCES += \
    $$PWD/ginsdatalistmodel.cpp

HEADERS += \
    $$PWD/ginsdatalistmodel.h

include($$PWD/../../proto/gins/proto/proto.pri)

}