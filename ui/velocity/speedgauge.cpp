#include "speedgauge.h"
#include <QPainter>
#include <QPainterPath>
#include <cmath>

SpeedGauge::SpeedGauge(QWidget *parent)
    : QWidget(parent)
{
}

SpeedGauge::~SpeedGauge()
{
}

void SpeedGauge::setSpeed(double speed)
{
    const double clamped = qBound(m_minSpeed, speed, m_maxSpeed);
    if (qFuzzyCompare(m_speed, clamped)) {
        return;
    }
    m_speed = clamped;
    update();
}

double SpeedGauge::speed() const
{
    return m_speed;
}

void SpeedGauge::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);
    painter.fillRect(rect(), Qt::white);

    painter.setPen(QPen(Qt::black, 2));
    painter.setRenderHint(QPainter::Antialiasing);
    int centerW = width() / 2;
    int centerH = height() / 2;

    double leftStartX = centerW - (80 * std::sqrt(3.0));
    double leftStartY = centerH - 80;
    double leftEndX = centerW - (100 * std::sqrt(3.0));
    double leftEndY = centerH - 100;
    double rightStartX = centerW + (80 * std::sqrt(3.0));
    double rightStartY = centerH - 80;
    double rightEndX = centerW + (100 * std::sqrt(3.0));
    double rightEndY = centerH - 100;

    painter.drawLine(QPoint(leftStartX, leftStartY), QPoint(leftEndX, leftEndY));
    painter.drawLine(QPoint(rightStartX, rightStartY), QPoint(rightEndX, rightEndY));

    QRect outerRect(centerW - 200, centerH - 200, 400, 400);
    QRect innerRect(centerW - 160, centerH - 160, 320, 320);

    QPainterPath gaugePath;
    gaugePath.arcMoveTo(outerRect, 150.0);
    gaugePath.arcTo(outerRect, 150.0, -120.0);
    gaugePath.arcTo(innerRect, 30.0, 120.0);
    gaugePath.closeSubpath();
    painter.fillPath(gaugePath, QBrush(Qt::lightGray));

    if (m_speed > 0.0) {
        const qreal ratio = qBound(0.0, (m_speed - m_minSpeed) / (m_maxSpeed - m_minSpeed), 1.0);
        const qreal progressSpan = -120.0 * ratio;
        const qreal endAngle = 150.0 + progressSpan;
        const QColor activeColor = QColor::fromHsvF((0.55 - 0.55 * ratio), 0.95, 1.0);

        QPainterPath activePath;
        activePath.arcMoveTo(outerRect, 150.0);
        activePath.arcTo(outerRect, 150.0, progressSpan);
        activePath.arcTo(innerRect, endAngle, -progressSpan);
        activePath.closeSubpath();
        painter.fillPath(activePath, QBrush(activeColor));
    }

    painter.drawArc(outerRect, 30 * 16, 120 * 16);
    painter.drawArc(innerRect, 30 * 16, 120 * 16);

}