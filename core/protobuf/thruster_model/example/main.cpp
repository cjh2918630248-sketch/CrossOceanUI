#include <QApplication>
#include "widget.h"
#include <google/protobuf/stubs/common.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    Widget w;
    w.show();

    const int ret = app.exec();
    google::protobuf::ShutdownProtobufLibrary();
    return ret;
}
