#include "left/speedgaugewidget.h"

namespace {
constexpr int kOuterSize   = 348;
constexpr int kInnerSize   = 327;
constexpr int kArcPenPx    = 6;
constexpr int kTextColWidth = 255;
} // namespace

SpeedGaugeWidget::SpeedGaugeWidget(QWidget *parent)
    : QWidget(parent)
{
    setObjectName(QStringLiteral("speedGaugeWidget"));
    setFixedSize(kOuterSize, kOuterSize);
    setAttribute(Qt::WA_TranslucentBackground, true);

    auto *root = new QVBoxLayout(this);
    root->setContentsMargins(0, 0, 0, 0);
    root->setSpacing(0);
    root->addStretch(5);

    auto *col = new QWidget(this);
    col->setAttribute(Qt::WA_TranslucentBackground);
    col->setStyleSheet(QStringLiteral("background:transparent;"));
    auto *vl = new QVBoxLayout(col);
    vl->setContentsMargins(0, 0, 0, 0);
    vl->setSpacing(6);
    vl->setAlignment(Qt::AlignHCenter);

    auto *title = new QLabel(QStringLiteral("SPEED"), col);
    title->setObjectName(QStringLiteral("speedGaugeTitleLabel"));
    title->setAlignment(Qt::AlignCenter);
    title->setFixedSize(kTextColWidth, 42);

    m_valueLabel = new QLabel(QStringLiteral("0.0"), col);
    m_valueLabel->setObjectName(QStringLiteral("speedGaugeValueLabel"));
    m_valueLabel->setAlignment(Qt::AlignCenter);
    m_valueLabel->setFixedSize(kTextColWidth, 95);

    auto *unit = new QLabel(QStringLiteral("M/S"), col);
    unit->setObjectName(QStringLiteral("speedGaugeUnitLabel"));
    unit->setAlignment(Qt::AlignCenter);
    unit->setFixedSize(kTextColWidth, 42);

    vl->addWidget(title);
    vl->addWidget(m_valueLabel);
    vl->addWidget(unit);

    root->addWidget(col, 0, Qt::AlignHCenter);
    root->addStretch(6);
}

void SpeedGaugeWidget::setSpeedMps(double speedMps)
{
    m_speedMps = speedMps;
    if (m_valueLabel) {
        m_valueLabel->setText(QString::number(speedMps, 'f', 1));
    }
    update();
}

void SpeedGaugeWidget::setMaxSpeedMps(double maxSpeedMps)
{
    m_maxSpeedMps = (maxSpeedMps > 0.0) ? maxSpeedMps : 20.0;
    update();
}

void SpeedGaugeWidget::setSpeedValueText(const QString &text)
{
    if (m_valueLabel) {
        m_valueLabel->setText(text);
    }
    update();
}

void SpeedGaugeWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    const QRectF outerCircle(0, 0, kOuterSize, kOuterSize);

    QLinearGradient grad(0, 0, 0, height());
    grad.setColorAt(0.0, QColor(0, 0, 0, 128));
    grad.setColorAt(0.354, QColor(0, 0, 0, 128));
    grad.setColorAt(0.7872, QColor(0, 0, 0, 0));
    grad.setColorAt(1.0, QColor(0, 0, 0, 0));

    QPainterPath disk;
    disk.addEllipse(outerCircle);
    p.fillPath(disk, QBrush(grad));

    const double margin = (static_cast<double>(kOuterSize - kInnerSize)) * 0.5;
    const QRectF inner(margin, margin, kInnerSize, kInnerSize);
    const QPointF center = inner.center();

    p.setPen(QPen(QColor(255, 255, 255, 0x80), kArcPenPx, Qt::SolidLine, Qt::FlatCap));
    p.setBrush(Qt::NoBrush);

    const QRect innerRect = inner.toRect();
    p.setClipRect(QRectF(0, 0, width(), center.y() + 2.0));

    // 底轨：完整上半圆弧（半透明白）
    p.setPen(QPen(QColor(255, 255, 255, 0x80), kArcPenPx, Qt::SolidLine, Qt::FlatCap));
    p.setBrush(Qt::NoBrush);
    p.drawArc(innerRect, 180 * 16, -180 * 16);

    // 当前船速填充：同半径 6px 实白弧，长度 = (speed/max) 保留一位小数 × 180°
    double rawRatio = 0.0;
    if (m_maxSpeedMps > 0.0) {
        rawRatio = m_speedMps / m_maxSpeedMps;
    }
    rawRatio = qBound(0.0, rawRatio, 1.0);
    const double fillRatio = std::round(rawRatio * 10.0) / 10.0;
    const int spanFullSixteenths = -180 * 16;
    const int spanFillSixteenths = static_cast<int>(std::lround(static_cast<double>(spanFullSixteenths) * fillRatio));

    if (spanFillSixteenths != 0) {
        p.setPen(QPen(Qt::white, kArcPenPx, Qt::SolidLine, Qt::FlatCap));
        p.drawArc(innerRect, 180 * 16, spanFillSixteenths);
    }

    p.setClipping(false);
}
