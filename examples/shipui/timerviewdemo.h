#ifndef TIMERVIEWDEMO_H
#define TIMERVIEWDEMO_H

#include <algorithm>
#include <cmath>

#include <QObject>
#include <QCoreApplication>
#include <QDir>
#include <QRandomGenerator>
#include <QTimer>

#include "main/shipuiwidget.h"
#include "main/speedmaxconfig.h"
/**
 * 所有通过 QTimer 驱动、仅用于演示的界面数值更新均集中在此（经纬度、随机船速、从文件同步满量程等）。
 * 正式运行请直接调用 ShipUiWidget::setGeoInfo / setSpeedInfo / setMaxSpeedMps / loadMaxSpeedFromFile。
 */
class TimerViewDemo : public QObject
{
    Q_OBJECT

public:
    explicit TimerViewDemo(ShipUiWidget *shipUi, QObject *parent = nullptr);

    void start(int intervalMs = 1000);
    void stop();

private slots:
    void onDemoTimerTimeout();

private:
    void applyInitialDemoState();
    double syncMaxSpeedFromFileAndApply();

    QString maxSpeedFilePath() const;

    ShipUiWidget *m_shipUi = nullptr;
    QTimer *m_timer = nullptr;
    int m_geoElapsedSec = 0;
};

#endif // TIMERVIEWDEMO_H
