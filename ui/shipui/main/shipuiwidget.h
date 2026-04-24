#ifndef SHIPUIWIDGET_H
#define SHIPUIWIDGET_H

#include <QWidget>
#include <QString>

#include "right/rightwidget.h"
#include "top/clocktoolbar.h"
#include "main/speedmaxconfig.h"
#include "left/leftwidget.h"
#include "middle/midwidget.h"
#include "top/topwidget.h"
#include "top/vesselstatus.h"
/**
 * 顶层 UI 根容器。
 * 固定 2560 × 1600 设计稿画布；LeftWidget / TopWidget / MidWidget / RightWidget
 * 四个子容器按绝对坐标摆放，交由 ScaledView 做等比缩放。
 */
class ShipUiWidget : public QWidget
{
    Q_OBJECT

public:
    using GeoInfo    = RightWidget::GeoInfo;
    using SpeedInfo  = RightWidget::SpeedInfo;
    using RudderInfo = RightWidget::RudderInfo;
    using NavInfo    = RightWidget::NavInfo;

    explicit ShipUiWidget(QWidget *parent = nullptr);

    void setGeoInfo(const GeoInfo &info);
    void setSpeedInfo(const SpeedInfo &info);
    void setRudderInfo(const RudderInfo &info);
    void setNavInfo(const NavInfo &info);
    void setMaxSpeedMps(double maxSpeedMps); //手动设置满量程船速（m/s）
    /// 从文件加载满量程船速（m/s），格式见 speedmaxconfig.h
    bool loadMaxSpeedFromFile(const QString &absolutePath);
    void setMapBackground(const QPixmap &pixmap);

    LeftWidget  *leftWidget()   const { return m_leftWidget; }
    TopWidget   *topWidget()    const { return m_topWidget; }
    MidWidget   *midWidget()    const { return m_midWidget; }
    RightWidget *rightWidget()  const { return m_rightWidget; }

signals:
    // 左侧导航
    void homeButtonClicked();
    void mapButtonClicked();
    void shipButtonClicked();
    void chartButtonClicked();
    void cameraButtonClicked();
    void settingButtonClicked();

    // 顶栏 — 左半部（VesselStatus）
    void rcButtonClicked();
    void mcButtonClicked();
    void autoButtonClicked();
    void stopButtonClicked();
    void controllerButtonClicked();

    // 顶栏 — 右半部（ClockToolbar）
    void nightThemeButtonClicked();
    void dayThemeButtonClicked();
    void ringAlertButtonClicked();

    // 右侧动作
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
    void buildUi();
    void setupConnections();
    void applyTheme();

    LeftWidget  *m_leftWidget  = nullptr;
    TopWidget   *m_topWidget   = nullptr;
    MidWidget   *m_midWidget   = nullptr;
    RightWidget *m_rightWidget = nullptr;
};

#endif // SHIPUIWIDGET_H
