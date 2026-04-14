# thrusterUI
#
# 使用方式：在工程 .pro 文件中 include(path/to/thrusterUI.pri)
# 注意：需同时引入数据模型：
#   include(path/to/core/protobuf/thruster_model/thrusterData.pri)

INCLUDEPATH += $$PWD

SOURCES += \
    $$PWD/thrusterdatapanel.cpp \
    $$PWD/thrusterdataview.cpp \
    $$PWD/thrusterwidget.cpp

HEADERS += \
    $$PWD/thrusterdatapanel.h \
    $$PWD/thrusterdataview.h \
    $$PWD/thrusterwidget.h
