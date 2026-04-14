#include "thrusterdatapanel.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QFont>

ThrusterDataPanel::ThrusterDataPanel(QWidget *parent)
    : QWidget(parent)
{
    buildUi();
}

QGroupBox *ThrusterDataPanel::makeGroup(const QString &title,
                                         const QStringList &rowNames,
                                         QList<QLabel *> &outLabels)
{
    auto *box  = new QGroupBox(title, this);
    auto *form = new QFormLayout(box);
    form->setLabelAlignment(Qt::AlignRight);
    form->setHorizontalSpacing(12);
    form->setVerticalSpacing(5);

    for (const QString &name : rowNames) {
        auto *val = new QLabel(QStringLiteral("--"), this);
        val->setObjectName("valueLabel");
        val->setTextInteractionFlags(Qt::TextSelectableByMouse);
        form->addRow(name + QStringLiteral("："), val);
        outLabels.append(val);
    }
    return box;
}

void ThrusterDataPanel::buildUi()
{
    auto *root = new QVBoxLayout(this);
    root->setContentsMargins(8, 8, 8, 8);
    root->setSpacing(6);

    // 标题行
    m_lblTitle = new QLabel(tr("推进器 --"), this);
    QFont f = m_lblTitle->font();
    f.setBold(true);
    f.setPointSize(12);
    m_lblTitle->setFont(f);
    m_lblTitle->setStyleSheet("color: #1a5276;");
    root->addWidget(m_lblTitle);

    auto *cols  = new QHBoxLayout;
    cols->setSpacing(8);
    auto *left  = new QVBoxLayout;
    auto *right = new QVBoxLayout;

    // 运行状态组
    {
        QList<QLabel *> out;
        auto *box = makeGroup(tr("运行状态"),
                              {tr("状态码"), tr("状态描述"), tr("错误码")},
                              out);
        m_lblStatus     = out[0];
        m_lblStatusText = out[1];
        m_lblErrorCode  = out[2];
        left->addWidget(box);
    }

    // 电气参数组
    {
        QList<QLabel *> out;
        auto *box = makeGroup(tr("电气参数"),
                              {tr("转速 (RPM)"), tr("角度 (°)"), tr("功率 (W)"),
                               tr("母线电压 (mV)"), tr("母线电流 (mA)"), tr("相电流 (mA)")},
                              out);
        m_lblRpm          = out[0];
        m_lblAngle        = out[1];
        m_lblPower        = out[2];
        m_lblBusVoltage   = out[3];
        m_lblBusCurrent   = out[4];
        m_lblPhaseCurrent = out[5];
        left->addWidget(box);
    }

    left->addStretch();

    // 温度组
    {
        QList<QLabel *> out;
        auto *box = makeGroup(tr("温度"),
                              {tr("电机温度 (°C)"), tr("MOS 温度 (°C)"), tr("环境温度 (°C)")},
                              out);
        m_lblMotorTemp = out[0];
        m_lblMosTemp   = out[1];
        m_lblTemp      = out[2];
        right->addWidget(box);
    }

    // 运行时间组
    {
        QList<QLabel *> out;
        auto *box = makeGroup(tr("运行时间"),
                              {tr("本次运行 (s)"), tr("累计运行 (s)")},
                              out);
        m_lblRunTime    = out[0];
        m_lblSumRunTime = out[1];
        right->addWidget(box);
    }

    right->addStretch();

    cols->addLayout(left);
    cols->addLayout(right);
    root->addLayout(cols);
}

void ThrusterDataPanel::setLabel(QLabel *lbl, const QString &text, bool highlight)
{
    lbl->setText(text);
    lbl->setObjectName(highlight ? "highlightLabel" : "valueLabel");
}

void ThrusterDataPanel::setData(const QVariantMap &m)
{
    const uint status = m.value("status").toUInt();

    // 运行状态为非正常时高亮
    const bool hasError = (status != 0);
    setLabel(m_lblStatus,     QString("0x%1").arg(status, 0, 16), hasError);
    setLabel(m_lblStatusText, m.value("statusText").toString(),   hasError);
    setLabel(m_lblErrorCode,  QString::number(m.value("errorCode").toUInt()),
             m.value("errorCode").toUInt() != 0);

    setLabel(m_lblRpm,          QString::number(m.value("rpm").toInt()),   true);
    setLabel(m_lblAngle,        QString::number(m.value("angle").toInt()));
    setLabel(m_lblPower,        QString::number(m.value("power").toInt()), true);
    setLabel(m_lblBusVoltage,   QString::number(m.value("busVoltage").toUInt()));
    setLabel(m_lblBusCurrent,   QString::number(m.value("busCurrent").toInt()));
    setLabel(m_lblPhaseCurrent, QString::number(m.value("phaseCurrent").toInt()));

    const int motorTemp = m.value("motorTemp").toInt();
    const int mosTemp   = m.value("mosTemp").toInt();
    setLabel(m_lblMotorTemp, QString::number(motorTemp), motorTemp > 80);
    setLabel(m_lblMosTemp,   QString::number(mosTemp),   mosTemp   > 80);
    setLabel(m_lblTemp,      QString::number(m.value("temp").toInt()));

    setLabel(m_lblRunTime,    QString::number(m.value("runTime").toUInt()));
    setLabel(m_lblSumRunTime, QString::number(m.value("sumRunTime").toUInt()));
}

void ThrusterDataPanel::clear()
{
    for (auto *lbl : {m_lblStatus, m_lblStatusText, m_lblErrorCode,
                      m_lblRpm, m_lblAngle, m_lblPower,
                      m_lblBusVoltage, m_lblBusCurrent, m_lblPhaseCurrent,
                      m_lblMotorTemp, m_lblMosTemp, m_lblTemp,
                      m_lblRunTime, m_lblSumRunTime}) {
        lbl->setText(QStringLiteral("--"));
        lbl->setObjectName("valueLabel");
    }
}
