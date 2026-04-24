#ifndef RIGHTTHRUSTERWIDGET_H
#define RIGHTTHRUSTERWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPainter>
#include <QPainterPath>
#include <QPaintEvent>
#include <QSizePolicy>
#include <QVBoxLayout>
#include <algorithm>
/// 单柱 RPM 条：中轴参照线，正值向上按 /2000 比例填充，负值向下按 /1500 比例填充（逻辑同 ThrusterRpmWidget）
class ThrusterRpmBarWidget : public QWidget
{
public:
    explicit ThrusterRpmBarWidget(QWidget *parent = nullptr);

    void setRpm(int rpm);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    int m_rpm = 0;
};

/// 模块 2：左列「LEFT / 数值 / RPM」、中间双柱水平、右列「RIGHT / 数值 / RPM」
class RightThrusterRpmPairWidget : public QWidget
{
public:
    explicit RightThrusterRpmPairWidget(QWidget *parent = nullptr);

    void setLeftRpm(int rpm);
    void setRightRpm(int rpm);

private:
    QLabel *m_leftTitleLabel   = nullptr;
    QLabel *m_leftValueLabel    = nullptr;
    QLabel *m_leftRpmUnitLabel  = nullptr;
    QLabel *m_rightTitleLabel   = nullptr;
    QLabel *m_rightValueLabel   = nullptr;
    QLabel *m_rightRpmUnitLabel = nullptr;
    ThrusterRpmBarWidget *m_leftBar  = nullptr;
    ThrusterRpmBarWidget *m_rightBar = nullptr;
};

#endif // RIGHTTHRUSTERWIDGET_H
