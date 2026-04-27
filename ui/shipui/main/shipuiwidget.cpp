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
    if (auto *ct = m_topWidget->clockToolbar()) {
        const bool night = ct->nightThemeButton()->isChecked();
        m_leftWidget->setAppNightMode(night);
        ct->setAppNightMode(night);
    }
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
    connect(m_leftWidget, &LeftWidget::dockingButtonClicked,   this, &ShipUiWidget::dockingButtonClicked);
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
        connect(ct, &ClockToolbar::nightThemeButtonClicked, this, [this] {
            setAppNightMode(true);
            if (m_leftWidget)  m_leftWidget->setAppNightMode(true);
            if (m_rightWidget) m_rightWidget->setAppNightMode(true);
            if (auto *vs = m_topWidget->vesselStatus())  vs->setAppNightMode(true);
            if (auto *c  = m_topWidget->clockToolbar())  c->setAppNightMode(true);
        });
        connect(ct, &ClockToolbar::dayThemeButtonClicked, this, [this] {
            setAppNightMode(false);
            if (m_leftWidget)  m_leftWidget->setAppNightMode(false);
            if (m_rightWidget) m_rightWidget->setAppNightMode(false);
            if (auto *vs = m_topWidget->vesselStatus())  vs->setAppNightMode(false);
            if (auto *c  = m_topWidget->clockToolbar())  c->setAppNightMode(false);
        });
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

static QString buildStyleSheet(bool isNight)
{
    const QString rootBg         = isNight ? QStringLiteral("#252525")             : QStringLiteral("#D2DCF2");
    const QString moduleBg       = isNight ? QStringLiteral("#323232")             : QStringLiteral("#F3F6FF");
    const QString genLabel       = isNight ? QStringLiteral("#f1f2f2")             : QStringLiteral("#5B73AB");
    const QString genBtn         = isNight ? QStringLiteral("#e4e6ea")             : QStringLiteral("#5B73AB");
    const QString clockColor     = isNight ? QStringLiteral("#FFFFFF")             : QStringLiteral("#5B73AB");
    const QString dimLabel       = isNight ? QStringLiteral("#8d9098")             : QStringLiteral("#5B73AB");
    const QString rpmTitleColor  = isNight ? QStringLiteral("#80FFFFFF")           : QStringLiteral("rgba(91,115,171,128)");
    const QString f4Label        = isNight ? QStringLiteral("#f4f5f7")             : QStringLiteral("#5B73AB");

    QString s;
    s.reserve(3072);
    s += "QWidget#shipUiRoot{background-color:"  + rootBg   + ";}";

    s += "QWidget#leftWidget{background-color:"  + moduleBg + ";border-radius:24px;}";
    s += "QWidget#topWidget{background-color:transparent;}";
    s += "QWidget#midWidget{background-color:"   + moduleBg + ";border-radius:24px;}";
    s += "QLabel#mapSurface{background-color:"   + moduleBg + ";border-radius:24px;}";
    s += "QFrame#rightModulePanel{background-color:" + moduleBg + ";border-radius:24px;}";

    s += "QLabel{background:transparent;border:none;color:" + genLabel + ";}";
    s += "QPushButton{background:transparent;border:none;color:" + genBtn + ";}";
    s += "QPushButton:hover{background:transparent;}";
    s += "QPushButton:pressed{background:transparent;}";

    s += "QPushButton#navButton{background:transparent;border:none;border-radius:40px;}";
    s += "QPushButton#navButton:checked{background-color:#1E90FF;border:none;border-radius:40px;}";

    s += "QPushButton#actionButton{color:" + genBtn + ";font-size:24px;font-weight:600;padding:0 6px;}";
    s += "QLabel#batteryPercentLabel{color:" + genLabel + ";font-size:22px;font-weight:600;min-width:56px;}";

    s += "QLabel#topBarClockLabel{color:" + clockColor + ";font-size:30px;font-weight:500;}";

    s += "QLabel#infoTitleLabel{color:" + dimLabel + ";font-size:22px;letter-spacing:1px;}";
    s += "QLabel#infoValueLabel{color:" + genLabel + ";font-size:38px;font-weight:600;}";
    s += "QLabel#geoCoordTitleLabel{color:" + dimLabel + ";"
         "font-family:\"Bai Jamjuree\";font-size:25px;font-weight:600;"
         "line-height:100%;letter-spacing:0px;text-align:center;text-transform:uppercase;}";
    s += "QLabel#geoCoordValueLabel{color:" + genLabel + ";"
         "font-family:\"Bai Jamjuree\";font-size:35px;font-weight:600;"
         "line-height:40px;letter-spacing:0.05em;text-align:center;}";
    s += "QWidget#speedGaugeWidget{background:transparent;}";
    s += "QLabel#speedGaugeTitleLabel{color:" + genLabel + ";"
         "font-family:\"Bai Jamjuree\";font-size:25px;font-weight:600;"
         "line-height:100%;letter-spacing:0px;text-align:center;text-transform:uppercase;"
         "background-color:rgba(255,255,255,0.5);border:none;border-radius:2px;}";
    s += "QLabel#speedGaugeValueLabel{color:" + genLabel + ";"
         "font-family:\"Bai Jamjuree\";font-size:100px;font-weight:600;"
         "line-height:40px;letter-spacing:0.05em;text-align:center;background:transparent;}";
    s += "QLabel#speedGaugeUnitLabel{color:" + genLabel + ";"
         "font-family:\"Bai Jamjuree\";font-size:20px;font-weight:600;"
         "line-height:100%;letter-spacing:0px;text-align:center;text-transform:uppercase;background:transparent;}";
    s += "QLabel#rightThrusterRpmTitleLabel{color:" + rpmTitleColor + ";"
         "font-family:\"Bai Jamjuree\";font-weight:600;font-size:25px;"
         "line-height:100%;letter-spacing:0px;"
         "text-align:center;text-transform:uppercase;"
         "background-color:transparent;border:none;}";
    s += "QLabel#rightThrusterRpmValueLabel{color:" + genLabel + ";"
         "font-family:\"Bai Jamjuree\";font-size:60px;font-weight:600;"
         "letter-spacing:3px;text-align:center;"
         "background-color:transparent;border:none;}";
    s += "QLabel#rpmValueLabel{color:"    + f4Label + ";font-size:72px;font-weight:700;}";
    s += "QLabel#rudderValueLabel{color:" + f4Label + ";font-size:32px;font-weight:600;}";

    s += "QPushButton#rightActionButton{color:" + genBtn + ";font-size:48px;}";
    return s;
}

void ShipUiWidget::applyTheme()
{
    setStyleSheet(buildStyleSheet(true));
}

void ShipUiWidget::setAppNightMode(bool isNight)
{
    setStyleSheet(buildStyleSheet(isNight));
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
