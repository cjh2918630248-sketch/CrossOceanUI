#include <QApplication>
#include "widget.h"
#include <google/protobuf/stubs/common.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // 启动演示窗口，展示 GinsDataListModel 的基本用法。
    Widget w;
    w.show();

    int ret = app.exec();
    // 按 protobuf 官方建议，在程序退出前释放其全局资源。
    google::protobuf::ShutdownProtobufLibrary();
    return ret;
}
