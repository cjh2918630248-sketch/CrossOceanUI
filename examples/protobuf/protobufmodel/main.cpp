#include "mainwidget.h"
#include <QApplication>
#include <google/protobuf/stubs/common.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWidget w;
    w.show();

    int ret = a.exec();
    google::protobuf::ShutdownProtobufLibrary();
    return ret;
}
