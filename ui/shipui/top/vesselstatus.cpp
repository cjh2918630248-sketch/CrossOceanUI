#include "top/vesselstatus.h"

namespace {
constexpr int kVesselStatusWidth  = 643;
constexpr int kVesselStatusHeight = 42;

QPushButton *makeActionButton(const QString &text, QWidget *parent)
{
    auto *button = new QPushButton(text, parent);
    button->setCursor(Qt::PointingHandCursor);
    button->setFlat(true);
    button->setMinimumHeight(kVesselStatusHeight);
    button->setObjectName(QStringLiteral("actionButton"));
    return button;
}

int clampPercent(int percent)
{
    if (percent < 0) {
        return 0;
    }
    if (percent > 100) {
        return 100;
    }
    return percent;
}

QColor batteryLevelColor(int percent)
{
    if (percent <= 20) {
        return QColor(QStringLiteral("#ff4d4f"));
    }
    if (percent <= 50) {
        return QColor(QStringLiteral("#f5a623"));
    }
    return QColor(QStringLiteral("#2ecc71"));
}
//画一个电池
QPixmap makeBatteryPixmap(int percent)
{
    const int clampedPercent = clampPercent(percent);
    const QSize iconSize(44, 22);
    QPixmap pixmap(iconSize);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing, true);

    const QRect bodyRect(1, 2, 37, 18);
    const QRect headRect(39, 7, 4, 8);
    const QColor borderColor(QStringLiteral("#d0d2d6"));
    const QColor fillColor = batteryLevelColor(clampedPercent);

    painter.setPen(QPen(borderColor, 1.5));
    painter.setBrush(Qt::NoBrush);
    painter.drawRoundedRect(bodyRect, 3, 3);
    painter.drawRoundedRect(headRect, 1, 1);

    const int innerWidth = bodyRect.width() - 4;
    const int fillWidth = (innerWidth * clampedPercent) / 100;
    if (fillWidth > 0) {
        QRect fillRect(bodyRect.left() + 2, bodyRect.top() + 2, fillWidth, bodyRect.height() - 4);
        painter.setPen(Qt::NoPen);
        painter.setBrush(fillColor);
        painter.drawRoundedRect(fillRect, 2, 2);
    }

    return pixmap;
}
//画代表电量的波形
QPixmap makeWaveformPixmap()
{
    const QSize iconSize(24, 24);
    QPixmap pixmap(iconSize);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(QStringLiteral("#3ecf5e")));

    const int barHeights[5] = {6, 12, 18, 12, 6};
    const int barWidth = 3;
    const int barGap = 2;
    const int totalWidth = 5 * barWidth + 4 * barGap;
    const int startX = (iconSize.width() - totalWidth) / 2;
    const int centerY = iconSize.height() / 2;
    for (int i = 0; i < 5; ++i) {
        const int h = barHeights[i];
        const int x = startX + i * (barWidth + barGap);
        painter.drawRoundedRect(QRect(x, centerY - h / 2, barWidth, h), 1, 1);
    }
    return pixmap;
}
} // namespace

VesselStatus::VesselStatus(QWidget *parent)
    : QWidget(parent)
{
    setupUi();
    setupConnections();
    updateBatteryDisplay();
}

void VesselStatus::onRcButtonClicked()
{
    emit rcButtonClicked();
    qDebug() << "VesselStatus::onRcButtonClicked()";
}

void VesselStatus::onMcButtonClicked()
{
    emit mcButtonClicked();
    qDebug() << "VesselStatus::onMcButtonClicked()";
}

void VesselStatus::onAutoButtonClicked()
{
    emit autoButtonClicked();
    qDebug() << "VesselStatus::onAutoButtonClicked()";
}

void VesselStatus::onStopButtonClicked()
{
    emit stopButtonClicked();
    qDebug() << "VesselStatus::onStopButtonClicked()";
}

void VesselStatus::onControllerButtonClicked()
{
    emit controllerButtonClicked();
    qDebug() << "VesselStatus::onControllerButtonClicked()";
}

void VesselStatus::setupUi()
{
    setObjectName(QStringLiteral("vesselStatusPanel"));
    setFixedSize(kVesselStatusWidth, kVesselStatusHeight);

    auto *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(16);

    auto *waveformLabel = new QLabel(this);
    waveformLabel->setObjectName(QStringLiteral("waveformLabel"));
    waveformLabel->setFixedSize(24, kVesselStatusHeight);
    waveformLabel->setAlignment(Qt::AlignCenter);
    waveformLabel->setAttribute(Qt::WA_TransparentForMouseEvents);
    waveformLabel->setPixmap(makeWaveformPixmap());

    m_rcButton         = makeActionButton(QStringLiteral("RC"), this);
    m_mcButton         = makeActionButton(QStringLiteral("M/C"), this);
    m_autoModeButton   = makeActionButton(QStringLiteral("AUTO"), this);
    m_stopButton       = makeActionButton(QStringLiteral("STOP"), this);
    m_controllerButton = makeActionButton(QStringLiteral("遥控"), this);

    m_batteryPercentLabel = new QLabel(this);
    m_batteryPercentLabel->setObjectName(QStringLiteral("batteryPercentLabel"));
    m_batteryPercentLabel->setAlignment(Qt::AlignVCenter | Qt::AlignRight);

    m_batteryIndicator = new QLabel(this);
    m_batteryIndicator->setObjectName(QStringLiteral("batteryIndicatorLabel"));
    m_batteryIndicator->setFixedSize(44, 22);
    m_batteryIndicator->setAttribute(Qt::WA_TransparentForMouseEvents);

    layout->addWidget(waveformLabel,         0, Qt::AlignVCenter);
    layout->addWidget(m_rcButton,            0, Qt::AlignVCenter);
    layout->addWidget(m_mcButton,            0, Qt::AlignVCenter);
    layout->addWidget(m_autoModeButton,      0, Qt::AlignVCenter);
    layout->addWidget(m_stopButton,          0, Qt::AlignVCenter);
    layout->addWidget(m_controllerButton,    0, Qt::AlignVCenter);
    layout->addStretch(1);
    layout->addWidget(m_batteryPercentLabel, 0, Qt::AlignVCenter);
    layout->addWidget(m_batteryIndicator,    0, Qt::AlignVCenter);
}

void VesselStatus::setupConnections()
{
    connect(m_rcButton, &QPushButton::clicked, this, &VesselStatus::onRcButtonClicked);
    connect(m_mcButton, &QPushButton::clicked, this, &VesselStatus::onMcButtonClicked);
    connect(m_autoModeButton, &QPushButton::clicked, this, &VesselStatus::onAutoButtonClicked);
    connect(m_stopButton, &QPushButton::clicked, this, &VesselStatus::onStopButtonClicked);
    connect(m_controllerButton, &QPushButton::clicked, this, &VesselStatus::onControllerButtonClicked);
}

void VesselStatus::setBatteryPercent(int percent)
{
    const int clampedPercent = clampPercent(percent);
    if (m_batteryPercent == clampedPercent) {
        return;
    }

    m_batteryPercent = clampedPercent;
    updateBatteryDisplay();
}

void VesselStatus::updateBatteryDisplay()
{
    if (m_batteryPercentLabel) {
        m_batteryPercentLabel->setText(QStringLiteral("%1%").arg(m_batteryPercent));
    }
    if (m_batteryIndicator) {
        m_batteryIndicator->setPixmap(makeBatteryPixmap(m_batteryPercent));
    }
}

void VesselStatus::setAppNightMode(bool isNight)
{
    const QString btnStyle   = isNight ? QString() : QStringLiteral("QPushButton#actionButton{color:#000000;}");
    const QString labelStyle = isNight ? QString() : QStringLiteral("color:#000000;");
    for (auto *b : {m_rcButton, m_mcButton, m_autoModeButton, m_stopButton, m_controllerButton}) {
        if (b) {
            b->setStyleSheet(btnStyle);
        }
    }
    if (m_batteryPercentLabel) {
        m_batteryPercentLabel->setStyleSheet(labelStyle);
    }
}
