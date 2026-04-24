#include "right/rightwidget.h"

namespace {
constexpr int kRightWidgetWidth  = 740;
constexpr int kRightWidgetHeight = 1390;

// 模块统一宽度（前两个稍高，后三个同宽且矮一些）
// 校验：410 + 472 + 3×140 + 4×22 = 1390
constexpr int kModuleWidth   = 740;
constexpr int kModule1Height = 410;
constexpr int kModule2Height = 472;

constexpr int kActionRowHeight  = 140;
constexpr int kModuleSpacing    = 22;  // 五个模块两两之间的间隔
} // namespace

QString RightWidget::formatDecimalDegreesAsDms(double decimalDegrees)
{
    const double a = std::fabs(decimalDegrees);
    const int deg = static_cast<int>(std::floor(a)); //度=整数部分向下取整
    const double minutesFloat = (a - static_cast<double>(deg)) * 60.0; //分=小数部分*60
    const int min = static_cast<int>(std::floor(minutesFloat));
    const double sec = (minutesFloat - static_cast<double>(min)) * 60.0; //秒=分的小数部分*60
    return QStringLiteral("%1°%2'%3\"").arg(deg).arg(min).arg(sec, 0, 'f', 2);
}

RightWidget::RightWidget(QWidget *parent)
    : QWidget(parent)
{
    setObjectName(QStringLiteral("rightWidget"));
    setFixedSize(kRightWidgetWidth, kRightWidgetHeight);

    m_speedInfo.speedMps = 3.2;
    m_speedInfo.leftRpm  = 1586;
    m_speedInfo.rightRpm = 1586;
    m_rudderInfo.leftDeg   = 10.5;
    m_rudderInfo.centerDeg = 180.5;
    m_rudderInfo.rightDeg  = 3.2;

    setupUi();
    setupConnections();
    refresh();
}

QFrame *RightWidget::createModulePanel(const QSize &size, const QString &objectName)
{
    auto *panel = new QFrame(this);
    panel->setObjectName(objectName);
    panel->setFixedSize(size);
    return panel;
}

QPushButton *RightWidget::createActionButton(const QString &text, QWidget *parent)
{
    auto *button = new QPushButton(text, parent);
    button->setCursor(Qt::PointingHandCursor);
    button->setFlat(true);
    button->setObjectName(QStringLiteral("rightActionButton"));
    button->setMinimumHeight(96);
    return button;
}

void RightWidget::setupUi()
{
    auto *root = new QVBoxLayout(this);
    root->setContentsMargins(0, 0, 0, 0);
    root->setSpacing(kModuleSpacing);
    // ------ 模块 1：位置 + 航速 ------
    auto* module1 = Init_GeoUi(); //左边部分初始化
    root->addWidget(module1, 0, Qt::AlignLeft);
    // ------ 模块 2：RPM + 舵角 ------
    auto *module2 = Init_RpmUi();
    root->addWidget(module2, 0, Qt::AlignLeft);
    // ------ 模块 3/4/5：三条动作按钮条（独立模块，彼此之间也留间隔） ------
    auto makeActionRow = [&](std::initializer_list<QPushButton **> buttonSlots,
                             std::initializer_list<QString> labels) {
        auto *row = createModulePanel(QSize(kModuleWidth, kActionRowHeight),
                                      QStringLiteral("rightModulePanel"));
        auto *rowLayout = new QHBoxLayout(row);
        rowLayout->setContentsMargins(28, 18, 28, 18);
        rowLayout->setSpacing(22);
        auto slotIt = buttonSlots.begin();
        auto textIt = labels.begin();
        for (; slotIt != buttonSlots.end() && textIt != labels.end(); ++slotIt, ++textIt) {
            auto *btn = createActionButton(*textIt, row);
            *(*slotIt) = btn;
            rowLayout->addWidget(btn, 1);
        }
        return row;
    };

    root->addWidget(makeActionRow(
        {&m_modeSwitchButton, &m_lockButton, &m_previewButton, &m_photoButton},
        {QStringLiteral("S"), QStringLiteral("🔒"), QStringLiteral("👁"), QStringLiteral("📷")}),
        0, Qt::AlignLeft);

    root->addWidget(makeActionRow(
        {&m_navArrowButton, &m_autoDriveButton, &m_targetButton, &m_alarmButton},
        {QStringLiteral("▲"), QStringLiteral("A"), QStringLiteral("◎"), QStringLiteral("!")}),
        0, Qt::AlignLeft);

    root->addWidget(makeActionRow(
        {&m_gamepadButton, &m_uploadButton, &m_downloadButton, &m_micButton},
        {QStringLiteral("🎮"), QStringLiteral("⇪"), QStringLiteral("⇩"), QStringLiteral("🎤")}),
        0, Qt::AlignLeft);

}

QFrame *RightWidget::Init_GeoUi()
{
    auto *module1 = createModulePanel(QSize(kModuleWidth, kModule1Height),
                                      QStringLiteral("rightModulePanel"));
    auto *m1Layout = new QHBoxLayout(module1);
    m1Layout->setContentsMargins(48, 38, 48, 38);
    m1Layout->setSpacing(24);

    auto *geoBox = new QVBoxLayout();
    geoBox->setContentsMargins(0, 0, 0, 0);
    geoBox->setSpacing(18);

    auto *latTitle = new QLabel(QStringLiteral("LATITUDE"), module1);
    latTitle->setObjectName(QStringLiteral("geoCoordTitleLabel"));
    latTitle->setAlignment(Qt::AlignCenter);
    latTitle->setFixedSize(178, 27);

    m_latValueLabel = new QLabel(module1);
    m_latValueLabel->setObjectName(QStringLiteral("geoCoordValueLabel"));
    m_latValueLabel->setAlignment(Qt::AlignCenter);
    m_latValueLabel->setFixedSize(306, 63);

    auto *lonTitle = new QLabel(QStringLiteral("LONGITUDE"), module1);
    lonTitle->setObjectName(QStringLiteral("geoCoordTitleLabel"));
    lonTitle->setAlignment(Qt::AlignCenter);
    lonTitle->setFixedSize(178, 27);

    m_lonValueLabel = new QLabel(module1);
    m_lonValueLabel->setObjectName(QStringLiteral("geoCoordValueLabel"));
    m_lonValueLabel->setAlignment(Qt::AlignCenter);
    m_lonValueLabel->setFixedSize(306, 63);

    geoBox->addStretch(1);
    geoBox->addWidget(latTitle, 0, Qt::AlignHCenter);
    geoBox->addWidget(m_latValueLabel, 0, Qt::AlignHCenter);
    geoBox->addSpacing(12);
    geoBox->addWidget(lonTitle, 0, Qt::AlignHCenter);
    geoBox->addWidget(m_lonValueLabel, 0, Qt::AlignHCenter);
    geoBox->addStretch(1);
    m1Layout->addLayout(geoBox, 1);

    m_speedGauge = new SpeedGaugeWidget(module1);
    m1Layout->addWidget(m_speedGauge, 0, Qt::AlignVCenter | Qt::AlignRight);
    return module1;
}

QFrame *RightWidget::Init_RpmUi()
{
    auto *module2 = createModulePanel(QSize(kModuleWidth, kModule2Height),
                                      QStringLiteral("rightModulePanel"));
    auto *m2Layout = new QVBoxLayout(module2);
    m2Layout->setContentsMargins(48, 34, 48, 34);
    m2Layout->setSpacing(18);

    // 推进器区按设计稿约束为 584×256；画布位置 (2508, 2073) 与模块 2 左/上内边距 48、34 对齐
    m_thrusterRpm = new RightThrusterRpmPairWidget(module2);
    m2Layout->addWidget(m_thrusterRpm, 0, Qt::AlignLeft | Qt::AlignTop);

    // 仪表盘：尺寸 740×370，圆心在控件底部中央，上半圆可见
    // 放置于模块 2 底部（y = kModule2Height - 370 = 102），跨越完整宽度
    // lower() 确保仪表盘在推进器控件层叠顺序之下
    m_rudderGauge = new RudderGaugeWidget(module2);
    m_rudderGauge->move(0, kModule2Height - 370);
    m_rudderGauge->lower();
    return module2;
}

void RightWidget::setupConnections()
{
    connect(m_modeSwitchButton, &QPushButton::clicked, this, [this]{ qDebug() << "RightWidget: modeSwitch"; emit modeSwitchButtonClicked(); });
    connect(m_lockButton,       &QPushButton::clicked, this, [this]{ qDebug() << "RightWidget: lock";       emit lockButtonClicked(); });
    connect(m_previewButton,    &QPushButton::clicked, this, [this]{ qDebug() << "RightWidget: preview";    emit previewButtonClicked(); });
    connect(m_photoButton,      &QPushButton::clicked, this, [this]{ qDebug() << "RightWidget: photo";      emit photoButtonClicked(); });

    connect(m_navArrowButton,   &QPushButton::clicked, this, [this]{ qDebug() << "RightWidget: navArrow";   emit navArrowButtonClicked(); });
    connect(m_autoDriveButton,  &QPushButton::clicked, this, [this]{ qDebug() << "RightWidget: autoDrive";  emit autoDriveButtonClicked(); });
    connect(m_targetButton,     &QPushButton::clicked, this, [this]{ qDebug() << "RightWidget: target";     emit targetButtonClicked(); });
    connect(m_alarmButton,      &QPushButton::clicked, this, [this]{ qDebug() << "RightWidget: alarm";      emit alarmButtonClicked(); });

    connect(m_gamepadButton,    &QPushButton::clicked, this, [this]{ qDebug() << "RightWidget: gamepad";    emit gamepadButtonClicked(); });
    connect(m_uploadButton,     &QPushButton::clicked, this, [this]{ qDebug() << "RightWidget: upload";     emit uploadButtonClicked(); });
    connect(m_downloadButton,   &QPushButton::clicked, this, [this]{ qDebug() << "RightWidget: download";   emit downloadButtonClicked(); });
    connect(m_micButton,        &QPushButton::clicked, this, [this]{ qDebug() << "RightWidget: mic";        emit micButtonClicked(); });
}

void RightWidget::setGeoInfo(const GeoInfo &info)
{
    m_geoInfo = info;
    refresh();
}

void RightWidget::setSpeedInfo(const SpeedInfo &info)
{
    m_speedInfo = info;
    refresh();
}

void RightWidget::setMaxSpeedMps(double maxSpeedMps)
{
    m_maxSpeedMps = maxSpeedMps;
    refresh();
}

void RightWidget::setRudderInfo(const RudderInfo &info)
{
    m_rudderInfo = info;
    refresh();
}

void RightWidget::setNavInfo(const NavInfo &info)
{
    m_navInfo = info;
    if (m_rudderGauge) {
        m_rudderGauge->setRoll(info.roll);
        m_rudderGauge->setPitch(info.pitch);
        m_rudderGauge->setHeading(info.heading);
    }
}

void RightWidget::refresh()
{
    if (m_latValueLabel) {
        m_latValueLabel->setText(formatDecimalDegreesAsDms(m_geoInfo.latitudeDegrees));
    }
    if (m_lonValueLabel) {
        m_lonValueLabel->setText(formatDecimalDegreesAsDms(m_geoInfo.longitudeDegrees));
    }
    if (m_speedGauge) {
        m_speedGauge->setMaxSpeedMps(m_maxSpeedMps);
        m_speedGauge->setSpeedMps(m_speedInfo.speedMps);
    }
    if (m_thrusterRpm) {
        m_thrusterRpm->setLeftRpm(m_speedInfo.leftRpm);
        m_thrusterRpm->setRightRpm(m_speedInfo.rightRpm);
    }
}
