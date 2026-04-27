#ifndef SPEEDGAUGEWIDGET_H
#define SPEEDGAUGEWIDGET_H

#include <cmath>

#include <QWidget>
#include <QLabel>
#include <QLinearGradient>
#include <QPainter>
#include <QPaintEvent>
#include <QPainterPath>
#include <QVBoxLayout>
/**
 * 模块 1 右侧半月形船速表：渐变底 + 上半圆描边，中央 SPEED / 数值 / M/S 垂直排列。
 */
class SpeedGaugeWidget : public QWidget
{
public:
    explicit SpeedGaugeWidget(QWidget *parent = nullptr);

    /// 当前船速 (m/s)，更新数字与弧段填充
    void setSpeedMps(double speedMps);
    /// 表盘满量程 (m/s)，用于 current/max 弧长比例（比例保留一位小数后绘制）
    void setMaxSpeedMps(double maxSpeedMps);

    void setSpeedValueText(const QString &text);
    void setNightMode(bool night);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QLabel *m_valueLabel = nullptr;
    double  m_speedMps    = 0.0;
    double  m_maxSpeedMps = 20.0;
    bool    m_nightMode   = true;
};

#endif // SPEEDGAUGEWIDGET_H
