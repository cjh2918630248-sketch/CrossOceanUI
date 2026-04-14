#include "mainwidget.h"
#include "thrusterdatamodel.h"
#include "thrusterdataview.h"
#include "thrusterdatapanel.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QGroupBox>
#include <QFormLayout>
#include <QSpinBox>
#include <QLabel>
#include <QTextEdit>
#include <QSplitter>
#include <QFont>
#include <QDateTime>
#include <QScrollBar>

#include "msg_thruster.pb.h"

static QVariantMap makeSample(int seq)
{
    const uint32_t st = (seq % 4 == 0)
                            ? static_cast<uint32_t>(thruster::NORMAL | thruster::CHARGING)
                            : static_cast<uint32_t>(thruster::NORMAL);
    return {
        { "status",       st },
        { "errorCode",    static_cast<uint>(seq % 3) },
        { "rpm",          1000 + seq * 120 },
        { "angle",        -60 + seq * 10 },
        { "power",        600 + seq * 30 },
        { "busVoltage",   static_cast<uint>(480 + seq * 2) },
        { "busCurrent",   10 + seq },
        { "phaseCurrent", 15 + seq * 2 },
        { "motorTemp",    30 + seq * 4 },
        { "mosTemp",      38 + seq * 3 },
        { "temp",         25 + seq },
        { "runTime",      static_cast<uint>(500u + seq * 60) },
        { "sumRunTime",   static_cast<uint>(20000u + seq * 500) },
    };
}

static QPushButton *makeBtn(const QString &text, const QString &tip, QWidget *parent)
{
    auto *btn = new QPushButton(text, parent);
    btn->setToolTip(tip);
    btn->setMinimumHeight(28);
    btn->setStyleSheet(
        "QPushButton {"
        "  background: #f6f8fa; border: 1px solid #d0d7de;"
        "  border-radius: 6px; padding: 4px 10px; font-size: 12px;"
        "}"
        "QPushButton:hover  { background: #dbeafe; border-color: #2980b9; }"
        "QPushButton:pressed{ background: #bfdbfe; }");
    return btn;
}

static QGroupBox *makeGroup(const QString &title, QWidget *parent)
{
    auto *box = new QGroupBox(title, parent);
    box->setStyleSheet(
        "QGroupBox { font-weight: bold; font-size: 11px; color: #1a5276;"
        "  border: 1px solid #d0d7de; border-radius: 6px;"
        "  margin-top: 14px; padding-top: 6px; }"
        "QGroupBox::title { subcontrol-origin: margin; left: 10px; top: 1px;"
        "  padding: 0 4px; }");
    return box;
}

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle(tr("thrusterUI 接口示例（单数据模型）"));
    resize(1100, 700);
    buildUi();
    connectModel();
    onLoadSample();
}

void MainWidget::buildUi()
{
    auto *root = new QVBoxLayout(this);
    root->setContentsMargins(10, 10, 10, 6);
    root->setSpacing(6);

    auto *titleLbl = new QLabel(tr("thrusterUI  ·  单数据模型示例"), this);
    QFont tf = titleLbl->font();
    tf.setBold(true);
    tf.setPointSize(14);
    titleLbl->setFont(tf);
    titleLbl->setStyleSheet("color: #1a5276;");
    root->addWidget(titleLbl);

    auto *splitter = new QSplitter(Qt::Horizontal, this);
    splitter->setHandleWidth(6);

    m_model = new ThrusterDataModel(this);

    m_listView = new ThrusterDataView(this);
    m_listView->setModel(m_model);
    m_listView->setMinimumWidth(300);
    splitter->addWidget(m_listView);

    auto *rightWidget = new QWidget(this);
    auto *rightVBox = new QVBoxLayout(rightWidget);
    rightVBox->setContentsMargins(0, 0, 0, 0);
    rightVBox->setSpacing(6);

    m_panel = new ThrusterDataPanel(rightWidget);
    m_panel->clear();
    rightVBox->addWidget(m_panel, 1);

    auto *opsWidget = new QWidget(rightWidget);
    auto *opsGrid = new QGridLayout(opsWidget);
    opsGrid->setSpacing(6);
    opsGrid->setContentsMargins(0, 0, 0, 0);

    {
        auto *g = makeGroup(tr("数据操作（单条）"), opsWidget);
        auto *gl = new QGridLayout(g);
        auto *btnLoad = makeBtn(tr("加载示例"), tr("fromMap(示例数据)"), g);
        auto *btnOverwrite = makeBtn(tr("覆盖示例"), tr("再次写入新的示例数据"), g);
        auto *btnClear = makeBtn(tr("清空"), tr("清零模型字段"), g);
        gl->addWidget(btnLoad, 0, 0);
        gl->addWidget(btnOverwrite, 0, 1);
        gl->addWidget(btnClear, 1, 0, 1, 2);
        connect(btnLoad, &QPushButton::clicked, this, &MainWidget::onLoadSample);
        connect(btnOverwrite, &QPushButton::clicked, this, &MainWidget::onOverwriteSample);
        connect(btnClear, &QPushButton::clicked, this, &MainWidget::onClear);
        opsGrid->addWidget(g, 0, 0);
    }

    {
        auto *g = makeGroup(tr("元信息"), opsWidget);
        auto *fl = new QFormLayout(g);
        fl->setHorizontalSpacing(8);
        fl->setVerticalSpacing(4);
        m_spinPort = new QSpinBox(g);
        m_spinPort->setRange(0, 65535);
        m_spinPort->setValue(static_cast<int>(m_model->port()));
        fl->addRow(tr("port："), m_spinPort);
        m_spinThrusterNum = new QSpinBox(g);
        m_spinThrusterNum->setRange(0, 32);
        m_spinThrusterNum->setValue(static_cast<int>(m_model->thrusterNum()));
        fl->addRow(tr("thrusterNum："), m_spinThrusterNum);
        auto *btnSetPort = makeBtn(tr("setPort()"), tr("model->setPort(value)"), g);
        auto *btnSetNum = makeBtn(tr("setThrusterNum()"), tr("model->setThrusterNum(value)"), g);
        fl->addRow(btnSetPort, btnSetNum);
        connect(btnSetPort, &QPushButton::clicked, this, &MainWidget::onSetPort);
        connect(btnSetNum, &QPushButton::clicked, this, &MainWidget::onSetThrusterNum);
        opsGrid->addWidget(g, 0, 1);
    }

    {
        auto *g = makeGroup(tr("序列化"), opsWidget);
        auto *gl = new QGridLayout(g);
        auto *btnSerialize = makeBtn(tr("serialize"), tr("序列化当前单条数据"), g);
        auto *btnDeserialize = makeBtn(tr("deserialize"), tr("从下方 hex 输入反序列化"), g);
        gl->addWidget(btnSerialize, 0, 0);
        gl->addWidget(btnDeserialize, 0, 1);
        connect(btnSerialize, &QPushButton::clicked, this, &MainWidget::onSerialize);
        connect(btnDeserialize, &QPushButton::clicked, this, &MainWidget::onDeserialize);
        opsGrid->addWidget(g, 1, 0);
    }

    {
        auto *g = makeGroup(tr("Proto 导入导出"), opsWidget);
        auto *gl = new QGridLayout(g);
        auto *btnLoadProto = makeBtn(tr("loadFromProto"), tr("构造 Data 后导入"), g);
        auto *btnExportProto = makeBtn(tr("exportToProto"), tr("导出并打印字段"), g);
        auto *btnPrint = makeBtn(tr("printData"), tr("打印当前数据到 stdout"), g);
        gl->addWidget(btnLoadProto, 0, 0);
        gl->addWidget(btnExportProto, 0, 1);
        gl->addWidget(btnPrint, 1, 0, 1, 2);
        connect(btnLoadProto, &QPushButton::clicked, this, &MainWidget::onLoadProto);
        connect(btnExportProto, &QPushButton::clicked, this, &MainWidget::onExportProto);
        connect(btnPrint, &QPushButton::clicked, this, &MainWidget::onPrintData);
        opsGrid->addWidget(g, 1, 1);
    }

    rightVBox->addWidget(opsWidget);
    splitter->addWidget(rightWidget);
    splitter->setStretchFactor(0, 0);
    splitter->setStretchFactor(1, 1);
    root->addWidget(splitter, 1);

    auto *ioGroup = makeGroup(tr("Hex 输入 / 输出 与日志"), this);
    auto *ioVBox = new QVBoxLayout(ioGroup);
    ioVBox->setSpacing(4);

    m_hexEdit = new QTextEdit(ioGroup);
    m_hexEdit->setPlaceholderText(tr("serialize 输出 hex 到这里；也可粘贴 hex 后点 deserialize"));
    m_hexEdit->setFixedHeight(64);
    m_hexEdit->setStyleSheet(
        "QTextEdit { font-family: 'Consolas','Courier New',monospace;"
        "  font-size: 11px; background: #f6f8fa; color: #1f2328;"
        "  border: 1px solid #d0d7de; border-radius: 4px; }");
    ioVBox->addWidget(m_hexEdit);

    m_logEdit = new QTextEdit(ioGroup);
    m_logEdit->setReadOnly(true);
    m_logEdit->setFixedHeight(120);
    m_logEdit->setStyleSheet(
        "QTextEdit { font-family: 'Consolas','Courier New',monospace;"
        "  font-size: 11px; background: #0d1117; color: #c9d1d9;"
        "  border: 1px solid #30363d; border-radius: 4px; }");
    ioVBox->addWidget(m_logEdit);
    root->addWidget(ioGroup);

    m_statusBar = new QLabel(tr("就绪"), this);
    m_statusBar->setStyleSheet(
        "QLabel { font-size: 11px; color: #555;"
        "  background: #f6f8fa; border: 1px solid #d0d7de;"
        "  border-radius: 4px; padding: 2px 8px; }");
    root->addWidget(m_statusBar);
}

void MainWidget::connectModel()
{
    connect(m_model, &ThrusterDataModel::dataUpdated,
            this, &MainWidget::onDataUpdated);
    connect(m_model, &ThrusterDataModel::errorOccurred,
            this, &MainWidget::onErrorOccurred);
}

void MainWidget::onLoadSample()
{
    m_model->setPort(static_cast<uint>(9000 + m_sampleSeq));
    m_model->setThrusterNum(static_cast<uint>(m_sampleSeq % 8));
    m_model->fromMap(makeSample(m_sampleSeq++));
    log(tr("[loadSample] 已加载单条示例数据"));
}

void MainWidget::onOverwriteSample()
{
    m_model->fromMap(makeSample(m_sampleSeq++));
    log(tr("[overwriteSample] 已覆盖当前数据"));
}

void MainWidget::onClear()
{
    thruster::Data d;
    m_model->loadFromProto(d);
    m_model->setPort(0);
    m_model->setThrusterNum(0);
    m_panel->clear();
    log(tr("[clear] 已清零模型数据"));
}

void MainWidget::onSetPort()
{
    const uint v = static_cast<uint>(m_spinPort->value());
    m_model->setPort(v);
    log(tr("[setPort] port = %1").arg(v));
}

void MainWidget::onSetThrusterNum()
{
    const uint v = static_cast<uint>(m_spinThrusterNum->value());
    m_model->setThrusterNum(v);
    log(tr("[setThrusterNum] thrusterNum = %1").arg(v));
}

void MainWidget::onSerialize()
{
    const QByteArray bytes = m_model->serialize();
    if (bytes.isEmpty()) {
        log(tr("[serialize] 失败"));
        return;
    }
    const QString hex = QString::fromLatin1(bytes.toHex());
    m_hexEdit->setPlainText(hex);
    log(tr("[serialize] %1 bytes").arg(bytes.size()));
}

void MainWidget::onDeserialize()
{
    const QString raw = m_hexEdit->toPlainText().simplified().remove(' ').remove('\n');
    if (raw.isEmpty()) {
        log(tr("[WARN] hex 输入为空"));
        return;
    }
    const QByteArray bytes = QByteArray::fromHex(raw.toLatin1());
    if (m_model->deserialize(bytes))
        log(tr("[deserialize] 成功"));
    else
        log(tr("[deserialize] 失败"));
}

void MainWidget::onLoadProto()
{
    const QVariantMap vm = makeSample(m_sampleSeq++);
    thruster::Data d;
    d.set_status(vm.value("status").toUInt());
    d.set_error_code(vm.value("errorCode").toUInt());
    d.set_rpm(vm.value("rpm").toInt());
    d.set_angle(vm.value("angle").toInt());
    d.set_power(vm.value("power").toInt());
    d.set_bus_voltage(vm.value("busVoltage").toUInt());
    d.set_bus_current(vm.value("busCurrent").toInt());
    d.set_phase_current(vm.value("phaseCurrent").toInt());
    d.set_motor_temp(vm.value("motorTemp").toInt());
    d.set_mos_temp(vm.value("mosTemp").toInt());
    d.set__temp(vm.value("temp").toInt());
    d.set_run_time(vm.value("runTime").toUInt());
    d.set_sum_run_time(vm.value("sumRunTime").toUInt());
    m_model->loadFromProto(d);
    log(tr("[loadFromProto] 已导入一条 proto::Data"));
}

void MainWidget::onExportProto()
{
    thruster::Data out;
    m_model->exportToProto(out);
    log(tr("[exportToProto] status=%1 rpm=%2 power=%3 runTime=%4")
            .arg(out.status())
            .arg(out.rpm())
            .arg(out.power())
            .arg(out.run_time()));
}

void MainWidget::onPrintData()
{
    m_model->printData();
    log(tr("[printData] 已输出到 stdout"));
}

void MainWidget::onDataUpdated()
{
    const QVariantMap vm = m_model->toMap();
    m_panel->setData(vm);
    m_statusBar->setText(tr("port=%1  |  thrusterNum=%2  |  status=%3  rpm=%4")
                             .arg(m_model->port())
                             .arg(m_model->thrusterNum())
                             .arg(vm.value("status").toUInt())
                             .arg(vm.value("rpm").toInt()));
    log(tr("[signal] dataUpdated"));
}

void MainWidget::onErrorOccurred(int type, const QString &msg)
{
    log(tr("[signal] errorOccurred  type=%1  msg=%2").arg(type).arg(msg));
}

void MainWidget::log(const QString &text)
{
    const QString ts = QDateTime::currentDateTime().toString("hh:mm:ss.zzz");
    m_logEdit->append(QString("[%1]  %2").arg(ts, text));
    m_logEdit->verticalScrollBar()->setValue(m_logEdit->verticalScrollBar()->maximum());
}
