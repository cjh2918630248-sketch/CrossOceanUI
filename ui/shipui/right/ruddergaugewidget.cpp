#include "right/ruddergaugewidget.h"

#include <QPainterPath>

namespace {
// ── 整体尺寸 ────────────────────────────────────────────────────────
constexpr int    kGaugeW   = 740;
constexpr int    kGaugeH   = 370;

// ── 圆弧几何（60° 弧，两端点落在控件左下/右下角）────────────────────
// 弦长 = 720（x=10 到 x=730），弧角 = 60°
// R = 弦长 / (2·sin30°) = 720；弧心在控件底部下方 R·cos30° ≈ 623.54 px
constexpr double kRadius       = 720.0;
constexpr double kArcCenterDY  = 623.54; // ≈ 720 × cos(30°) = 720 × √3/2
constexpr double kArcHalfAngle = 30.0;   // 弧线覆盖 ±30°（以弧顶正上方为 0°）

// 刻度尺寸（朝远离弧心方向外画，即弧线上方）
constexpr double kMajorLen = 18.0;  // 大刻度（每10°）
//constexpr double kMinorLen = 18.0;  // 中刻度（每5°）
constexpr double kMicroLen = 11.0;  // 小刻度（每1°）
// 刻度数字在大刻度外端之外再留 8px
constexpr double kNumberR  = kRadius + kMajorLen + 8.0; // ≈ 764

// ── 固定三角形指针（弧顶正下方，尖端触及弧线，指向当前刻度）──────────
constexpr double kTriHeight = 18.0;
constexpr double kTriHW     =  8.0;

// ── 画笔 ────────────────────────────────────────────────────────────
constexpr int kArcPenW   = 2;
constexpr int kMajorPenW = 2;
constexpr int kMinorPenW = 1;

// ── 字体（Roll/Pitch/Heading 三列较原 12/18px 放大 ≥70%：21/31px）──
constexpr int kTickFontPx = 15;
constexpr int kValFontPx  = 31;   // 数值
constexpr int kLblFontPx  = 21;   // 列标题

// ── 文字位置（gauge 控件坐标）──────────────────────────────────────────
// 弧线内侧（凹面）：arc 端点 y=370 附近；底部两行文需落在完全可见的弧内侧
// 三列以控件中心为对称轴，列间距 = kColGap（≤10），与两侧列的间隙相同
constexpr double kGaugeCenterX = kGaugeW * 0.5; // 370
constexpr double kColGap       = 8.0;   // 相邻两列外缘间距（需 ≤ 10）
constexpr double kTextW        = 108.0; // 每列水平分配宽度（可略调避免裁字）
constexpr double kRolX  = kGaugeCenterX - kTextW - kColGap;
constexpr double kHdgX  = kGaugeCenterX;
constexpr double kPitX  = kGaugeCenterX + kTextW + kColGap;
constexpr double kValH  =  36.0;  // 数值行（上）
constexpr double kLblH  =  24.0;  // 列标题行（下）
constexpr double kValY  = 308.0;  // 数值行 top
constexpr double kLblY  = 346.0;  // 标签行 top，346+24=370 贴底
} // namespace

// ── 辅助：设置字体 ───────────────────────────────────────────────────
static void setGaugeFont(QPainter &p, int pixelSize, bool bold = false)
{
    QFont f;
    f.setFamily(QStringLiteral("Bai Jamjuree"));
    f.setPixelSize(pixelSize);
    f.setWeight(bold ? QFont::DemiBold : QFont::Normal);
    p.setFont(f);
}

// ────────────────────────────────────────────────────────────────────

RudderGaugeWidget::RudderGaugeWidget(QWidget *parent)
    : QWidget(parent)
{
    setFixedSize(kGaugeW, kGaugeH);
    setAttribute(Qt::WA_TranslucentBackground, true);
}

void RudderGaugeWidget::setHeading(double degrees)
{
    m_heading = degrees;
    update();
}

void RudderGaugeWidget::setRoll(double degrees)
{
    m_roll = degrees;
    update();
}

void RudderGaugeWidget::setPitch(double degrees)
{
    m_pitch = degrees;
    update();
}

void RudderGaugeWidget::setNightMode(bool night)
{
    m_nightMode = night;
    update();
}

void RudderGaugeWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    p.setClipRect(rect());

    // 弧心在控件底部下方 kArcCenterDY 处（控件内不可见）
    const double cx       = kGaugeW * 0.5;          // 370
    const double cy       = kGaugeH + kArcCenterDY;  // ≈ 993.54
    const double arcPeakY = cy - kRadius;            // ≈ 273.54（弧线最高点 y 坐标）

    // 日/夜模式颜色
    const QColor arcBgColor = m_nightMode ? QColor(QStringLiteral("#2B2B2B")) : QColor(QStringLiteral("#C8D4EE"));
    const QColor arcFgColor = m_nightMode ? QColor(QStringLiteral("#FFFFFF")) : QColor(QStringLiteral("#5B73AB"));
    const QColor colWhite   = m_nightMode ? QColor(QStringLiteral("#FFFFFF")) : QColor(QStringLiteral("#5B73AB"));
    const QColor colGray    = m_nightMode ? QColor(QStringLiteral("#8F8F8F")) : QColor(QStringLiteral("#9AAED0"));
    const QColor colText    = m_nightMode ? QColor(QStringLiteral("#FFFFFF")) : QColor(QStringLiteral("#5B73AB"));
    const QColor colLbl     = m_nightMode ? QColor(255, 255, 255, 0x80)       : QColor(91, 115, 171, 0x80);

    // ── 大刻度等宽的背景宽圆弧（径向厚度 = kMajorLen）──────────────────
    {
        const double        rIn  = kRadius;
        const double        rOut = kRadius + kMajorLen;
        const double        a60  = 60.0;
        const double        a120 = 120.0;
        const QRectF        outerR(cx - rOut, cy - rOut, 2.0 * rOut, 2.0 * rOut);
        const QRectF        innerR(cx - rIn,  cy - rIn,  2.0 * rIn,  2.0 * rIn);
        QPainterPath        wideArc;
        wideArc.moveTo(cx + rOut * std::cos(qDegreesToRadians(a60)),
                      cy - rOut * std::sin(qDegreesToRadians(a60)));
        wideArc.arcTo(outerR, a60, 60.0);
        wideArc.lineTo(cx + rIn * std::cos(qDegreesToRadians(a120)),
                      cy - rIn * std::sin(qDegreesToRadians(a120)));
        wideArc.arcTo(innerR, a120, -60.0);
        wideArc.closeSubpath();
        p.setPen(Qt::NoPen);
        p.setBrush(arcBgColor);
        p.drawPath(wideArc);
    }

    // ── 主弧线（60°，从左下角经顶部到右下角）─────────────────────────
    {
        const QRectF arcRect(cx - kRadius, cy - kRadius,
                             kRadius * 2.0, kRadius * 2.0);
        p.setPen(QPen(arcFgColor, kArcPenW, Qt::SolidLine, Qt::FlatCap));
        p.setBrush(Qt::NoBrush);
        // Qt drawArc：0=3点钟，正值逆时针，单位 1/16°
        // 从 60°（右端点，即 3点+30°）逆时针转 60° 到 120°（左端点）= 经过 90°（顶部）
        p.drawArc(arcRect, 60 * 16, 60 * 16);
    }

    // ── 旋转刻度（当前航向始终对齐弧线中央）──────────────────────────
    // arcPos = 某刻度相对弧线中心的偏移量（正值 = 顺时针 = 右侧）
    // 数学角 = 90° − arcPos；Qt 坐标：x = cx + R·cos, y = cy − R·sin
    {
        setGaugeFont(p, kTickFontPx, true);

        const int centerDeg = static_cast<int>(std::floor(m_heading));
        for (int g = centerDeg - 31; g <= centerDeg + 31; ++g) {
            const double arcPos = static_cast<double>(g) - m_heading;
            if (arcPos < -kArcHalfAngle || arcPos > kArcHalfAngle) continue;

            const double mathRad = qDegreesToRadians(90.0 - arcPos);
            const double cosA    = qCos(mathRad);
            const double sinA    = qSin(mathRad);

            // 弧线上的外侧点
            const double ox = cx + kRadius * cosA;
            const double oy = cy - kRadius * sinA;

            // 当前格对应的显示航向值（0–359）
            const int dispVal = ((g % 360) + 360) % 360;

            if (g % 10 == 0) {
                // 大刻度：从弧线向外侧（远离弧心方向）延伸
                const double ex = cx + (kRadius + kMajorLen) * cosA;
                const double ey = cy - (kRadius + kMajorLen) * sinA;
                p.setPen(QPen(colWhite, kMajorPenW, Qt::SolidLine, Qt::FlatCap));
                p.drawLine(QPointF(ox, oy), QPointF(ex, ey));

                // 数字在大刻度外端之外
                const double nx = cx + kNumberR * cosA;
                const double ny = cy - kNumberR * sinA;
                p.setPen(colWhite);
                p.drawText(QRectF(nx - 22.0, ny - 13.0, 44.0, 26.0),
                           Qt::AlignCenter, QString::number(dispVal));
            } /*else if (g % 5 == 0) {
                // 中刻度（每 5°）：向外侧延伸
                const double ex = cx + (kRadius + kMinorLen) * cosA;
                const double ey = cy - (kRadius + kMinorLen) * sinA;
                p.setPen(QPen(colWhite, kMinorPenW, Qt::SolidLine, Qt::FlatCap));
                p.drawLine(QPointF(ox, oy), QPointF(ex, ey));
            }*/ else {
                // 小刻度（每 1°）：向外侧延伸
                const double ex = cx + (kRadius + kMicroLen) * cosA;
                const double ey = cy - (kRadius + kMicroLen) * sinA;
                p.setPen(QPen(colGray, kMinorPenW, Qt::SolidLine, Qt::FlatCap));
                p.drawLine(QPointF(ox, oy), QPointF(ex, ey));
            }
        }
    }

    // ── 黄色三角形指针（固定在弧线中央顶点，尖端朝上指向当前刻度）──────
    // 三角形不随航向移动；刻度盘旋转使当前航向始终位于三角形正上方
    const QPointF tip   (cx,           arcPeakY);               // 尖端触及弧顶
    const QPointF baseL (cx - kTriHW,  arcPeakY + kTriHeight);  // 左底角
    const QPointF baseR (cx + kTriHW,  arcPeakY + kTriHeight);  // 右底角

    QPolygonF tri;
    tri << tip << baseL << baseR;

    p.setPen(Qt::NoPen);
    p.setBrush(QColor(QStringLiteral("#FFBF00")));
    p.drawPolygon(tri);

    // ── Roll / Pitch / Heading：数值在上、标签（Roll/Pitch/Heading）在下 ─
    // ROL（左列）
    setGaugeFont(p, kValFontPx, true);
    p.setPen(colText);
    p.drawText(QRectF(kRolX - kTextW * 0.5, kValY, kTextW, kValH),
               Qt::AlignCenter,
               QString::number(m_roll, 'f', 1) + QStringLiteral("°"));
    setGaugeFont(p, kLblFontPx, false);
    p.setPen(colLbl);
    p.drawText(QRectF(kRolX - kTextW * 0.5, kLblY, kTextW, kLblH),
               Qt::AlignCenter, QStringLiteral("Roll"));

    // PIT（右列）
    setGaugeFont(p, kValFontPx, true);
    p.setPen(colText);
    p.drawText(QRectF(kPitX - kTextW * 0.5, kValY, kTextW, kValH),
               Qt::AlignCenter,
               QString::number(m_pitch, 'f', 1) + QStringLiteral("°"));
    setGaugeFont(p, kLblFontPx, false);
    p.setPen(colLbl);
    p.drawText(QRectF(kPitX - kTextW * 0.5, kLblY, kTextW, kLblH),
               Qt::AlignCenter, QStringLiteral("Pitch"));

    // HDG（中列）
    setGaugeFont(p, kValFontPx, true);
    p.setPen(colText);
    p.drawText(QRectF(kHdgX - kTextW * 0.5, kValY, kTextW, kValH),
               Qt::AlignCenter,
               QString::number(m_heading, 'f', 1) + QStringLiteral("°"));
    setGaugeFont(p, kLblFontPx, false);
    p.setPen(colLbl);
    p.drawText(QRectF(kHdgX - kTextW * 0.5, kLblY, kTextW, kLblH),
               Qt::AlignCenter, QStringLiteral("Heading"));
}
