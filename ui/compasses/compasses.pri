# compassesUI
#
# 使用方式：在工程 .pro 文件中 include(path/to/compassesUI.pri)

QT += svg
INCLUDEPATH += $$PWD

SOURCES += \
    $$PWD/compasseswidget.cpp

HEADERS += \
    $$PWD/compasseswidget.h

RESOURCES += \
    $$PWD/compasses.qrc
