#ifndef RUDDERGAUGEWIDGET_H
#define RUDDERGAUGEWIDGET_H

#include <cmath>

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QPolygonF>
#include <QtMath>
/**
 * 60° 弧线航向仪表盘（航向/Roll/Pitch 显示）
 *
 * 固定尺寸 740 × 370。
 * 弧线半径 720px，弧心位于控件底部下方约 623px 处（不可见）。
 * 弧线覆盖 ±30°（共 60°），两端点恰好对应控件左下角和右下角。
 *
 * 刻度盘随航向旋转：当前 heading 始终对齐弧线正中央（顶点）。
 * 大刻度（每 10°）+ 数字显示在弧线外侧上方；小刻度（每 1°/5°）向弧心方向内画。
 *
 * 黄色小三角（#FFBF00）固定在弧线顶点，尖端朝上始终指向当前刻度，不随航向移动。
 * Roll、Pitch、Heading 数值显示在弧线上方可视区域。
 */
class RudderGaugeWidget : public QWidget
{
public:
    explicit RudderGaugeWidget(QWidget *parent = nullptr);

    /// 设置当前航向角（0–360°，三角形指针随之移动）
    void setHeading(double degrees);
    /// 设置横滚角（一位小数显示）
    void setRoll(double degrees);
    /// 设置俯仰角（一位小数显示）
    void setPitch(double degrees);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    double m_heading = 0.0;
    double m_roll    = 0.0;
    double m_pitch   = 0.0;
};

#endif // RUDDERGAUGEWIDGET_H
