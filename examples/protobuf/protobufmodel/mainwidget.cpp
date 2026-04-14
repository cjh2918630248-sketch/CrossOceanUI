#include "mainwidget.h"
#include "ui_mainwidget.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QGroupBox>
#include <QSplitter>
#include <QMessageBox>
#include <QDateTime>
#include <QFont>

static const QString kStyleCard =
    "QGroupBox {"
    "  font-weight: bold; font-size: 13px;"
    "  border: 1px solid #c0c0c0; border-radius: 6px;"
    "  margin-top: 14px; padding: 12px 8px 8px 8px;"
    "  background: #fafbfc;"
    "}"
    "QGroupBox::title {"
    "  subcontrol-origin: margin;"
    "  left: 12px; top: 2px;"
    "  padding: 0 6px;"
    "  color: #2c6fbb;"
    "}";

static const QString kStyleValue =
    "QLabel { font-family: 'Consolas','Courier New',monospace;"
    "  font-size: 13px; color: #1a1a2e; }";

static const QString kStyleHighlight =
    "QLabel { font-family: 'Consolas','Courier New',monospace;"
    "  font-size: 13px; font-weight: bold; color: #0e6655; }";

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWidget)
{
    ui->setupUi(this);
    setWindowTitle(tr("GINS 导航数据可视化 - Protobuf 示例"));
    resize(860, 640);

    setupDataPanel();
}

MainWidget::~MainWidget()
{
    delete ui;
}

QGroupBox *MainWidget::createGroupBox(const QString &title,
                                       const QStringList &labels,
                                       QList<QLabel *> &outputs)
{
    auto *box = new QGroupBox(title, this);
    box->setStyleSheet(kStyleCard);

    auto *form = new QFormLayout(box);
    form->setLabelAlignment(Qt::AlignRight);
    form->setHorizontalSpacing(16);
    form->setVerticalSpacing(6);

    for (const QString &name : labels) {
        auto *val = new QLabel(QStringLiteral("--"), this);
        val->setStyleSheet(kStyleValue);
        val->setTextInteractionFlags(Qt::TextSelectableByMouse);
        form->addRow(name + "：", val);
        outputs.append(val);
    }
    return box;
}

void MainWidget::setupDataPanel()
{
    auto *root = new QVBoxLayout(this);
    root->setContentsMargins(12, 12, 12, 12);
    root->setSpacing(8);

    // ---- 标题栏 ----
    auto *titleLabel = new QLabel(tr("GINS 惯性导航数据面板"), this);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(16);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("color: #1a5276; padding: 4px;");
    root->addWidget(titleLabel);

    // ---- 数据区：左右两栏 ----
    auto *dataRow = new QHBoxLayout;
    dataRow->setSpacing(10);

    // 左栏
    auto *leftCol = new QVBoxLayout;

    {
        QList<QLabel *> out;
        QStringList names = {tr("端口号"), tr("UTC时间"), tr("纬度 (°)"),
                             tr("经度 (°)"), tr("高度 (m)")};
        auto *box = createGroupBox(tr("📍 位置信息"), names, out);
        m_lblPort      = out[0];
        m_lblUtcLocal  = out[1];
        m_lblLatitude  = out[2];
        m_lblLongitude = out[3];
        m_lblAltitude  = out[4];
        leftCol->addWidget(box);
    }

    {
        QList<QLabel *> out;
        QStringList names = {tr("横滚 (°)"), tr("俯仰 (°)"), tr("航向 (°)")};
        auto *box = createGroupBox(tr("🧭 姿态信息"), names, out);
        m_lblRoll    = out[0];
        m_lblPitch   = out[1];
        m_lblHeading = out[2];
        leftCol->addWidget(box);
    }

    leftCol->addStretch();
    dataRow->addLayout(leftCol);

    // 右栏
    auto *rightCol = new QVBoxLayout;

    {
        QList<QLabel *> out;
        QStringList names = {tr("北向速度 (m/s)"), tr("东向速度 (m/s)"),
                             tr("转向速率"), tr("Ax"), tr("Ay"), tr("Aaz")};
        auto *box = createGroupBox(tr("⚡ 速度与加速度"), names, out);
        m_lblVn   = out[0];
        m_lblVe   = out[1];
        m_lblRoti = out[2];
        m_lblAx   = out[3];
        m_lblAy   = out[4];
        m_lblAaz  = out[5];
        rightCol->addWidget(box);
    }

    {
        QList<QLabel *> out;
        QStringList names = {tr("GINS 状态"), tr("HDOP"), tr("航向标准差")};
        auto *box = createGroupBox(tr("📊 状态信息"), names, out);
        m_lblGinsStatus = out[0];
        m_lblHdop       = out[1];
        m_lblHeadStd    = out[2];
        rightCol->addWidget(box);
    }

    rightCol->addStretch();
    dataRow->addLayout(rightCol);

    root->addLayout(dataRow, 1);

    // ---- 序列化区域 ----
    auto *serialBox = new QGroupBox(tr("🔧 Protobuf 序列化 / 反序列化"), this);
    serialBox->setStyleSheet(kStyleCard);
    auto *serialLayout = new QVBoxLayout(serialBox);

    auto *hexRow = new QHBoxLayout;
    m_txtSerializedHex = new QTextEdit(this);
    m_txtSerializedHex->setReadOnly(true);
    m_txtSerializedHex->setMaximumHeight(80);
    m_txtSerializedHex->setStyleSheet(
        "QTextEdit { font-family: 'Consolas','Courier New',monospace;"
        "  font-size: 12px; background: #f0f3f5; border: 1px solid #d5d8dc;"
        "  border-radius: 4px; padding: 4px; }");
    m_txtSerializedHex->setPlaceholderText(tr("序列化后的十六进制数据将显示在此..."));
    hexRow->addWidget(m_txtSerializedHex);

    auto *infoCol = new QVBoxLayout;
    m_lblByteCount = new QLabel(tr("字节数: 0"), this);
    m_lblByteCount->setStyleSheet("font-weight: bold; font-size: 12px; color: #555;");
    infoCol->addWidget(m_lblByteCount);

    m_barStatus = new QProgressBar(this);
    m_barStatus->setRange(0, 100);
    m_barStatus->setValue(0);
    m_barStatus->setTextVisible(false);
    m_barStatus->setMaximumHeight(8);
    m_barStatus->setStyleSheet(
        "QProgressBar { background: #e8e8e8; border-radius: 4px; }"
        "QProgressBar::chunk { background: #27ae60; border-radius: 4px; }");
    infoCol->addWidget(m_barStatus);
    infoCol->addStretch();
    hexRow->addLayout(infoCol);

    serialLayout->addLayout(hexRow);
    root->addWidget(serialBox);

    // ---- 按钮栏 ----
    auto *btnRow = new QHBoxLayout;
    btnRow->setSpacing(10);

    auto makeBtn = [this](const QString &text, const QString &color) {
        auto *btn = new QPushButton(text, this);
        btn->setMinimumHeight(36);
        btn->setCursor(Qt::PointingHandCursor);
        btn->setStyleSheet(
            QString("QPushButton { background: %1; color: white; border: none;"
                    "  border-radius: 6px; font-size: 13px; font-weight: bold;"
                    "  padding: 6px 20px; }"
                    "QPushButton:hover { background: %2; }"
                    "QPushButton:pressed { background: %3; }")
                .arg(color,
                     QColor(color).lighter(115).name(),
                     QColor(color).darker(115).name()));
        return btn;
    };

    auto *btnLoad        = makeBtn(tr("加载示例数据"), "#2980b9");
    auto *btnSerialize   = makeBtn(tr("序列化"),      "#27ae60");
    auto *btnDeserialize = makeBtn(tr("反序列化"),     "#8e44ad");
    auto *btnClear       = makeBtn(tr("清空"),         "#95a5a6");

    btnRow->addWidget(btnLoad);
    btnRow->addWidget(btnSerialize);
    btnRow->addWidget(btnDeserialize);
    btnRow->addStretch();
    btnRow->addWidget(btnClear);

    root->addLayout(btnRow);

    connect(btnLoad,        &QPushButton::clicked, this, &MainWidget::onLoadSampleData);
    connect(btnSerialize,   &QPushButton::clicked, this, &MainWidget::onSerialize);
    connect(btnDeserialize, &QPushButton::clicked, this, &MainWidget::onDeserialize);
    connect(btnClear,       &QPushButton::clicked, this, &MainWidget::onClearAll);
}

void MainWidget::setLabelValue(QLabel *label, const QString &text, bool highlight)
{
    label->setText(text);
    label->setStyleSheet(highlight ? kStyleHighlight : kStyleValue);
}

void MainWidget::updateDisplay(const GinsModel &model)
{
    setLabelValue(m_lblPort,      QString::number(model.port()));
    setLabelValue(m_lblUtcLocal,
                  QDateTime::fromSecsSinceEpoch(
                      static_cast<qint64>(model.utcLocal()), Qt::UTC)
                      .toString("yyyy-MM-dd HH:mm:ss UTC"));
    setLabelValue(m_lblLatitude,  QString::number(model.latitude(),  'f', 6), true);
    setLabelValue(m_lblLongitude, QString::number(model.longitude(), 'f', 6), true);
    setLabelValue(m_lblAltitude,  QString::number(model.altitude(),  'f', 2));

    setLabelValue(m_lblRoll,    QString::number(model.rollDeg(),    'f', 2));
    setLabelValue(m_lblPitch,   QString::number(model.pitchDeg(),   'f', 2));
    setLabelValue(m_lblHeading, QString::number(model.headingDeg(), 'f', 2), true);

    setLabelValue(m_lblVn,   QString::number(model.vnMps(), 'f', 3));
    setLabelValue(m_lblVe,   QString::number(model.veMps(), 'f', 3));
    setLabelValue(m_lblRoti, QString::number(model.roti(),  'f', 4));
    setLabelValue(m_lblAx,   QString::number(model.ax(),    'f', 4));
    setLabelValue(m_lblAy,   QString::number(model.ay(),    'f', 4));
    setLabelValue(m_lblAaz,  QString::number(model.aaz(),   'f', 4));

    auto statusText = [](uint32_t s) -> QString {
        switch (s) {
        case 0: return QStringLiteral("未初始化");
        case 1: return QStringLiteral("单点定位");
        case 2: return QStringLiteral("差分定位");
        case 4: return QStringLiteral("组合导航");
        default: return QString("未知 (%1)").arg(s);
        }
    };
    setLabelValue(m_lblGinsStatus, statusText(model.ginsStatus()), true);
    setLabelValue(m_lblHdop,       QString::number(model.hdop(), 'f', 2));
    setLabelValue(m_lblHeadStd,    QString::number(model.headStd(), 'f', 3));

    m_barStatus->setValue(50);
}

// ---- 槽实现 ----

void MainWidget::onLoadSampleData()
{
    m_currentModel = GinsModel(
        /*port*/        1,
        /*utc_local*/   1711900800.0,
        /*latitude*/    31.230416,
        /*longitude*/   121.473701,
        /*altitude*/    10.5f,
        /*roll_deg10*/  -12,
        /*pitch_deg10*/ 5,
        /*heading_deg*/ 270.0f,
        /*vn_mps100*/   350,
        /*ve_mps100*/   -120,
        /*roti*/        0.01f,
        /*ax*/          0.05f,
        /*ay*/          -0.03f,
        /*aaz*/         0.001f,
        /*gins_status*/ 4,
        /*hdop10*/      12,
        /*head_std*/    0.5f);

    updateDisplay(m_currentModel);

    m_serializedBytes.clear();
    m_txtSerializedHex->clear();
    m_lblByteCount->setText(tr("字节数: 0"));
    m_barStatus->setValue(25);
}

void MainWidget::onSerialize()
{
    try {
        m_serializedBytes = m_currentModel.serialize();
    } catch (const std::exception &e) {
        QMessageBox::warning(this, tr("序列化失败"), QString::fromUtf8(e.what()));
        return;
    }

    QByteArray raw(m_serializedBytes.data(),
                   static_cast<int>(m_serializedBytes.size()));
    QString hex;
    for (int i = 0; i < raw.size(); ++i) {
        if (i > 0 && i % 16 == 0) hex += '\n';
        else if (i > 0)           hex += ' ';
        hex += QString("%1").arg(static_cast<uint8_t>(raw[i]), 2, 16, QChar('0')).toUpper();
    }
    m_txtSerializedHex->setPlainText(hex);
    m_lblByteCount->setText(tr("字节数: %1").arg(raw.size()));
    m_barStatus->setValue(75);
}

void MainWidget::onDeserialize()
{
    if (m_serializedBytes.empty()) {
        QMessageBox::information(this, tr("提示"), tr("请先加载数据并序列化。"));
        return;
    }

    try {
        GinsModel restored = GinsModel::fromBytes(m_serializedBytes);
        updateDisplay(restored);
        m_currentModel = restored;
    } catch (const std::exception &e) {
        QMessageBox::warning(this, tr("反序列化失败"), QString::fromUtf8(e.what()));
        return;
    }
    m_barStatus->setValue(100);
}

void MainWidget::onClearAll()
{
    m_currentModel = GinsModel();
    m_serializedBytes.clear();

    auto resetLabel = [this](QLabel *lbl) {
        lbl->setText(QStringLiteral("--"));
        lbl->setStyleSheet(kStyleValue);
    };

    for (auto *lbl : {m_lblPort, m_lblUtcLocal, m_lblLatitude, m_lblLongitude,
                      m_lblAltitude, m_lblRoll, m_lblPitch, m_lblHeading,
                      m_lblVn, m_lblVe, m_lblRoti, m_lblAx, m_lblAy, m_lblAaz,
                      m_lblGinsStatus, m_lblHdop, m_lblHeadStd}) {
        resetLabel(lbl);
    }

    m_txtSerializedHex->clear();
    m_lblByteCount->setText(tr("字节数: 0"));
    m_barStatus->setValue(0);
}
