#include "shipuiwidget.h"

namespace {
constexpr int kCanvasWidth  = 2560;
constexpr int kCanvasHeight = 1600;

// 总体布局：LeftWidget | [ TopWidget 上 / (MidWidget + RightWidget) 下 ]
// 要求 LeftWidget 顶边对齐 TopWidget 顶边；LeftWidget / MidWidget / RightWidget 底边对齐。
//   - TopWidget: y=75,   h=106   -> [75,  181]
//   - MidWidget: y=194,  h=1390  -> [194, 1584]
//   - RightWidget: y=194, h=1390 -> [194, 1584]
//   - LeftWidget: y=75,  h=1509  -> [75,  1584]
constexpr int kLeftWidgetX  = 28;
constexpr int kLeftWidgetY  = 75;
constexpr int kTopWidgetX   = 288; //285
constexpr int kTopWidgetY   = 75;
constexpr int kMidWidgetX   = 288; //285
constexpr int kMidWidgetY   = 194;
constexpr int kRightX       = 1760; //算出结果应该是1736
constexpr int kRightY       = 194;
} // namespace

ShipUiWidget::ShipUiWidget(QWidget *parent)
    : QWidget(parent)
{
    setObjectName(QStringLiteral("shipUiRoot")); //设置对象名，qss中使用该名称
    setFixedSize(kCanvasWidth, kCanvasHeight); //固定尺寸
    buildUi();
    setupConnections();
    applyTheme();
}

void ShipUiWidget::buildUi()
{
    m_leftWidget  = new LeftWidget(this);
    m_topWidget   = new TopWidget(this);
    m_midWidget   = new MidWidget(this);
    m_rightWidget = new RightWidget(this);
    //移动到指定坐标位置
    m_leftWidget->move(kLeftWidgetX,  kLeftWidgetY);
    m_topWidget->move(kTopWidgetX,    kTopWidgetY);
    m_midWidget->move(kMidWidgetX,    kMidWidgetY);
    m_rightWidget->move(kRightX,      kRightY);
}

void ShipUiWidget::setupConnections()
{
    // 左侧导航
    connect(m_leftWidget, &LeftWidget::homeButtonClicked,    this, &ShipUiWidget::homeButtonClicked);
    connect(m_leftWidget, &LeftWidget::mapButtonClicked,     this, &ShipUiWidget::mapButtonClicked);
    connect(m_leftWidget, &LeftWidget::shipButtonClicked,    this, &ShipUiWidget::shipButtonClicked);
    connect(m_leftWidget, &LeftWidget::chartButtonClicked,   this, &ShipUiWidget::chartButtonClicked);
    connect(m_leftWidget, &LeftWidget::cameraButtonClicked,  this, &ShipUiWidget::cameraButtonClicked);
    connect(m_leftWidget, &LeftWidget::settingButtonClicked, this, &ShipUiWidget::settingButtonClicked);

    // 顶栏 — 左半部
    if (auto *vs = m_topWidget->vesselStatus()) {
        connect(vs, &VesselStatus::rcButtonClicked,         this, &ShipUiWidget::rcButtonClicked);
        connect(vs, &VesselStatus::mcButtonClicked,         this, &ShipUiWidget::mcButtonClicked);
        connect(vs, &VesselStatus::autoButtonClicked,       this, &ShipUiWidget::autoButtonClicked);
        connect(vs, &VesselStatus::stopButtonClicked,       this, &ShipUiWidget::stopButtonClicked);
        connect(vs, &VesselStatus::controllerButtonClicked, this, &ShipUiWidget::controllerButtonClicked);
    }

    // 顶栏 — 右半部
    if (auto *ct = m_topWidget->clockToolbar()) {
        connect(ct, &ClockToolbar::nightThemeButtonClicked, this, &ShipUiWidget::nightThemeButtonClicked);
        connect(ct, &ClockToolbar::dayThemeButtonClicked,   this, &ShipUiWidget::dayThemeButtonClicked);
        connect(ct, &ClockToolbar::ringAlertButtonClicked,  this, &ShipUiWidget::ringAlertButtonClicked);
    }

    // 右侧动作
    connect(m_rightWidget, &RightWidget::modeSwitchButtonClicked, this, &ShipUiWidget::modeSwitchButtonClicked);
    connect(m_rightWidget, &RightWidget::lockButtonClicked,       this, &ShipUiWidget::lockButtonClicked);
    connect(m_rightWidget, &RightWidget::previewButtonClicked,    this, &ShipUiWidget::previewButtonClicked);
    connect(m_rightWidget, &RightWidget::photoButtonClicked,      this, &ShipUiWidget::photoButtonClicked);
    connect(m_rightWidget, &RightWidget::navArrowButtonClicked,   this, &ShipUiWidget::navArrowButtonClicked);
    connect(m_rightWidget, &RightWidget::autoDriveButtonClicked,  this, &ShipUiWidget::autoDriveButtonClicked);
    connect(m_rightWidget, &RightWidget::targetButtonClicked,     this, &ShipUiWidget::targetButtonClicked);
    connect(m_rightWidget, &RightWidget::alarmButtonClicked,      this, &ShipUiWidget::alarmButtonClicked);
    connect(m_rightWidget, &RightWidget::gamepadButtonClicked,    this, &ShipUiWidget::gamepadButtonClicked);
    connect(m_rightWidget, &RightWidget::uploadButtonClicked,     this, &ShipUiWidget::uploadButtonClicked);
    connect(m_rightWidget, &RightWidget::downloadButtonClicked,   this, &ShipUiWidget::downloadButtonClicked);
    connect(m_rightWidget, &RightWidget::micButtonClicked,        this, &ShipUiWidget::micButtonClicked);
}

void ShipUiWidget::applyTheme()
{
    // 设计规范：
    //  - 主背景 #252525
    //  - 各模块背景 #323232
    //  - label / pushbutton 默认透明、无边框
    setStyleSheet(QStringLiteral(
        "QWidget#shipUiRoot{background-color:#252525;}"

        // 模块外观
        "QWidget#leftWidget{background-color:#323232;border-radius:24px;}"
        "QWidget#topWidget{background-color:transparent;}"
        "QWidget#midWidget{background-color:#323232;border-radius:24px;}"
        "QLabel#mapSurface{background-color:#323232;border-radius:24px;}"
        "QFrame#rightModulePanel{background-color:#323232;border-radius:24px;}"

        // 全局 label / button 透明 + 无边框
        "QLabel{background:transparent;border:none;color:#f1f2f2;}"
        "QPushButton{background:transparent;border:none;color:#e4e6ea;}"
        "QPushButton:hover{background:transparent;}"
        "QPushButton:pressed{background:transparent;}"

        // LeftWidget 导航按钮
        "QPushButton#navButton{color:#b6b8bc;font-size:40px;border-radius:18px;}"
        "QPushButton#navButton:checked{background-color:#1E90FF;color:#ffffff;}"

        // VesselStatus 左半部按钮 / 标签
        "QPushButton#actionButton{color:#e4e6ea;font-size:24px;font-weight:600;padding:0 6px;}"
        "QLabel#batteryPercentLabel{color:#f1f2f2;font-size:22px;font-weight:600;min-width:56px;}"

        // ClockToolbar 右半部按钮 / 时钟
        "QLabel#topBarClockLabel{color:#FFFFFF;font-size:30px;font-weight:500;}"
        "QPushButton#nightModeButton,"
        "QPushButton#dayModeButton,"
        "QPushButton#ringAlertButton{color:#d0d2d6;font-size:26px;border-radius:35px;}"
        "QPushButton#dayModeButton:checked{background-color:#F3F6FF;color:#1e1f23;}"

        // RightWidget 内部标签样式
        "QLabel#infoTitleLabel{color:#8d9098;font-size:22px;letter-spacing:1px;}"
        "QLabel#infoValueLabel{color:#f1f2f2;font-size:38px;font-weight:600;}"
        "QLabel#geoCoordTitleLabel{color:#8d9098;font-family:\"Bai Jamjuree\";font-size:25px;font-weight:600;"
        "line-height:100%;letter-spacing:0px;text-align:center;text-transform:uppercase;}"
        "QLabel#geoCoordValueLabel{color:#f1f2f2;font-family:\"Bai Jamjuree\";font-size:35px;font-weight:600;"
        "line-height:40px;letter-spacing:0.05em;text-align:center;}"
        "QWidget#speedGaugeWidget{background:transparent;}"
        "QLabel#speedGaugeTitleLabel{color:#f1f2f2;font-family:\"Bai Jamjuree\";font-size:25px;font-weight:600;"
        "line-height:100%;letter-spacing:0px;text-align:center;text-transform:uppercase;"
        "background-color:rgba(255,255,255,0.5);border:none;border-radius:2px;}"
        "QLabel#speedGaugeValueLabel{color:#f1f2f2;font-family:\"Bai Jamjuree\";font-size:100px;font-weight:600;"
        "line-height:40px;letter-spacing:0.05em;text-align:center;background:transparent;}"
        "QLabel#speedGaugeUnitLabel{color:#f1f2f2;font-family:\"Bai Jamjuree\";font-size:20px;font-weight:600;"
        "line-height:100%;letter-spacing:0px;text-align:center;text-transform:uppercase;background:transparent;}"
        "QLabel#rightThrusterRpmTitleLabel{"
        "color:#80FFFFFF;"
        "font-family:\"Bai Jamjuree\";font-weight:600;font-size:25px;"
        "line-height:100%;letter-spacing:0px;"
        "text-align:center;text-transform:uppercase;"
        "background-color:transparent;border:none;}"
        "QLabel#rightThrusterRpmValueLabel{"
        "color:#f1f2f2;"
        "font-family:\"Bai Jamjuree\";font-size:60px;font-weight:600;"
        "letter-spacing:3px;text-align:center;"
        "background-color:transparent;"
        "border:none;}"
        "QLabel#rpmValueLabel{color:#f4f5f7;font-size:72px;font-weight:700;}"
        "QLabel#rudderValueLabel{color:#f4f5f7;font-size:32px;font-weight:600;}"

        // RightWidget 动作按钮
        "QPushButton#rightActionButton{color:#e4e6ea;font-size:48px;}"
    ));
}

void ShipUiWidget::setGeoInfo(const GeoInfo &info)
{
    if (m_rightWidget) {
        m_rightWidget->setGeoInfo(info);
    }
}

void ShipUiWidget::setSpeedInfo(const SpeedInfo &info)
{
    if (m_rightWidget) {
        m_rightWidget->setSpeedInfo(info);
    }
}

void ShipUiWidget::setMaxSpeedMps(double maxSpeedMps)
{
    if (m_rightWidget) {
        m_rightWidget->setMaxSpeedMps(maxSpeedMps);
    }
}

bool ShipUiWidget::loadMaxSpeedFromFile(const QString &absolutePath)
{
    double max = 0.0;
    if (!readMaxSpeedMpsFromFile(absolutePath, &max)) {
        return false;
    }
    setMaxSpeedMps(max);
    return true;
}

void ShipUiWidget::setRudderInfo(const RudderInfo &info)
{
    if (m_rightWidget) {
        m_rightWidget->setRudderInfo(info);
    }
}

void ShipUiWidget::setNavInfo(const NavInfo &info)
{
    if (m_rightWidget) {
        m_rightWidget->setNavInfo(info);
    }
}

void ShipUiWidget::setMapBackground(const QPixmap &pixmap)
{
    if (m_midWidget) {
        m_midWidget->setMapBackground(pixmap);
    }
}
