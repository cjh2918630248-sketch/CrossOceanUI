#include "thrusterrpmwidget.h"

#include <QPainter>
#include <QPainterPath>

#include <algorithm>
#include <cmath>

ThrusterRpmWidget::ThrusterRpmWidget(QWidget *parent)
    : QWidget(parent)
{
}

ThrusterRpmWidget::~ThrusterRpmWidget()
{
}

void ThrusterRpmWidget::setLeftRpm(double rpmValue)
{
    m_leftRpm = clampRpm(rpmValue);
    update();
}

void ThrusterRpmWidget::setRightRpm(double rpmValue)
{
    m_rightRpm = clampRpm(rpmValue);
    update();
}

void ThrusterRpmWidget::setRpms(double leftRpm, double rightRpm)
{
    m_leftRpm = clampRpm(leftRpm);
    m_rightRpm = clampRpm(rightRpm);
    update();
}

void ThrusterRpmWidget::setMaxAbsRpm(double maxAbsRpm)
{
    m_maxAbsRpm = std::max(1.0, maxAbsRpm);
    m_leftRpm = clampRpm(m_leftRpm);
    m_rightRpm = clampRpm(m_rightRpm);
    update();
}

double ThrusterRpmWidget::leftRpm() const
{
    return m_leftRpm;
}

double ThrusterRpmWidget::rightRpm() const
{
    return m_rightRpm;
}

double ThrusterRpmWidget::maxAbsRpm() const
{
    return m_maxAbsRpm;
}

double ThrusterRpmWidget::clampRpm(double value) const
{
    const double maxAbs = std::max(1.0, m_maxAbsRpm);
    return std::clamp(value, -maxAbs, maxAbs);
}

void ThrusterRpmWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing); //抗锯齿
    painter.fillRect(rect(), Qt::white);

    const double baseWidth = 800.0;
    const double baseHeight = 600.0;
    const double scale = std::min(width() / baseWidth, height() / baseHeight);
    const QPointF center(width() * 0.5, height() * 0.5);
    const QRectF leftRect(center.x() - 80.0 * scale, center.y() - 200.0 * scale, 40.0 * scale, 280.0 * scale);
    const QRectF rightRect(center.x() + 80.0 * scale, center.y() - 200.0 * scale, 40.0 * scale, 280.0 * scale);
    const qreal cornerRadius = 10.0 * scale;
    const qreal borderWidth = std::max<qreal>(2.0, 4.0 * scale);

    const auto drawThrusterBar = [&](const QRectF &barRect, double rpmValue) {
        const double clampedNorm = std::clamp(rpmValue / m_maxAbsRpm, -1.0, 1.0);
        const qreal halfHeight = barRect.height() * 0.5;
        const qreal fillHeight = halfHeight * std::abs(clampedNorm);
        const qreal centerY = barRect.center().y();
        const qreal innerMargin = borderWidth * 0.5 + 1.0;
        const QRectF innerRect = barRect.adjusted(innerMargin, innerMargin, -innerMargin, -innerMargin);

        painter.save();
        QPainterPath clipPath;
        clipPath.addRoundedRect(innerRect,
                                std::max<qreal>(0.0, cornerRadius - innerMargin),
                                std::max<qreal>(0.0, cornerRadius - innerMargin));
        painter.setClipPath(clipPath);

        QRectF fillRect(innerRect.left(), centerY, innerRect.width(), 0.0);
        if (clampedNorm >= 0.0) {
            fillRect = QRectF(innerRect.left(), centerY - fillHeight, innerRect.width(), fillHeight);
        } else {
            fillRect = QRectF(innerRect.left(), centerY, innerRect.width(), fillHeight);
        }

        const QColor fillColor = (clampedNorm >= 0.0) ? QColor(58, 160, 255) : QColor(255, 128, 96);
        painter.fillRect(fillRect, fillColor);
        painter.restore();

        painter.setPen(QPen(QColor(90, 90, 90), std::max<qreal>(1.0, 1.2 * scale)));
        painter.drawLine(QPointF(innerRect.left(), centerY), QPointF(innerRect.right(), centerY));

        painter.setPen(QPen(Qt::black, borderWidth));
        painter.setBrush(Qt::NoBrush);
        painter.drawRoundedRect(barRect, cornerRadius, cornerRadius);
    };

    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(232, 232, 232));
    painter.drawRoundedRect(leftRect, cornerRadius, cornerRadius);
    painter.drawRoundedRect(rightRect, cornerRadius, cornerRadius);

    drawThrusterBar(leftRect, m_leftRpm);
    drawThrusterBar(rightRect, m_rightRpm);

    const QFont baseFont = painter.font();
    QFont labelFont = baseFont;
    labelFont.setPointSizeF(std::max<qreal>(10.0, 12.0 * scale));
    painter.setFont(labelFont);
    painter.setPen(Qt::black);

    const qreal textWidth = 120.0 * scale;
    const qreal textHeight = 30.0 * scale;
    const qreal textSpacing = 12.0 * scale;
    const qreal textGap = 20.0 * scale;
    const qreal blockHeight = textHeight * 3.0 + textSpacing * 2.0;
    const qreal blockTop = center.y() - blockHeight * 0.5;

    const QRectF leftLabel1(leftRect.left() - textGap - textWidth, blockTop, textWidth, textHeight);
    const QRectF leftLabel2(leftRect.left() - textGap - textWidth, blockTop + textHeight + textSpacing, textWidth, textHeight);
    const QRectF leftLabel3(leftRect.left() - textGap - textWidth, blockTop + (textHeight + textSpacing) * 2.0, textWidth, textHeight);
    painter.drawText(leftLabel1, Qt::AlignCenter, QStringLiteral("左推进器"));
    painter.drawText(leftLabel2, Qt::AlignCenter, QString::number(m_leftRpm, 'f', 0));
    painter.drawText(leftLabel3, Qt::AlignCenter, QStringLiteral("RPM"));

    const QRectF rightLabel1(rightRect.right() + textGap, blockTop, textWidth, textHeight);
    const QRectF rightLabel2(rightRect.right() + textGap, blockTop + textHeight + textSpacing, textWidth, textHeight);
    const QRectF rightLabel3(rightRect.right() + textGap, blockTop + (textHeight + textSpacing) * 2.0, textWidth, textHeight);
    painter.drawText(rightLabel1, Qt::AlignCenter, QStringLiteral("右推进器"));
    painter.drawText(rightLabel2, Qt::AlignCenter, QString::number(m_rightRpm, 'f', 0));
    painter.drawText(rightLabel3, Qt::AlignCenter, QStringLiteral("RPM"));
}
