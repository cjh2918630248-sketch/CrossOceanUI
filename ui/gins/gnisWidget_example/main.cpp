#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
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

    /* ========== 数据准备 ========== */

    // 单条船数据
    GinsDataModel singleModel;
    singleModel.fromMap({
        {"port", 1u},         {"utcLocal", 1711900800.0},
        {"latitude", 31.230416}, {"longitude", 121.473701}, {"altitude", 10.5},
        {"rollDeg", -1.2},    {"pitchDeg", 0.5},    {"headingDeg", 270.0},
        {"vnMps", 3.5},       {"veMps", -1.2},      {"roti", 0.01},
        {"ax", 0.05},         {"ay", -0.03},         {"aaz", 0.001},
        {"ginsStatus", 4u},   {"hdop", 1.2},         {"headStd", 0.5}
    });

    // 多条船数据
    auto *listModel = new GinsDataListModel;
    listModel->append({
        {"port", 1u},         {"utcLocal", 1711900800.0},
        {"latitude", 31.230416}, {"longitude", 121.473701}, {"altitude", 10.5},
        {"rollDeg", -1.2},    {"pitchDeg", 0.5},    {"headingDeg", 270.0},
        {"vnMps", 3.5},       {"veMps", -1.2},      {"roti", 0.01},
        {"ax", 0.05},         {"ay", -0.03},         {"aaz", 0.001},
        {"ginsStatus", 4u},   {"hdop", 1.2},         {"headStd", 0.5}
    });
    listModel->append({
        {"port", 2u},         {"utcLocal", 1711900860.0},
        {"latitude", 39.904202}, {"longitude", 116.407394}, {"altitude", 11.0},
        {"rollDeg", 0.3},     {"pitchDeg", -0.2},   {"headingDeg", 180.0},
        {"vnMps", 4.0},       {"veMps", 0.5},       {"roti", 0.02},
        {"ax", 0.02},         {"ay", 0.01},          {"aaz", 0.0},
        {"ginsStatus", 2u},   {"hdop", 1.0},         {"headStd", 0.3}
    });
    listModel->append({
        {"port", 3u},         {"utcLocal", 1711900920.0},
        {"latitude", 22.543096}, {"longitude", 114.057865}, {"altitude", 5.2},
        {"rollDeg", 0.8},     {"pitchDeg", 0.1},    {"headingDeg", 90.5},
        {"vnMps", 1.2},       {"veMps", 2.8},       {"roti", 0.005},
        {"ax", -0.01},        {"ay", 0.02},          {"aaz", 0.003},
        {"ginsStatus", 4u},   {"hdop", 0.8},         {"headStd", 0.2}
    });

    /* ========== 主窗口 ========== */

    QWidget w;
    w.setWindowTitle(QStringLiteral("GinsWidget 功能验证"));
    w.resize(960, 560);

    auto *root = new QVBoxLayout(&w);

    // 状态提示
    auto *lblStatus = new QLabel(QStringLiteral("当前模式：单船模式"));
    lblStatus->setAlignment(Qt::AlignCenter);
    lblStatus->setStyleSheet("font-size: 14px; font-weight: bold; padding: 4px;");
    root->addWidget(lblStatus);

    // GinsWidget 主体
    auto *ginsWidget = new GinsWidget;
    ginsWidget->setData(singleModel);
    root->addWidget(ginsWidget, 1);

    // 底部按钮行
    auto *btnRow    = new QHBoxLayout;
    auto *btnSingle = new QPushButton(QStringLiteral("单船模式"));
    auto *btnList   = new QPushButton(QStringLiteral("多船模式"));
    auto *btnClear  = new QPushButton(QStringLiteral("清空"));
    btnRow->addWidget(btnSingle);
    btnRow->addWidget(btnList);
    btnRow->addWidget(btnClear);
    btnRow->addStretch();
    root->addLayout(btnRow);

    // 单船模式：隐藏列表，直接显示详情
    QObject::connect(btnSingle, &QPushButton::clicked, [&]() {
        ginsWidget->setData(singleModel);
        lblStatus->setText(QStringLiteral("当前模式：单船模式"));
    });

    // 多船模式：显示左侧列表 + 右侧详情联动
    QObject::connect(btnList, &QPushButton::clicked, [&]() {
        ginsWidget->setModel(listModel);
        lblStatus->setText(QStringLiteral("当前模式：多船模式（点击左侧列表选择船只）"));
    });

    // 清空
    QObject::connect(btnClear, &QPushButton::clicked, [&]() {
        ginsWidget->clear();
        lblStatus->setText(QStringLiteral("当前模式：已清空"));
    });

    w.show();
    return app.exec();
}
