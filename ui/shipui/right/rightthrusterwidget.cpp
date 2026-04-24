#include "right/rightthrusterwidget.h"

namespace {
// ── 整块约束（设计稿）────────────────────────────────────────────
constexpr int kPairDesignW = 584;
constexpr int kPairDesignH = 256;

// ── 文字列（左/右，justify-content: space-between）──────────────
// 左列: top=52.5≈53, left=0,   w=177, h=151
// 右列: top=52.5≈53, left=407, w=177, h=151
constexpr int kColW      = 177;
constexpr int kColH      = 151;
constexpr int kColTopY   = 53;
constexpr int kRightColX = 407;

// ── 标签固定尺寸 ────────────────────────────────────────────────
constexpr int kTitleW  = 177;   // LEFT / RIGHT / RPM 标签宽（= kColW）
constexpr int kTitleH  = 34;
constexpr int kValueW  = 177;   // 数值标签宽
constexpr int kValueH  = 72;

// ── 矩形柱 ──────────────────────────────────────────────────────
constexpr int kBarW = 56;
constexpr int kBarH = 209;
// 两柱在 x=[177,407] 的 230px 间隙内居中（含 20px 柱间距）
// (230 - 56×2 - 20) / 2 = 49 → 左柱 x=226, 右柱 x=302
constexpr int kLeftBarX  = 226;
constexpr int kRightBarX = 302;
constexpr int kBarTopY   = 23;  // (256 - 209) / 2, 垂直居中

// ── 柱绘制参数 ──────────────────────────────────────────────────
constexpr int kRpmMaxPositive  = 2000;
constexpr int kRpmMinMagnitude = 1500;
constexpr int kRefLineW   = 43;
constexpr int kRefPen     = 3;
constexpr int kBorderPx   = 2;
constexpr int kRadius     = 8;
constexpr int kFillInnerW = 50;
} // namespace

ThrusterRpmBarWidget::ThrusterRpmBarWidget(QWidget *parent)
    : QWidget(parent)
{
    setObjectName(QStringLiteral("thrusterRpmBarWidget"));
    setFixedSize(kBarW, kBarH);
    setAttribute(Qt::WA_TranslucentBackground, true);
}

void ThrusterRpmBarWidget::setRpm(int rpm)
{
    m_rpm = rpm;
    update();
}

void ThrusterRpmBarWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    int r = m_rpm;
    r = std::clamp(r, -kRpmMinMagnitude, kRpmMaxPositive);

    double ratio = 0.0;
    if (r >= 0) {
        ratio = kRpmMaxPositive > 0 ? static_cast<double>(r) / static_cast<double>(kRpmMaxPositive) : 0.0;
    } else {
        ratio = kRpmMinMagnitude > 0 ? static_cast<double>(-r) / static_cast<double>(kRpmMinMagnitude) : 0.0;
    }
    ratio = std::clamp(ratio, 0.0, 1.0);

    const QRectF outer(0, 0, kBarW, kBarH);
    p.setPen(QPen(QColor(255, 255, 255, 0x80), kBorderPx));
    p.setBrush(Qt::NoBrush);
    p.drawRoundedRect(outer, kRadius, kRadius);

    const QRectF inner = outer.adjusted(kBorderPx + 0.5, kBorderPx + 0.5, -kBorderPx - 0.5, -kBorderPx - 0.5);
    const double centerY = inner.center().y();
    const double fillX = (static_cast<double>(kBarW) - kFillInnerW) * 0.5;
    const double fillW = kFillInnerW;

    p.save();
    QPainterPath clipPath;
    const double innerRx = std::max(0.0, static_cast<double>(kRadius - kBorderPx));
    clipPath.addRoundedRect(inner, innerRx, innerRx);
    p.setClipPath(clipPath);

    if (r >= 0 && ratio > 0.0) {
        const double fillH = (centerY - inner.top()) * ratio;
        p.fillRect(QRectF(fillX, centerY - fillH, fillW, fillH), QColor(QStringLiteral("#FFFFFF")));
    } else if (r < 0 && ratio > 0.0) {
        const double fillH = (inner.bottom() - centerY) * ratio;
        p.fillRect(QRectF(fillX, centerY, fillW, fillH), QColor(QStringLiteral("#FFFFFF")));
    }
    p.restore();

    const double lineY = centerY;
    const double lineHalf = static_cast<double>(kRefLineW) * 0.5;
    const double midX = static_cast<double>(kBarW) * 0.5;
    p.setPen(QPen(QColor(QStringLiteral("#FFBD0A")), kRefPen, Qt::SolidLine, Qt::FlatCap));
    p.drawLine(QPointF(midX - lineHalf, lineY), QPointF(midX + lineHalf, lineY));
}

RightThrusterRpmPairWidget::RightThrusterRpmPairWidget(QWidget *parent)
    : QWidget(parent)
{
    setObjectName(QStringLiteral("rightThrusterRpmPairWidget"));
    setFixedSize(kPairDesignW, kPairDesignH);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setAttribute(Qt::WA_TranslucentBackground, true);

    // ── 左列 (x=0, y=53, w=177, h=151)，justify-content: space-between ──
    auto *leftContainer = new QWidget(this);
    leftContainer->setGeometry(0, kColTopY, kColW, kColH);
    leftContainer->setAttribute(Qt::WA_TranslucentBackground, true);

    auto *leftCol = new QVBoxLayout(leftContainer);
    leftCol->setContentsMargins(0, 0, 0, 0);
    leftCol->setSpacing(0);

    m_leftTitleLabel = new QLabel(QStringLiteral("LEFT"), leftContainer);
    m_leftTitleLabel->setObjectName(QStringLiteral("rightThrusterRpmTitleLabel"));
    m_leftTitleLabel->setAlignment(Qt::AlignCenter);
    m_leftTitleLabel->setFixedSize(kTitleW, kTitleH);
    leftCol->addWidget(m_leftTitleLabel, 0, Qt::AlignHCenter);

    leftCol->addStretch(1);

    m_leftValueLabel = new QLabel(QStringLiteral("0"), leftContainer);
    m_leftValueLabel->setObjectName(QStringLiteral("rightThrusterRpmValueLabel"));
    m_leftValueLabel->setAlignment(Qt::AlignCenter);
    m_leftValueLabel->setFixedSize(kValueW, kValueH);
    leftCol->addWidget(m_leftValueLabel, 0, Qt::AlignHCenter);

    leftCol->addStretch(1);

    m_leftRpmUnitLabel = new QLabel(QStringLiteral("RPM"), leftContainer);
    m_leftRpmUnitLabel->setObjectName(QStringLiteral("rightThrusterRpmTitleLabel"));
    m_leftRpmUnitLabel->setAlignment(Qt::AlignCenter);
    m_leftRpmUnitLabel->setFixedSize(kTitleW, kTitleH);
    leftCol->addWidget(m_leftRpmUnitLabel, 0, Qt::AlignHCenter);

    // ── 左柱 / 右柱（居中在 x=[177,407] 的 230px 间隙内）──
    m_leftBar = new ThrusterRpmBarWidget(this);
    m_leftBar->move(kLeftBarX, kBarTopY);

    m_rightBar = new ThrusterRpmBarWidget(this);
    m_rightBar->move(kRightBarX, kBarTopY);

    // ── 右列 (x=407, y=53, w=177, h=151)，justify-content: space-between ──
    auto *rightContainer = new QWidget(this);
    rightContainer->setGeometry(kRightColX, kColTopY, kColW, kColH);
    rightContainer->setAttribute(Qt::WA_TranslucentBackground, true);

    auto *rightCol = new QVBoxLayout(rightContainer);
    rightCol->setContentsMargins(0, 0, 0, 0);
    rightCol->setSpacing(0);

    m_rightTitleLabel = new QLabel(QStringLiteral("RIGHT"), rightContainer);
    m_rightTitleLabel->setObjectName(QStringLiteral("rightThrusterRpmTitleLabel"));
    m_rightTitleLabel->setAlignment(Qt::AlignCenter);
    m_rightTitleLabel->setFixedSize(kTitleW, kTitleH);
    rightCol->addWidget(m_rightTitleLabel, 0, Qt::AlignHCenter);

    rightCol->addStretch(1);

    m_rightValueLabel = new QLabel(QStringLiteral("0"), rightContainer);
    m_rightValueLabel->setObjectName(QStringLiteral("rightThrusterRpmValueLabel"));
    m_rightValueLabel->setAlignment(Qt::AlignCenter);
    m_rightValueLabel->setFixedSize(kValueW, kValueH);
    rightCol->addWidget(m_rightValueLabel, 0, Qt::AlignHCenter);

    rightCol->addStretch(1);

    m_rightRpmUnitLabel = new QLabel(QStringLiteral("RPM"), rightContainer);
    m_rightRpmUnitLabel->setObjectName(QStringLiteral("rightThrusterRpmTitleLabel"));
    m_rightRpmUnitLabel->setAlignment(Qt::AlignCenter);
    m_rightRpmUnitLabel->setFixedSize(kTitleW, kTitleH);
    rightCol->addWidget(m_rightRpmUnitLabel, 0, Qt::AlignHCenter);
}

void RightThrusterRpmPairWidget::setLeftRpm(int rpm)
{
    if (m_leftValueLabel) {
        m_leftValueLabel->setText(QString::number(rpm));
    }
    if (m_leftBar) {
        m_leftBar->setRpm(rpm);
    }
}

void RightThrusterRpmPairWidget::setRightRpm(int rpm)
{
    if (m_rightValueLabel) {
        m_rightValueLabel->setText(QString::number(rpm));
    }
    if (m_rightBar) {
        m_rightBar->setRpm(rpm);
    }
}
