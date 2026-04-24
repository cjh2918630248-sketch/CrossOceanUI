#include "timerviewdemo.h"

namespace {
constexpr double kDemoBaseLatDeg = 39.0 + 55.0 / 60.0 + 44.04 / 3600.0;
constexpr double kDemoBaseLonDeg = 139.0 + 45.0 / 60.0 + 24.87 / 3600.0;
constexpr double kOneArcSecondDeg = 1.0 / 3600.0;
constexpr double kDefaultMaxSpeedMps = 80.0;

constexpr int kRpmClampMax = 2000;
constexpr int kRpmClampMin = -1500;

static int demoLeftRpmForSecond(int sec)
{
    const double x = static_cast<double>(sec) * 0.12;
    return std::clamp(static_cast<int>(std::lround(2000.0 * std::cos(x))), kRpmClampMin, kRpmClampMax);
}

static int demoRightRpmForSecond(int sec)
{
    const double x = static_cast<double>(sec) * 0.09;
    return std::clamp(static_cast<int>(std::lround(2000.0 * std::sin(x))), kRpmClampMin, kRpmClampMax);
}
} // namespace

TimerViewDemo::TimerViewDemo(ShipUiWidget *shipUi, QObject *parent)
    : QObject(parent)
    , m_shipUi(shipUi)
{
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &TimerViewDemo::onDemoTimerTimeout);
}

QString TimerViewDemo::maxSpeedFilePath() const
{
    return QDir(QCoreApplication::applicationDirPath())
        .filePath(QStringLiteral("ship_max_speed_mps.txt"));
}

double TimerViewDemo::syncMaxSpeedFromFileAndApply()
{
    const QString path = maxSpeedFilePath();
    ensureDefaultMaxSpeedFile(path, kDefaultMaxSpeedMps);

    double maxMps = kDefaultMaxSpeedMps;
    if (!readMaxSpeedMpsFromFile(path, &maxMps)) {
        maxMps = kDefaultMaxSpeedMps;
    }
    if (m_shipUi) {
        m_shipUi->setMaxSpeedMps(maxMps);
    }
    return maxMps;
}

void TimerViewDemo::start(int intervalMs)
{
    if (!m_shipUi) {
        return;
    }
    m_geoElapsedSec = 0;
    applyInitialDemoState();
    m_timer->start(intervalMs);
}

void TimerViewDemo::stop()
{
    if (m_timer) {
        m_timer->stop();
    }
}

void TimerViewDemo::applyInitialDemoState()
{
    const double maxMps = syncMaxSpeedFromFileAndApply();

    ShipUiWidget::GeoInfo initial;
    initial.latitudeDegrees  = kDemoBaseLatDeg;
    initial.longitudeDegrees = kDemoBaseLonDeg;
    m_shipUi->setGeoInfo(initial);

    ShipUiWidget::SpeedInfo s;
    s.speedMps = QRandomGenerator::global()->generateDouble() * maxMps;
    s.leftRpm  = demoLeftRpmForSecond(0);
    s.rightRpm = demoRightRpmForSecond(0);
    m_shipUi->setSpeedInfo(s);
}

void TimerViewDemo::onDemoTimerTimeout()
{
    if (!m_shipUi) {
        return;
    }
    const double maxMps = syncMaxSpeedFromFileAndApply();

    ++m_geoElapsedSec;
    ShipUiWidget::GeoInfo g;
    g.latitudeDegrees  = kDemoBaseLatDeg + m_geoElapsedSec * kOneArcSecondDeg;
    g.longitudeDegrees = kDemoBaseLonDeg + m_geoElapsedSec * kOneArcSecondDeg * 1.3;
    m_shipUi->setGeoInfo(g);

    ShipUiWidget::SpeedInfo s;
    s.speedMps = QRandomGenerator::global()->generateDouble() * maxMps;
    s.leftRpm  = demoLeftRpmForSecond(m_geoElapsedSec);
    s.rightRpm = demoRightRpmForSecond(m_geoElapsedSec);
    m_shipUi->setSpeedInfo(s);

    // Roll = 10·cos(t)，Pitch = 10·sin(t)，t 以秒为单位缓慢变化
    // Heading 在 0–360° 内匀速旋转（3°/秒），保持在仪表盘可见弧段
    const double t = static_cast<double>(m_geoElapsedSec) * 0.1;
    ShipUiWidget::NavInfo nav;
    nav.roll    = 10.0 * std::sin(t);
    nav.pitch   = 10.0 * std::cos(t);
    nav.heading = std::fmod(m_geoElapsedSec * 3.0, 360.0);
    m_shipUi->setNavInfo(nav);
}
