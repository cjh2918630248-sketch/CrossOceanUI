#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QFile>
#include <QTextStream>

#include "ginswidget.h"
#include "ginsdatamodel.h"
#include "ginsdatalistmodel.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QFile styleFile(":/example/gins.qss");
    if (styleFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream ts(&styleFile);
        app.setStyleSheet(ts.readAll());
        styleFile.close();
    }

    // ---- 数据准备 ----

    // 单条数据
    GinsDataModel singleModel;
    singleModel.fromMap({
        {"port", 1u}, {"utcLocal", 1711900800.0},
        {"latitude", 31.230416}, {"longitude", 121.473701}, {"altitude", 10.5},
        {"rollDeg", -1.2}, {"pitchDeg", 0.5}, {"headingDeg", 270.0},
        {"vnMps", 3.5}, {"veMps", -1.2}, {"roti", 0.01},
        {"ax", 0.05}, {"ay", -0.03}, {"aaz", 0.001},
        {"ginsStatus", 4u}, {"hdop", 1.2}, {"headStd", 0.5}
    });

    // 多条数据
    auto *listModel = new GinsDataListModel;
    listModel->append({
        {"port", 1u}, {"utcLocal", 1711900800.0},
        {"latitude", 31.230416}, {"longitude", 121.473701}, {"altitude", 10.5},
        {"rollDeg", -1.2}, {"pitchDeg", 0.5}, {"headingDeg", 270.0},
        {"vnMps", 3.5}, {"veMps", -1.2}, {"roti", 0.01},
        {"ax", 0.05}, {"ay", -0.03}, {"aaz", 0.001},
        {"ginsStatus", 4u}, {"hdop", 1.2}, {"headStd", 0.5}
    });
    listModel->append({
        {"port", 2u}, {"utcLocal", 1711900860.0},
        {"latitude", 31.231000}, {"longitude", 121.474500}, {"altitude", 11.0},
        {"rollDeg", 0.3}, {"pitchDeg", -0.2}, {"headingDeg", 265.0},
        {"vnMps", 4.0}, {"veMps", 0.5}, {"roti", 0.02},
        {"ax", 0.02}, {"ay", 0.01}, {"aaz", 0.0},
        {"ginsStatus", 4u}, {"hdop", 1.0}, {"headStd", 0.3}
    });

    // ---- 主窗口 ----

    QWidget w;
    w.setWindowTitle(QStringLiteral("GinsWidget 示例"));
    w.resize(920, 520);

    auto *root = new QVBoxLayout(&w);

    auto *ginsWidget = new GinsWidget;
    ginsWidget->setData(singleModel);          // 默认以单船模式启动
    root->addWidget(ginsWidget, 1);

    // 底部切换按钮
    auto *btnRow     = new QHBoxLayout;
    auto *btnSingle  = new QPushButton(QStringLiteral("单船模式"));
    auto *btnList    = new QPushButton(QStringLiteral("多船模式"));
    auto *btnClear   = new QPushButton(QStringLiteral("清空"));
    btnRow->addWidget(btnSingle);
    btnRow->addWidget(btnList);
    btnRow->addWidget(btnClear);
    btnRow->addStretch();
    root->addLayout(btnRow);

    QObject::connect(btnSingle, &QPushButton::clicked, [&]() {
        ginsWidget->setData(singleModel);
    });
    QObject::connect(btnList, &QPushButton::clicked, [&]() {
        ginsWidget->setModel(listModel);
    });
    QObject::connect(btnClear, &QPushButton::clicked, [&]() {
        ginsWidget->clear();
    });

    w.show();
    return app.exec();
}
