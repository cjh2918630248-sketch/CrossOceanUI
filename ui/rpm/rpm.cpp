#include "rpm.h"

#include <QPainter>
#include <QPainterPath>

#include <algorithm>
#include <cmath>

RPM::RPM(QWidget *parent)
    : QWidget(parent)
{
}

RPM::~RPM()
{
}

void RPM::setRpm(double rpmValue)
{
    const double maxAbs = std::max(1.0, m_maxAbsRpm); //std::max()用于返回多个值中的最大值,#include <algorithm>
    m_rpm = std::max(-maxAbs, std::min(rpmValue, maxAbs));
    update();
}

void RPM::setAngle(double angleValue)
{
    const double maxAbs = std::max(1.0, m_maxAbsAngle);
    m_angle = std::max(-maxAbs, std::min(angleValue, maxAbs));
    update();
}

void RPM::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(rect(), Qt::white);

    // 用固定设计稿尺寸做缩放基准，保证控件在不同窗口尺寸下比例一致。
    const double baseWidth = 800.0;
    const double baseHeight = 600.0;
    const double scale = std::min(width() / baseWidth, height() / baseHeight);

    const QPointF center(width() * 0.5, height() * 0.5);

    painter.setPen(QPen(Qt::black, 2));
    painter.setBrush(Qt::NoBrush);

    const QRectF outerRect(center.x() - 200.0 * scale, center.y() - 200.0 * scale,
                           400.0 * scale, 400.0 * scale);
    const QRectF innerRect(center.x() - 160.0 * scale, center.y() - 160.0 * scale,
                           320.0 * scale, 320.0 * scale);
    const QRectF midRect(center.x() - 20.0 * scale, center.y() - 200.0 * scale,
                         40.0 * scale, 280.0 * scale);

    // 计算外环起止点（210°~330°），用于后面画边界连接线。
    const double startDeg = 210.0;
    const double endDeg = 330.0;
    const double startRad = startDeg * M_PI / 180.0;
    const double endRad = endDeg * M_PI / 180.0;
    const QPointF arcCenter = outerRect.center();
    const double outerRadius = outerRect.width() * 0.5;
    const double innerRadius = innerRect.width() * 0.5;

    const QPointF outerStart(arcCenter.x() + outerRadius * std::cos(startRad),
                             arcCenter.y() - outerRadius * std::sin(startRad));
    const QPointF innerStart(arcCenter.x() + innerRadius * std::cos(startRad),
                             arcCenter.y() - innerRadius * std::sin(startRad));
    const QPointF outerEnd(arcCenter.x() + outerRadius * std::cos(endRad),
                           arcCenter.y() - outerRadius * std::sin(endRad));
    const QPointF innerEnd(arcCenter.x() + innerRadius * std::cos(endRad),
                           arcCenter.y() - innerRadius * std::sin(endRad));

    // 1) 中间 RPM 竖条底色。
    painter.fillRect(midRect, QColor(232, 232, 232));

    // 2) 中间 RPM 活跃填充：
    const double rpmAbsRatio = std::min(std::abs(m_rpm) / std::max(1.0, m_maxAbsRpm), 1.0);
    const QColor rpmColor = (m_rpm >= 0.0) ? QColor(58, 160, 255) : QColor(255, 128, 96);
    if (rpmAbsRatio > 0.0) {
        const double halfHeight = midRect.height() * 0.5;
        const double fillHeight = halfHeight * rpmAbsRatio;
        const QRectF rpmFillRect(midRect.left(),
                                   (m_rpm >= 0.0) ? (midRect.center().y() - fillHeight) : midRect.center().y(),
                                   midRect.width(),
                                   fillHeight);
        painter.fillRect(rpmFillRect, rpmColor);
    }

    // 3) 外环 Angle 底色（环带区域）。
    const double angleAbsRatio = std::min(std::abs(m_angle) / std::max(1.0, m_maxAbsAngle), 1.0);
    QPainterPath angleBasePath;
    angleBasePath.arcMoveTo(outerRect, 210.0);
    angleBasePath.arcTo(outerRect, 210.0, 120.0);
    angleBasePath.arcTo(innerRect, 330.0, -120.0);
    angleBasePath.closeSubpath();
    painter.fillPath(angleBasePath, QBrush(QColor(232, 232, 232)));

    // 4) 外环 Angle 活跃填充：
    //    以 270° 为中点，正值向右、负值向左扩展，最大扩展 60°。
    if (angleAbsRatio > 0.0) {
        const double startAngle = 270.0;
        const double span = 60.0 * angleAbsRatio;
        const double sweep = (m_angle >= 0.0) ? span : -span;
        const double endAngle = startAngle + sweep;
        const QColor angleColor = QColor::fromHsvF((0.55 - 0.55 * angleAbsRatio), 0.9, 1.0);

        QPainterPath angleActivePath;
        angleActivePath.arcMoveTo(outerRect, startAngle);
        angleActivePath.arcTo(outerRect, startAngle, sweep);
        angleActivePath.arcTo(innerRect, endAngle, -sweep);
        angleActivePath.closeSubpath();
        painter.fillPath(angleActivePath, QBrush(angleColor));
    }

    // 5) 参照指示条（与参考图一致）：橙色，RPM 为条内水平线，Angle 为弧上径向短线。
    const QColor markerColor(255, 130, 40);

    // RPM：水平条沿竖条上下移动，位置与填充“前沿”一致（0 在中间，正上负下）。
    {
        const double halfHeight = midRect.height() * 0.5;
        const double rpmNorm = std::max(-1.0, std::min(m_rpm / std::max(1.0, m_maxAbsRpm), 1.0));
        const double markerY = midRect.center().y() - rpmNorm * halfHeight;
        const double barH = std::max(3.0, 5.0 * scale);
        const double inset = std::max(1.0, 2.0 * scale);
        const QRectF rpmMarkerRect(midRect.left() + inset,
                                   markerY - barH * 0.5,
                                   midRect.width() - 2.0 * inset,
                                   barH);
        painter.setPen(Qt::NoPen);
        painter.setBrush(markerColor);
        painter.drawRoundedRect(rpmMarkerRect, barH * 0.5, barH * 0.5);
    }

    // Angle：在环带内沿当前角度画径向短线（-90°~+90° 映射到弧上 210°~330°，与填充范围一致）。
    {
        const double maxAbsAngle = std::max(1.0, m_maxAbsAngle);
        const double angNorm = std::max(-1.0, std::min(m_angle / maxAbsAngle, 1.0));
        const double needleDeg = 270.0 + angNorm * 60.0;
        const double nd = needleDeg * M_PI / 180.0;
        const QPointF pInner(arcCenter.x() + innerRadius * std::cos(nd),
                             arcCenter.y() - innerRadius * std::sin(nd));
        const QPointF pOuter(arcCenter.x() + outerRadius * std::cos(nd),
                             arcCenter.y() - outerRadius * std::sin(nd));
        painter.setBrush(Qt::NoBrush);
        painter.setPen(QPen(markerColor, std::max(2.5, 4.0 * scale), Qt::SolidLine, Qt::FlatCap));
        painter.drawLine(pInner, pOuter);
    }

    // 6) 最后绘制轮廓线，让边界更清晰。
    painter.setPen(QPen(Qt::black, 2));
    painter.setBrush(Qt::NoBrush);
    painter.drawArc(outerRect, 210 * 16, 120 * 16);
    painter.drawArc(innerRect, 210 * 16, 120 * 16);
    painter.drawLine(outerStart, innerStart);
    painter.drawLine(outerEnd, innerEnd);
    painter.drawRect(midRect);

    // 7) 绘制文本标签。
    painter.setPen(QPen(Qt::black, 1));
    QFont font = painter.font();
    font.setPointSizeF(std::max(12.0, 18.0 * scale));
    painter.setFont(font);
    const QString rpmLabel = QString("RPM %1").arg(m_rpm, 0, 'f', 0);
    painter.drawText(QPointF(midRect.left() - 110.0 * scale, midRect.center().y()), rpmLabel);
    const QString angleLabel = QString("Angle %1").arg(m_angle, 0, 'f', 0);
    painter.drawText(QPointF(midRect.right() + 18.0 * scale, midRect.center().y()), angleLabel);
}
