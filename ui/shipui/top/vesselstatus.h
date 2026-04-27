#ifndef VESSELSTATUS_H
#define VESSELSTATUS_H

#include <QWidget>
#include <QColor>
#include <QCursor>
#include <QHBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QPen>
#include <QPixmap>
#include <QPushButton>
#include <QSizePolicy>
#include <QDebug>

class VesselStatus : public QWidget
{
    Q_OBJECT

public:
    explicit VesselStatus(QWidget *parent = nullptr);

    QPushButton *rcButton() const { return m_rcButton; }
    QPushButton *mcButton() const { return m_mcButton; }
    QPushButton *autoModeButton() const { return m_autoModeButton; }
    QPushButton *stopButton() const { return m_stopButton; }
    QPushButton *controllerButton() const { return m_controllerButton; }
    void setBatteryPercent(int percent);
    void setAppNightMode(bool isNight);

signals:
    void rcButtonClicked();
    void mcButtonClicked();
    void autoButtonClicked();
    void stopButtonClicked();
    void controllerButtonClicked();

private slots:
    void onRcButtonClicked();
    void onMcButtonClicked();
    void onAutoButtonClicked();
    void onStopButtonClicked();
    void onControllerButtonClicked();

private:
    void setupUi();
    void setupConnections();
    void updateBatteryDisplay();

    QPushButton *m_rcButton = nullptr;
    QPushButton *m_mcButton = nullptr;
    QPushButton *m_autoModeButton = nullptr;
    QPushButton *m_stopButton = nullptr;
    QPushButton *m_controllerButton = nullptr;
    QLabel *m_batteryPercentLabel = nullptr; //显示电池百分比
    QLabel *m_batteryIndicator = nullptr; //显示电池图标
    int m_batteryPercent = 90; //存储当前电量百分比
};

#endif // VESSELSTATUS_H
