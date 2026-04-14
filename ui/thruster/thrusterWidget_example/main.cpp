#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QWidget>
#include <QLabel>
#include <QTextEdit>

#include <google/protobuf/stubs/common.h>

#include "thrusterwidget.h"
#include "thrusterdatamodel.h"

static QVariantMap makeSample(int seq)
{
    const uint32_t st = (seq % 4 == 0)
                            ? static_cast<uint32_t>(thruster::NORMAL | thruster::CHARGING)
                            : static_cast<uint32_t>(thruster::NORMAL);
    return {
        { "status",       st },
        { "errorCode",    static_cast<uint>(seq % 3) },
        { "rpm",          1200 + seq * 80 },
        { "angle",        -30 + seq * 6 },
        { "power",        700 + seq * 25 },
        { "busVoltage",   static_cast<uint>(480 + seq) },
        { "busCurrent",   10 + seq },
        { "phaseCurrent", 14 + seq * 2 },
        { "motorTemp",    32 + seq * 3 },
        { "mosTemp",      38 + seq * 2 },
        { "temp",         26 + seq },
        { "runTime",      static_cast<uint>(600u + seq * 50) },
        { "sumRunTime",   static_cast<uint>(12000u + seq * 300) },
    };
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setStyle("Fusion");

    QWidget window;
    window.setWindowTitle(QStringLiteral("ThrusterWidget 外部调用示例"));
    window.resize(980, 620);

    auto *root = new QVBoxLayout(&window);

    auto *title = new QLabel(QStringLiteral("模拟外部项目调用 `ThrusterWidget`"), &window);
    title->setStyleSheet("font-size: 16px; font-weight: bold; color: #1a5276;");
    root->addWidget(title);

    auto *widget = new ThrusterWidget(&window);
    root->addWidget(widget, 1);

    auto *logEdit = new QTextEdit(&window);
    logEdit->setReadOnly(true);
    logEdit->setFixedHeight(120);
    logEdit->setStyleSheet(
        "QTextEdit { font-family: 'Consolas','Courier New',monospace;"
        "  font-size: 11px; background: #0d1117; color: #c9d1d9;"
        "  border: 1px solid #30363d; border-radius: 4px; }");

    auto log = [logEdit](const QString &text) {
        logEdit->append(text);
    };

    auto *externalModel = new ThrusterDataModel(&window);
    int seq = 0;

    auto *btnRow1 = new QHBoxLayout;
    auto *btnBindExternal = new QPushButton(QStringLiteral("绑定外部模型"), &window);
    auto *btnPushExternal = new QPushButton(QStringLiteral("外部模型写入样例"), &window);
    auto *btnSetMeta = new QPushButton(QStringLiteral("外部模型设置元信息"), &window);
    btnRow1->addWidget(btnBindExternal);
    btnRow1->addWidget(btnPushExternal);
    btnRow1->addWidget(btnSetMeta);
    btnRow1->addStretch();
    root->addLayout(btnRow1);

    auto *btnRow2 = new QHBoxLayout;
    auto *btnDirectSetData = new QPushButton(QStringLiteral("直接调用 widget->setData"), &window);
    auto *btnClear = new QPushButton(QStringLiteral("清空 UI"), &window);
    btnRow2->addWidget(btnDirectSetData);
    btnRow2->addWidget(btnClear);
    btnRow2->addStretch();
    root->addLayout(btnRow2);

    root->addWidget(logEdit);

    QObject::connect(btnBindExternal, &QPushButton::clicked, [&]() {
        widget->setModel(externalModel);
        log(QStringLiteral("[外部] widget->setModel(externalModel)"));
    });

    QObject::connect(btnPushExternal, &QPushButton::clicked, [&]() {
        externalModel->fromMap(makeSample(seq++));
        log(QStringLiteral("[外部] externalModel->fromMap(sample)"));
    });

    QObject::connect(btnSetMeta, &QPushButton::clicked, [&]() {
        externalModel->setPort(static_cast<uint>(9100 + seq));
        externalModel->setThrusterNum(static_cast<uint>(seq % 8));
        log(QStringLiteral("[外部] externalModel->setPort()/setThrusterNum()"));
    });

    QObject::connect(btnDirectSetData, &QPushButton::clicked, [&]() {
        widget->setData(makeSample(seq++));
        log(QStringLiteral("[外部] widget->setData(sample)"));
    });

    QObject::connect(btnClear, &QPushButton::clicked, [&]() {
        widget->clear();
        log(QStringLiteral("[外部] widget->clear()"));
    });

    widget->setModel(externalModel);
    externalModel->setPort(9100u);
    externalModel->setThrusterNum(1u);
    externalModel->fromMap(makeSample(seq++));
    log(QStringLiteral("[初始化] 已绑定外部模型并写入首条样例"));

    window.show();
    const int ret = app.exec();
    google::protobuf::ShutdownProtobufLibrary();
    return ret;
}
