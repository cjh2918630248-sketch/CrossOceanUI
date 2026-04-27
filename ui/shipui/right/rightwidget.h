#ifndef RIGHTWIDGET_H
#define RIGHTWIDGET_H

#include <cmath>

#include <QWidget>
#include <QDebug>
#include <QFrame>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

#include "left/speedgaugewidget.h"
#include "right/rightthrusterwidget.h"
#include "right/ruddergaugewidget.h"
/**
 * 右侧信息/操作栏。
 * 固定尺寸 740 × 1390，包含 5 个模块（两两之间留 22px 间隔）：
 *   - 模块 1 位置 / 航速    740 × 410
 *   - 模块 2 RPM / 舵角     740 × 472（推进器条带设计框 584×256）
 *   - 模块 3/4/5 动作按钮条  各 740 × 140
 */
class RightWidget : public QWidget
{
    Q_OBJECT

public:
    /// 地理坐标（十进制度，外部通过 setGeoInfo 传入；界面显示为度分秒）
    struct GeoInfo
    {
        double latitudeDegrees  = 0.0;
        double longitudeDegrees = 0.0;
    };

    /// 将十进制度格式化为「度°分′秒″」（分、秒为整数位 + 秒保留两位小数）
    static QString formatDecimalDegreesAsDms(double decimalDegrees);

    struct SpeedInfo
    {
        double speedMps = 0.0;
        int leftRpm  = 0;
        int rightRpm = 0;
    };

    struct RudderInfo
    {
        double leftDeg   = 0.0;
        double centerDeg = 0.0;
        double rightDeg  = 0.0;
    };

    struct NavInfo
    {
        double roll    = 0.0;  ///< 横滚角（°），= 10·sin(t)
        double pitch   = 0.0;  ///< 俯仰角（°），= 10·cos(t)
        double heading = 0.0;  ///< 航向角（°，0–360），仪表盘三角所指值
    };

    explicit RightWidget(QWidget *parent = nullptr);

    void setGeoInfo(const GeoInfo &info);
    void setSpeedInfo(const SpeedInfo &info);
    void setRudderInfo(const RudderInfo &info);
    void setNavInfo(const NavInfo &info);
    /// 船速表满量程 (m/s)，与 SpeedInfo.speedMps 独立设置
    void setMaxSpeedMps(double maxSpeedMps);
    void setAppNightMode(bool isNight);

signals:
    void modeSwitchButtonClicked();
    void lockButtonClicked();
    void previewButtonClicked();
    void photoButtonClicked();

    void navArrowButtonClicked();
    void autoDriveButtonClicked();
    void targetButtonClicked();
    void alarmButtonClicked();

    void gamepadButtonClicked();
    void uploadButtonClicked();
    void downloadButtonClicked();
    void micButtonClicked();

private:
    void setupUi();
    QFrame* Init_GeoUi();
    QFrame* Init_RpmUi();
    void setupConnections();
    void refresh();

    QFrame      *createModulePanel(const QSize &size, const QString &objectName);
    QPushButton *createActionButton(const QString &text, QWidget *parent);

    // 模块 1
    QLabel *m_latValueLabel  = nullptr;
    QLabel *m_lonValueLabel  = nullptr;
    SpeedGaugeWidget *m_speedGauge = nullptr;

    // 模块 2（推进器 RPM + 仪表盘）
    RightThrusterRpmPairWidget *m_thrusterRpm  = nullptr;
    RudderGaugeWidget          *m_rudderGauge  = nullptr;

    // 模块 3
    QPushButton *m_modeSwitchButton = nullptr;
    QPushButton *m_lockButton       = nullptr;
    QPushButton *m_previewButton    = nullptr;
    QPushButton *m_photoButton      = nullptr;

    // 模块 4
    QPushButton *m_navArrowButton   = nullptr;
    QPushButton *m_autoDriveButton  = nullptr;
    QPushButton *m_targetButton     = nullptr;
    QPushButton *m_alarmButton      = nullptr;

    // 模块 5
    QPushButton *m_gamepadButton    = nullptr;
    QPushButton *m_uploadButton     = nullptr;
    QPushButton *m_downloadButton   = nullptr;
    QPushButton *m_micButton        = nullptr;

    GeoInfo    m_geoInfo;
    SpeedInfo  m_speedInfo;
    RudderInfo m_rudderInfo;
    NavInfo    m_navInfo;
    double     m_maxSpeedMps = 20.0;
};

#endif // RIGHTWIDGET_H
