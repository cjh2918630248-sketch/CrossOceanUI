#include "thrusterview.h"
#include "thrusterdatamodel.h"

#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QPushButton>
#include <QFont>
#include <QFrame>
#include <QSizePolicy>

#include "msg_thruster.pb.h"

// ─────────────────────────────────────────────────────────────────────────────
// 公共样式常量
// ─────────────────────────────────────────────────────────────────────────────

static const QString kStyleCard =
    "QGroupBox {"
    "  font-weight: bold; font-size: 12px;"
    "  border: 1px solid #d0d7de; border-radius: 8px;"
    "  margin-top: 16px; padding: 10px 8px 8px 8px;"
    "  background: #ffffff;"
    "}"
    "QGroupBox::title {"
    "  subcontrol-origin: margin;"
    "  left: 12px; top: 1px;"
    "  padding: 0 6px;"
    "  color: #0969da;"
    "}";

static const QString kStyleValue =
    "QLabel { font-family: 'Consolas','Courier New',monospace;"
    "  font-size: 12px; color: #1f2328; }";

static const QString kStyleValueMono =
    "QLabel { font-family: 'Consolas','Courier New',monospace;"
    "  font-size: 12px; color: #1f2328; background: #f6f8fa;"
    "  border: 1px solid #d0d7de; border-radius: 4px; padding: 1px 6px; }";

static const QString kStyleTempWarn =
    "QLabel { font-family: 'Consolas','Courier New',monospace;"
    "  font-size: 12px; font-weight: bold; color: #9a6700; background: #fff8c5;"
    "  border: 1px solid #d4a72c; border-radius: 4px; padding: 1px 6px; }";

static const QString kStyleTempDanger =
    "QLabel { font-family: 'Consolas','Courier New',monospace;"
    "  font-size: 12px; font-weight: bold; color: #d1242f; background: #fff0ee;"
    "  border: 1px solid #f0857b; border-radius: 4px; padding: 1px 6px; }";

static const QString kStyleBadgeNormal =
    "QLabel { font-weight: bold; font-size: 12px; color: #1a7f37;"
    "  background: #dafbe1; border: 1px solid #82cfaa;"
    "  border-radius: 10px; padding: 2px 10px; }";

static const QString kStyleBadgeFault =
    "QLabel { font-weight: bold; font-size: 12px; color: #d1242f;"
    "  background: #fff0ee; border: 1px solid #f0857b;"
    "  border-radius: 10px; padding: 2px 10px; }";

static const QString kStyleBadgeWarn =
    "QLabel { font-weight: bold; font-size: 12px; color: #9a6700;"
    "  background: #fff8c5; border: 1px solid #d4a72c;"
    "  border-radius: 10px; padding: 2px 10px; }";

static const QString kBarStyleRpm =
    "QProgressBar { background: #eef1f3; border: none; border-radius: 4px; height: 6px; }"
    "QProgressBar::chunk { background: #0969da; border-radius: 4px; }";

static const QString kBarStylePower =
    "QProgressBar { background: #eef1f3; border: none; border-radius: 4px; height: 6px; }"
    "QProgressBar::chunk { background: #8250df; border-radius: 4px; }";

// ─────────────────────────────────────────────────────────────────────────────
// 演示数据生成
// ─────────────────────────────────────────────────────────────────────────────
static QVariantMap sampleData(int seq)
{
    const uint32_t st = (seq % 5 == 0)
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
        { "motorTemp",    35 + seq * 3 },
        { "mosTemp",      40 + seq * 2 },
        { "temp",         28 + seq },
        { "runTime",      static_cast<uint>(1000u * seq) },
        { "sumRunTime",   static_cast<uint>(50000u + seq * 100) },
    };
}

// ─────────────────────────────────────────────────────────────────────────────
// ThrusterCardWidget 实现
// ─────────────────────────────────────────────────────────────────────────────

QLabel *ThrusterCardWidget::makeValueLabel(const QString &init, QWidget *parent)
{
    auto *lbl = new QLabel(init, parent);
    lbl->setStyleSheet(kStyleValueMono);
    lbl->setTextInteractionFlags(Qt::TextSelectableByMouse);
    return lbl;
}

QProgressBar *ThrusterCardWidget::makeBar(int max, QWidget *parent)
{
    auto *bar = new QProgressBar(parent);
    bar->setRange(0, max);
    bar->setValue(0);
    bar->setTextVisible(false);
    bar->setFixedHeight(6);
    return bar;
}

ThrusterCardWidget::ThrusterCardWidget(const QString &title, QWidget *parent)
    : QGroupBox(title, parent)
{
    setStyleSheet(kStyleCard);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    setMinimumWidth(320);

    auto *root = new QVBoxLayout(this);
    root->setSpacing(6);
    root->setContentsMargins(10, 6, 10, 10);

    // ── 状态徽章行 ──
    auto *statusRow = new QHBoxLayout;
    statusRow->setSpacing(8);

    m_lblStatusBadge = new QLabel("● Normal", this);
    m_lblStatusBadge->setStyleSheet(kStyleBadgeNormal);
    m_lblStatusBadge->setAlignment(Qt::AlignCenter);

    auto *errLbl = new QLabel("错误码:", this);
    errLbl->setStyleSheet(kStyleValue);
    m_lblErrorCode = makeValueLabel("0", this);

    statusRow->addWidget(m_lblStatusBadge);
    statusRow->addStretch();
    statusRow->addWidget(errLbl);
    statusRow->addWidget(m_lblErrorCode);
    root->addLayout(statusRow);

    // ── 分割线 ──
    auto makeHLine = [this]() {
        auto *line = new QFrame(this);
        line->setFrameShape(QFrame::HLine);
        line->setStyleSheet("color: #d0d7de;");
        return line;
    };

    root->addWidget(makeHLine());

    // ── 运动参数：RPM + 角度 ──
    auto *motionForm = new QFormLayout;
    motionForm->setHorizontalSpacing(12);
    motionForm->setVerticalSpacing(4);
    motionForm->setLabelAlignment(Qt::AlignRight);

    m_lblRpm   = makeValueLabel("--", this);
    m_lblAngle = makeValueLabel("--", this);
    motionForm->addRow("转速 (RPM):", m_lblRpm);
    motionForm->addRow("角度 (°):", m_lblAngle);

    m_barRpm = makeBar(3000, this);
    m_barRpm->setStyleSheet(kBarStyleRpm);
    motionForm->addRow("", m_barRpm);
    root->addLayout(motionForm);

    root->addWidget(makeHLine());

    // ── 功率 ──
    auto *powerForm = new QFormLayout;
    powerForm->setHorizontalSpacing(12);
    powerForm->setVerticalSpacing(4);
    powerForm->setLabelAlignment(Qt::AlignRight);

    m_lblPower = makeValueLabel("--", this);
    powerForm->addRow("功率 (W):", m_lblPower);

    m_barPower = makeBar(2000, this);
    m_barPower->setStyleSheet(kBarStylePower);
    powerForm->addRow("", m_barPower);
    root->addLayout(powerForm);

    root->addWidget(makeHLine());

    // ── 电气参数：总线电压 / 电流 / 相电流 ──
    auto *elecForm = new QFormLayout;
    elecForm->setHorizontalSpacing(12);
    elecForm->setVerticalSpacing(4);
    elecForm->setLabelAlignment(Qt::AlignRight);

    m_lblBusVoltage   = makeValueLabel("--", this);
    m_lblBusCurrent   = makeValueLabel("--", this);
    m_lblPhaseCurrent = makeValueLabel("--", this);
    elecForm->addRow("总线电压 (V):", m_lblBusVoltage);
    elecForm->addRow("总线电流 (A):", m_lblBusCurrent);
    elecForm->addRow("相电流 (A):",   m_lblPhaseCurrent);
    root->addLayout(elecForm);

    root->addWidget(makeHLine());

    // ── 温度 ──
    auto *tempForm = new QFormLayout;
    tempForm->setHorizontalSpacing(12);
    tempForm->setVerticalSpacing(4);
    tempForm->setLabelAlignment(Qt::AlignRight);

    m_lblMotorTemp = makeValueLabel("--", this);
    m_lblMosTemp   = makeValueLabel("--", this);
    m_lblTemp      = makeValueLabel("--", this);
    tempForm->addRow("电机温度 (°C):", m_lblMotorTemp);
    tempForm->addRow("MOS温度 (°C):",  m_lblMosTemp);
    tempForm->addRow("环境温度 (°C):", m_lblTemp);
    root->addLayout(tempForm);

    root->addWidget(makeHLine());

    // ── 运行时间 ──
    auto *timeForm = new QFormLayout;
    timeForm->setHorizontalSpacing(12);
    timeForm->setVerticalSpacing(4);
    timeForm->setLabelAlignment(Qt::AlignRight);

    m_lblRunTime    = makeValueLabel("--", this);
    m_lblSumRunTime = makeValueLabel("--", this);
    timeForm->addRow("本次运行 (s):", m_lblRunTime);
    timeForm->addRow("累计运行 (s):", m_lblSumRunTime);
    root->addLayout(timeForm);
}

void ThrusterCardWidget::applyTempColor(QLabel *lbl, int tempC)
{
    if (tempC >= 80)
        lbl->setStyleSheet(kStyleTempDanger);
    else if (tempC >= 60)
        lbl->setStyleSheet(kStyleTempWarn);
    else
        lbl->setStyleSheet(kStyleValueMono);
}

void ThrusterCardWidget::applyStatusBadge(QLabel *lbl,
                                           const QString &statusText,
                                           uint status)
{
    const bool isNormal  = (status == thruster::NORMAL);
    const bool isCharging = (status == static_cast<uint>(thruster::NORMAL | thruster::CHARGING));

    if (isNormal || isCharging) {
        lbl->setText(isCharging ? "⚡ Charging" : "● Normal");
        lbl->setStyleSheet(isCharging ? kStyleBadgeWarn : kStyleBadgeNormal);
    } else {
        lbl->setText("✕ " + statusText.split(',').first().trimmed());
        lbl->setStyleSheet(kStyleBadgeFault);
    }
}

void ThrusterCardWidget::updateData(const QVariantMap &data)
{
    if (data.isEmpty()) {
        reset();
        return;
    }

    const uint    status       = data["status"].toUInt();
    const QString statusText   = data["statusText"].toString();
    const int     rpm          = data["rpm"].toInt();
    const int     angle        = data["angle"].toInt();
    const int     power        = data["power"].toInt();
    const uint    busVoltage   = data["busVoltage"].toUInt();
    const int     busCurrent   = data["busCurrent"].toInt();
    const int     phaseCurrent = data["phaseCurrent"].toInt();
    const int     motorTemp    = data["motorTemp"].toInt();
    const int     mosTemp      = data["mosTemp"].toInt();
    const int     temp         = data["temp"].toInt();
    const uint    runTime      = data["runTime"].toUInt();
    const uint    sumRunTime   = data["sumRunTime"].toUInt();

    applyStatusBadge(m_lblStatusBadge, statusText, status);
    m_lblErrorCode->setText(QString::number(data["errorCode"].toUInt()));

    m_lblRpm->setText(QString::number(rpm));
    m_barRpm->setValue(qBound(0, rpm, 3000));

    m_lblAngle->setText(QString("%1°").arg(angle));

    m_lblPower->setText(QString("%1 W").arg(power));
    m_barPower->setValue(qBound(0, power, 2000));

    m_lblBusVoltage->setText(QString("%1 V").arg(busVoltage));
    m_lblBusCurrent->setText(QString("%1 A").arg(busCurrent));
    m_lblPhaseCurrent->setText(QString("%1 A").arg(phaseCurrent));

    m_lblMotorTemp->setText(QString("%1°C").arg(motorTemp));
    m_lblMosTemp->setText(QString("%1°C").arg(mosTemp));
    m_lblTemp->setText(QString("%1°C").arg(temp));
    applyTempColor(m_lblMotorTemp, motorTemp);
    applyTempColor(m_lblMosTemp,   mosTemp);
    applyTempColor(m_lblTemp,      temp);

    m_lblRunTime->setText(QString::number(runTime));
    m_lblSumRunTime->setText(QString::number(sumRunTime));
}

void ThrusterCardWidget::reset()
{
    m_lblStatusBadge->setText("● Normal");
    m_lblStatusBadge->setStyleSheet(kStyleBadgeNormal);
    m_lblErrorCode->setText("0");

    for (auto *lbl : { m_lblRpm, m_lblAngle, m_lblPower,
                       m_lblBusVoltage, m_lblBusCurrent, m_lblPhaseCurrent,
                       m_lblMotorTemp, m_lblMosTemp, m_lblTemp,
                       m_lblRunTime, m_lblSumRunTime }) {
        lbl->setText("--");
        lbl->setStyleSheet(kStyleValueMono);
    }
    m_barRpm->setValue(0);
    m_barPower->setValue(0);
}

// ─────────────────────────────────────────────────────────────────────────────
// ThrusterView 实现
// ─────────────────────────────────────────────────────────────────────────────

ThrusterView::ThrusterView(QWidget *parent)
    : QWidget(parent)
{
    m_model = new ThrusterDataModel(this);
    setupUI();

    connect(m_model, &ThrusterDataModel::dataUpdated,
            this, &ThrusterView::onModelDataUpdated);
    connect(m_model, &ThrusterDataModel::portChanged,
            this, &ThrusterView::updateHeaderInfo);
    connect(m_model, &ThrusterDataModel::thrusterNumChanged,
            this, &ThrusterView::updateHeaderInfo);
    connect(m_model, &ThrusterDataModel::errorOccurred,
            this, &ThrusterView::onModelError);
}

void ThrusterView::setSampleIndex(const int &sample_index)
{
    m_sampleIndex = sample_index;
}

void ThrusterView::setupUI()
{
    setWindowTitle("单推进器状态监控 — Thruster View");
    resize(500, 700);
    setMinimumSize(420, 520);

    auto *root = new QVBoxLayout(this);
    root->setContentsMargins(14, 14, 14, 10);
    root->setSpacing(10);

    // ── 标题栏 ──────────────────────────────────────────────────────────────
    auto *headerBox = new QFrame(this);
    headerBox->setStyleSheet(
        "QFrame { background: #0969da; border-radius: 8px; padding: 4px; }");
    auto *headerLayout = new QHBoxLayout(headerBox);
    headerLayout->setContentsMargins(16, 8, 16, 8);

    auto *titleLabel = new QLabel("单推进器状态监控", headerBox);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(14);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setStyleSheet("color: white;");

    auto *metaLayout = new QHBoxLayout;
    auto makeMetaLabel = [headerBox](const QString &text) {
        auto *lbl = new QLabel(text, headerBox);
        lbl->setStyleSheet(
            "QLabel { color: #cae0ff; font-size: 12px; font-weight: bold;"
            "  background: rgba(255,255,255,0.15); border-radius: 4px;"
            "  padding: 2px 10px; }");
        return lbl;
    };

    m_lblPort        = makeMetaLabel("Port: --");
    m_lblThrusterNum = makeMetaLabel("编号: --");

    metaLayout->setSpacing(8);
    metaLayout->addWidget(m_lblPort);
    metaLayout->addWidget(m_lblThrusterNum);

    headerLayout->addWidget(titleLabel);
    headerLayout->addStretch();
    headerLayout->addLayout(metaLayout);

    root->addWidget(headerBox);

    // ── 推进器数据卡片 ───────────────────────────────────────────────────────
    m_card = new ThrusterCardWidget("推进器数据", this);
    root->addWidget(m_card);
    root->addStretch(1);

    // ── 序列化区域 ───────────────────────────────────────────────────────────
    auto *serialBox = new QGroupBox("Protobuf 序列化 / 反序列化", this);
    serialBox->setStyleSheet(
        "QGroupBox { font-weight: bold; font-size: 12px;"
        "  border: 1px solid #d0d7de; border-radius: 6px;"
        "  margin-top: 14px; padding: 10px 8px 8px 8px;"
        "  background: #ffffff; }"
        "QGroupBox::title { subcontrol-origin: margin; left: 12px; top: 1px;"
        "  padding: 0 6px; color: #6e7781; }");
    auto *serialLayout = new QHBoxLayout(serialBox);
    serialLayout->setSpacing(10);

    m_txtHex = new QTextEdit(this);
    m_txtHex->setReadOnly(false);
    m_txtHex->setMaximumHeight(64);
    m_txtHex->setPlaceholderText(
        "序列化后的十六进制数据显示在此；也可粘贴 hex 后点击「反序列化」...");
    m_txtHex->setStyleSheet(
        "QTextEdit { font-family: 'Consolas','Courier New',monospace;"
        "  font-size: 11px; background: #f6f8fa; border: 1px solid #d0d7de;"
        "  border-radius: 4px; padding: 4px; }");

    m_lblByteInfo = new QLabel("字节数: 0", this);
    m_lblByteInfo->setStyleSheet(
        "QLabel { color: #6e7781; font-size: 12px; font-weight: bold;"
        "  min-width: 80px; }");
    m_lblByteInfo->setAlignment(Qt::AlignCenter);

    serialLayout->addWidget(m_txtHex, 1);
    serialLayout->addWidget(m_lblByteInfo);
    root->addWidget(serialBox);

    // ── 操作按钮栏 ───────────────────────────────────────────────────────────
    auto *btnLayout = new QHBoxLayout;
    btnLayout->setSpacing(8);

    auto makeBtn = [this](const QString &text, const QString &bg, const QString &hover) {
        auto *btn = new QPushButton(text, this);
        btn->setMinimumHeight(34);
        btn->setCursor(Qt::PointingHandCursor);
        btn->setStyleSheet(
            QString("QPushButton { background: %1; color: white; border: none;"
                    "  border-radius: 6px; font-size: 12px; font-weight: bold;"
                    "  padding: 4px 16px; }"
                    "QPushButton:hover { background: %2; }"
                    "QPushButton:pressed { background: %3; }")
                .arg(bg, hover, QColor(bg).darker(115).name()));
        return btn;
    };

    auto *btnLoad    = makeBtn("加载示例数据", "#0969da", "#0860ca");
    auto *btnSer     = makeBtn("序列化",       "#8250df", "#7b47d4");
    auto *btnDeser   = makeBtn("反序列化",     "#1a7f37", "#187034");
    auto *btnClear   = makeBtn("清空",         "#6e7781", "#636b75");

    btnLayout->addWidget(btnLoad);
    btnLayout->addWidget(btnSer);
    btnLayout->addWidget(btnDeser);
    btnLayout->addStretch();
    btnLayout->addWidget(btnClear);
    root->addLayout(btnLayout);

    // ── 状态栏 ───────────────────────────────────────────────────────────────
    m_statusBar = new QLabel("就绪  |  port=--  num=--", this);
    m_statusBar->setStyleSheet(
        "QLabel { color: #6e7781; font-size: 11px; padding: 2px 4px; }");
    root->addWidget(m_statusBar);

    connect(btnLoad,   &QPushButton::clicked, this, &ThrusterView::onLoadSampleData);
    connect(btnSer,    &QPushButton::clicked, this, &ThrusterView::onSerialize);
    connect(btnDeser,  &QPushButton::clicked, this, &ThrusterView::onDeserialize);
    connect(btnClear,  &QPushButton::clicked, this, &ThrusterView::onClear);
}

// ── 刷新卡片 ─────────────────────────────────────────────────────────────────

void ThrusterView::refreshCard()
{
    m_card->updateData(m_model->toMap());
    updateHeaderInfo();
}

void ThrusterView::updateHeaderInfo()
{
    m_lblPort->setText(QString("Port: %1").arg(m_model->port()));
    m_lblThrusterNum->setText(QString("编号: %1").arg(m_model->thrusterNum()));
    m_statusBar->setText(
        QString("就绪  |  port=%1  thrusterNum=%2")
            .arg(m_model->port())
            .arg(m_model->thrusterNum()));
}

// ── 槽 ───────────────────────────────────────────────────────────────────────

void ThrusterView::onModelDataUpdated()
{
    refreshCard();
}

void ThrusterView::onModelError(int /*type*/, const QString &msg)
{
    m_statusBar->setStyleSheet(
        "QLabel { color: #d1242f; font-size: 11px; font-weight: bold; padding: 2px 4px; }");
    m_statusBar->setText(QString("错误: %1").arg(msg));
}

void ThrusterView::onLoadSampleData()
{
    m_model->setPort(9100u + static_cast<uint>(m_sampleIndex % 100));
    m_model->setThrusterNum(static_cast<uint>(m_sampleIndex));

    m_model->fromMap(sampleData(m_sampleIndex++));
    m_model->printData();

    m_statusBar->setStyleSheet(
        "QLabel { color: #6e7781; font-size: 11px; padding: 2px 4px; }");
    m_txtHex->clear();
    m_lblByteInfo->setText("字节数: 0");
}

void ThrusterView::onSerialize()
{
    const QByteArray bytes = m_model->serialize();
    if (bytes.isEmpty()) {
        m_statusBar->setText("序列化失败（数据为空？）");
        return;
    }

    QString hex;
    for (int i = 0; i < bytes.size(); ++i) {
        if (i > 0 && i % 16 == 0) hex += '\n';
        else if (i > 0)           hex += ' ';
        hex += QString("%1").arg(
            static_cast<uint8_t>(bytes[i]), 2, 16, QChar('0')).toUpper();
    }
    m_txtHex->setPlainText(hex);
    m_lblByteInfo->setText(QString("字节数:\n%1").arg(bytes.size()));
    m_statusBar->setText(
        QString("序列化完成，共 %1 字节").arg(bytes.size()));
}

void ThrusterView::onDeserialize()
{
    const QString raw = m_txtHex->toPlainText().simplified().remove(' ').remove('\n');
    if (raw.isEmpty()) {
        m_statusBar->setText("hex 区域为空，请先序列化或粘贴数据");
        return;
    }
    const QByteArray bytes = QByteArray::fromHex(raw.toLatin1());
    if (m_model->deserialize(bytes)) {
        m_statusBar->setStyleSheet(
            "QLabel { color: #6e7781; font-size: 11px; padding: 2px 4px; }");
        m_statusBar->setText("反序列化成功");
    }
}

void ThrusterView::onClear()
{
    //m_sampleIndex = 0; //点击清空之后索引从0开始
    this->setSampleIndex(0);
    m_model->fromMap(QVariantMap());
    m_model->setPort(0);
    m_model->setThrusterNum(0);
    m_card->reset();
    m_txtHex->clear();
    m_lblByteInfo->setText("字节数: 0");
    m_statusBar->setStyleSheet(
        "QLabel { color: #6e7781; font-size: 11px; padding: 2px 4px; }");
    m_statusBar->setText("已清空  |  port=0  num=0");
    updateHeaderInfo();
}
