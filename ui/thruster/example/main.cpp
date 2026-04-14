#include <QApplication>
#include "mainwidget.h"
#include <google/protobuf/stubs/common.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setStyle("Fusion");

    MainWidget w;
    w.show();

    const int ret = app.exec();
    google::protobuf::ShutdownProtobufLibrary();
    return ret;
}
