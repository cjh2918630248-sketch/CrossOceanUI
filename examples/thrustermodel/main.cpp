#include <QApplication>
#include "thrusterview.h"
#include <google/protobuf/stubs/common.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setStyle("Fusion");

    ThrusterView w;
    w.show();

    const int ret = app.exec();
    google::protobuf::ShutdownProtobufLibrary();
    return ret;
}
