#include "widget.h"
#include "../thrusterdatamodel.h"

#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>

#include "msg_thruster.pb.h"

static QVariantMap sampleData(int seq)
{
    const uint32_t st = (seq % 4 == 0)
                            ? static_cast<uint32_t>(thruster::NORMAL | thruster::CHARGING)
                            : static_cast<uint32_t>(thruster::NORMAL);
    return {
        { "status",       st },
        { "errorCode",    static_cast<uint>(seq % 3) },
        { "rpm",          1200 + seq * 15 },
        { "angle",        -45 + seq * 5 },
        { "power",        800 + seq * 20 },
        { "busVoltage",   static_cast<uint>(480 + seq) },
        { "busCurrent",   12 + seq },
        { "phaseCurrent", 18 + seq * 2 },
        { "motorTemp",    35 + seq },
        { "mosTemp",      40 + seq },
        { "temp",         28 + seq },
        { "runTime",      static_cast<uint>(1000u * seq) },
        { "sumRunTime",   static_cast<uint>(50000u + seq * 100) },
    };
}

static thruster::Data mapToProto(const QVariantMap &map)
{
    thruster::Data out;
    out.set_status(map.value("status").toUInt());
    out.set_error_code(map.value("errorCode").toUInt());
    out.set_rpm(map.value("rpm").toInt());
    out.set_angle(map.value("angle").toInt());
    out.set_power(map.value("power").toInt());
    out.set_bus_voltage(map.value("busVoltage").toUInt());
    out.set_bus_current(map.value("busCurrent").toInt());
    out.set_phase_current(map.value("phaseCurrent").toInt());
    out.set_motor_temp(map.value("motorTemp").toInt());
    out.set_mos_temp(map.value("mosTemp").toInt());
    out.set__temp(map.value("temp").toInt());
    out.set_run_time(map.value("runTime").toUInt());
    out.set_sum_run_time(map.value("sumRunTime").toUInt());
    return out;
}

static QString mapToString(const QVariantMap &m)
{
    return QStringLiteral(
               "status=%1 (%2)\n"
               "errorCode=%3\n"
               "rpm=%4  angle=%5  power=%6\n"
               "busVoltage=%7  busCurrent=%8  phaseCurrent=%9\n"
               "motorTemp=%10  mosTemp=%11  temp=%12\n"
               "runTime=%13  sumRunTime=%14")
        .arg(m.value("status").toUInt())
        .arg(m.value("statusText").toString())
        .arg(m.value("errorCode").toUInt())
        .arg(m.value("rpm").toInt())
        .arg(m.value("angle").toInt())
        .arg(m.value("power").toInt())
        .arg(m.value("busVoltage").toUInt())
        .arg(m.value("busCurrent").toInt())
        .arg(m.value("phaseCurrent").toInt())
        .arg(m.value("motorTemp").toInt())
        .arg(m.value("mosTemp").toInt())
        .arg(m.value("temp").toInt())
        .arg(m.value("runTime").toUInt())
        .arg(m.value("sumRunTime").toUInt());
}

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle(QStringLiteral("ThrusterDataModel 示例"));
    resize(720, 460);

    m_model = new ThrusterDataModel(this);

    auto *titleLabel = new QLabel(QStringLiteral("单对象 ThrusterDataModel 示例"), this);
    titleLabel->setStyleSheet(
        QStringLiteral("QLabel { font-size: 16px; font-weight: bold; color: #1a5276; }"));

    m_detailLabel = new QLabel(this);
    m_detailLabel->setWordWrap(true);
    m_detailLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    m_detailLabel->setMinimumHeight(180);
    m_detailLabel->setStyleSheet(
        QStringLiteral("QLabel { background: #f5f5f5; padding: 8px; border: 1px solid #ccc; }"));

    m_hexEdit = new QTextEdit(this);
    m_hexEdit->setFixedHeight(70);
    m_hexEdit->setPlaceholderText(QStringLiteral("serialize() 输出 hex，或粘贴 hex 后点击反序列化"));

    m_statusLabel = new QLabel(QStringLiteral("就绪"), this);

    auto *btnLoadSample = new QPushButton(QStringLiteral("加载示例"), this);
    auto *btnOverwrite  = new QPushButton(QStringLiteral("覆盖示例"), this);
    auto *btnClear      = new QPushButton(QStringLiteral("清空"), this);
    auto *btnLoadProto  = new QPushButton(QStringLiteral("从 Proto 加载"), this);
    auto *btnExport     = new QPushButton(QStringLiteral("导出 Proto"), this);
    auto *btnSerialize  = new QPushButton(QStringLiteral("序列化"), this);
    auto *btnDeserialize = new QPushButton(QStringLiteral("反序列化"), this);
    auto *btnPrint      = new QPushButton(QStringLiteral("printData"), this);

    auto *buttonGrid = new QGridLayout;
    buttonGrid->addWidget(btnLoadSample, 0, 0);
    buttonGrid->addWidget(btnOverwrite,  0, 1);
    buttonGrid->addWidget(btnClear,      0, 2);
    buttonGrid->addWidget(btnLoadProto,  1, 0);
    buttonGrid->addWidget(btnExport,     1, 1);
    buttonGrid->addWidget(btnPrint,      1, 2);
    buttonGrid->addWidget(btnSerialize,  2, 0);
    buttonGrid->addWidget(btnDeserialize, 2, 1);

    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(titleLabel);
    mainLayout->addLayout(buttonGrid);
    mainLayout->addWidget(new QLabel(QStringLiteral("当前模型数据："), this));
    mainLayout->addWidget(m_detailLabel, 1);
    mainLayout->addWidget(new QLabel(QStringLiteral("Hex："), this));
    mainLayout->addWidget(m_hexEdit);
    mainLayout->addWidget(m_statusLabel);

    connect(btnLoadSample,  &QPushButton::clicked, this, &Widget::onLoadSample);
    connect(btnOverwrite,   &QPushButton::clicked, this, &Widget::onOverwriteSample);
    connect(btnClear,       &QPushButton::clicked, this, &Widget::onClear);
    connect(btnLoadProto,   &QPushButton::clicked, this, &Widget::onLoadProto);
    connect(btnExport,      &QPushButton::clicked, this, &Widget::onExportProto);
    connect(btnSerialize,   &QPushButton::clicked, this, &Widget::onSerialize);
    connect(btnDeserialize, &QPushButton::clicked, this, &Widget::onDeserialize);
    connect(btnPrint,       &QPushButton::clicked, this, &Widget::onPrintData);

    connect(m_model, &ThrusterDataModel::dataUpdated,      this, &Widget::refreshView);
    connect(m_model, &ThrusterDataModel::portChanged,      this, &Widget::refreshView);
    connect(m_model, &ThrusterDataModel::thrusterNumChanged, this, &Widget::refreshView);
    connect(m_model, &ThrusterDataModel::errorOccurred,    this, &Widget::onErrorOccurred);

    refreshView();
}

void Widget::onLoadSample()
{
    const int seq = m_sampleIndex++;
    m_model->setPort(static_cast<uint>(9100 + seq));
    m_model->setThrusterNum(static_cast<uint>(seq % 8));
    m_model->fromMap(sampleData(seq));
    m_statusLabel->setText(QStringLiteral("已加载示例数据"));
}

void Widget::onOverwriteSample()
{
    QVariantMap modified = sampleData(m_sampleIndex++);
    modified["errorCode"] = 99u;
    m_model->fromMap(modified);
    m_statusLabel->setText(QStringLiteral("已覆盖当前数据"));
}

void Widget::onClear()
{
    thruster::Data empty;
    m_model->loadFromProto(empty);
    m_model->setPort(0);
    m_model->setThrusterNum(0);
    m_hexEdit->clear();
    m_statusLabel->setText(QStringLiteral("已清空"));
}

void Widget::onLoadProto()
{
    const QVariantMap map = sampleData(m_sampleIndex++);
    m_model->loadFromProto(mapToProto(map));
    m_statusLabel->setText(QStringLiteral("已通过 loadFromProto() 导入"));
}

void Widget::onExportProto()
{
    thruster::Data out;
    m_model->exportToProto(out);
    m_statusLabel->setText(
        QStringLiteral("exportToProto: status=%1 rpm=%2 power=%3")
            .arg(out.status())
            .arg(out.rpm())
            .arg(out.power()));
}

void Widget::onSerialize()
{
    const QByteArray bytes = m_model->serialize();
    if (bytes.isEmpty()) {
        m_statusLabel->setText(QStringLiteral("序列化失败"));
        return;
    }

    m_hexEdit->setPlainText(QString::fromLatin1(bytes.toHex(' ')));
    m_statusLabel->setText(QStringLiteral("序列化成功：%1 字节").arg(bytes.size()));
}

void Widget::onDeserialize()
{
    const QString hex = m_hexEdit->toPlainText().simplified().remove(' ');
    if (hex.isEmpty()) {
        m_statusLabel->setText(QStringLiteral("hex 为空"));
        return;
    }

    const QByteArray bytes = QByteArray::fromHex(hex.toLatin1());
    if (m_model->deserialize(bytes))
        m_statusLabel->setText(QStringLiteral("反序列化成功"));
    else
        m_statusLabel->setText(QStringLiteral("反序列化失败"));
}

void Widget::onPrintData()
{
    m_model->printData();
    m_statusLabel->setText(QStringLiteral("已调用 printData()"));
}

void Widget::refreshView()
{
    const QVariantMap map = m_model->toMap();
    m_detailLabel->setText(mapToString(map));
    m_statusLabel->setText(
        QStringLiteral("port=%1  thrusterNum=%2  status=%3  rpm=%4")
            .arg(m_model->port())
            .arg(m_model->thrusterNum())
            .arg(map.value("status").toUInt())
            .arg(map.value("rpm").toInt()));
}

void Widget::onErrorOccurred(int type, const QString &msg)
{
    m_statusLabel->setText(QStringLiteral("错误(type=%1): %2").arg(type).arg(msg));
}
